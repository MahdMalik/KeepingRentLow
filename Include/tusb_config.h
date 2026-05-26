#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

// 1. Specify your target hardware platform
#define CFG_TUSB_MCU             OPT_MCU_STM32F4

// 2. Map the physical USB peripheral port on your Black Pill
// On the STM32F411, Port 0 is the native Full-Speed (FS) OTG engine
#define CFG_TUSB_RHPORT0_MODE    (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)

// 3. Enable the device stack software framework
#define CFG_TUD_ENABLED          1

// 4. Enable the Virtual COM Port (CDC) middleware engine
#define CFG_TUD_CDC              1
#define CFG_TUD_CDC_RX_BUFSIZE   64
#define CFG_TUD_CDC_TX_BUFSIZE   64

#endif // _TUSB_CONFIG_H_
