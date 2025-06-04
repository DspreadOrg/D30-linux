/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"
#include "../lvgl.h"
#include <linux/input.h>
#include "osal.h"
#include "Disp.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);

static void mouse_init(void);
static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool mouse_is_pressed(void);
static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y);

static void keypad_init(void);
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static uint32_t keypad_get_key(void);

static void encoder_init(void);
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static void encoder_handler(void);

static void button_init(void);
static void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static int8_t button_get_pressed_id(void);
static bool button_is_pressed(uint8_t id);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_touchpad;
lv_indev_t * indev_mouse;
lv_indev_t * indev_keypad;
lv_indev_t * indev_encoder;
lv_indev_t * indev_button;
lv_group_t * group_keypad_indev;

static int32_t encoder_diff;
static lv_indev_state_t encoder_state;

static lv_coord_t last_x = 0;
static lv_coord_t last_y = 0;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_port_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad if you have*/
    touchpad_init();

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);

    // /*------------------
    //  * Mouse
    //  * -----------------*/

    // /*Initialize your mouse if you have*/
    // mouse_init();

    // /*Register a mouse input device*/
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_POINTER;
    // indev_drv.read_cb = mouse_read;
    // indev_mouse = lv_indev_drv_register(&indev_drv);

    // /*Set cursor. For simplicity set a HOME symbol now.*/
    // lv_obj_t * mouse_cursor = lv_img_create(lv_scr_act());
    // lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
    // lv_indev_set_cursor(indev_mouse, mouse_cursor);

    /*------------------
     * Keypad
     * -----------------*/

    static lv_indev_drv_t indev_drv_keypad;
    /*Initialize your keypad or keyboard if you have*/
    keypad_init();

    /*Register a keypad input device*/
    lv_indev_drv_init(&indev_drv_keypad);
    indev_drv_keypad.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_keypad.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv_keypad);
    group_keypad_indev = lv_group_create();
    lv_indev_set_group(indev_keypad, group_keypad_indev);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_keypad, group);`*/

    // /*------------------
    //  * Encoder
    //  * -----------------*/

    // /*Initialize your encoder if you have*/
    // encoder_init();

    // /*Register a encoder input device*/
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_ENCODER;
    // indev_drv.read_cb = encoder_read;
    // indev_encoder = lv_indev_drv_register(&indev_drv);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_encoder, group);`*/

    // /*------------------
    //  * Button
    //  * -----------------*/

    // /*Initialize your button if you have*/
    // button_init();

    // /*Register a button input device*/
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_BUTTON;
    // indev_drv.read_cb = button_read;
    // indev_button = lv_indev_drv_register(&indev_drv);

    // /*Assign buttons to points on the screen*/
    // static const lv_point_t btn_points[2] = {
    //     {10, 10},   /*Button 0 -> x:10; y:10*/
    //     {40, 100},  /*Button 1 -> x:40; y:100*/
    // };
    // lv_indev_set_button_points(indev_button, btn_points);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Touchpad
 * -----------------*/
static int tpdev_fd;
static int tpdev_root_x;
static int tpdev_root_y;
static lv_indev_state_t tpdev_button;
static int tpdev_key_val;

/*Initialize your touchpad*/
static void touchpad_init(void)
{

}

/*Will be called by the library to read the touchpad*/
typedef struct key_struct {
    int touch_x;    //Touch point x
    int touch_y;    //Touch point y
    int down;   //Touch status 1: press 0: release-1: move
    int valid;  //Determine whether the data is valid 1: valid 0: invalid
}touch_key, *p_touch_key;

static touch_key last_touch_key = { 0, 0, -1, 0 };
touch_key g_touch_key = { 0, 0, -1, 0 };

