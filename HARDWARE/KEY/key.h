#ifndef __KEY_H
#define __KEY_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t key_type;
enum {
    HWM_KEY0 = 0X01,
    HWM_KEY1 = 0X02,
    HWM_KEY2 = 0X04,
    HWM_KEY3 = 0X08,
    HWM_WKUP = HWM_KEY3,
    HWM_KEY0_HELD = 0x10,
    HWM_KEY1_HELD = 0x20,
    HWM_KEY2_HELD = 0x40,

    HWM_KEY3_HELD = 0x80
};

void key_init();
key_type get_key();

#if __cplusplus
}
#endif
#endif