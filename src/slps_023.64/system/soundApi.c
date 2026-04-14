#include "common.h"
#include "psyq/libcd.h"
#include "system/sound.h"
#include "system/soundCommand.h"
#include "system/soundCutscene.h"

//----------------------------------------------------------------------------------------------------------------------
s32 InitSound()
{
    Sound_Start();
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
s32 TeardownSound()
{
    Sound_Stop();
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
bool Sound_BindAkaoSfxBlob( FAkaoFileBlob* in_Blob )
{
    bool isNotAkao;
    u8* p;

    isNotAkao = Sound_IsNotAkaoFile(in_Blob);

    if( isNotAkao == 0 )
    {
        p = (u8*)in_Blob->ProgramOffsets;
        g_Sound_Sfx_ProgramOffsets = (u16*)p;

        p = (u8*)in_Blob->MetadataTableA;
        g_Sound_Sfx_MetadataTableA = (u16*)p;

        p = (u8*)in_Blob->AdditionalProgramCounts;
        g_Sound_Sfx_AdditionalProgramCounts = (u16*)p;

        p = (u8*)in_Blob->ProgramData;
        g_Sound_Sfx_ProgramData = (u8*)p;
    }

    return isNotAkao;
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_StartFieldMusic( u32 in_Unk )
{
    g_Sound_Vm2Params.Param1 = in_Unk;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_10_START_FIELD_MUSIC );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_StopMusicById( u32 arg0 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_11_STOP_MUSIC_BY_ID );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_StartBattleMusic( u32 arg0, u32 arg1 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_14_START_BATTLE_MUSIC );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_PushMusicState()
{
    Sound_ExecuteSoundVm2Function( SOUND_CMD_40_PUSH_MUSIC_STATE );
}

//----------------------------------------------------------------------------------------------------------------------
// TODO(jperos): This should be easy enough to name if I can remember what saved/pushed configs do
s32 func_8004A05C()
{
    if( !g_pSuspendedMusicContext )
    {
        g_SuspendedMusicContext.MusicId = 0;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetMusicLevelImmediate( u32 arg0, s32 arg1 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_19_SET_MUSIC_LEVEL_IMM );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_StartMasterAndMusicVolumeFade( u32 arg0, u32 arg1, s32 arg2 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1;
    g_Sound_Vm2Params.Param3 = arg2 & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_1A_START_MASTER_AND_MUSIC_VOLUME_FADE );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_StartFieldMusicLooped( u32 arg0, u32 arg1 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_12_START_FIELD_MUSIC_LOOPED );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_PlaySfx( s32 arg0, s32 arg1, s32 arg2, s32 arg3 )
{
    g_Sound_Vm2Params.Param1 = arg0 & 0x3FF;
    g_Sound_Vm2Params.Param2 = arg1 & 0xFFFFFF;
    g_Sound_Vm2Params.Param3 = arg2 & 0xFF;
    g_Sound_Vm2Params.Param4 = arg3 & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_20_PLAY_SFX );
}

//----------------------------------------------------------------------------------------------------------------------
void* Sound_PlaySfxFromPointer( void* arg0, s32 arg1, s32 arg2, s32 arg3 )
{
    if( Sound_IsNotAkaoFile( arg0 ) )
    {
        // TODO(jperos): What happens when this gets VOICE_INVALID_INDEX?
        return Sound_PlaySfxProtected( VOICE_INVALID_INDEX );
    }
    else
    {
        g_Sound_Vm2Params.Param1 = (u32)arg0;
        g_Sound_Vm2Params.Param2 = arg1 & 0xFFFFFF;
        g_Sound_Vm2Params.Param3 = arg2 & 0xFF;
        g_Sound_Vm2Params.Param4 = arg3 & 0x7F;
        Sound_ExecuteSoundVm2Function( SOUND_CMD_24_PLAY_SFX_FROM_POINTER );
        return arg0;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_EvictSfx( u32 arg0, s32 arg1 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0xFFFFFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_21_EVICT_SFX_VOICE );
}

//----------------------------------------------------------------------------------------------------------------------
void* Sound_PlaySfxProtected( s32 in_VoiceIndex )
{
    g_Sound_Vm2Params.Param1 = in_VoiceIndex & 0x3FF;
    return Sound_ExecuteSoundVm2Function( SOUND_CMD_30_PROTECTED );
}

//----------------------------------------------------------------------------------------------------------------------
s32 Sound_GetAllSfxUnkFlags()
{
    s32 out_UnkFlags;
    s32 AllVoiceMask;
    FSoundChannel *pChannel;
    s32 Mask;
  
    out_UnkFlags = g_Sound_SfxState.ActiveVoiceMask == 0;
    if( out_UnkFlags )
    {
        return 0;
    }

    pChannel = g_SfxSoundChannels;
    out_UnkFlags = 0;
    Mask = 1 << SOUND_SFX_CHANNEL_START_INDEX;
    AllVoiceMask = 0xFFFFFF;

    while( Mask & AllVoiceMask )
    {
        if( g_Sound_SfxState.ActiveVoiceMask & Mask )
        {
            out_UnkFlags |= pChannel->unk_Flags;
        }
  
        Mask <<= 1;
        pChannel++;
      
    };
    return out_UnkFlags & 0xFFFFFF;
}

//----------------------------------------------------------------------------------------------------------------------
s32 Sound_IsSfxActiveWithUnkFlags( s32 in_Flags )
{
    u32 CurrentChannelMask;
    u32 ActiveChannelMask;
    FSoundChannel* pChannel;

    if( in_Flags == 0 )
    {
        return 0;
    }

    ActiveChannelMask = g_Sound_SfxState.ActiveVoiceMask;

    if( ActiveChannelMask == 0 )
    {
        return 0;
    }

    pChannel = g_SfxSoundChannels;
    CurrentChannelMask = 1 << SOUND_SFX_CHANNEL_START_INDEX;

    do
    {
        if( ActiveChannelMask & CurrentChannelMask )
        {
            if( in_Flags == pChannel->unk_Flags )
            {
                return 1;
            }
        }
        CurrentChannelMask <<= 1;
        pChannel++;
    } while( CurrentChannelMask & VOICE_MASK_ALL );

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
s32 Sound_SetTempoMultiplier( s32 in_Mode )
{
    u32 Flags;

    Flags = g_Sound_SfxState.TempoMultiplier & ~( (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) );
    g_Sound_SfxState.TempoMultiplier = Flags;

    switch ( in_Mode )
    {
        case -2:
            g_Sound_SfxState.TempoMultiplier = Flags | (1 << 0);
            break;
        case -1:
            g_Sound_SfxState.TempoMultiplier = Flags | (1 << 1);
            break;
        case 1:
            g_Sound_SfxState.TempoMultiplier = Flags | (1 << 2);
            break;
        case 2:
            g_Sound_SfxState.TempoMultiplier = Flags | (1 << 3);
            break;
        default:
            in_Mode = 0;
            break;
    }

    return in_Mode;
}

//----------------------------------------------------------------------------------------------------------------------
// TODO(jperos): Find an appropriate public header for this
// this is the public API for the private command
typedef enum ESpeakerMode
{
    SPEAKER_MODE_STEREO,
    SPEAKER_MODE_MONO
} ESpeakerMode;

void Sound_SetSpeakerMode( s32 in_Mode )
{
    u32 OpCode = in_Mode;

    if( OpCode == SPEAKER_MODE_MONO )
    {
        OpCode = SOUND_CMD_81_SET_MODE_MONO;
    }
    else
    {
        OpCode = SOUND_CMD_80_SET_MODE_STEREO;
    }
    Sound_ExecuteSoundVm2Function( OpCode );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetMutedMusicChannelMask( u32 in_ChannelMask )
{
    g_Sound_Vm2Params.Param1 = in_ChannelMask;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_90_SET_MUTED_MUSIC_CHANNELS );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetMusicJumpThreshold( u32 arg0 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_92_SET_MUSIC_BRANCH_THRESHHOLD );
}

//----------------------------------------------------------------------------------------------------------------------
typedef enum ESoundChannelType
{
    ESoundChannelType_Music    = 1,
    ESoundChannelType_Sfx      = 2,
    ESoundChannelType_Cutscene = 3,
} ESoundChannelType;
void Sound_SuspendChannelsByType( u32 in_ChannelType )
{
    s32 OpCode;

    switch( in_ChannelType )
    {
        case ESoundChannelType_Music:     OpCode = SOUND_CMD_9B_SUSPEND_MUSIC;          break;
        case ESoundChannelType_Sfx:       OpCode = SOUND_CMD_9D_SUSPEND_SFX;            break;
        case ESoundChannelType_Cutscene:  OpCode = SOUND_CMD_9F_SUSPEND_CUTSCENE_AUDIO; break;
        default: OpCode = SOUND_CMD_99_NULL; break;
    }

    Sound_ExecuteSoundVm2Function( OpCode );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_RestoreChannelsByType( u32 in_ChannelType )
{
    s32 OpCode;

    switch( in_ChannelType )
    {
        case ESoundChannelType_Music:     OpCode = SOUND_CMD_9A_RESTORE_MUSIC;          break;
        case ESoundChannelType_Sfx:       OpCode = SOUND_CMD_9C_RESTORE_SFX;            break;
        case ESoundChannelType_Cutscene:  OpCode = SOUND_CMD_9E_RESTORE_CUTSCENE_AUDIO; break;
        default: OpCode = SOUND_CMD_98_NULL; break;
    }

    Sound_ExecuteSoundVm2Function( OpCode );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetAllSfxVolumeMod( s32 in_VolumeMod )
{
    g_Sound_Vm2Params.Param1 = in_VolumeMod & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_A8_SET_ALL_SFX_VOLUME_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeAllSfxVolumeMod( u32 arg0, s32 in_VolumeMod )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = in_VolumeMod & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_A9_FADE_ALL_SFX_VOLUME_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetSfxVolumeMod( u32 arg0, s32 in_VoiceMask, s32 in_VolumeMod )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = in_VoiceMask & VOICE_MASK_ALL;
    g_Sound_Vm2Params.Param3 = in_VolumeMod & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_A0_SET_SFX_VOLUME_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeSfxVolumeMod( u32 arg0, s32 in_VoiceMask, u32 arg2, s32 in_VolumeMod )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = in_VoiceMask & VOICE_MASK_ALL;
    g_Sound_Vm2Params.Param3 = arg2;
    g_Sound_Vm2Params.Param4 = in_VolumeMod & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_A1_FADE_SFX_VOLUME_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetAllSfxPanMod( s32 in_Target )
{
    g_Sound_Vm2Params.Param1 = in_Target & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_AA_SET_ALL_SFX_PAN_MOD );

}
//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeAllSfxPanMod( u32 in_Length, s32 in_Target )
{
    g_Sound_Vm2Params.Param1 = in_Length;
    g_Sound_Vm2Params.Param2 = in_Target & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_AB_FADE_ALL_SFX_PAN_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetSfxPanMod( u32 arg0, s32 in_VoiceMask, s32 in_Target )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = in_VoiceMask & 0xFFFFFF;
    g_Sound_Vm2Params.Param3 = in_Target & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_A2_SET_SFX_PAN_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeSfxPanMod( u32 arg0, s32 in_VoiceMask, u32 arg2, s32 in_Target )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = in_VoiceMask & 0xFFFFFF;
    g_Sound_Vm2Params.Param3 = arg2;
    g_Sound_Vm2Params.Param4 = in_Target & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_A3_FADE_SFX_PAN_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetAllSfxPitchMod( s32 in_Target )
{
    g_Sound_Vm2Params.Param1 = in_Target & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_AC_SET_ALL_SFX_PITCH_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeAllSfxPitchMod( u32 in_Length, s32 in_Target )
{
    g_Sound_Vm2Params.Param1 = in_Length;
    g_Sound_Vm2Params.Param2 = in_Target & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_AD_FADE_ALL_SFX_PITCH_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetSfxPitchMod( u32 arg0, s32 in_VoiceMask, s32 in_Target )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = in_VoiceMask & 0xFFFFFF;
    g_Sound_Vm2Params.Param3 = in_Target & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_A4_SET_SFX_PITCH_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeSfxPitchMod( u32 arg0, s32 arg1, u32 arg2, s32 arg3 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0xFFFFFF;
    g_Sound_Vm2Params.Param3 = arg2;
    g_Sound_Vm2Params.Param4 = arg3 & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_A5_FADE_SFX_PITCH_MOD );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetMasterVolumeByMusicId( u32 arg0, s32 arg1 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_C0_SET_MASTER_VOLUME_BY_MUSIC_ID );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeMasterVolumeByMusicId( u32 arg0, u32 arg1, s32 arg2 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1;
    g_Sound_Vm2Params.Param3 = arg2 & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_C1_FADE_MASTER_VOLUME_BY_MUSIC_ID );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeMasterVolumeFromByMusicId( u32 arg0, u32 arg1, s32 arg2, s32 arg3 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1;
    g_Sound_Vm2Params.Param3 = arg2 & 0x7F;
    g_Sound_Vm2Params.Param4 = arg3 & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_C2_FADE_MASTER_VOLUME_FROM_BY_MUSIC_ID );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetPanByMusicId( u32 arg0, s32 arg1 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_C4_SET_PAN_BY_MUSIC_ID );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadePanByMusicId( u32 arg0, u32 arg1, s32 arg2 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1;
    g_Sound_Vm2Params.Param3 = arg2 & 0x7F;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_C5_FADE_PAN_BY_MUSIC_ID );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetCdVolume( u32 in_TargetVolume )
{
    g_Sound_Vm2Params.Param1 = in_TargetVolume;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_70_SET_CD_VOLUME );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeCdVolume( u32 arg0, u32 arg1 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_71_FADE_CD_VOLUME );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeCdVolumeFrom( u32 arg0, u32 arg1, u32 arg2 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1;
    g_Sound_Vm2Params.Param3 = arg2;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_72_FADE_CD_VOLUME_FROM );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetTempoScale( s32 arg0 )
{
    g_Sound_Vm2Params.Param1 = arg0 & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_D0_SET_TEMPO_SCALE );
}
 
//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeTempoScale( u32 arg0, s32 arg1 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_D1_FADE_TEMPO_SCALE );
}
 
//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeTempoScaleFrom( u32 arg0, s32 arg1, s32 arg2 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0xFF;
    g_Sound_Vm2Params.Param3 = arg2 & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_D2_FADE_TEMPO_SCALE_FROM );
}
 
//----------------------------------------------------------------------------------------------------------------------
void Sound_SetMasterPitchScale( s32 arg0 )
{
    g_Sound_Vm2Params.Param1 = arg0 & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_D4_SET_MASTER_PITCH_SCALE );
}
 
//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeMasterPitchScale( u32 arg0, s32 arg1 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_D5_FADE_MASTER_PITCH_SCALE );
}
 
//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeMasterPitchScaleFrom( u32 arg0, s32 arg1, s32 arg2 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0xFF;
    g_Sound_Vm2Params.Param3 = arg2 & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_D6_FADE_MASTER_PITCH_SCALE_FROM );
}
 
//----------------------------------------------------------------------------------------------------------------------
void func_8004AAB0( s32 arg0 )
{
    g_Sound_Vm2Params.Param1 = arg0 & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_D8_UNK );
}
 
//----------------------------------------------------------------------------------------------------------------------
void func_8004AADC( u32 arg0, s32 arg1 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_D9_UNK );
}
 
//----------------------------------------------------------------------------------------------------------------------
void func_8004AB10( u32 arg0, s32 arg1, s32 arg2 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1 & 0xFF;
    g_Sound_Vm2Params.Param3 = arg2 & 0xFF;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_DA_UNK );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_StopMusic()
{
    Sound_ExecuteSoundVm2Function( SOUND_CMD_F0_STOP_MUSIC );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_StopSfx()
{
    Sound_ExecuteSoundVm2Function( SOUND_CMD_F1_STOP_SFX );
}

//----------------------------------------------------------------------------------------------------------------------
// TODO(jperos): this function and the downstream ones may take in a void* and cast it somewhere along the line
s32 func_8004AB8C( void* in_Data, s32 in_bWait )
{
    s32 bLoadSuccessful;
    do
    {
        bLoadSuccessful = Sound_TryLoadInstrumentBank( (FAkaoSequence*)in_Data, in_bWait );
    } while( bLoadSuccessful == AKAO_LOAD_RETRY );

    if( bLoadSuccessful == AKAO_LOAD_FAILURE )
    {
        // TODO(jperos): What happens in here when you play a SFX on an invalid voice index?
        Sound_PlaySfxProtected( VOICE_INVALID_INDEX );
    }

    return bLoadSuccessful;
}

//----------------------------------------------------------------------------------------------------------------------
s32 IsSpuTransferring()
{
    return g_bSpuTransferring;
}

//----------------------------------------------------------------------------------------------------------------------
s32 func_8004AC0C()
{
    g_Sound_AkaoLoadState.SpuTransferStartAddr = 0;
    g_Sound_GlobalFlags.ControlLatches |= SOUND_CTL_AKAO_TRANSFER_ACTIVE;
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
u32 Sound_StreamAkaoBankChunk( s32* in_Data, u32 in_Size, s32 in_bWait )
{
    u32 RemainingSize;
    u32 TransferSize;
    u32 WriteSize;
    u32 WordCount;
    u32 CopySize;

    RemainingSize = in_Size;

    if( g_Sound_GlobalFlags.ControlLatches & SOUND_CTL_AKAO_TRANSFER_ACTIVE )
    {
        if( g_Sound_AkaoLoadState.SpuTransferStartAddr == 0 )
        {
            if( Sound_IsNotAkaoFile( in_Data ) == false )
            {
                memcpy32( in_Data, (s32*)&g_Sound_CurrentAkaoSequence, sizeof_FAkaoSequenceHeader);
                in_Data += sizeof(FSoundInstrumentInfo);
                RemainingSize -= sizeof_FAkaoSequenceHeader;
                g_Sound_AkaoLoadState.SpuTransferStartAddr = g_Sound_CurrentAkaoSequence.InstrumentStartAddr;
                // NOTE(jperos): This is obvious compiler artifact trash
                g_Sound_CurrentAkaoSequence.InstrumentIndex = g_Sound_CurrentAkaoSequence.InstrumentIndex;
                g_Sound_AkaoLoadState.SampleBytesRemaining = g_Sound_CurrentAkaoSequence.SampleDataSize;
                g_Sound_AkaoLoadState.pInstrumentInfoCursor = &g_InstrumentInfo[ g_Sound_CurrentAkaoSequence.InstrumentIndex ];
                g_Sound_AkaoLoadState.InstrumentInfoBytesRemaining = g_Sound_CurrentAkaoSequence.InstrumentCount * sizeof(FSoundInstrumentInfo);
            }
            else
            {
                Sound_PlaySfxProtected( VOICE_INVALID_INDEX );
                RemainingSize = 0;
                g_Sound_AkaoLoadState.SampleBytesRemaining = 0;
                g_Sound_AkaoLoadState.InstrumentInfoBytesRemaining = 0;
            }
        }

        if( g_Sound_AkaoLoadState.InstrumentInfoBytesRemaining != 0 )
        {
            CopySize = g_Sound_AkaoLoadState.InstrumentInfoBytesRemaining;

            if( RemainingSize != 0 )
            {
                if( CopySize >= RemainingSize )
                {
                    CopySize = RemainingSize;
                }

                memcpy32( in_Data, g_Sound_AkaoLoadState.pInstrumentInfoCursor, CopySize );
                // HACK: preserves original scheduling for WordCount assignment
                do { WordCount = CopySize / 4; } while (0);
                in_Data += WordCount;
                g_Sound_AkaoLoadState.pInstrumentInfoCursor = (u8*)g_Sound_AkaoLoadState.pInstrumentInfoCursor + ( WordCount * 4 );
                g_Sound_AkaoLoadState.InstrumentInfoBytesRemaining -= CopySize;
                RemainingSize -= CopySize;

                if( g_Sound_AkaoLoadState.InstrumentInfoBytesRemaining == 0 )
                {
                    Sound_CopyAndRelocateInstruments(
                        &g_InstrumentInfo[g_Sound_CurrentAkaoSequence.InstrumentIndex],
                        &g_InstrumentInfo[g_Sound_CurrentAkaoSequence.InstrumentIndex],
                        (s32)g_Sound_CurrentAkaoSequence.InstrumentStartAddr,
                        (s32)g_Sound_CurrentAkaoSequence.InstrumentCount );
                }
            }
        }

        if( RemainingSize != 0 )
        {
            TransferSize = g_Sound_AkaoLoadState.SampleBytesRemaining;

            if( g_Sound_AkaoLoadState.SampleBytesRemaining != 0 )
            {
                WriteSize = TransferSize;

                if( WriteSize >= RemainingSize )
                {
                    WriteSize = RemainingSize;
                }

                RemainingSize = WriteSize;
                SpuSetTransferStartAddr( g_Sound_AkaoLoadState.SpuTransferStartAddr );
                WriteSpu( in_Data, (s32)RemainingSize );
                g_Sound_AkaoLoadState.SpuTransferStartAddr += RemainingSize;
                g_Sound_AkaoLoadState.SampleBytesRemaining -= RemainingSize;

                if( in_bWait )
                {
                    WaitForSpuTransfer();
                }
            }
        }

        if( g_Sound_AkaoLoadState.SampleBytesRemaining == 0 )
        {
            g_Sound_GlobalFlags.ControlLatches &= ~SOUND_CTL_AKAO_TRANSFER_ACTIVE;
        }
    }

    return g_Sound_AkaoLoadState.SampleBytesRemaining;
}

//----------------------------------------------------------------------------------------------------------------------
void* func_8004AE4C( void* arg0, s32 arg1, s32 arg2 )
{
    return func_8004AB8C( arg0, arg2 );
}

//----------------------------------------------------------------------------------------------------------------------
int CdMix(CdlATV *vol);

s32 func_8004AE6C( s32 arg0 )
{

    if( g_Sound_GlobalFlags.MixBehavior & MIX_MODE_MONO )
    {
        CdlATV* p = &g_Sound_CdlATV;
        u32 Val;
        Val = (u32)( arg0 * 0xB570 ) >> 0x11;
        p->val3 = (s8)Val;
        p->val1 = (s8)Val;
        p->val2 = (s8)Val;
        p->val0 = (s8)Val;
    }
    else // MIX_MODE_STEREO
    {
        CdlATV* p = &g_Sound_CdlATV;
        p->val2 = arg0;
        p->val0 = arg0;
        p->val3 = 0;
        p->val1 = 0;
    }
    CdMix( &g_Sound_CdlATV );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_StopCutsceneStream()
{
    Sound_ExecuteSoundVm2Function( SOUND_CMD_E2_STOP_CUTSCENE_STREAM );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_SetCutsceneVolume( s32 in_Volume )
{
    g_Sound_Vm2Params.Param1 = ( in_Volume & 0x7F ) << 8;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_E4_SET_CUTSCENE_VOLUME );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_FadeOutCutscene( u32 in_Length, s32 in_Volume )
{
    g_Sound_Vm2Params.Param1 = in_Length;
    g_Sound_Vm2Params.Param2 = ( in_Volume & 0x7F ) << 8;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_E5_FADE_OUT_CUTSCENE );
}

//----------------------------------------------------------------------------------------------------------------------
s32 func_8004AF88( u32 arg0, u32 arg1 )
{
    if( arg1 == 0 )
    {
        return -1;
    }

    g_Sound_Vm2Params.Param1 = arg0;
    g_Sound_Vm2Params.Param2 = arg1;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_E8_UNK );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
u32 func_8004AFC8( void )
{
    u32 temp_v1;

    g_Sound_Cutscene_StreamState.field9_0x24++;
    temp_v1 = g_Sound_Cutscene_StreamState.field14_0x38 + 1;
    g_Sound_Cutscene_StreamState.field14_0x38 = (s32)temp_v1;
    if( (u32)( g_Sound_Cutscene_StreamState.PageRingBufferSize - 1 ) < temp_v1 )
    {
        g_Sound_Cutscene_StreamState.field14_0x38 = 0;
    }
    if( ( g_Sound_Cutscene_StreamState.field2_0x8 & 0x01000000 ) && ( (u32)g_Sound_Cutscene_StreamState.field14_0x38 >= 2U ) )
    {
        Sound_Cutscene_StartStream();
    }
    return g_Sound_Cutscene_StreamState.StreamPageIndex;
}

//----------------------------------------------------------------------------------------------------------------------
u32 Sound_MuteSfx( u32 arg0 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_AE_MUTE_SFX );
    return arg0;
}

//----------------------------------------------------------------------------------------------------------------------
u32 Sound_UnmuteSfx( u32 arg0 )
{
    g_Sound_Vm2Params.Param1 = arg0;
    Sound_ExecuteSoundVm2Function( SOUND_CMD_AF_UNMUTE_SFX );
    return arg0;
}
