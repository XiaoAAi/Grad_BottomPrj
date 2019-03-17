#ifndef _BSP_WIFI_H
#define _BSP_WIFI_H

#include "stm32f10x_conf.h"


u8* esp8266_check_cmd(char *str);
void wifi_start_trans(void);//WiFi开启透传模式

#endif

