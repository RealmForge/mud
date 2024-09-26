//----------------------------------------------------------------------------
//  EDGE SDL Video Code
//----------------------------------------------------------------------------
//
//  Copyright (c) 1999-2024 The EDGE Team.
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//----------------------------------------------------------------------------

#include "i_video.h"

#ifdef _WIN32
// clang-format off
#include <math.h>
#include <windows.h>
#include <dwmapi.h>
#include <VersionHelpers.h>
// clang-format on
#endif

#include "dm_state.h"
#include "edge_profiling.h"
#include "epi_str_compare.h"
#include "epi_str_util.h"
#include "i_defs_gl.h"
#include "i_system.h"
#include "m_argv.h"
#include "m_misc.h"
#include "n_network.h"
#include "r_modes.h"
#include "version.h"

SDL_Window *program_window;

int graphics_shutdown = 0;

// I think grab_mouse should be an internal bool instead of a cvar...why would a
// user need to adjust this on the fly? - Dasho
EDGE_DEFINE_CONSOLE_VARIABLE(grab_mouse, "1", kConsoleVariableFlagArchive)
EDGE_DEFINE_CONSOLE_VARIABLE(vsync, "1", kConsoleVariableFlagArchive)
EDGE_DEFINE_CONSOLE_VARIABLE_CLAMPED(gamma_correction, "0", kConsoleVariableFlagArchive, -1.0, 1.0)

// this is the Monitor Size setting, really an aspect ratio.
// it defaults to 16:9, as that is the most common monitor size nowadays.
EDGE_DEFINE_CONSOLE_VARIABLE(monitor_aspect_ratio, "1.77777", kConsoleVariableFlagArchive)

// these are zero until StartupGraphics is called.
// after that they never change (we assume the desktop won't become other
// resolutions while EC is running).
EDGE_DEFINE_CONSOLE_VARIABLE(desktop_resolution_width, "0", kConsoleVariableFlagReadOnly)
EDGE_DEFINE_CONSOLE_VARIABLE(desktop_resolution_height, "0", kConsoleVariableFlagReadOnly)

EDGE_DEFINE_CONSOLE_VARIABLE(pixel_aspect_ratio, "1.0", kConsoleVariableFlagReadOnly);

// when > 0, this will force the pixel_aspect to a particular value, for
// cases where a normal logic fails.  however, it will apply to *all* modes,
// including windowed mode.
EDGE_DEFINE_CONSOLE_VARIABLE(forced_pixel_aspect_ratio, "0", kConsoleVariableFlagArchive)

EDGE_DEFINE_CONSOLE_VARIABLE(framerate_limit, "500", kConsoleVariableFlagArchive)

static bool grab_state;

extern ConsoleVariable renderer_far_clip;
extern ConsoleVariable draw_culling;
extern ConsoleVariable draw_culling_distance;

void GrabCursor(bool enable)
{
#ifdef EDGE_WEB
    // On web, cursor lock is exclusively handled by selecting canvas
    return;
#endif

    if (!program_window || graphics_shutdown)
        return;

    grab_state = enable;

    if (grab_state && grab_mouse.d_)
    {
        SDL_SetWindowRelativeMouseMode(program_window, true);
    }
    else
    {
        SDL_SetWindowRelativeMouseMode(program_window, false);
    }
}

void DeterminePixelAspect()
{
    // the pixel aspect is the shape of pixels on the monitor for the current
    // video mode.  on modern LCDs (etc) it is usuall 1.0 (1:1).  knowing this
    // is critical to get things drawn correctly.  for example, Doom assets
    // assumed a 320x200 resolution on a 4:3 monitor, a pixel aspect of 5:6 or
    // 0.833333, and we must adjust image drawing to get "correct" results.

    // allow user to override
    if (forced_pixel_aspect_ratio.f_ > 0.1)
    {
        pixel_aspect_ratio = forced_pixel_aspect_ratio.f_;
        return;
    }

    // if not a fullscreen mode, check for a modern LCD (etc) monitor -- they
    // will have square pixels (1:1 aspect).
    bool is_crt = (desktop_resolution_width.d_ < desktop_resolution_height.d_ * 7 / 5);

    bool is_fullscreen = (current_window_mode > 0);
    if (is_fullscreen && current_screen_width == desktop_resolution_width.d_ &&
        current_screen_height == desktop_resolution_height.d_ && graphics_shutdown)
        is_fullscreen = false;

    if (!is_fullscreen && !is_crt)
    {
        pixel_aspect_ratio = 1.0f;
        return;
    }

    // in fullscreen modes, or a CRT monitor, compute the pixel aspect from
    // the current resolution and Monitor Size setting.  this assumes that the
    // video mode is filling the whole monitor (i.e. the monitor is not doing
    // any letter-boxing or pillar-boxing).  DPI setting does not matter here.

    pixel_aspect_ratio = monitor_aspect_ratio.f_ * (float)current_screen_height / (float)current_screen_width;
}

