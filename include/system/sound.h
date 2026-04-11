#ifndef _SOUND_H
#define _SOUND_H

#include "common.h"
#include "psyq/libcd.h"
#include "psyq/libspu.h"

#define VOICE_COUNT 24
#define VOICE_INVALID_INDEX VOICE_COUNT
#define VOICE_MASK_ALL ( 0xFFFFFFU )

#define SOUND_CHANNEL_COUNT (0x20)
#define SOUND_LFO_COUNT     (0x10)

#define SOUND_SFX_CHANNEL_COUNT       (12) // Number of FSoundChannels assigned to SFX
#define SOUND_SFX_CHANNEL_START_INDEX (VOICE_COUNT - SOUND_SFX_CHANNEL_COUNT) // Starting voice in th SPU from which SFX can be assigned
#define SOUND_SFX_CHANNEL_LAST_INDEX  (VOICE_COUNT) // Last voice in th SPU from which SFX can be assigned

#define MUSIC_ID_ANY (0)

typedef enum EVoiceParamFlags
{
    VOICE_PARAM_VOLUME_L      = 1U <<  0, /* volume (left) */
    VOICE_PARAM_VOLUME_R      = 1U <<  1, /* volume (right) */
    VOICE_PARAM_VOLUME        = VOICE_PARAM_VOLUME_L | VOICE_PARAM_VOLUME_R,
    VOICE_PARAM_VOLMODE_L     = 1U <<  2, /* volume mode (left) */
    VOICE_PARAM_VOLMODE_R     = 1U <<  3, /* volume mode (right) */
    VOICE_PARAM_SAMPLE_RATE   = 1U <<  4, /* tone (pitch setting) */
    VOICE_PARAM_NOTE          = 1U <<  5, /* tone (note setting) */
    VOICE_PARAM_SAMPLE_NOTE   = 1U <<  6, /* waveform data sample note */
    VOICE_PARAM_START_ADDR    = 1U <<  7, /* waveform data start address */
    VOICE_PARAM_ADSR_AMODE    = 1U <<  8, /* ADSR Attack rate mode */
    VOICE_PARAM_ADSR_SMODE    = 1U <<  9, /* ADSR Sustain rate mode */
    VOICE_PARAM_ADSR_RMODE    = 1U << 10, /* ADSR Release rate mode */
    VOICE_PARAM_ADSR_AR       = 1U << 11, /* ADSR Attack rate */
    VOICE_PARAM_ADSR_DR       = 1U << 12, /* ADSR Decay rate */
    VOICE_PARAM_ADSR_SR       = 1U << 13, /* ADSR Sustain rate */
    VOICE_PARAM_ADSR_RR       = 1U << 14, /* ADSR Release rate */
    VOICE_PARAM_ADSR_SL       = 1U << 15, /* ADSR Sustain level */
    VOICE_PARAM_ADSR_LOWER    = VOICE_PARAM_ADSR_AMODE | VOICE_PARAM_ADSR_AR | VOICE_PARAM_ADSR_DR | VOICE_PARAM_ADSR_SL,
    VOICE_PARAM_ADSR_UPPER    = VOICE_PARAM_ADSR_SMODE | VOICE_PARAM_ADSR_RMODE | VOICE_PARAM_ADSR_SR | VOICE_PARAM_ADSR_RR,
    VOICE_PARAM_ADSR_FULL     = VOICE_PARAM_ADSR_LOWER | VOICE_PARAM_ADSR_UPPER,
    VOICE_PARAM_LOOP_ADDR     = 1U << 16, /* start address for loop */
    VOICE_PARAM_ADSR_ADSR1    = 1U << 17, /* ADSR adsr1 for `VagAtr' */
    VOICE_PARAM_ADSR_ADSR2    = 1U << 18, /* ADSR adsr2 for `VagAtr' */
} EVoiceParamFlags;

typedef enum ESoundChannelTypes
{
    SOUND_CHANNEL_TYPE_MUSIC = 0,
    SOUND_CHANNEL_TYPE_SFX   = 1,
    SOUND_CHANNEL_TYPE_MENU  = 2
} ESoundChannelTypes;

typedef enum ESoundChannelConfigs
{
    SOUND_CHANNEL_CONFIG_STEREO          = 1U << 0,
    SOUND_CHANNEL_CONFIG_MONO            = 1U << 1,
    SOUND_CHANNEL_CONFIG_STEREO_CHANNELS = 1U << 2
} ESoundChannelConfigs;

typedef enum ESoundArticulationFlags
{
    SOUND_ARTICULATION_LEGATO = 1 << 0, // Slur the next note
    SOUND_ARTICULATION_TIED   = 1 << 1, // This note was slurred from the last
    SOUND_ARTICULATION_TENUTO = 1 << 2  // This note plays its full length without the breath length in it
} ESoundArticulationFlags;

typedef enum ESoundUpdateFlags
{
    SOUND_CHANNEL_UPDATE_VIBRATO           = 1U <<  0,
    SOUND_CHANNEL_UPDATE_TREMOLO           = 1U <<  1,
    SOUND_CHANNEL_UPDATE_PAN_LFO           = 1U <<  2,
    SOUND_CHANNEL_UPDATE_DRUM_MODE         = 1U <<  3,
    SOUND_CHANNEL_UPDATE_SIDE_CHAIN_PITCH  = 1U <<  4,
    SOUND_CHANNEL_UPDATE_SIDE_CHAIN_VOL    = 1U <<  5,
    SOUND_CHANNEL_UPDATE_TENUTO            = 1U <<  6,
    SOUND_CHANNEL_UPDATE_OVERLAY           = 1U <<  8,
    SOUND_CHANNEL_UPDATE_ALTERNATIVE       = 1U <<  9,
    SOUND_CHANNEL_UPDATE_UNKNOWN_12        = 1U << 12,
    SOUND_CHANNEL_UPDATE_STEREO_LINKED     = 1U << 16, // I do not think this means what I think it means
    SOUND_CHANNEL_UPDATE_UNKNOWN_17        = 1U << 17,
    SOUND_CHANNEL_UPDATE_VOICE_ACTIVE      = 1U << 20,  // Voice is actively processing  
    SOUND_CHANNEL_UPDATE_PENDING_RELEASE   = 1U << 21,  // Voice marked for release
    SOUND_CHANNEL_UPDATE_LOCK_ATTACK_RATE  = 1U << 24,
    SOUND_CHANNEL_UPDATE_LOCK_SUSTAIN_RATE = 1U << 27,
    SOUND_CHANNEL_UPDATE_LOCK_RELEASE_RATE = 1U << 28
} ESoundUpdateFlags;


#define SOUND_CHANNEL_UNK_FLAGS_25 (1 << 25)


