//------------------------------------------------------------------------
//  Sound Format Detection
//------------------------------------------------------------------------
// 
//  Copyright (c) 2022 - The EDGE-Classic Team
// 
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//------------------------------------------------------------------------

#include "epi.h"

#include "path.h"
#include "sound_types.h"

#include "gme.h"
#include "modplug.h"

namespace epi
{

sound_format_e Sound_DetectFormat(byte *data, int header_len)
{
	// Start by trying the simple reliable header checks

	if (data[0] == 'R' && data[1] == 'I' &&
		data[2] == 'F'  && data[3] == 'F')
	{
		return FMT_WAV;
	}

	if (data[0] == 'O' && data[1] == 'g' &&
		data[2] == 'g')
	{
		return FMT_OGG;
	}

	if ((data[0] == 'P' || data[0] == 'R') && data[1] == 'S' &&
		data[2] == 'I' && data[3] == 'D')
	{
		return FMT_SID;
	}

	if (data[0] == 'M' && data[1] == 'U' &&
		data[2] == 'S')
	{
		return FMT_MUS;
	}

	if (data[0] == 'M' && data[1] == 'T' &&
		data[2] == 'h'  && data[3] == 'd')
	{
		return FMT_MIDI;
	}

	// Moving on to more specialized or less reliable detections

	if (!std::string(gme_identify_header(data)).empty())
	{
		return FMT_GME;
	}

	ModPlugFile *mod_checker = ModPlug_Load(data, header_len);
	if (mod_checker)
	{
		ModPlug_Unload(mod_checker);
		return FMT_MOD;
	}

	if ((data[0] == 'I' && data[1] == 'D' && data[2] == '3') ||
		(data[0] == 0xFF && (data[1] >> 4 & 0xF)))
	{
		return FMT_MP3;
	}

	if (data[0] == 0x3)
	{
		return FMT_DOOM;
	}

	if (data[0] == 0x0)
	{
		return FMT_SPK;
	}

	return FMT_Unknown;
}

sound_format_e Sound_FilenameToFormat(const std::string& filename)
{
	std::string ext = epi::PATH_GetExtension(filename.c_str());

	if (ext == ".wav" || ext == ".wave")
		return FMT_WAV;

	if (ext == ".ogg")
		return FMT_OGG;

	if (ext == ".mp3")
		return FMT_MP3;

	if (ext == ".sid" || ext == ".psid")
		return FMT_SID;

	if (ext == ".mus")
		return FMT_MUS;

	if (ext == ".mid" || ext == ".midi")
		return FMT_MIDI;

	if (ext == ".mod" || ext == ".s3m" || ext == ".xm" || ext == ".it" || ext == ".669" ||
		ext == ".amf" || ext == ".ams" || ext == ".dbm" || ext == ".dmf" || ext == ".dsm" ||
		ext == ".far" || ext == ".mdl" || ext == ".med" || ext == ".mtm" || ext == ".okt" ||
		ext == ".ptm" || ext == ".stm" || ext == ".ult" || ext == ".umx" || ext == ".mt2" ||
		ext == ".psm")
		return FMT_MOD;

	if (ext == ".ay" || ext == ".gbs" || ext == ".gym" || ext == ".hes" || ext == ".nsf" ||
		ext == ".sap" || ext == ".spc" || ext == ".vgm")
		return FMT_GME;

	// Not sure if these will ever be encountered in the wild, but according to the VGMPF Wiki
	// they are valid DMX file extensions
	if (ext == ".dsp" || ext == ".pcs" || ext == ".gsp" || ext == ".gsw")
		return FMT_DOOM;

	return FMT_Unknown;
}

} // namespace epi

//--- editor settings ---
// vi:ts=4:sw=4:noexpandtab