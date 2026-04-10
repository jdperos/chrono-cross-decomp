#include "system/spu.h"

#include "common.h"

#include "psyq/kernel.h"
#include "psyq/libspu.h"
#include "psyq/libapi.h"

#include "hw.h"

#include "system/sound.h"
#include "system/soundCommand.h"
#include "system/soundCutscene.h"

//----------------------------------------------------------------------------------------------------------------------
void Sound_CopyAndRelocateInstruments( FSoundInstrumentInfo* in_A, FSoundInstrumentInfo* in_B, s32 in_AddrOffset, s32 in_Count )
{
    do {
        in_B->StartAddr = in_A->StartAddr + in_AddrOffset;
        in_B->LoopAddr = in_A->LoopAddr + in_AddrOffset;
        *(s32*)&in_B->FineTune = *(s32*)&in_A->FineTune;
        *(s32*)&in_B->AdsrLower = *(s32*)&in_A->AdsrLower;
        in_A++;
        in_B++;
        in_Count--;
    } while( in_Count != 0 );
}

//----------------------------------------------------------------------------------------------------------------------
// NOTE(jperos): I'm beginning to think that there are different AKAO structs that all use this function...
bool Sound_IsNotAkaoFile( void* in_Blob )
{
    return ((s32*)in_Blob)[0] - AKAO_FILE_MAGIC;
}

//----------------------------------------------------------------------------------------------------------------------
void ClearSpuTransferCallback()
{
  SpuSetTransferCallback( NULL );
  g_bSpuTransferring = 0;
}

//----------------------------------------------------------------------------------------------------------------------
void SetSpuTransferCallback()
{
    g_bSpuTransferring = 1;
    SpuSetTransferCallback( &ClearSpuTransferCallback );
}

//----------------------------------------------------------------------------------------------------------------------
void WriteSpu(s32 in_Addr, s32 in_Size)
{
    g_bSpuTransferring = 1;
    SpuSetTransferCallback( &ClearSpuTransferCallback );
    SpuWrite( (u8*)in_Addr, in_Size );
}

//----------------------------------------------------------------------------------------------------------------------
void ReadSpu(s32 in_Addr, s32 in_Size)
{
    SetSpuTransferCallback();
    SpuRead( (u8*)in_Addr, in_Size );
}