typedef enum ESoundGlobalUpdateFlags
{
    SOUND_GLOBAL_UPDATE_00              = 1U <<  0,
    SOUND_GLOBAL_UPDATE_01              = 1U <<  1,
    SOUND_GLOBAL_UPDATE_02              = 1U <<  2,
    SOUND_GLOBAL_UPDATE_03              = 1U <<  3,
    SOUND_GLOBAL_UPDATE_NOISE_CLOCK     = 1U <<  4,
    SOUND_GLOBAL_UPDATE_05              = 1U <<  5,
    SOUND_GLOBAL_UPDATE_06              = 1U <<  6,
    SOUND_GLOBAL_UPDATE_REVERB_DEPTH    = 1U <<  7,
    SOUND_GLOBAL_UPDATE_VOICE_MODES     = 1U <<  8,
    SOUND_GLOBAL_UPDATE_09              = 1U <<  9,
    SOUND_GLOBAL_UPDATE_10              = 1U << 10,
    SOUND_GLOBAL_UPDATE_11              = 1U << 11,
    SOUND_GLOBAL_UPDATE_12              = 1U << 12,
    SOUND_GLOBAL_UPDATE_13              = 1U << 13,
    SOUND_GLOBAL_UPDATE_14              = 1U << 14,
    SOUND_GLOBAL_UPDATE_15              = 1U << 15,
    SOUND_GLOBAL_UPDATE_16              = 1U << 16,
    SOUND_GLOBAL_UPDATE_17              = 1U << 17,
    SOUND_GLOBAL_UPDATE_18              = 1U << 18,
    SOUND_GLOBAL_UPDATE_19              = 1U << 19,
    SOUND_GLOBAL_UPDATE_20              = 1U << 20,
    SOUND_GLOBAL_UPDATE_21              = 1U << 21,
    SOUND_GLOBAL_UPDATE_22              = 1U << 22,
    SOUND_GLOBAL_UPDATE_23              = 1U << 23,
    SOUND_GLOBAL_UPDATE_24              = 1U << 24,
    SOUND_GLOBAL_UPDATE_25              = 1U << 25,
    SOUND_GLOBAL_UPDATE_26              = 1U << 26,
    SOUND_GLOBAL_UPDATE_27              = 1U << 27,
    SOUND_GLOBAL_UPDATE_28              = 1U << 28,
    SOUND_GLOBAL_UPDATE_29              = 1U << 29,
    SOUND_GLOBAL_UPDATE_30              = 1U << 30,
    SOUND_GLOBAL_UPDATE_31              = 1U << 31,
    SOUND_GLOBAL_UPDATE_32              = 1U << 32,
} ESoundGlobalUpdateFlags;

#define BIT_MASK(width)               ((1U << (width)) - 1U)
#define FIELD_MASK(width, shift)      (BIT_MASK(width) << (shift))

/*
  ____lower 16bit (at 1F801C08h+N*10h)___________________________________
  15    Attack Mode       (0=Linear, 1=Exponential)
  -     Attack Direction  (Fixed, always Increase) (until Level 7FFFh)
  14-10 Attack Shift      (0..1Fh = Fast..Slow)
  9-8   Attack Step       (0..3 = "+7,+6,+5,+4")
  -     Decay Mode        (Fixed, always Exponential)
  -     Decay Direction   (Fixed, always Decrease) (until Sustain Level)
  7-4   Decay Shift       (0..0Fh = Fast..Slow)
  -     Decay Step        (Fixed, always "-8")
  3-0   Sustain Level     (0..0Fh)  ;Level=(N+1)*800h
 */
#define SOUND_ADSR_ATTACK_MODE_MASK    (BIT_MASK(SOUND_ADSR_ATTACK_MODE_WIDTH) << SOUND_ADSR_ATTACK_MODE_SHIFT) // 0x8000  // bit 15
#define SOUND_ADSR_ATTACK_RATE_MASK    (BIT_MASK(SOUND_ADSR_ATTACK_RATE_WIDTH) << SOUND_ADSR_ATTACK_RATE_SHIFT) // 0x7F00  // bits 14–8
#define SOUND_ADSR_DECAY_RATE_MASK     (BIT_MASK(SOUND_ADSR_DECAY_RATE_WIDTH ) << SOUND_ADSR_DECAY_RATE_SHIFT ) // 0x00F0  // bits 7–4
#define SOUND_ADSR_SUS_LEVEL_MASK      (BIT_MASK(SOUND_ADSR_SUS_LEVEL_WIDTH  ) << SOUND_ADSR_SUS_LEVEL_SHIFT  ) // 0x000F  // bits 3–0

#define SOUND_ADSR_ATTACK_MODE_SHIFT   15
#define SOUND_ADSR_ATTACK_RATE_SHIFT    8
#define SOUND_ADSR_DECAY_RATE_SHIFT     4
#define SOUND_ADSR_SUS_LEVEL_SHIFT      0

#define SOUND_ADSR_ATTACK_MODE_WIDTH    1
#define SOUND_ADSR_ATTACK_RATE_WIDTH    7
#define SOUND_ADSR_DECAY_RATE_WIDTH     4
#define SOUND_ADSR_SUS_LEVEL_WIDTH      4

/*
  ____lower 16bit (at 1F801C08h+N*10h)___________________________________
  15    Attack Mode       (0=Linear, 1=Exponential)
  -     Attack Direction  (Fixed, always Increase) (until Level 7FFFh)
  14-10 Attack Shift      (0..1Fh = Fast..Slow)
  9-8   Attack Step       (0..3 = "+7,+6,+5,+4")
  -     Decay Mode        (Fixed, always Exponential)
  -     Decay Direction   (Fixed, always Decrease) (until Sustain Level)
  7-4   Decay Shift       (0..0Fh = Fast..Slow)
  -     Decay Step        (Fixed, always "-8")
  3-0   Sustain Level     (0..0Fh)  ;Level=(N+1)*800h
  ____upper 16bit (at 1F801C0Ah+N*10h)___________________________________
  31    Sustain Mode      (0=Linear, 1=Exponential)
  30    Sustain Direction (0=Increase, 1=Decrease) (until Key OFF flag)
  29    Not used?         (should be zero)
  28-24 Sustain Shift     (0..1Fh = Fast..Slow)
  23-22 Sustain Step      (0..3 = "+7,+6,+5,+4" or "-8,-7,-6,-5") (inc/dec)
  21    Release Mode      (0=Linear, 1=Exponential)
  -     Release Direction (Fixed, always Decrease) (until Level 0000h)
  20-16 Release Shift     (0..1Fh = Fast..Slow)
  -     Release Step      (Fixed, always "-8")
 */
#define SOUND_ADSR_SUS_MODE_MASK     (BIT_MASK(SOUND_ADSR_SUS_MODE_WIDTH    ) << SOUND_ADSR_SUS_MODE_SHIFT    ) //   0x8000
#define SOUND_ADSR_SUS_DIR_MASK      (BIT_MASK(SOUND_ADSR_SUS_DIR_WIDTH     ) << SOUND_ADSR_SUS_DIR_SHIFT     ) //   0x4000
#define SOUND_ADSR_UNKNOWN_MASK      (BIT_MASK(SOUND_ADSR_UNKNOWN_WIDTH     ) << SOUND_ADSR_UNKNOWN_SHIFT     ) //   0x2000
#define SOUND_ADSR_SUS_RATE_MASK     (BIT_MASK(SOUND_ADSR_SUS_RATE_WIDTH    ) << SOUND_ADSR_SUS_RATE_SHIFT    ) //   0x1FC0
#define SOUND_ADSR_RELEASE_MODE_MASK (BIT_MASK(SOUND_ADSR_RELEASE_MODE_WIDTH) << SOUND_ADSR_RELEASE_MODE_SHIFT) //   0x0020
#define SOUND_ADSR_RELEASE_RATE_MASK (BIT_MASK(SOUND_ADSR_RELEASE_RATE_WIDTH) << SOUND_ADSR_RELEASE_RATE_SHIFT) //   0x001F

