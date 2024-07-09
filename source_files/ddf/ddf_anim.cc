//----------------------------------------------------------------------------
//  EDGE Data Definition File Code (Animated textures)
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
//
// Animated Texture/Flat Setup and Parser Code
//

#include "ddf_anim.h"

#include <string.h>

#include "ddf_local.h"

static AnimationDefinition *dynamic_anim;

static void DDFAnimGetType(const char *info, void *storage);
static void DDFAnimGetPic(const char *info, void *storage);

// -ACB- 1998/08/10 Use DDFMainGetLumpName for getting the..lump name.
// -KM- 1998/09/27 Use DDFMainGetTime for getting tics

static AnimationDefinition dummy_anim;

static const DDFCommandList anim_commands[] = {DDF_FIELD("TYPE", dummy_anim, type_, DDFAnimGetType),
                                               DDF_FIELD("SEQUENCE", dummy_anim, pics_, DDFAnimGetPic),
                                               DDF_FIELD("SPEED", dummy_anim, speed_, DDFMainGetTime),

                                               {nullptr, nullptr, 0, nullptr}};

// -ACB- 2004/06/03 Replaced array and size with purpose-built class
AnimationDefinitionContainer animdefs;

AnimationDefinition *AnimationDefinitionContainer::Lookup(const char *refname)
{
    if (!refname || !refname[0])
        return nullptr;

    for (std::vector<AnimationDefinition *>::iterator iter = begin(), iter_end = end(); iter != iter_end; iter++)
    {
        AnimationDefinition *anim = *iter;
        if (DDFCompareName(anim->name_.c_str(), refname) == 0)
            return anim;
    }

    return nullptr;
}

//
//  DDF PARSE ROUTINES
//
static void AnimStartEntry(const char *name, bool extend)
{
    if (!name || !name[0])
    {
        DDFWarnError("New anim entry is missing a name!");
        name = "ANIM_WITH_NO_NAME";
    }

    dynamic_anim = animdefs.Lookup(name);

    if (extend)
    {
        if (!dynamic_anim)
            DDFError("Unknown animdef to extend: %s\n", name);
        return;
    }

    // replaces an existing entry
    if (dynamic_anim)
    {
        dynamic_anim->Default();
        return;
    }

    // not found, create a new one
    dynamic_anim = new AnimationDefinition;

    dynamic_anim->name_ = name;

    animdefs.push_back(dynamic_anim);
}

static void AnimParseField(const char *field, const char *contents, int index, bool is_last)
{
#if (DDF_DEBUG)
    LogDebug("ANIM_PARSE: %s = %s;\n", field, contents);
#endif

    if (DDFMainParseField(anim_commands, field, contents, (uint8_t *)dynamic_anim))
        return;

    DDFError("Unknown anims.ddf command: %s\n", field);
}

static void AnimFinishEntry(void)
{
    if (dynamic_anim->speed_ <= 0)
    {
        DDFWarnError("Bad TICS value for anim: %d\n", dynamic_anim->speed_);
        dynamic_anim->speed_ = 8;
    }

    if (dynamic_anim->pics_.empty())
    {
        if (dynamic_anim->type_ == AnimationDefinition::kAnimationTypeGraphic)
            DDFError("Animation missing the SEQUENCE command.\n");
    }
}

static void AnimClearAll(void)
{
    // 100% safe to delete all animations
    for (AnimationDefinition *anim : animdefs)
    {
        delete anim;
        anim = nullptr;
    }
    animdefs.clear();
}

void DDFReadAnims(const std::string &data)
{
    DDFReadInfo anims;

    anims.tag      = "ANIMATIONS";
    anims.short_name = "DDFANIM";

    anims.start_entry  = AnimStartEntry;
    anims.parse_field  = AnimParseField;
    anims.finish_entry = AnimFinishEntry;
    anims.clear_all    = AnimClearAll;

    DDFMainReadFile(&anims, data);
}

//
// DDFAnimInit
//
void DDFAnimInit(void)
{
    AnimClearAll();
}

//
// DDFAnimCleanUp
//
void DDFAnimCleanUp(void)
{
    animdefs.shrink_to_fit(); // <-- Reduce to allocated size
}

//
// DDFAnimGetType
//
static void DDFAnimGetType(const char *info, void *storage)
{
    EPI_ASSERT(storage);

    int *type = (int *)storage;

    if (DDFCompareName(info, "FLAT") == 0)
        (*type) = AnimationDefinition::kAnimationTypeFlat;
    else if (DDFCompareName(info, "TEXTURE") == 0)
        (*type) = AnimationDefinition::kAnimationTypeTexture;
    else if (DDFCompareName(info, "GRAPHIC") == 0)
        (*type) = AnimationDefinition::kAnimationTypeGraphic;
    else
    {
        DDFError("Unknown animation type: %s\n", info);
        (*type) = AnimationDefinition::kAnimationTypeFlat;
    }
}

static void DDFAnimGetPic(const char *info, void *storage)
{
    dynamic_anim->pics_.push_back(info);
}

// ---> animdef_c class

//
// animdef_c constructor
//
AnimationDefinition::AnimationDefinition() : name_(), pics_()
{
    Default();
}

//
// animdef_c::CopyDetail()
//
// Copies all the detail with the exception of ddf info
//
void AnimationDefinition::CopyDetail(AnimationDefinition &src)
{
    type_       = src.type_;
    pics_       = src.pics_;
    speed_      = src.speed_;
}

//
// animdef_c::Default()
//
void AnimationDefinition::Default()
{
    type_ = kAnimationTypeTexture;

    pics_.clear();

    speed_ = 8;
}

//--- editor settings ---
// vi:ts=4:sw=4:noexpandtab
