//----------------------------------------------------------------------------
//  EPI - Predefined ENames
//----------------------------------------------------------------------------
//
//  Copyright (c) 2024 The EDGE Team
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

// clang-format off

//------------ General Names ---------------------------

// 'None' must always be the first name.

xx(None)
xx(Null)
xx(_)

//------------ File Extensions ---------------------------

// Must strip leading . when creating ENames to compare

// images
xx(Png)
xx(Tga)
xx(Jpg)
xx(Jpeg)
xx(Lmp)
xx(Gif)
xx(Bmp)
xx(Dds)

// instrument banks
xx(Sf2)
xx(Op2)
xx(Ad)
xx(Opl)
xx(Tmb)

// EDGE-specific
xx(Edm)
xx(Epk)
xx(Rts)
xx(Ddf)
xx(Ldf)

// DOOM-related
xx(Wad)
xx(Pk3)
xx(Deh)
xx(Bex)

// sounds
xx(Wav)
xx(Wave)
xx(Flac)
xx(Ogg)
xx(Mp3)
xx(Mus)
xx(Mid)
xx(Midi)
xx(Xmi)
xx(Rmi)
xx(Rmid)
xx(Mod)
xx(S3m)
xx(Xm)
xx(It)
xx(Rad)
xx(Dsp)
xx(Pcs)
xx(Gsp)
xx(Gsw)

//------------ UDMF ---------------------------

// generic keys
xx(Special)
xx(Id)
xx(X)
xx(Y)
xx(Sector)
xx(Thing)
xx(Vertex)
xx(Linedef)
xx(Sidedef)

// vertexes
xx(Zfloor)
xx(Zceiling)

// linedefs
xx(V1)
xx(V2)
xx(Sidefront)
xx(Sideback)
xx(Blocking)
xx(Blockmonsters)
xx(Twosided)
xx(Dontpegtop)
xx(Dontpegbottom)
xx(Secret)
xx(Blocksound)
xx(Dontdraw)
xx(Mapped)
xx(Passuse)
xx(Blockplayers)
xx(Blocksight)

// sidedefs
xx(Offsetx)
xx(Offsety)
xx(Offsetx_bottom)
xx(Offsetx_mid)
xx(Offsetx_top)
xx(Offsety_bottom)
xx(Offsety_mid)
xx(Offsety_top)
xx(Scalex_bottom)
xx(Scalex_mid)
xx(Scalex_top)
xx(Scaley_bottom)
xx(Scaley_mid)
xx(Scaley_top)
xx(Texturetop)
xx(Texturebottom)
xx(Texturemiddle)

// sectors
xx(Heightfloor)
xx(Heightceiling)
xx(Texturefloor)
xx(Textureceiling)
xx(Lightlevel)
xx(Lightcolor)
xx(Fadecolor)
xx(Fogdensity)
xx(Xpanningfloor)
xx(Ypanningfloor)
xx(Xpanningceiling)
xx(Ypanningceiling)
xx(Xscalefloor)
xx(Yscalefloor)
xx(Xscaleceiling)
xx(Yscaleceiling)
xx(Rotationfloor)
xx(Rotationceiling)
xx(Gravity)

// things
xx(Height)
xx(Angle)
xx(Type)
xx(Skill1)
xx(Skill2)
xx(Skill3)
xx(Skill4)
xx(Skill5)
xx(Ambush)
xx(Single)
xx(Dm)
xx(Coop)
xx(Friend)
xx(Health)
xx(Alpha)
xx(Scale)
xx(Scalex)
xx(Scaley)

//------------ UMAPINFO ---------------------------
xx(Levelname)
xx(Label)
xx(Next)
xx(Nextsecret)
xx(Levelpic)
xx(Skytexture)
xx(Music)
xx(Endpic)
xx(Endcast)
xx(Endbunny)
xx(Endgame)
xx(Exitpic)
xx(Enterpic)
xx(Nointermission)
xx(Partime)
xx(Intertext)
xx(Intertextsecret)
xx(Interbackdrop)
xx(Intermusic)
xx(Episode)
xx(Bossaction)
xx(Author)