#define SOUND_ADSR_SUS_MODE_SHIFT      15
#define SOUND_ADSR_SUS_DIR_SHIFT       14
#define SOUND_ADSR_UNKNOWN_SHIFT       13
#define SOUND_ADSR_SUS_RATE_SHIFT       6
#define SOUND_ADSR_RELEASE_MODE_SHIFT   5
#define SOUND_ADSR_RELEASE_RATE_SHIFT   0

#define SOUND_ADSR_SUS_MODE_WIDTH       1
#define SOUND_ADSR_SUS_DIR_WIDTH        1
#define SOUND_ADSR_UNKNOWN_WIDTH        1
#define SOUND_ADSR_SUS_RATE_WIDTH       7
#define SOUND_ADSR_RELEASE_MODE_WIDTH   1
#define SOUND_ADSR_RELEASE_RATE_WIDTH   5

#define SOUND_AMODE_1                   1
#define SOUND_AMODE_5                   5
#define SOUND_RMODE_7                   7

#define VIBRATO_FLAG_ABSOLUTE         ( 1 << 15 )

#define SOUND_CHANNEL_UPDATE_NOISE_CLOCK 0x10
#define SOUND_CHANNEL_UPDATE_REVERB      0x80

#define AKAO_FILE_MAGIC                    (0x4F414B41U) // AKAO in ASCII


typedef enum ESustainMode
{
    SUS_MODE_LIN = 0,
    SUS_MODE_EXP = 1,
} ESustainMode;

typedef enum ESustainDir
{
    SUS_DIR_INC = 0,
    SUS_DIR_DEC = 1,
} ESustainDir;

typedef enum ESustainUnk
{
    SUS_UNK_0 = 0,
    SUS_UNK_1 = 1,
} ESustainUnk;

typedef enum ESustainModeShifts
{
    SUS_MODE_SHIFT_UNK  = 0,
    SUS_MODE_SHIFT_DIR  = 1,
    SUS_MODE_SHIFT_MODE = 2,
} ESustainModeShifts;

#define SUSTAIN_MODE_CODE(Mode, Dir, Unk) \
    ((Mode << SUS_MODE_SHIFT_MODE) | (Dir << SUS_MODE_SHIFT_DIR) | (Unk << SUS_MODE_SHIFT_UNK))

typedef enum ESustainModeCode
{
    SUS_CODE_LINEAR_INCREASE       = SUSTAIN_MODE_CODE(SUS_MODE_LIN, SUS_DIR_INC, SUS_UNK_1),  // 1
    SUS_CODE_LINEAR_DECREASE       = SUSTAIN_MODE_CODE(SUS_MODE_LIN, SUS_DIR_DEC, SUS_UNK_1),  // 3
    SUS_CODE_EXPONENTIAL_INCREASE  = SUSTAIN_MODE_CODE(SUS_MODE_EXP, SUS_DIR_INC, SUS_UNK_1),  // 5
    SUS_CODE_EXPONENTIAL_DECREASE  = SUSTAIN_MODE_CODE(SUS_MODE_EXP, SUS_DIR_DEC, SUS_UNK_1),  // 7
} ESustainModeCode;

typedef enum EPanMode {
    PAN_MODE_STEREO = 0,  // use PanPosition field with gain table
    PAN_MODE_LEFT   = 1,
    PAN_MODE_RIGHT  = 2,
    PAN_MODE_CENTER = 3,
} EPanMode;

#define AKAO_NUM_PROGRAMS (0x100)
typedef struct FAkaoFileBlob
{
    /* 0x000 */ s32  Magic;                                      // AKAO
    /* 0x004 */ u8   unk_0x4[0xC];                               // Padding? Version? Counts? Music Akao blobs have a different flag in here...
    /* 0x010 */ u16  ProgramOffsets[2][AKAO_NUM_PROGRAMS];       // Offsets into bytecode - indexed by Sfx ID
    /* 0x410 */ u16  MetadataTableA[AKAO_NUM_PROGRAMS];          // Some per sfx table
    /* 0x610 */ u16  AdditionalProgramCounts[AKAO_NUM_PROGRAMS]; // Per Sfx, how many additional programs to trigger after the given Sfx index
    /* 0x810 */ u8   ProgramData[1];                             // Sfx bytecode
} FAkaoFileBlob;
static_assert( offsetof(FAkaoFileBlob, ProgramOffsets) == 0x010 );
static_assert( offsetof(FAkaoFileBlob, MetadataTableA) == 0x410 );
static_assert( offsetof(FAkaoFileBlob, AdditionalProgramCounts) == 0x610 );
static_assert( offsetof(FAkaoFileBlob, ProgramData   ) == 0x810 );
static_assert( sizeof(FAkaoFileBlob) - align(sizeof(member_type(FAkaoFileBlob, ProgramData))) == 0x810 );

// This is very strange because this seems to be an enum stored in FSoundGlobalFlags::MixBehavior
// But it mixes an enum for the low 2 bits with a flag at bit 8.
// Maybe they are both u8?
typedef enum EMixMode
{
    MIX_MODE_STEREO         = 1,       /* written at init; not meaningfully read in observed paths - likely historical or reserved */
    MIX_MODE_MONO           = 2,       /* force dual-mono: bypasses pan law, L=R via 0x440a scale; affects music, SFX, and CD audio */
    MIX_FLAG_MASTER_FADING  = 1U << 8  /* secondary music fade/teardown in progress; enables periodic fade processing, cleared when channels die */
} EMixMode;

typedef enum EControlLatches
{
    SOUND_CTL_INSTRUMENT_TRANSFER_ACTIVE    = 1U <<  0, /* SPU instrument upload/relocation in progress; set before transfer, cleared on finish, gates unk_Spu_8004ac2c() */
    SOUND_CTL_SFX_FADE_END_CALLBACK_PENDING = 1U << 16, /* set when global SFX fade starts; triggers Sound_Cmd_9D when last channel VolumeModStepsRemaining hits 0 */
} EControlLatches;

typedef struct
{
    u32 ControlLatches;   // one-shot / transactional engine state flags
    u32 MixBehavior;      // global mixing & music-stack behavior flags (see EMixMode's note)
    u32 UpdateFlags;      // deferred SPU / voice-mode update flags
} FSoundGlobalFlags;
static_assert( sizeof(FSoundGlobalFlags) == 0xC );

typedef struct
{
    /* 0x00 */ u32 StartAddr;
    /* 0x04 */ u32 LoopAddr;
    /* 0x08 */ s16 FineTune;
    /* 0x0A */ s16 SampleNote;
    /* 0x0C */ u16 AdsrLower;
    /* 0x0E */ u16 AdsrUpper;
} FSoundInstrumentInfo; /* size 0x10 */
static_assert( sizeof(FSoundInstrumentInfo) == 0x10 );

// NOTE(jperos): I am beginning to think that this might be the SFX analogue to FSoundMusicContext
typedef struct FSoundSfxState
{
    /* 0x00 */ u32 ActiveVoiceMask;
    /* 0x04 */ u32 KeyOnFlags;
    /* 0x08 */ u32 KeyedFlags;
    /* 0x0C */ u32 KeyOffFlags;
    /* 0x10 */ undefined4 SuspendedVoiceMask;
    /* 0x14 */ undefined4 TempoBase;
    /* 0x18 */ undefined4 TempoAccumulator;
    /* 0x1C */ undefined4 NoiseVoiceFlags;
    /* 0x20 */ undefined4 ReverbVoiceFlags;
    /* 0x24 */ undefined4 FmVoiceFlags;
    /* 0x28 */ u16 NoiseClock;
    /* 0x2A */ undefined field11_0x2a;
    /* 0x2B */ undefined field12_0x2b;
    /* 0x2C */ undefined4 TempoMultiplier;
} FSoundSfxState; /* size 0x30 */
static_assert( sizeof(FSoundSfxState) == 0x30 );

