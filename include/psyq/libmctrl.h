#ifndef _LIBMCTRL_H
#define _LIBMCTRL_H

// Memory Control Delay/Size Register bit definitions (applies to all delay/size registers)
// 1F801008h - Expansion 1, 1F80100Ch - Expansion 3, 1F801010h - BIOS ROM,
// 1F801014h - SPU, 1F801018h - CDROM, 1F80101Ch - Expansion 2

// Write/Read Delay fields (bits 0-7)
#define MCTRL_DELAY_WRITE_DELAY_MASK          ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3))      // 0-3
#define MCTRL_DELAY_READ_DELAY_MASK           ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))      // 4-7
#define MCTRL_DELAY_WRITE_DELAY_SHIFT         0
#define MCTRL_DELAY_READ_DELAY_SHIFT          4

// Timing control bits (bits 8-11)
#define MCTRL_DELAY_RECOVERY_PERIOD           (1 << 8)   // Uses COM0 timings
#define MCTRL_DELAY_HOLD_PERIOD               (1 << 9)   // Uses COM1 timings
#define MCTRL_DELAY_FLOATING_PERIOD           (1 << 10)  // Uses COM2 timings
#define MCTRL_DELAY_PRE_STROBE_PERIOD         (1 << 11)  // Uses COM3 timings

// Bus configuration bits (bits 12-13)
#define MCTRL_DELAY_DATA_BUS_WIDTH_16BIT      (1 << 12)  // 0=8bits, 1=16bits
#define MCTRL_DELAY_AUTO_INCREMENT            (1 << 13)  // 0=No, 1=Yes

// Unknown bits (bits 14-15)
#define MCTRL_DELAY_UNKNOWN_14_15_MASK        ((1 << 14) | (1 << 15)) // 14-15

// Address bits field (bits 16-20)
#define MCTRL_DELAY_ADDRESS_BITS_MASK         ((1 << 16) | (1 << 17) | (1 << 18) | (1 << 19) | (1 << 20)) // 16-20
#define MCTRL_DELAY_ADDRESS_BITS_SHIFT        16

// DMA timing control (bits 24-31)
#define MCTRL_DELAY_DMA_TIMING_OVERRIDE_MASK  ((1 << 24) | (1 << 25) | (1 << 26) | (1 << 27)) // 24-27
#define MCTRL_DELAY_DMA_TIMING_OVERRIDE_SHIFT 24
#define MCTRL_DELAY_ADDRESS_ERROR_FLAG        (1 << 28)  // Write 1 to clear
#define MCTRL_DELAY_DMA_TIMING_SELECT         (1 << 29)  // 0=normal, 1=use bits 24-27
#define MCTRL_DELAY_WIDE_DMA                  (1 << 30)  // 0=use bit 12, 1=32 bits
#define MCTRL_DELAY_WAIT                      (1 << 31)  // Wait on external device

#endif