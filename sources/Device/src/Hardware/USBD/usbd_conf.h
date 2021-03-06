#pragma once

#include <stm32f4xx_hal.h>
#include <cstdlib>

#define USBD_LPM_ENABLED 0
#define USE_RTOS         0U


#define USBD_MAX_NUM_INTERFACES               1
#define USBD_MAX_NUM_CONFIGURATION            1
#define USBD_MAX_STR_DESC_SIZ                 0x100
#define USBD_SUPPORT_USER_STRING              0
#define USBD_SELF_POWERED                     1
#define USBD_DEBUG_LEVEL                      0
 
#define USBD_malloc std::malloc
#define USBD_free   std::free
#define USBD_memset               memset
#define USBD_memcpy               memcpy


#if (USBD_DEBUG_LEVEL > 0)
#define USBD_UsrLog(...)  Log_Write(__VA_ARGS__)
#define USBD_ErrLog(...)  Log_Write(__VA_ARGS__)
#define USBD_DbgLog(...)  Log_Write(__VA_ARGS__)
#else
#define USBD_UsrLog(...) {}
#define USBD_ErrLog(...) {}
#define USBD_DbgLog(...) {}
#endif