typedef struct
{
    /* 0x00 */ u32 AssignedVoiceNumber;
    /* 0x04 */ u32 VoiceParamFlags;
    /* 0x08 */ u32 StartAddress;
    /* 0x0C */ u32 LoopAddress;
    /* 0x10 */ u16 SampleRate;
    /* 0x12 */ u16 AdsrLower;
    /* 0x14 */ u16 AdsrUpper;
    /* 0x16 */ u16 VolumeScale;
    /* 0x18 */ SpuVolume Volume;
} FSoundVoiceParams; /* size 0x1C */
static_assert( sizeof(FSoundVoiceParams) == 0x1C );

#define SOUND_LOOP_STACK_SIZE (4)
#define SOUND_LOOP_STACK_MAX_INDEX (SOUND_LOOP_STACK_SIZE - 1)

typedef struct FSoundChannel
{
    /* 0x000 */ u8*  ProgramCounter;
    /* 0x004 */ u8*  LoopStartPc[SOUND_LOOP_STACK_SIZE];
    /* 0x014 */ u8*  ReturnProgramCounter;
    /* 0x018 */ u8*  Keymap;
    /* 0x01C */ s16* VibratoWave;
    /* 0x020 */ s16* TremeloWave;
    /* 0x024 */ s16* AutoPanWave;
    /* 0x028 */ s32  unk_Flags;
    /* 0x02C */ s32  PitchBase;
    /* 0x030 */ s32  PitchSlide;
    /* 0x034 */ s32  UpdateFlags;
    /* 0x038 */ u32  VibratoRatePhase;
    /* 0x03C */ s32  VibratoRateSlideStep;
    /* 0x040 */ u32  TremeloRatePhase;
    /* 0x044 */ s32  TremeloRateSlideStep;
    /* 0x048 */ u32  AutoPanRatePhase;
    /* 0x04C */ s32  AutoPanRateSlideStep;
    /* 0x050 */ s32  AkaoProgramIndex;
    /* 0x054 */ u32  PitchMod;
    /* 0x058 */ s32  PitchModStep;
    /* 0x05C */ s32  Volume;
    /* 0x060 */ s32  VolumeSlideStep;
    /* 0x064 */ s32  PitchSlideStep;
    /* 0x068 */ u32  FinePitchDelta;
    /* 0x06C */ s32  Priority;
    /* 0x070 */ s32  KeyOnVolume;
    /* 0x074 */ s32  KeyOnVolumeSlideStep;
    /* 0x078 */ u16  Type; /* Music, SFX, Menu */
    /* 0x07A */ u16  NoteLength; /* Full given length of the note */
    /* 0x07C */ u16  KeyLength;  /* Length of the key on portion, minus the "breath" portion at the end of the note */
    /* 0x07E */ u16  InstrumentIndex;
    /* 0x080 */ u16  PanMod;
    /* 0x082 */ u16  PanModStepsRemaining;
    /* 0x084 */ u16  OpcodeStepCounter;
    /* 0x086 */ u16  LoopIterationCount[SOUND_LOOP_STACK_SIZE];
    /* 0x08E */ s16  LoopStepCounterSnapshot[SOUND_LOOP_STACK_SIZE];
    /* 0x096 */ u16  VolumeBalance; /* Volume is set by "volume << 8" */
    /* 0x098 */ u16  VolumeBalanceSlideLength;
    /* 0x09A */ u16  PitchModStepsRemaining;
    /* 0x09C */ u16  ChannelVolumeSlideLength;
    /* 0x09E */ u16  KeyOnVolumeSlideLength;
    /* 0x0A0 */ u16  VolumeModStepsRemaining;
    /* 0x0A2 */ u16  ChannelPan;
    /* 0x0A4 */ u16  ChannelPanSlideLength;
    /* 0x0A6 */ u16  PitchSlideStepsCurrent;
    /* 0x0A8 */ u16  Octave;
    /* 0x0AA */ s16  PitchBendSlideLength;
    /* 0x0AC */ u16  KeyStored;
    /* 0x0AE */ u16  PortamentoSteps;
    /* 0x0B0 */ u16  Articulation;
    /* 0x0B2 */ s16  VibratoDelay;
    /* 0x0B4 */ u16  VibratoDelayCurrent;
    /* 0x0B6 */ u16  VibratoRateSlideLength;
    /* 0x0B8 */ u16  VibratoRateCurrent;
    /* 0x0BA */ u16  VibratoType;
    /* 0x0BC */ u16  VibratoBase;
    /* 0x0BE */ u16  VibratoDepth;
    /* 0x0C0 */ u16  VibratoDepthSlideLength;
    /* 0x0C2 */ u16  VibratoDepthSlideStep;
    /* 0x0C4 */ s16  TremeloDelay;
    /* 0x0C6 */ u16  TremeloDelayCurrent;
    /* 0x0C8 */ u16  TremeloRateSlideLength;
    /* 0x0CA */ s16  TremeloRateCurrent;
    /* 0x0CC */ u16  TremeloType;
    /* 0x0CE */ u16  TremeloDepth;
    /* 0x0D0 */ u16  TremeloDepthSlideLength;
    /* 0x0D2 */ s16  TremeloDepthSlideStep;
    /* 0x0D4 */ u16  AutoPanRateSlideLength;
    /* 0x0D6 */ u16  AutoPanRateCurrent;
    /* 0x0D8 */ u16  AutoPanType;
    /* 0x0DA */ u16  AutoPanDepth;
    /* 0x0DC */ u16  AutoPanDepthSlideLength;
    /* 0x0DE */ s16  AutoPanDepthSlideStep;
    /* 0x0E0 */ u16  NoiseTimer;
    /* 0x0E2 */ u16  FmTimer;
    /* 0x0E4 */ u16  LoopStackTop;
    /* 0x0E6 */ u16  RandomPitchDepth;
    /* 0x0E8 */ s16  LengthStored;
    /* 0x0EA */ s16  FixedNoteLength;
    /* 0x0EC */ s16  VolumeBalanceSlideStep;
    /* 0x0EE */ s16  PanModStep;
    /* 0x0F0 */ s16  VolumeMod;
    /* 0x0F2 */ s16  VolumeModStep;
    /* 0x0F4 */ s16  PanSlideStep;
    /* 0x0F6 */ s16  Transpose;
    /* 0x0F8 */ s16  FineTune;
    /* 0x0FA */ s16  Key;
    /* 0x0FC */ s16  PitchBendSlideTranspose;
    /* 0x0FE */ s16  TransposeStored;
    /* 0x100 */ s16  VibratoPitch;
    /* 0x102 */ s16  TremeloVolume;
    /* 0x104 */ s16  AutoPanVolume;
    /* 0x106 */ u8   field112_0x106;
    /* 0x107 */ u8   field113_0x107;
    /* 0x108 */ FSoundVoiceParams VoiceParams;
} FSoundChannel; /* size 0x124 */
static_assert( sizeof(FSoundChannel) == 0x124 );