//----------------------------------------------------------------------------------------------------------------------
void WaitForSpuTransfer()
{
    while (g_bSpuTransferring == 1)
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------
s32 Sound_TryLoadInstrumentBank( FAkaoSequence* in_pAkao, s32 in_bWait)
{
    if( Sound_IsNotAkaoFile( in_pAkao ) == false )
    {
        Sound_LoadInstrumentBank( in_pAkao, in_bWait, in_pAkao->unk18, in_pAkao->unk10 );
        return AKAO_LOAD_SUCCESS;
    }
    return AKAO_LOAD_FAILURE;
}

//----------------------------------------------------------------------------------------------------------------------
s32 Sound_LoadInstrumentBank( FAkaoSequence* in_Akao, s32 in_bWait, s32 in_InstrumentIndex, u32 in_StartAddr )
{
    FAkaoSequence* Sequence;
    FSoundInstrumentInfo* InstrumentInfo;
    FSoundInstrumentInfo* Addr;

    WaitForSpuTransfer();

    if( Sound_IsNotAkaoFile( in_Akao ) == false )
    {
        Sequence = in_Akao;

        SpuSetTransferStartAddr( in_StartAddr );

        in_Akao = (FAkaoSequence*)in_Akao->Payload;
        InstrumentInfo = (FSoundInstrumentInfo*)in_Akao;
        Addr = InstrumentInfo + Sequence->unk1C;
        in_Akao = (FAkaoSequence*)in_Akao->Payload;

        WriteSpu( Addr, Sequence->unk14 );
        Sound_CopyAndRelocateInstruments( InstrumentInfo, &g_InstrumentInfo[ in_InstrumentIndex ], in_StartAddr, Sequence->unk1C );

        if( in_bWait != 0 )
        {
            WaitForSpuTransfer();
        }
        return 0;
    }

    g_bSpuTransferring = -1;
    return AKAO_LOAD_FAILURE;
}

//----------------------------------------------------------------------------------------------------------------------
#ifndef NON_MATCHING
INCLUDE_ASM("asm/slps_023.64/nonmatchings/system/spu", Sound_Setup);
#else
extern struct
{
    s32 unk0;
    s32 unk4;
} D_800909F8;
extern s32 D_80090A30;
extern s16 g_Sound_MasterPitchScaleStepsRemaining;
extern s16 g_Sound_TempoScaleStepsRemaining;
extern s32 g_Sound_LfoPhase;
extern s32 g_Sound_TempoScale;
extern s32 g_Sound_MutedMusicChannelMask;
extern FSoundChannel* g_Sound_pMusicSoudChannels;

void Sound_Setup()
{
    s32 AssignedVoiceNumber;
    s32 Count;
    s32 VoiceIndex;
    u32 var_s0_3;
    FSoundChannel* pActiveMusicChannel;
    FSoundChannel* pChannel;
    u32 var_a0;
    u16 i;

    g_pActiveMusicContext = &g_PrimaryMusicContext;
    g_pSuspendedMusicContext = 0;
    g_Sound_pMusicSoudChannels = g_ActiveMusicChannels;
    g_pSecondaryMusicChannels = 0;
    g_Sound_LfoPhase = 0;
    g_Sound_GlobalFlags.ControlLatches = 0;
    g_Sound_GlobalFlags.MixBehavior = 1;
    g_Sound_SfxState.ActiveVoiceMask = 0;
    g_PrimaryMusicContext.ActiveChannelMask = 0;
    g_PrimaryMusicContext.KeyedMask = 0;
    g_PrimaryMusicContext.MusicId = 0;
    g_Sound_SfxState.SuspendedVoiceMask = 0;
    g_PrimaryMusicContext.SuspendedChannelMask = 0;
    g_Sound_SfxState.TempoMultiplier = 0;
    g_SuspendedMusicContext.MusicId = 0;
    g_SuspendedMusicContext.ActiveChannelMask = 0;
    g_PrimaryMusicContext.MasterVolume = 0x7F0000;
    g_PrimaryMusicContext.MasterPanOffset = 0x400000;
    g_CdVolume = 0x7FFF0000;
    g_Sound_MasterPitchScaleStepsRemaining = 0;
    g_Sound_MasterPitchScaleQ16_16 = 0;
    g_Sound_TempoScaleStepsRemaining = 0;
    g_Sound_TempoScale = 0;
    g_PrimaryMusicContext.MasterVolumeStepsRemaining = 0;
    g_PrimaryMusicContext.MasterPanStepsRemaining = 0;
    g_Sound_CdVolumeFadeLength = 0;
    g_Sound_SfxState.NoiseVoiceFlags = 0;
    g_PrimaryMusicContext.NoiseChannelFlags = 0;
    g_Sound_SfxState.ReverbVoiceFlags = 0;
    g_PrimaryMusicContext.ReverbChannelFlags = 0;
    g_Sound_SfxState.FmVoiceFlags = 0;
    g_PrimaryMusicContext.FmChannelFlags = 0;
    g_PrimaryMusicContext.TimerLower = 0;
    g_PrimaryMusicContext.TimerUpperCurrent = 0;
    g_PrimaryMusicContext.TimerUpper = 0;
    g_PrimaryMusicContext.TimerTopCurrent = 0;

    *SPU_MAIN_VOL_L = 0x3FFF;
    *SPU_MAIN_VOL_R = 0x3FFF;
    *CD_VOL_L = 0x7FFF;
    *CD_VOL_R = 0x7FFF;
    Count = *SPU_CTRL_REG_CPUCNT;

    g_Music_LoopCounter = 0;
    g_Sound_MutedMusicChannelMask = 0;

    D_80094FFC = 0;
    g_Sound_Cutscene_StreamState.VolFadeStepsRemaining = 0;
    g_Sound_Cutscene_StreamState.Volume = 0x7F00;
    g_Sound_VoiceModeFlags.Fm = 0;
    g_Sound_VoiceModeFlags.Noise = 0;
    g_Sound_VoiceModeFlags.Reverb = 0;
    g_Sound_MasterFadeTimer.TicksRemaining = 0;

    AssignedVoiceNumber = Count;
    *SPU_CTRL_REG_CPUCNT = ( AssignedVoiceNumber & 0xFFFA ) | 1;

    Count = 0;
    pActiveMusicChannel = &g_ActiveMusicChannels[0];

    while( ( (u32)( Count & 0xFFFF ) ) < SOUND_CHANNEL_COUNT )
    {
        Count++;
        pActiveMusicChannel->UpdateFlags = 0;
        pActiveMusicChannel->VoiceParams.AssignedVoiceNumber = VOICE_INVALID_INDEX;
        pActiveMusicChannel->Type = SOUND_CHANNEL_TYPE_MUSIC;
        pActiveMusicChannel->Priority = 0;
        pActiveMusicChannel++;
    };

    VoiceIndex = SOUND_SFX_CHANNEL_COUNT;
    pChannel = &g_SfxSoundChannels[0];

    while( ( (u32)( VoiceIndex & 0xFFFF ) ) < VOICE_COUNT )
    {
        AssignedVoiceNumber = VoiceIndex & 0xFFFF;
        VoiceIndex++;
        pChannel->UpdateFlags = 0;
        pChannel->VoiceParams.AssignedVoiceNumber = AssignedVoiceNumber;
        pChannel->Type = SOUND_CHANNEL_TYPE_SFX;
        pChannel->Priority = 0;
        pChannel->VolumeMod = 0x7F00;
        pChannel->VolumeModStepsRemaining = 0;
        pChannel->PitchModStepsRemaining = 0;
        pChannel->PitchMod = 0;
        pChannel->KeyOnVolumeSlideLength = 0;
        pChannel++;
    };

    g_pActiveMusicContext->PendingKeyOffMask = 0;
    g_pActiveMusicContext->ActiveNoteMask = 0;
    g_pActiveMusicContext->PendingKeyOnMask = 0;
    g_Sound_SfxState.TempoAccumulator = 1;
    g_Sound_SfxState.TempoBase = 0x66A80000;
    g_Sound_SfxState.KeyOffFlags = 0;
    g_Sound_SfxState.KeyedFlags = 0;
    g_Sound_SfxState.KeyOnFlags = 0;
    g_pActiveMusicContext->RevDepth = 0x03FFF000;
    g_pActiveMusicContext->ReverbDepthSlideStep = 0;
    g_pActiveMusicContext->ReverbDepthSlideLength = 0;
    g_Sound_GlobalFlags.UpdateFlags |= 0x80;

    Sound_SetReverbMode( SPU_REV_MODE_STUDIO_C );
    SpuSetReverb( SPU_ON );
    var_s0_3 = 0;
    var_a0 = 0 & 0xFFFF;

    for( i = 0; i < VOICE_COUNT; i++ )
    {
        SetVoiceRepeatAddr( i, 0x1030U );
    };

    D_800909F8.unk4 = 0;
    D_800909F8.unk0 = 0;
    D_80090A30 = 0;
}
#endif

//----------------------------------------------------------------------------------------------------------------------
void Sound_Start()
{
    s32 temp_v0;

    SpuStart();
    SpuInitMalloc( SPU_MALLOC_NUM_BLOCKS, g_SpuMallocRecTable );
    SpuSetTransferMode( SPU_TRANSFER_BY_DMA );
    SpuSetTransferStartAddr( SPU_WAVEFORM_DATA_START );
    WriteSpu( (s32)g_Sound_NullWaveformBuf, SOUND_NULL_WAVEFORM_BUF_SIZE );
    WaitForSpuTransfer();
    Sound_Setup();
    SpuSetIRQ( SPU_OFF );
    SpuSetIRQCallback( NULL );

    do {
    } while( SetRCnt( RCntCNT2, SOUND_TIMER_TARGET, RCntMdINTR ) == 0 );

    do {
    } while( StartRCnt( RCntCNT2 ) == 0 );

    do {
        temp_v0 = OpenEvent( RCntCNT2, EvSpINT, EvMdINTR, Sound_MainLoop );
        g_Sound_EventDescriptor = temp_v0;
    } while( temp_v0 == -1 );

    do {
    } while( EnableEvent(g_Sound_EventDescriptor) == 0 );
}

//----------------------------------------------------------------------------------------------------------------------
void Sound_Stop()
{
    if( g_bSpuTransferring == true )
    {
        WriteSpu( (s32) g_Sound_NullWaveformBuf, SOUND_NULL_WAVEFORM_BUF_SIZE );
        WaitForSpuTransfer();
    }

    do {
    } while( StopRCnt( RCntCNT2 ) == 0 );

    UnDeliverEvent( RCntCNT2, EvSpINT );

    do {
    } while( DisableEvent( g_Sound_EventDescriptor ) == 0 );

    do {
    } while( CloseEvent( g_Sound_EventDescriptor ) == 0 );

    SetVoiceKeyOff( VOICE_MASK_ALL );
    SpuQuit();
}