int readTouchKey(p_touch_key ptouch_key)
{
    struct input_event event;

    while (1)
    {
        /*Read touch data circularly*/
        if (Disp_GetTouchPadLinux(&event, 1)!=0) {            
            //OsLog(LOG_ERROR, "==============Disp_GetTouchPadLinux Error=========================");
            return -1;
        }

        #if defined(CFG_DEBUG_TOUCHKEY)
        OsLog(LOG_ERROR, "Disp_GetTouchPadLinux type = %08x, code = %08x, value = %08x", event.type, event.code, event.value);
        // OsLog(LOG_ERROR, "EV_KEY = %d, EV_ABS =%d, EV_SYN = %d", EV_KEY, EV_ABS, EV_SYN);
        // OsLog(LOG_ERROR, "BTN_TOUCH = %d, ABS_X =%d, ABS_Y = %d, SYN_REPORT = %d", BTN_TOUCH, ABS_X, ABS_Y,SYN_REPORT);
        #endif

        switch(event.type) {
            case EV_KEY :   //Key event
                if(event.code = BTN_TOUCH) {
                    ptouch_key->down = event.value;   // Save the value of the key event
                    ptouch_key->valid = 1;
                    switch(ptouch_key->down) {
                        case 1: //Press the button
                            #if defined(CFG_DEBUG_TOUCHKEY)
                            OsLog(LOG_ERROR, "Press touch key %d, %d\n", ptouch_key->touch_x, ptouch_key->touch_y);
                            #endif
                            break;
                            
                        case 0://Release the button
                            #if defined(CFG_DEBUG_TOUCHKEY)
                            OsLog(LOG_ERROR, "Release touch key %d, %d\n", ptouch_key->touch_x, ptouch_key->touch_y);                              
                            #endif
                            break;
                        case -1:
                            #if defined(CFG_DEBUG_TOUCHKEY)
                            OsLog(LOG_ERROR,"Move touchkey %d, %d\n",ptouch_key->touch_x,ptouch_key->touch_y);
                            #endif
                            break;
                    }

                }
                break;
 
            case EV_ABS : //Absolute displacement event
                switch (event.code)
                {
                    case ABS_X: //Absolute displacement X coordinate
                    case 0x35:
                        ptouch_key->touch_x = event.value;    //Save X coordinat
                        break;
                    case ABS_Y: //Absolute displacement Y coordinate
                    case 0x36:
                        ptouch_key->touch_y = event.value;    //Save Y coordinat
                        break;
                }
                break;            
        }
    }

    return 0;
}

static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    #if 1
    readTouchKey(&g_touch_key);

    #if defined(CFG_DEBUG_TOUCHKEY)
    OsLog(LOG_ERROR, " g_touch_key.down =%d, g_touch_key.touch_x = %08x, g_touch_key.touch_y = %08x", g_touch_key.down, g_touch_key.touch_x,g_touch_key.touch_y);
    #endif

    if (g_touch_key.valid == 0 || g_touch_key.touch_x > 480 || g_touch_key.touch_y > 854)
    {
        return;
    }

    {
        switch (g_touch_key.down)
        {
        case 1:
            data->point.x = g_touch_key.touch_x;
            data->point.y = g_touch_key.touch_y;
            data->state = LV_INDEV_STATE_PR;

            #if defined(CFG_DEBUG_TOUCHKEY)
            OsLog(LOG_ERROR, "Dspread: touch key pressed %d, %d\n", data->point.x, data->point.y);
            #endif
            break;
        case 0:
            data->point.x = g_touch_key.touch_x;
            data->point.y = g_touch_key.touch_y;
            data->state = LV_INDEV_STATE_REL;
            #if defined(CFG_DEBUG_TOUCHKEY)
            OsLog(LOG_ERROR, "Dspread: CFG_DEBUG_TOUCHKEYtouch key released %d, %d\n", data->point.x, data->point.y);
            #endif
            g_touch_key.down = -1;
            g_touch_key.valid = 0;
            g_touch_key.touch_x = 0;
            g_touch_key.touch_y = 0;
            break;
        default:
            break;
        }
    }
    
    return;

    #else
    /*Save the pressed coordinates and the state*/
    if(touchpad_is_pressed()) {
        touchpad_get_xy(&last_x, &last_y);
        // LOG("x y = %d, %d", last_x, last_y);
        // OsLog(LOG_DEBUG,"last_x, last_y: %d, %d", last_x, last_y);
        // todo 这里先做一个防飘的处理，后续由底层处理
        if(last_x > 320 || last_y > 240)
        {
            return;
        }
        data->state = LV_INDEV_STATE_PR;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;
    #endif
}

typedef enum{
    KM_KEY_1 = '1',
    KM_KEY_2 = '2',
    KM_KEY_3 = '3',
    KM_KEY_4 = '4',
    KM_KEY_5 = '5',
    KM_KEY_6 = '6',
    KM_KEY_7 = '7',
    KM_KEY_8 = '8',
    KM_KEY_9 = '9',
    KM_KEY_0 = '0',
    KM_KEY_CANCEL = KEY_CANCEL,
    KM_KEY_CLEAR = KEY_CLEAR,
    KM_KEY_ENTER = KEY_ENTER,
    KM_KEY_UP = KEY_NEXT,
    KM_KEY_DOWN = KEY_NEXT,
    KM_KEY_POWER = KEY_POWER,
}KEYPAD_MAP;