#define KEYMAP_ENTRY_REV_MASK    (BIT_MASK(1) << 7)
#define KEYMAP_ENTRY_PAN_MASK    (BIT_MASK(7) << 0)
#define KEYMAP_ENTRY_REV_ENABLE  ((1 << 7) & KEYMAP_ENTRY_REV_MASK )
typedef struct
{
    /* 0x0 */ u8 InstrumentIndex;
    /* 0x1 */ u8 Note;
    /* 0x2 */ u8 AdsrAttackRate;
    /* 0x3 */ u8 AdsrSustainRate;
    /* 0x4 */ u8 SustainModeCode; /* values 3/5/7 map to 0x4000/0x8000/0xC000 */
    /* 0x5 */ u8 ReleaseRate;
    /* 0x6 */ u8 VolumeScale;
    /* 0x7 */ u8 PanAndReverb; /* low 7 bits pan, high bit reverb-enable */
} FSoundKeymapEntry8;
static_assert( sizeof(FSoundKeymapEntry8) == 0x8 );

typedef struct
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ u32 unk04;
    /* 0x08 */ u32 unk08;
    /* 0x0C */ u32 unk0C;

    /* 0x10 */ u32 unk10;
    /* 0x14 */ u32 unk14;
    /* 0x18 */ u32 unk18;
    /* 0x1C */ u32 unk1C;

    /* 0x20 */ u32 ChannelEnableMask;
    /* 0x24 */ u32 KeyedMask;
    /* 0x28 */ u32 AllocatedVoiceMask;
    /* 0x2C */ u32 unk2C;

    /* 0x30 */ s32 PatchRegionOffset;   // field-relative
    /* 0x34 */ s32 KeymapRegionOffset;  // field-relative
    /* 0x38 */ u32 unk38;
    /* 0x3C */ u32 unk3C;
    /* 0x40 */ u8  Payload[1];  // starts at 0x40 (variable length)
} FAkaoSequence; // size 0x40 (header), data blob variable
static_assert( sizeof(FAkaoSequence) - align(sizeof(member_type(FAkaoSequence,Payload))) == 0x40 );

typedef enum EAkaoLoadStatus
{
    AKAO_LOAD_SUCCESS  =  0,
    AKAO_LOAD_RETRY    =  1,
    AKAO_LOAD_FAILURE  = -1,
} EAkaoLoadStatus;


typedef enum EMusicContextStatusFlags
{
    VOICE_ALLOC_FLAG_EXHAUSTED      = 1U << 0, // No voice available even after stealing
    VOICE_ALLOC_FLAG_STOLE          = 1U << 1, // Voice stealing occurred
    SOUND_BANK_FLAG_ALT_SAMPLE_BANK = 1U << 5  // 0x20 toggles whether we use the alternate sample bank
} EMusicContextStatusFlags;

typedef struct 
{
    /* 0x00 */ u32 StatusFlags;
    /* 0x04 */ u32 ActiveChannelMask;
    /* 0x08 */ u32 KeyedMask; /* SPU voices currently keyed-on */
    /* 0x0C */ u32 AllocatedVoiceMask; /* Channels with SPU voices allocated */
    /* 0x10 */ s32 PendingKeyOnMask; /* Pending key-ons */
    /* 0x14 */ s32 ActiveNoteMask; /* Currently playing notes (not rests) */
    /* 0x18 */ u32 PreventRekeyOnMusicResumeMask; /* With the way music pushes and pops, if something like a one-shot was started and "paused" then it won't rekey/resume that note when the music resumes*/
    /* 0x1C */ s32 PendingKeyOffMask;
    /* 0x20 */ u32 SuspendedChannelMask;
    /* 0x24 */ s32 Tempo;
    /* 0x28 */ s32 TempoSlideStep;
    /* 0x2C */ s32 TempoUpdate;
    /* 0x30 */ FAkaoSequence* SequenceBase;
    /* 0x34 */ u16* SequencePatchTable;
    /* 0x38 */ FSoundKeymapEntry8* KeymapTable;
    /* 0x3C */ s32 SomeIndexRelatedToSpuVoiceInfo;
    /* 0x40 */ u32 NoiseChannelFlags;
    /* 0x44 */ u32 ReverbChannelFlags;
    /* 0x48 */ u32 FmChannelFlags;
    /* 0x4C */ s32 RevDepth;
    /* 0x50 */ s32 ReverbDepthSlideStep;
    /* 0x54 */ u32 MasterVolume;
    /* 0x58 */ s32 MasterVolumeStep;
    /* 0x5C */ s16 MasterVolumeStepsRemaining;
    /* 0x5E */ undefined field24_0x5e;
    /* 0x5F */ undefined field25_0x5f;
    /* 0x60 */ u32 MasterPanOffset;
    /* 0x64 */ s32 MasterPanStep;
    /* 0x68 */ s16 MasterPanStepsRemaining;
    /* 0x6A */ s16 ReverbDepthSlideLength;
    /* 0x6C */ u16 TempoSlideLength;
    /* 0x6E */ u16 MusicId;
    /* 0x70 */ u16 JumpThreshold;
    /* 0x72 */ u16 NoiseClock;
    /* 0x74 */ u16 TimerUpper;
    /* 0x76 */ u16 TimerUpperCurrent;
    /* 0x78 */ u16 TimerLower;
    /* 0x7A */ u16 TimerLowerCurrent;
    /* 0x7C */ s16 TimerTopCurrent;
    /* 0x7E */ undefined field39_0x7e;
    /* 0x7F */ undefined field40_0x7f;
} FSoundMusicContext; /* size 0x80 */
static_assert( sizeof(FSoundMusicContext) == 0x80 );

typedef struct FSoundCommandParams
{
    /* 0x00 */ u32 Param1;
    /* 0x04 */ u32 Param2;
    /* 0x08 */ u32 Param3; // Seems to be usually flags
    /* 0x0C */ u32 Param4;
    /* 0x10 */ u32 ExtParam1;
    /* 0x14 */ u32 ExtParam2;
} FSoundCommandParams; /* size 0x18 */
static_assert( sizeof(FSoundCommandParams) == 0x18 );

typedef struct
{
    /* 0x0 */ s32 unk0; // Seems unused
    /* 0x4 */ s16 pEnvx;
    /* 0x6 */ s16 unk6;
} FSpuVoiceInfo; /* size 0x8 */
static_assert( sizeof(FSpuVoiceInfo) == 0x8 );

typedef struct
{
    /* 0x00 */ s32 Value;
    /* 0x04 */ s32 Step;
    /* 0x08 */ s32 TicksRemaining;
    /* 0x0C */ s32 SavedValue;
} FSoundFadeTimer; /* size 0x10 */
static_assert( sizeof(FSoundFadeTimer) == 0x10 );

typedef struct
{
    /* 0x0 */ s32 Reverb;
    /* 0x4 */ s32 Noise;
    /* 0x8 */ s32 Fm;
} FSoundVoiceModeFlags; /* size 0xC */
static_assert( sizeof(FSoundVoiceModeFlags) == 0xC );

#define SEMITONES_IN_OCTAVE (12)