void StartupGraphics(void)
{
    std::string driver = ArgumentValue("videodriver");

    if (driver.empty())
    {
        const char *check = SDL_getenv("SDL_VIDEODRIVER");
        if (check)
            driver = check;
    }

    if (driver.empty())
        driver = "default";

    if (epi::StringCaseCompareASCII(driver, "default") != 0)
    {
        SDL_Environment *enviro = SDL_GetEnvironment();
        if (enviro)
            SDL_SetEnvironmentVariable(enviro, "SDL_VIDEODRIVER", driver.c_str(), true);
    }

    LogPrint("SDL_Video_Driver: %s\n", driver.c_str());

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
        FatalError("Couldn't init SDL VIDEO!\n%s\n", SDL_GetError());

    if (FindArgument("nograb") > 0)
        grab_mouse = 0;

    // -AJA- FIXME these are wrong (probably ignored though)
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
#ifdef EDGE_GL_ES2
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    initialize_gl4es();
#endif

    // -DS- 2005/06/27 Detect SDL Resolutions
    SDL_Point test_point = {0, 0};

    SDL_DisplayID test_id = SDL_GetDisplayForPoint(&test_point);

    if (test_id == 0)
        FatalError("Could not get desktop display mode! %s\n", SDL_GetError());

    const SDL_DisplayMode *info = SDL_GetDesktopDisplayMode(test_id);

    if (!info)
        FatalError("Could not get desktop display mode! %s\n", SDL_GetError());

    desktop_resolution_width  = info->w;
    desktop_resolution_height = info->h;

    if (current_screen_width > desktop_resolution_width.d_)
        current_screen_width = desktop_resolution_width.d_;
    if (current_screen_height > desktop_resolution_height.d_)
        current_screen_height = desktop_resolution_height.d_;

    LogPrint("Desktop resolution: %dx%d\n", desktop_resolution_width.d_, desktop_resolution_height.d_);

    int num_modes = 0;
    SDL_DisplayMode **all_modes = SDL_GetFullscreenDisplayModes(test_id, &num_modes);

    for (int i = 0; i < num_modes; i++)
    {
        SDL_DisplayMode *possible_mode = all_modes[i];

        if (possible_mode->w > desktop_resolution_width.d_ || possible_mode->h > desktop_resolution_height.d_)
            continue;

        DisplayMode test_mode;

        test_mode.width       = possible_mode->w;
        test_mode.height      = possible_mode->h;
        test_mode.depth       = SDL_BITSPERPIXEL(possible_mode->format);
        test_mode.window_mode = kWindowModeFullscreen;

        if ((test_mode.width & 15) != 0)
            continue;

        if (test_mode.depth == 15 || test_mode.depth == 16 || test_mode.depth == 24 || test_mode.depth == 32)
        {
            AddDisplayResolution(&test_mode);

            if (test_mode.width < desktop_resolution_width.d_ && test_mode.height < desktop_resolution_height.d_)
            {
                DisplayMode win_mode = test_mode;
                win_mode.window_mode = kWindowModeWindowed;
                AddDisplayResolution(&win_mode);
            }
        }
    }

    // If needed, set the default window toggle mode to the largest non-native
    // res
    if (toggle_windowed_window_mode.d_ == kWindowModeInvalid)
    {
        for (size_t i = 0; i < screen_modes.size(); i++)
        {
            DisplayMode *check = screen_modes[i];
            if (check->window_mode == kWindowModeWindowed)
            {
                toggle_windowed_window_mode = kWindowModeWindowed;
                toggle_windowed_height      = check->height;
                toggle_windowed_width       = check->width;
                toggle_windowed_depth       = check->depth;
                break;
            }
        }
    }

    // Fill in borderless mode scrmode with the native display info
    borderless_mode.window_mode = kWindowModeBorderless;
    borderless_mode.width       = info->w;
    borderless_mode.height      = info->h;
    borderless_mode.depth       = SDL_BITSPERPIXEL(info->format);

    // If needed, also make the default fullscreen toggle mode borderless
    if (toggle_fullscreen_window_mode.d_ == kWindowModeInvalid)
    {
        toggle_fullscreen_window_mode = kWindowModeBorderless;
        toggle_fullscreen_width       = info->w;
        toggle_fullscreen_height      = info->h;
        toggle_fullscreen_depth       = (int)SDL_BITSPERPIXEL(info->format);
    }

    if (all_modes)
        SDL_free(all_modes);

    LogPrint("StartupGraphics: initialisation OK\n");
}

