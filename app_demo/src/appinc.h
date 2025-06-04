#ifndef APP_INC_H
#define APP_INC_H

/*常见的头文件*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h> 
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>  
#include <fcntl.h> 
#include<sys/socket.h>
#include<arpa/inet.h>
#include <netdb.h>  

#include <openssl/rsa.h>  
#include <openssl/pem.h>  
#include <openssl/err.h> 
#include <openssl/bn.h> 
#include <openssl/ssl.h>

#include "typedefine.h"
#include "emvdeviceimpl.h"
#include "Disp.h"
#include "osal.h"

//LVGL
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

//network
#include "socket.h"
#include "debug.h"
#include "cmacro.h"

//dspread
#include "utility.h"
#include "function.h"
#include "proj_define.h"
#include "lib_common.h"
#include "tms_cfg.h"
//ui
#include "task_handle.h"
#include "ui_utils.h"
#include "ui_misc.h"
#include "ui_main.h"

#include "business.h"
#include "businessUtil.h"
#include "emvimpl.h"
#include "utility.h"
#include "record.h"

//utils
#include "debug.h"

#include "app_http.h"
#include "app_request.h"
#include "app_server.h"

#include "ui_wifi.h"
#include "ota.h"

#endif