// Semitone pitch multipliers (fixed-point, 0x1000 = 1.0)
// extern const s32 g_SemitonePitchTable[SEMITONES_IN_OCTAVE];
// Only referenced in SoundVM functions - maybe move there if we process the sound VM in that source file
/* {
    0x00001000,  // C  - base pitch (1.0)
    0x000010F3,  // C# - 2^(1/12)
    0x000011F5,  // D  - 2^(2/12)
    0x00001306,  // D# - 2^(3/12)
    0x00001428,  // E  - 2^(4/12)
    0x0000155B,  // F  - 2^(5/12)
    0x000016A0,  // F# - 2^(6/12)
    0x000017F9,  // G  - 2^(7/12)
    0x00001966,  // G# - 2^(8/12)
    0x00001AE8,  // A  - 2^(9/12)
    0x00001C82,  // A# - 2^(10/12)
    0x00001E34   // B  - 2^(11/12)
}; */

// Sound API - IDK I'm just picking names right now....
s32 InitSound();
bool Sound_BindAkaoSfxBlob( FAkaoFileBlob* in_Blob );
void Sound_StartFieldMusic( u32 in_Unk );
void func_80049FE4( u32 arg0 );
void Sound_StartBattleMusic( u32 arg0, u32 arg1 );
void Sound_PushMusicState();
void Sound_SetMusicLevelImmediate( u32 arg0, s32 arg1 );
void Sound_StartMasterAndMusicVolumeFade( u32 arg0, u32 arg1, s32 arg2 );
void Sound_StartFieldMusicLooped( u32 arg0, u32 arg1 );
void Sound_PlaySfx( s32 arg0, s32 arg1, s32 arg2, s32 arg3 );
s32 Sound_SetTempoMultiplier( s32 in_Mode );
// void Sound_SetSpeakerMode( s32 in_Mode ); // When you copy this, copy ESpeakerMode from the source
void Sound_SetMutedMusicChannelMask( u32 in_ChannelMask );
void Sound_SetMusicJumpThreshold( u32 arg0 );
void Sound_SuspendChannelsByType( u32 in_ChannelType );
void Sound_RestoreChannelsByType( u32 in_ChannelType );
void Sound_FadeOutCutscene( u32 arg0, s32 arg1 );

// SPU management
void Sound_CopyAndRelocateInstruments( FSoundInstrumentInfo* in_A, FSoundInstrumentInfo* in_B, s32 in_AddrOffset, s32 in_Count);
bool Sound_IsNotAkaoFile( void* in_Blob );
void ClearSpuTransferCallback();
void SetSpuTransferCallback();
void WriteSpu( s32 in_Addr, s32 in_Size );
void ReadSpu( s32 in_Addr, s32 in_Size );
void WaitForSpuTransfer();
s32 Sound_TryLoadInstrumentBank( FAkaoSequence* in_pAkao, s32 arg1 );
s32 Sound_LoadInstrumentBank( FAkaoSequence* in_Akao, s32 in_bWait, s32 arg2, u32 in_StartAddr );
void Sound_Setup();
void Sound_Start();
void Sound_Stop();

// Sound
void SetVoiceKeyOn( u32 in_KeyOn );
void SetVoiceKeyOff( u32 in_KeyOff );
void SetVoiceReverbMode( u32 in_ReverbMode );
void SetVoiceNoiseMode( u32 in_NoiseMode );
void SetVoiceFmMode( u32 in_FmMode );
void SetVoiceVolume( u32 in_VoiceIndex, u32 in_VolL, u32 in_VolR, u32 in_VolumeScale );
void SetVoiceSampleRate( u32 in_VoiceIndex, s32 in_SampleRate );
void SetVoiceStartAddr( u32 in_VoiceIndex, u32 in_Addr );
void SetVoiceRepeatAddr( u32 in_VoiceIndex, u32 in_Addr );
void SetVoiceAdsrLower( u32 in_VoiceIndex, u16 in_Register );
void SetVoiceAdsrUpper( u32 in_VoiceIndex, u16 in_Register );
void SetVoiceAdsrAttackRateAndMode( u32 in_VoiceIndex, s32 in_AttackStep, u32 in_AttackMode );
void SetVoiceAdsrDecayRate( u32 in_VoiceIndex, s32 in_DecayRate );
void SetVoiceAdsrSustainLevel( u32 in_VoiceIndex, s32 in_SustainLevel );
void SetVoiceAdsrSustainRateAndDirection( u32 in_VoiceIndex, s32 in_SustainRate, u32 in_SustainDirection );
void SetVoiceAdsrReleaseRateAndMode( u32 in_VoiceIndex, s32 in_ReleaseRate, u32 in_ReleaseMode );
void SetVoiceParams( u32 in_VoiceIndex, FSoundVoiceParams* in_VoiceParams, s32 in_VolumeScale );
void SetVoiceParamsByFlags( u32 in_VoiceIndex, FSoundVoiceParams* in_VoiceParams, s32 in_UpdateFlags );
void Sound_UpdateSlidesAndDelays( FSoundChannel* in_pChannel, u32 in_VoiceFlags, ESoundChannelTypes in_ChannelType );
void Sound_UpdateModulation( FSoundChannel* in_pChannel, int arg1 );
void func_8004CA1C( FSoundChannel* in_pChannel );
s32 Sound_StealQuietestVoice( s32 in_bForceFullScan );
s32 Sound_FindFreeVoice( s32 in_bForceFullScan );
void Sound_AssignAndUpdateMusicVoice( FSoundChannel* in_pChannel, u32 in_Flags1, u32 in_Flags2, u32* out_KeyOnFlags );
void Sound_UpdateVoiceEnvelopeStates( u32 in_ProtextedVoiceMask );
void Sound_ApplyMasterFadeToChannelVolume( FSoundMusicContext* in_Context );
void Sound_RestoreChannelVolumeFromMasterFade ( FSoundMusicContext* in_Context );
void UnassignVoicesFromChannels( FSoundChannel* in_pChannel, s32 );
void ChannelMaskToVoiceMaskFiltered( FSoundChannel* in_Channel, s32* io_VoiceMask, s32 in_ChannelMask, s32 in_VoiceMaskFilter );
void Sound_ProcessKeyOffRequests();
void Sound_BuildVoiceModeMask( s32* out_VoiceModeMask, s32 in_SavedChannelModeMask, s32 in_ActiveChannelModeMask, s32 in_PersistentVoiceMask );

// Sound 2
u32 ChannelMaskToVoiceMask( FSoundChannel* in_pChannel, u32 in_ChannelMask );
u16 Sound_MapInstrumentToAltSampleBank( u32 in_Flags, FSoundChannel* in_Channel );
u16 Sound_MapInstrumentToBaseSampleBank( u32 in_Flags, FSoundChannel* in_Channel );
void Sound_ReconcileSavedMusicVoices();
void Sound_ResetChannel( FSoundChannel* in_pChannel, u8* in_pProgramCounter );
void Sound_LoadAkaoSequence( FAkaoSequence* in_Sequence, s32 in_Mask );
void Sound_KillMusicContext( FSoundMusicContext *in_Struct,FSoundChannel *in_pChannel, uint);
void Sound_EvictSfxVoice( u32, u32 );
void unk_Sound_8004e7d8( FSoundChannel *in_Channel, FSoundCommandParams* in_pCommandParams, uint in_Flags, u8 *in_ProgramCounter );
void FreeVoiceChannels( FSoundChannel* in_Channel, u32 in_Voice );
void Sound_PlaySfxProgram( FSoundCommandParams* in_CommandParams, u8* in_ProgramCounter1, u8* in_ProgramCounter2, s32 );
void Sound_GetProgramCounters( u8** out_ProgramCounter1, u8** out_ProgramCounter2, s32 in_SfxIndex );
void Sound_MarkActiveChannelsVolumeDirty( FSoundMusicContext* in_pContext, FSoundChannel* in_pChannel );
void Sound_MarkScheduledSfxChannelsVolumeDirty();
void Sound_SetMusicSequence( FAkaoSequence *in_Sequence, int in_SwapWithSavedState );