static bool InitializeWindow(DisplayMode *mode)
{
    std::string temp_title = window_title.s_;
    temp_title.append(" ").append(edge_version.s_);

#if EDGE_WEB
    int resizeable = SDL_WINDOW_RESIZABLE;
#else
    int resizeable = 0;
#endif

    program_window =
        SDL_CreateWindow(temp_title.c_str(), mode->width, mode->height,
                         SDL_WINDOW_OPENGL |
                             (mode->window_mode == kWindowModeBorderless
                                  ? (SDL_WINDOW_BORDERLESS)
                                  : (mode->window_mode == kWindowModeFullscreen ? SDL_WINDOW_FULLSCREEN : 0)) |
                             resizeable);

    if (program_window == nullptr)
    {
        LogPrint("Failed to create window: %s\n", SDL_GetError());
        return false;
    }

    if (mode->window_mode == kWindowModeBorderless)
        SDL_GetWindowSize(program_window, &borderless_mode.width, &borderless_mode.height);

    if (mode->window_mode == kWindowModeWindowed)
    {
        toggle_windowed_depth       = mode->depth;
        toggle_windowed_height      = mode->height;
        toggle_windowed_width       = mode->width;
        toggle_windowed_window_mode = kWindowModeWindowed;
    }
    else if (mode->window_mode == kWindowModeFullscreen)
    {
        toggle_fullscreen_depth       = mode->depth;
        toggle_fullscreen_height      = mode->height;
        toggle_fullscreen_width       = mode->width;
        toggle_fullscreen_window_mode = kWindowModeFullscreen;
    }
    else
    {
        toggle_fullscreen_depth       = borderless_mode.depth;
        toggle_fullscreen_height      = borderless_mode.height;
        toggle_fullscreen_width       = borderless_mode.width;
        toggle_fullscreen_window_mode = kWindowModeBorderless;
    }

    if (SDL_GL_CreateContext(program_window) == nullptr)
        FatalError("Failed to create OpenGL context.\n");

    if (vsync.d_ == 2)
    {
        // Fallback to normal VSync if Adaptive doesn't work
        if (!SDL_GL_SetSwapInterval(-1))
        {
            vsync = 1;
            SDL_GL_SetSwapInterval(vsync.d_);
        }
    }
    else
        SDL_GL_SetSwapInterval(vsync.d_);

#ifndef EDGE_GL_ES2
    gladLoadGL();

    if (GLVersion.major < 2 || (GLVersion.major == 2 && GLVersion.minor < 1))
        FatalError("System only support GL %d.%d. Minimum GL version 2.1 required!\n", GLVersion.major, GLVersion.minor);
#endif

    return SetScreenSize(mode);
}