//------------ Actor Names ---------------------------
xx(DoomPlayer)
xx(ZombieMan)
xx(ShotgunGuy)
xx(Archvile)
xx(ArchvileFire)
xx(Revenant)
xx(RevenantTracer)
xx(RevenantTracerSmoke)
xx(Fatso)
xx(FatShot)
xx(ChaingunGuy)
xx(DoomImp)
xx(Demon)
xx(Spectre)
xx(Cacodemon)
xx(BaronOfHell)
xx(BaronBall)
xx(HellKnight)
xx(LostSoul)
xx(SpiderMastermind)
xx(Arachnotron)
xx(Cyberdemon)
xx(PainElemental)
xx(WolfensteinSS)
xx(CommanderKeen)
xx(BossBrain)
xx(BossEye)
xx(BossTarget)
xx(SpawnShot)
xx(SpawnFire)
xx(ExplosiveBarrel)
xx(DoomImpBall)
xx(CacodemonBall)
xx(Rocket)
xx(PlasmaBall)
xx(BFGBall)
xx(ArachnotronPlasma)
xx(BulletPuff)
xx(Blood)
xx(TeleportFog)
xx(ItemFog)
xx(TeleportDest)
xx(BFGExtra)
xx(GreenArmor)
xx(BlueArmor)
xx(HealthBonus)
xx(ArmorBonus)
xx(BlueCard)
xx(RedCard)
xx(YellowCard)
xx(YellowSkull)
xx(RedSkull)
xx(BlueSkull)
xx(Stimpack)
xx(Medikit)
xx(Soulsphere)
xx(InvulnerabilitySphere)
xx(Berserk)
xx(BlurSphere)
xx(RadSuit)
xx(Allmap)
xx(Infrared)
xx(Megasphere)
xx(Clip)
xx(ClipBox)
xx(RocketAmmo)
xx(RocketBox)
xx(Cell)
xx(CellPack)
xx(Shell)
xx(ShellBox)
xx(Backpack)
xx(BFG9000)
xx(Chaingun)
xx(Chainsaw)
xx(RocketLauncher)
xx(PlasmaRifle)
xx(Shotgun)
xx(SuperShotgun)
xx(TechLamp)
xx(TechLamp2)
xx(Column)
xx(TallGreenColumn)
xx(ShortGreenColumn)
xx(TallRedColumn)
xx(ShortRedColumn)
xx(SkullColumn)
xx(HeartColumn)
xx(EvilEye)
xx(FloatingSkull)
xx(TorchTree)
xx(BlueTorch)
xx(GreenTorch)
xx(RedTorch)
xx(ShortBlueTorch)
xx(ShortGreenTorch)
xx(ShortRedTorch)
xx(Stalagtite)
xx(TechPillar)
xx(CandleStick)
xx(Candelabra)
xx(BloodyTwitch)
xx(Meat2)
xx(Meat3)
xx(Meat4)
xx(Meat5)
xx(NonsolidMeat2)
xx(NonsolidMeat4)
xx(NonsolidMeat3)
xx(NonsolidMeat5)
xx(NonsolidTwitch)
xx(DeadCacodemon)
xx(DeadMarine)
xx(DeadZombieMan)
xx(DeadDemon)
xx(DeadLostSoul)
xx(DeadDoomImp)
xx(DeadShotgunGuy)
xx(GibbedMarine)
xx(GibbedMarineExtra)
xx(HeadsOnAStick)
xx(Gibs)
xx(HeadOnAStick)
xx(HeadCandles)
xx(DeadStick)
xx(LiveStick)
xx(BigTree)
xx(BurningBarrel)
xx(HangNoGuts)
xx(HangBNoBrain)
xx(HangTLookingDown)
xx(HangTSkull)
xx(HangTLookingUp)
xx(HangTNoBrain)
xx(ColonGibs)
xx(SmallBloodPool)
xx(BrainStem)
xx(PointPusher)
xx(PointPuller)
xx(MBFHelperDog)
xx(PlasmaBall1)
xx(PlasmaBall2)
xx(EvilSceptre)
xx(UnholyBible)
xx(MusicChanger)
xx(Deh_Actor_145)
xx(Deh_Actor_146)
xx(Deh_Actor_147)
xx(Deh_Actor_148)
xx(Deh_Actor_149)
xx(Deh_Actor_150)
xx(Deh_Actor_151)
xx(Deh_Actor_152)
xx(Deh_Actor_153)
xx(Deh_Actor_154)
xx(Deh_Actor_155)
xx(Deh_Actor_156)
xx(Deh_Actor_157)
xx(Deh_Actor_158)
xx(Deh_Actor_159)
xx(Deh_Actor_160)
xx(Deh_Actor_161)
xx(Deh_Actor_162)
xx(Deh_Actor_163)
xx(Deh_Actor_164)
xx(Deh_Actor_165)
xx(Deh_Actor_166)
xx(Deh_Actor_167)
xx(Deh_Actor_168)
xx(Deh_Actor_169)
xx(Deh_Actor_170)
xx(Deh_Actor_171)
xx(Deh_Actor_172)
xx(Deh_Actor_173)
xx(Deh_Actor_174)
xx(Deh_Actor_175)
xx(Deh_Actor_176)
xx(Deh_Actor_177)
xx(Deh_Actor_178)
xx(Deh_Actor_179)
xx(Deh_Actor_180)
xx(Deh_Actor_181)
xx(Deh_Actor_182)
xx(Deh_Actor_183)
xx(Deh_Actor_184)
xx(Deh_Actor_185)
xx(Deh_Actor_186)
xx(Deh_Actor_187)
xx(Deh_Actor_188)
xx(Deh_Actor_189)
xx(Deh_Actor_190)
xx(Deh_Actor_191)
xx(Deh_Actor_192)
xx(Deh_Actor_193)
xx(Deh_Actor_194)
xx(Deh_Actor_195)
xx(Deh_Actor_196)
xx(Deh_Actor_197)
xx(Deh_Actor_198)
xx(Deh_Actor_199)
xx(Deh_Actor_200)
xx(Deh_Actor_201)
xx(Deh_Actor_202)
xx(Deh_Actor_203)
xx(Deh_Actor_204)
xx(Deh_Actor_205)
xx(Deh_Actor_206)
xx(Deh_Actor_207)
xx(Deh_Actor_208)
xx(Deh_Actor_209)
xx(Deh_Actor_210)
xx(Deh_Actor_211)
xx(Deh_Actor_212)
xx(Deh_Actor_213)
xx(Deh_Actor_214)
xx(Deh_Actor_215)
xx(Deh_Actor_216)
xx(Deh_Actor_217)
xx(Deh_Actor_218)
xx(Deh_Actor_219)
xx(Deh_Actor_220)
xx(Deh_Actor_221)
xx(Deh_Actor_222)
xx(Deh_Actor_223)
xx(Deh_Actor_224)
xx(Deh_Actor_225)
xx(Deh_Actor_226)
xx(Deh_Actor_227)
xx(Deh_Actor_228)
xx(Deh_Actor_229)
xx(Deh_Actor_230)
xx(Deh_Actor_231)
xx(Deh_Actor_232)
xx(Deh_Actor_233)
xx(Deh_Actor_234)
xx(Deh_Actor_235)
xx(Deh_Actor_236)
xx(Deh_Actor_237)
xx(Deh_Actor_238)
xx(Deh_Actor_239)
xx(Deh_Actor_240)
xx(Deh_Actor_241)
xx(Deh_Actor_242)
xx(Deh_Actor_243)
xx(Deh_Actor_244)
xx(Deh_Actor_245)
xx(Deh_Actor_246)
xx(Deh_Actor_247)
xx(Deh_Actor_248)
xx(Deh_Actor_249)
    // clang-format on