// SoundCommand
void Sound_Cmd_10_StartFieldMusic( FSoundCommandParams* in_Params );
void Sound_Cmd_14_StartBattleMusic( FSoundCommandParams* in_Params );
void Sound_Cmd_40_PushMusicState( FSoundCommandParams* in_Params );
void Sound_Cmd_19_SetMusicLevelImmediate( FSoundCommandParams* in_Params );
void Sound_Cmd_1A_StartMasterAndMusicVolumeFade( FSoundCommandParams* in_Params );
void Sound_Cmd_12_PlayFieldMusicLooped( FSoundCommandParams* in_Params );
void Sound_Cmd_34_PlaySfxDirect( FSoundCommandParams* in_Params );
void Sound_Cmd_30_PlaySfxProtected( FSoundCommandParams* in_Params );
void Sound_Cmd_20_PlaySfx( FSoundCommandParams* in_Params );
void Sound_Cmd_24_PlaySfxFromPointer( FSoundCommandParams* in_Params );
void Sound_Cmd_21_EvictSfxVoice( FSoundCommandParams* in_Params );
void Sound_Cmd_C0_SetMasterVolumeByMusicId( FSoundCommandParams* in_Params );
void Sound_Cmd_C1_FadeMasterVolumeByMusicId( FSoundCommandParams* in_Params );
void Sound_Cmd_C2_FadeMasterVolumeFromByMusicId( FSoundCommandParams* in_Params );
void Sound_Cmd_C4_SetPanByMusicId( FSoundCommandParams* in_Params );
void Sound_Cmd_C5_FadePanByMusicId( FSoundCommandParams* in_Params );
void Sound_Cmd_70_SetCdVolume( FSoundCommandParams* in_Params );
void Sound_Cmd_71_FadeCdVolume( FSoundCommandParams* in_Params );
void Sound_Cmd_72_FadeCdVolumeFrom( FSoundCommandParams* in_Params );
void Sound_Cmd_A0_SetSfxVolumeMod( FSoundCommandParams* in_Params );
void Sound_Cmd_A1_FadeSfxVolumeMod( FSoundCommandParams* in_Params );
void Sound_Cmd_A8_SetAllSfxVolumeMod( FSoundCommandParams* in_Params );
void Sound_Cmd_A9_FadeAllSfxVolumeMod( FSoundCommandParams* in_Params );
void Sound_Cmd_A2_SetSfxPanMod( FSoundCommandParams* in_Params );
void Sound_Cmd_A3_FadeSfxPanMod( FSoundCommandParams* in_Params );
void Sound_Cmd_AA_SetAllSfxPanMod( FSoundCommandParams* in_Params );
void Sound_Cmd_AB_FadeAllSfxPanMod( FSoundCommandParams* in_Params );
void Sound_Cmd_A4_SetSfxPitchMod( FSoundCommandParams* in_Params );
void Sound_Cmd_A5_FadeSfxPitchMod( FSoundCommandParams* in_Params );
void Sound_Cmd_AC_SetAllSfxPitchMod( FSoundCommandParams* in_Params );
void Sound_Cmd_AD_FadeAllSfxPitchMod( FSoundCommandParams* in_Params );
void Sound_Cmd_D0_SetTempoScale( FSoundCommandParams* in_Params );
void Sound_Cmd_D1_FadeTempoScale( FSoundCommandParams* in_Params );
void Sound_Cmd_D2_FadeTempoScaleFrom( FSoundCommandParams* in_Params );
void Sound_Cmd_D4_SetMasterPitchScale( FSoundCommandParams* in_Params );
void Sound_Cmd_D5_FadeMasterPitchScale( FSoundCommandParams* in_Params );
void Sound_Cmd_D6_FadeMasterPitchScaleFrom( FSoundCommandParams* in_Params );
void Sound_Cmd_F0_StopMusic( FSoundCommandParams* in_Params );
void Sound_Cmd_11_StopMusicById( FSoundCommandParams* in_Params );
void Sound_Cmd_F1_StopSfx( FSoundCommandParams* in_Params );
void Sound_Cmd_80_SetModeStereo( FSoundCommandParams* in_Params );
void Sound_Cmd_81_SetModeMono( FSoundCommandParams* in_Params );
void Sound_Cmd_90_SetMutedMusicChannelMask( FSoundCommandParams* in_Params );
void Sound_Cmd_92_SetMusicJumpThreshold( FSoundCommandParams* in_Params );
void Sound_Cmd_9B_SuspendMusic( FSoundCommandParams* in_Params );
void Sound_Cmd_9A_RestoreMusic( FSoundCommandParams* in_Params );
void Sound_Cmd_9D_SuspendSfx( FSoundCommandParams* in_Params );
void Sound_Cmd_9C_RestoreSfx( FSoundCommandParams* in_Params );
void Sound_Cmd_9F_SuspendCutsceneAudio( FSoundCommandParams* in_Params );
void Sound_Cmd_9E_RestoreCutsceneAudio( FSoundCommandParams* in_Params );
void Sound_Cmd_AE_MuteSfx( FSoundCommandParams* in_Params );
void Sound_Cmd_AF_UnmuteSfx( FSoundCommandParams* in_Params );
void Sound_Cmd_XX_Null( FSoundCommandParams* in_Params );
void Sound_SetReverbMode( s32 in_ReverbMode );
void* Sound_ExecuteSoundVm2Function( u32 in_FunctionIndex );

typedef enum EMusicContextType
{
    MUSIC_CONTEXT_ACTIVE    = 0,
    MUSIC_CONTEXT_SUSPENDED = 1
} EMusicContextType;

