//----------------------------------------------------------------------------
//  EDGE GPU Texture Upload
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

#pragma once

#include "i_defs_gl.h"
#include "im_data.h"

enum TextureUploadFlag
{
    kUploadNone = 0,

    kUploadSmooth = (1 << 0),
    kUploadClamp  = (1 << 1),
    kUploadMipMap = (1 << 2),
    kUploadThresh = (1 << 3), // threshhold alpha (to 0 or 255)
};

GLuint UploadTexture(ImageData *img, int flags = kUploadNone, int max_pix = (1 << 30));

int DetermineOpacity(ImageData *img);

void BlackenClearAreas(ImageData *img);

//--- editor settings ---
// vi:ts=4:sw=4:noexpandtab