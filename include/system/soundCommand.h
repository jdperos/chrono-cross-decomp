#ifndef _SOUND_COMMAND_H
#define _SOUND_COMMAND_H

#include "common.h"
#include "sound.h"

typedef enum ESoundCommand
{
    SOUND_CMD_10_START_FIELD_MUSIC                   = 0x10,
    SOUND_CMD_11_STOP_MUSIC_BY_ID                    = 0x11,
    SOUND_CMD_12_START_FIELD_MUSIC_LOOPED            = 0x12,
    SOUND_CMD_14_START_BATTLE_MUSIC                  = 0x14,
    SOUND_CMD_19_SET_MUSIC_LEVEL_IMM                 = 0x19,
    SOUND_CMD_1A_START_MASTER_AND_MUSIC_VOLUME_FADE  = 0x1A,
    SOUND_CMD_20_PLAY_SFX                            = 0x20,
    SOUND_CMD_21_EVICT_SFX_VOICE                     = 0x21,
    SOUND_CMD_24_PLAY_SFX_FROM_POINTER               = 0x24,
    SOUND_CMD_30_PROTECTED                           = 0x30,
    SOUND_CMD_34_DIRECT                              = 0x34,
    SOUND_CMD_40_PUSH_MUSIC_STATE                    = 0x40,
    SOUND_CMD_70_SET_CD_VOLUME                       = 0x70,
    SOUND_CMD_71_FADE_CD_VOLUME                      = 0x71,
    SOUND_CMD_72_FADE_CD_VOLUME_FROM                 = 0x72,
    SOUND_CMD_80_SET_MODE_STEREO                     = 0x80,
    SOUND_CMD_81_SET_MODE_MONO                       = 0x81,
    SOUND_CMD_90_SET_MUTED_MUSIC_CHANNELS            = 0x90,
    SOUND_CMD_92_SET_MUSIC_BRANCH_THRESHHOLD         = 0x92,
    SOUND_CMD_98_NULL                                = 0x98,
    SOUND_CMD_99_NULL                                = 0x99,
    SOUND_CMD_9A_RESTORE_MUSIC                       = 0x9A,
    SOUND_CMD_9B_SUSPEND_MUSIC                       = 0x9B,
    SOUND_CMD_9C_RESTORE_SFX                         = 0x9C,
    SOUND_CMD_9D_SUSPEND_SFX                         = 0x9D,
    SOUND_CMD_9E_RESTORE_CUTSCENE_AUDIO              = 0x9E,
    SOUND_CMD_9F_SUSPEND_CUTSCENE_AUDIO              = 0x9F,
    SOUND_CMD_A0_SET_SFX_VOLUME_MOD                  = 0xA0,
    SOUND_CMD_A1_FADE_SFX_VOLUME_MOD                 = 0xA1,
    SOUND_CMD_A2_SET_SFX_PAN_MOD                     = 0xA2,
    SOUND_CMD_A3_FADE_SFX_PAN_MOD                    = 0xA3,
    SOUND_CMD_A4_SET_SFX_PITCH_MOD                   = 0xA4,
    SOUND_CMD_A5_FADE_SFX_PITCH_MOD                  = 0xA5,
    SOUND_CMD_A8_SET_ALL_SFX_VOLUME_MOD              = 0xA8,
    SOUND_CMD_A9_FADE_ALL_SFX_VOLUME_MOD             = 0xA9,
    SOUND_CMD_AA_SET_ALL_SFX_PAN_MOD                 = 0xAA,
    SOUND_CMD_AB_FADE_ALL_SFX_PAN_MOD                = 0xAB,
    SOUND_CMD_AC_SET_ALL_SFX_PITCH_MOD               = 0xAC,
    SOUND_CMD_AD_FADE_ALL_SFX_PITCH_MOD              = 0xAD,
    SOUND_CMD_AE_MUTE_SFX                            = 0xAE,
    SOUND_CMD_AF_UNMUTE_SFX                          = 0xAF,
    SOUND_CMD_C0_SET_MASTER_VOLUME_BY_MUSIC_ID       = 0xC0,
    SOUND_CMD_C1_FADE_MASTER_VOLUME_BY_MUSIC_ID      = 0xC1,
    SOUND_CMD_C2_FADE_MASTER_VOLUME_FROM_BY_MUSIC_ID = 0xC2,
    SOUND_CMD_C4_SET_PAN_BY_MUSIC_ID                 = 0xC4,
    SOUND_CMD_C5_FADE_PAN_BY_MUSIC_ID                = 0xC5,
    SOUND_CMD_D0_SET_TEMPO_SCALE                     = 0xD0,
    SOUND_CMD_D1_FADE_TEMPO_SCALE                    = 0xD1,
    SOUND_CMD_D2_FADE_TEMPO_SCALE_FROM               = 0xD2,
    SOUND_CMD_D4_SET_MASTER_PITCH_SCALE              = 0xD4,
    SOUND_CMD_D5_FADE_MASTER_PITCH_SCALE             = 0xD5,
    SOUND_CMD_D6_FADE_MASTER_PITCH_SCALE_FROM        = 0xD6,
    SOUND_CMD_D8_UNK                                 = 0xD8,
    SOUND_CMD_D9_UNK                                 = 0xD9,
    SOUND_CMD_DA_UNK                                 = 0xDA,
    SOUND_CMD_E2_STOP_CUTSCENE_STREAM                = 0xE2,
    SOUND_CMD_E4_SET_CUTSCENE_VOLUME                 = 0xE4,
    SOUND_CMD_E5_FADE_OUT_CUTSCENE                   = 0xE5,
    SOUND_CMD_E8_UNK                                 = 0xE8,
    SOUND_CMD_F0_STOP_MUSIC                          = 0xF0,
    SOUND_CMD_F1_STOP_SFX                            = 0xF1

} ESoundCommand;

typedef void (*PSoundCommandHandler)(struct FSoundCommandParams*);

extern PSoundCommandHandler g_Sound_CommandHandlers[0x100];
extern s32 g_Sound_MutedSfxVolumes[ SOUND_SFX_CHANNEL_COUNT ]; // num == SOUND_CHANNEL_COUNT or SOUND_SFX_CHANNEL_COUNT???
extern FSoundCommandParams g_Sound_CommandContext;
extern s32 g_Sound_MutedMusicChannelMask ;
extern s32 D_80094FFC;

#endif // _SOUND_COMMAND_H