// Sound 3
void Sound_UpdateCdVolume();
void memcpy32( s32* in_Src, s32* in_Dst, uint in_Size );
void memswap32( s32* in_A, s32* in_B, uint in_Size );
u32 Music_UpdateChannels( FSoundChannel* in_pChannel, EMusicContextType in_ContextType );
long Sound_MainLoop();
u8 Sound_PeekNextOpcode( FSoundChannel* in_pChannel );
s32 Sound_CalculatePitch( FSoundInstrumentInfo* in_pInstrumentInfo, s32 in_Note, u32 in_FineTune, s32* out_pPitchDelta );
s32 Sound_PlayKeymapNote( FSoundChannel* in_pChannel, s32 in_ChannelMask, s32 in_KeymapIndex );
void Sound_ProcessAkaoVM( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
s32 Sound_ComputeSlideStep( u32*, s32, s32, s32 );
void Sound_CopyInstrumentInfoToChannel( FSoundChannel* in_pChannel, FSoundInstrumentInfo* in_pInstrumentInfo, u32 in_StartAddress );
void Sound_SetInstrumentToChannel( FSoundChannel *in_Channel, u32 in_Index );
void Sound_ClearVoiceFromSfxState( FSoundChannel* in_pChannel, u32 in_VoiceFlags );

// SoundVM
void SoundVM_A0_FinishChannel( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE00_SetTempo( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE01_SetTempoSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE02_SetMasterReverbDepth( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE03_SetMasterReverbSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE06_JumpRelativeOffset( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE07_JumpRelativeWithThreshold( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE0E_CallRelativeOffset( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE0F_Return( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_A3_ChannelMasterVolume( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE12_VolumeBalanceSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_A8_ChannelVolume( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_A9_ChannelVolumeSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE19_KeyOnVolumeSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE1A_SetChannelTenuto( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE1B_UnsetChannelTenuto( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_AA_ChannelPan( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_AB_ChannelPanSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_A5_SetOctave( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_A6_IncreaseOctave( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_A7_DecreaseOctave( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_A1_LoadInstrument( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE0A_ClearInstrument( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE14_ChangePatch( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_B3_ResetAdsr( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_C0_ChannelTranspose_Absolute( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_C1_ChannelTranspose_Relative( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_A4_PitchBendSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_DA_EnablePortamento( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_DB_DisablePortamento( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_D8_ChannelFineTune_Absolute( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_D9_ChannelFineTune_Relative( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_B4_Vibrato( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_B5_VibratoDepth( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_DD_VibratoDepthSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_E4_VibratoRateSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_B6_DisableVibrato( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_B8_Tremelo( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_B9_TremeloDepth( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_DE_TremeloDepthSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_E5_TremeloRateSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_BA_DisableTremelo( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_BC_AutoPan( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_BD_AutoPanDepth( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_DF_AutoPanDepthSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_E6_AutoPanRateSlide( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_BE_DisableAutoPan( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_C4_EnableNoiseVoices( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_C5_DisableNoiseVoices( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_C6_EnableFmVoices( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_C7_DisableFmVoices( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_C2_EnableReverbVoices( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_C3_DisableReverbVoices( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_CC_EnableLegato( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_CD_DEBUG_80055078( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_D0_EnableSustainedNote( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_D1_DEBUG_8005509c( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_AC_NoiseClockFrequency( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_AD_AttackRate( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_AE_DecayRate( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_AF_SustainLevel( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_B1_SustainRate( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_B2_ReleaseRate( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_B7_AttackMode( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_BB_SustainMode( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_BF_ReleaseMode( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE10_8005536c( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE11_8005538c( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_C8_LoopPoint( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_C9_LoopN( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE08_JumpOnNthLoopPass( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE09_JumpAndPopStackOnNthLoopPass( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_CA_LoopInf( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_A2_OverwriteNextNoteLength( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_DC_FixNoteLength( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE04_ClearKeymapTable( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE05_MuteVoice( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE15_8005567c( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE16_800556b4( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_BO_DecayRateAndSustainLevel( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_CE_EnableNoiseAndDelayToggle( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_CF_ToggleNoiseOnDelay( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_D2_EnableFmAndDelayToggle( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_D3_ToggleFmDelay( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_CB_DisableVoiceModes( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_D4_EnablePlaybackRateSidechain( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_D5_DisablePlaybackRateSidechain( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_D6_EnablePitchVolumeSidechain( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_D7_DisablePitchVolumeSidechain( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE0B_800558cc( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_E0_80055944( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE1C_IncrementProgramCounter( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE1D_MarkVoicesKeyed( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE1E_ClearVoicesKeyed( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_E1_SetRandomPitchDepth( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_E2_ResetRandomPitchDepth( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_FE13_PreventVoicesFromRekeyingOnResume( FSoundChannel* in_pChannel, u32 in_VoiceFlags );
void SoundVM_XX_Unimplemented( FSoundChannel* in_pChannel, u32 in_VoiceFlags );

void* Sound_PlaySfxProtected( s32 in_VoiceIndex ); // TODO(jperos): Returns some sort of Akao file



extern s32 g_Sound_EventDescriptor;
#define SOUND_NULL_WAVEFORM_BUF_SIZE (64)
extern s8 g_Sound_NullWaveformBuf[ SOUND_NULL_WAVEFORM_BUF_SIZE ];
extern u16 g_Sound_NoteLengthTable[12]; // AKAO_NOTE_LENGTH_COUNT

// RODATA it seems
extern u32 g_Sound_ProgramCounter;
extern const u32 g_SemitonePitchTable[SEMITONES_IN_OCTAVE];
extern u8 g_Sound_LfoWave[0x100];

#define SPU_PAN_TABLE_SIZE   (0x100)
#define PAN_CENTER_INDEX     (SPU_PAN_TABLE_SIZE / 2)
extern s16 g_Sound_StereoPanGainTableQ15[SPU_PAN_TABLE_SIZE];

extern s16* g_Sound_LfoTable[SOUND_LFO_COUNT];

// DATA I think
extern FSoundChannel g_ActiveMusicChannels[SOUND_CHANNEL_COUNT];
extern u32 D_80090A34;
extern FSoundChannel g_SfxSoundChannels[SOUND_SFX_CHANNEL_COUNT];
extern CdlATV g_Sound_CdlATV;
extern FSoundChannel* g_pSecondaryMusicChannels;
extern FSoundMusicContext* g_pSuspendedMusicContext; // If non-null, points to a suspended music
extern FSpuVoiceInfo g_SpuVoiceInfo[VOICE_COUNT];
extern FSoundMusicContext g_PrimaryMusicContext;
extern s32 g_Sound_MasterPitchScaleStep;
extern s32 g_Sound_TempoScaleStep;
extern FSoundMusicContext g_SuspendedMusicContext;
extern s16 g_Sound_MasterPitchScaleStepsRemaining;
extern s16 g_Sound_TempoScaleStepsRemaining;
extern FSoundInstrumentInfo g_InstrumentInfo[256];
extern u32 g_Music_LoopCounter;
extern u16* g_Sound_Sfx_ProgramOffsets;
extern volatile bool g_bSpuTransferring;
extern u16* g_Sound_Sfx_MetadataTableA;
extern u8* g_Sound_Sfx_ProgramData;
extern FSoundCommandParams g_Sound_CommandParams_Vm_FE08;
extern FSoundMusicContext* g_pActiveMusicContext;
extern FSoundFadeTimer g_Sound_MasterFadeTimer;
extern s32 g_Sound_CdVolumeFadeStep;
extern s16 g_Sound_CdVolumeFadeLength;
extern FSoundSfxState g_Sound_SfxState;
extern FSoundCommandParams g_Sound_Vm2Params;
extern s32 g_CdVolume;
extern s32 g_Sound_MutedMusicChannelMask;
extern FSoundChannel g_PushedMusicChannels[SOUND_CHANNEL_COUNT];
extern u16* g_Sound_Sfx_AdditionalProgramCounts;
extern s32 g_Sound_LfoPhase;
extern s32 g_Sound_TempoScale;
extern s32 g_Sound_MasterPitchScaleQ16_16;
extern FSoundGlobalFlags g_Sound_GlobalFlags;
extern FSoundMusicContext* g_Sound_VoiceOwnerContexts[VOICE_COUNT];
extern FSoundVoiceModeFlags g_Sound_VoiceModeFlags;

#define SPU_MALLOC_NUM_BLOCKS (4)
char g_SpuMallocRecTable[ SPU_MALLOC_RECSIZ * (SPU_MALLOC_NUM_BLOCKS + 1) ];

#endif // _SOUND_COMMAND_H