typedef enum{
    LV_INDEV_KEYBOARD = 1,
    LV_INDEV_TOUCHPAD = 3,
}LV_INDEV_TYPE;

static bool touchpad_is_pressed(void)
{    
    #if 0
    int nRet;
    struct input_event event;

    nRet = Disp_GetTouchPadLinux(&event, 1);
    if (nRet == 0 && event.type == LV_INDEV_TOUCHPAD)
    {
        // OsLog(LOG_DEBUG,"event.code = %d",event.code);
        // OsLog(LOG_DEBUG,"event.type = %d",event.type);
        // OsLog(LOG_DEBUG,"event.value = %d",event.value);
        last_x = event.value;
        return true;
    }
    #endif

    return false;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{    
    int nRet;
    struct input_event event;

    nRet = Disp_GetTouchPadLinux(&event, 1);
    if (nRet == 0 && event.type == LV_INDEV_TOUCHPAD && event.code == 54)
    {
        // OsLog(LOG_DEBUG,"Disp_GetKeyAndTouchpadX = %d",nRet);
        // OsLog(LOG_DEBUG,"event.code = %d",event.code);
        // OsLog(LOG_DEBUG,"event.type = %d",event.type);
        // OsLog(LOG_DEBUG,"event.value = %d",event.value);
        last_y = event.value;
        return;
    }
    last_y = last_x = 0;
    return;
}

/*------------------
 * Mouse
 * -----------------*/

/*Initialize your mouse*/
static void mouse_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the mouse*/
static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    /*Get the current x and y coordinates*/
    mouse_get_xy(&data->point.x, &data->point.y);

    /*Get whether the mouse button is pressed or released*/
    if(mouse_is_pressed()) {
        data->state = LV_INDEV_STATE_PR;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }
}

/*Return true is the mouse button is pressed*/
static bool mouse_is_pressed(void)
{
    /*Your code comes here*/

    return false;
}

/*Get the x and y coordinates if the mouse is pressed*/
static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/

    (*x) = 0;
    (*y) = 0;
}

/*------------------
 * Keypad
 * -----------------*/

/*Initialize your keypad*/
static void keypad_init(void)
{

}

/*Will be called by the library to read the mouse*/
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint32_t last_key = 0;

    /*Get whether the a key is pressed and save the pressed key*/
    uint32_t act_key = keypad_get_key();
    if(act_key != 0) {
        data->state = LV_INDEV_STATE_PR;
        last_key = act_key;
        // OsLog(LOG_DEBUG,"last_key = %d",last_key);
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;
}

/*Get the currently being pressed key.  0 if no key is pressed*/
static uint32_t keypad_get_key(void)
{
    int nRet;
    struct input_event event;
    
    nRet = Disp_GetKeyLinux(&event, 10);
    if (nRet == 0 && event.type == LV_INDEV_KEYBOARD && event.value == 1)
    {
        if (event.code != 330)
        {
            // OsLog(LOG_DEBUG,"event.code = %d",event.code);
            // OsLog(LOG_DEBUG,"event.type = %d",event.type);
            // OsLog(LOG_DEBUG,"event.value = %d",event.value);
            return event.code;
        }
    }

    return 0;
}

/*------------------
 * Encoder
 * -----------------*/

/*Initialize your keypad*/
static void encoder_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{

    data->enc_diff = encoder_diff;
    data->state = encoder_state;
}

/*Call this function in an interrupt to process encoder events (turn, press)*/
static void encoder_handler(void)
{
    /*Your code comes here*/

    encoder_diff += 0;
    encoder_state = LV_INDEV_STATE_REL;
}

/*------------------
 * Button
 * -----------------*/

/*Initialize your buttons*/
static void button_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the button*/
static void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{

    static uint8_t last_btn = 0;

    /*Get the pressed button's ID*/
    int8_t btn_act = button_get_pressed_id();

    if(btn_act >= 0) {
        data->state = LV_INDEV_STATE_PR;
        last_btn = btn_act;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Save the last pressed button's ID*/
    data->btn_id = last_btn;
}

/*Get ID  (0, 1, 2 ..) of the pressed button*/
static int8_t button_get_pressed_id(void)
{
    uint8_t i;

    /*Check to buttons see which is being pressed (assume there are 2 buttons)*/
    for(i = 0; i < 2; i++) {
        /*Return the pressed button's ID*/
        if(button_is_pressed(i)) {
            return i;
        }
    }

    /*No button pressed*/
    return -1;
}

/*Test if `id` button is pressed or not*/
static bool button_is_pressed(uint8_t id)
{

    /*Your code comes here*/

    return false;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