bool SetScreenSize(DisplayMode *mode)
{
    GrabCursor(false);

    LogPrint("SetScreenSize: trying %dx%d %dbpp (%s)\n", mode->width, mode->height, mode->depth,
             mode->window_mode == kWindowModeBorderless
                 ? "borderless"
                 : (mode->window_mode == kWindowModeFullscreen ? "fullscreen" : "windowed"));

    if (program_window == nullptr)
    {
        if (!InitializeWindow(mode))
        {
            return false;
        }
    }
    else if (mode->window_mode == kWindowModeBorderless)
    {
        SDL_SetWindowFullscreenMode(program_window, nullptr);
        SDL_SetWindowFullscreen(program_window, true);
        SDL_GetWindowSize(program_window, &borderless_mode.width, &borderless_mode.height);
        LogPrint("SetScreenSize: mode now %dx%d %dbpp\n", mode->width, mode->height, mode->depth);
    }
    else if (mode->window_mode == kWindowModeFullscreen)
    {
        SDL_DisplayMode new_mode;
        if (!SDL_GetClosestFullscreenDisplayMode(SDL_GetDisplayForWindow(program_window), mode->width, mode->height, 0.0f, false, &new_mode))
        {
            LogPrint("SetScreenSize: No matching fullscreen mode for %dx%d found!\n", mode->width, mode->height);
            return false;
        }
        else
        {    
            SDL_SetWindowFullscreenMode(program_window, &new_mode);
            SDL_SetWindowFullscreen(program_window, true);
            SDL_SetWindowSize(program_window, mode->width, mode->height);
            LogPrint("SetScreenSize: mode now %dx%d %dbpp\n", mode->width, mode->height, mode->depth);
        }
    }
    else /* kWindowModeWindowed */
    {
        SDL_SetWindowFullscreen(program_window, false);
        SDL_SetWindowSize(program_window, mode->width, mode->height);
        SDL_SetWindowPosition(program_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        LogPrint("SetScreenSize: mode now %dx%d %dbpp\n", mode->width, mode->height, mode->depth);
    }

    // -AJA- turn off cursor -- BIG performance increase.
    //       Plus, the combination of no-cursor + grab gives
    //       continuous relative mouse motion.
    GrabCursor(true);

#ifdef DEVELOPERS
    // override SDL signal handlers (the so-called "parachute").
    signal(SIGFPE, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
#endif

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(program_window);

    return true;
}

void StartFrame(void)
{
    ec_frame_stats.Clear();
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (draw_culling.d_)
        renderer_far_clip.f_ = draw_culling_distance.f_;
    else
        renderer_far_clip.f_ = 64000.0;
}

static void SwapBuffers(void)
{
    EDGE_ZoneScoped;

    SDL_GL_SwapWindow(program_window);
}

void FinishFrame(void)
{
    SwapBuffers();

    EDGE_TracyPlot("draw_render_units", (int64_t)ec_frame_stats.draw_render_units);
    EDGE_TracyPlot("draw_wall_parts", (int64_t)ec_frame_stats.draw_wall_parts);
    EDGE_TracyPlot("draw_planes", (int64_t)ec_frame_stats.draw_planes);
    EDGE_TracyPlot("draw_things", (int64_t)ec_frame_stats.draw_things);
    EDGE_TracyPlot("draw_light_iterator", (int64_t)ec_frame_stats.draw_light_iterator);
    EDGE_TracyPlot("draw_sector_glow_iterator", (int64_t)ec_frame_stats.draw_sector_glow_iterator);

    EDGE_FrameMark;

    if (grab_mouse.CheckModified())
        GrabCursor(grab_state);

    if (uncapped_frames.d_ && !single_tics)
    {
        if (framerate_limit.d_ >= kTicRate)
        {
            uint64_t target_time = 1000000ull / framerate_limit.d_;
            static uint64_t start_time;

            while (1)
            {
                uint64_t current_time = GetMicroseconds();
                uint64_t elapsed_time = current_time - start_time;
                uint64_t remaining_time = 0;

                if (elapsed_time >= target_time)
                {
                    start_time = current_time;
                    break;
                }

                remaining_time = target_time - elapsed_time;

                if (remaining_time > 1000)
                {
                    SleepForMilliseconds((remaining_time - 1000) / 1000);
                }
            }
        }
    }

    if (uncapped_frames.d_)
        fractional_tic = (float)(GetMilliseconds() * kTicRate % 1000) / 1000;

    if (vsync.CheckModified())
    {
        if (vsync.d_ == 2)
        {
            // Fallback to normal VSync if Adaptive doesn't work
            if (!SDL_GL_SetSwapInterval(-1))
            {
                vsync = 1;
                SDL_GL_SetSwapInterval(vsync.d_);
            }
        }
        else
            SDL_GL_SetSwapInterval(vsync.d_);
    }

    if (monitor_aspect_ratio.CheckModified() || forced_pixel_aspect_ratio.CheckModified())
        DeterminePixelAspect();
}

void ShutdownGraphics(void)
{
    if (graphics_shutdown)
        return;

    graphics_shutdown = 1;

    if (SDL_WasInit(0))
    {
        DeterminePixelAspect();

        SDL_Quit();
    }

#ifdef EDGE_GL_ES2
    close_gl4es();
#endif
}

//--- editor settings ---
// vi:ts=4:sw=4:noexpandtab
