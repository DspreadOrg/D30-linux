#ifndef OTA_H
#define OTA_H

#include "appinc.h"
#include "tms_cfg.h"
#include "proj_define.h"
#include "cJSON.h"

void TmsDispCallback(u32 id, char*pMsg);
void larktms_init();
int ota_process();
#endif