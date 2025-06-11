#include "appinc.h"

// typedef struct {
// 	unsigned int w;
// 	unsigned int h;
// 	char *buf;
// }sign_buf_t;

#define LCD_DISPLAY_WIDTH       480
#define LCD_DISPLAY_HEIGHT      854
#define SIGNATURE_PAD_WIDTH     460
#define SIGNATURE_PAD_HEIGHT    400
#define MIN_SIGNATURE_DOT       10   // Minimum signature stroke length
#define SIGNATURE_PAD_BG_COLOR  lv_color_hex(0xFFFFFF)
// #define SIGNATURE_PAD_BG_COLOR  lv_color_hex(0x000000)

static uint8_t *sign_pad_buffer = NULL;
static uint32_t SignatureCount = 0;    // Used to detect the signature stroke length, submissions with too short strokes are not allowed
static lv_draw_label_dsc_t feature_label_obj = {0};
static lv_draw_line_dsc_t sign_line_obj;
static lv_obj_t *sign_pad_obj;
static lv_obj_t *button_list[2] = {0};

static void goto_txn_res(void* pra)
{
    event_ui_register(UI_RESULT_SUCCESS);
    return;
};


static void button_signature_confirm(void)
{
    sign_buf_t stEsignBuf= {0};
    #if 0
    char *szbitmap = malloc(SIGNATURE_PAD_HEIGHT*((SIGNATURE_PAD_WIDTH+7)/ 8));

    // Convert the colored part to a bit map
    for(int i = 0; i < SIGNATURE_PAD_HEIGHT; i ++)
    {
        for(int j = 0; j < SIGNATURE_PAD_WIDTH; j ++)
        {
            lv_color16_t color = lv_canvas_get_px(sign_pad_obj, j, i);
            lv_color16_t co = SIGNATURE_PAD_BG_COLOR;
            int offset = i*((SIGNATURE_PAD_WIDTH+7)/ 8)+j/8;
            if(memcmp(&color, &co, sizeof(lv_color16_t)))
            {
                szbitmap[offset] = szbitmap[offset] | (0x80 >> j % 8);
            }
        }
    }
    #else
    char *szbitmap = malloc(SIGNATURE_PAD_HEIGHT*((SIGNATURE_PAD_WIDTH+31)/ 32)*4);
    memset(szbitmap, 0, SIGNATURE_PAD_HEIGHT*((SIGNATURE_PAD_WIDTH+31)/ 32)*4); 
    for (int y = SIGNATURE_PAD_HEIGHT - 1; y >= 0; y--) { // BMP stores data in reverse order
        for (int x = 0; x < SIGNATURE_PAD_WIDTH; x++) {
            uint8_t mono = 1;
            // Get the pixel color (16-bit RGB565)
            lv_color_t color = lv_canvas_get_px(sign_pad_obj, x, y);
            lv_color_t co = SIGNATURE_PAD_BG_COLOR;
            // Convert to monochrome (0 or 1) based on the background
            if(memcmp(&color, &co, sizeof(lv_color16_t))) mono=0;
            // Write the monochrome pixel to the line buffer
            szbitmap[x / 8+((SIGNATURE_PAD_HEIGHT-y-1)*((SIGNATURE_PAD_WIDTH+31)/ 32)*4)] |= (mono << (7 - (x % 8)));
        }
    }
    #endif

    stEsignBuf.h = SIGNATURE_PAD_HEIGHT;
    stEsignBuf.w = SIGNATURE_PAD_WIDTH;
    stEsignBuf.buf = szbitmap;
    char filePath[128] = {0};
    char newFilePath[128] = {0};
    char appPath[128] = {0};

    getcwd(appPath,sizeof(appPath));
    sprintf(filePath,"%s/res/%s_tmp.bmp",appPath,get_transaction_data()->sTrace);
    if (0 == lv_canvas_save_as_mono_bmp(&stEsignBuf,filePath))  //todo Save BMP file
    {
        sprintf(newFilePath,"%s/res/%s.bmp",appPath,get_transaction_data()->sTrace);
        process_bmp(filePath,newFilePath,384);
        remove(filePath);
        event_ui_register(UI_PRINTER);
    }
    else
    {   
       
        char sz_feature_code[8 + 1] = {0};
        lv_canvas_fill_bg(sign_pad_obj, SIGNATURE_PAD_BG_COLOR, LV_OPA_COVER);
        SignatureCount = 0;
        get_feature_code(get_transaction_data()->sTransTime, get_transaction_data()->sTrace, sz_feature_code);
        lv_canvas_draw_text(sign_pad_obj, 0, (SIGNATURE_PAD_HEIGHT-20)/2, SIGNATURE_PAD_WIDTH, &feature_label_obj, sz_feature_code);
        lv_obj_add_state(button_list[1], LV_STATE_DISABLED);
    }
    free(szbitmap);
}

static void button_signature_clear(void)
{
    char sz_feature_code[8 + 1] = {0};

    SignatureCount = 0;
    lv_canvas_fill_bg(sign_pad_obj, SIGNATURE_PAD_BG_COLOR, LV_OPA_COVER);
    get_feature_code(get_transaction_data()->sTransTime, get_transaction_data()->sTrace, sz_feature_code);
    lv_canvas_draw_text(sign_pad_obj, 0, (SIGNATURE_PAD_HEIGHT-20)/2, SIGNATURE_PAD_WIDTH, &feature_label_obj, sz_feature_code);
    lv_obj_add_state(button_list[1], LV_STATE_DISABLED);
    return;
}

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        switch(keyCode){
            case KB_KEY_CANCEL://cancel
                lv_obj_set_style_bg_color(Main_Panel, lv_color_hex(0xFFFFFF), 0);
                event_ui_register(UI_IDLE);
                break;
            case KB_KEY_CLEAR:
                button_signature_clear();
                break;
            case KB_KEY_ENTER:
                lv_obj_set_style_bg_color(Main_Panel, lv_color_hex(0xFFFFFF), 0);
                button_signature_confirm();
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9: //cancel
                lv_obj_set_style_bg_color(Main_Panel, lv_color_hex(0xFFFFFF), 0);   
                event_ui_register(UI_IDLE);
                break;
            case 14:
                button_signature_clear();
                break;
            case 28:
                lv_obj_set_style_bg_color(Main_Panel, lv_color_hex(0xFFFFFF), 0);
                button_signature_confirm();
                break;
        }
    }
}

static void event_signature_draw(lv_event_t * e)
{
    static lv_coord_t last_x, last_y = -32768;
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_PRESSING)
    {
        // LOG("LV_EVENT_PRESSING");
        lv_obj_t * canvas = ( lv_obj_t *)e->user_data;
        lv_indev_t * indev = lv_indev_get_act();
        if(indev == NULL)  return;

        lv_point_t point;
        lv_indev_get_point(indev, &point);
        // Relative coordinates are required here
        point.x = point.x - ((LCD_DISPLAY_WIDTH - SIGNATURE_PAD_WIDTH) / 2);
        /* The Y coordinate needs to be slightly offset upwards. Since it's not a resistive screen, the Y-axis of the stroke should ideally be at the finger's position to prevent the stroke from covering and affecting the experience. However, this will cause a problem: when the finger moves from the bottom edge upwards with no existing strokes, the initial stroke will not appear at the bottom. */ 
        point.y = point.y - ((LCD_DISPLAY_HEIGHT - SIGNATURE_PAD_HEIGHT) / 2); 
        lv_point_t points[2];
        /*Release or first use*/
        if ((last_x == -32768) || (last_y == -32768))
        {
            last_x = point.x;
            last_y = point.y;
        }
        else
        {   
            if(SignatureCount <= 1000 && last_x != point.x && last_y != point.y )   // Actual finger movement
            {
                SignatureCount ++;
            }
            points[0].x = last_x;
            points[0].y = last_y;
            points[1].x = point.x;
            points[1].y = point.y;
            last_x = point.x;
            last_y = point.y;
            lv_canvas_draw_line(canvas, points, 2, &sign_line_obj);
            if(SignatureCount >= MIN_SIGNATURE_DOT)
            {
                lv_obj_clear_state(button_list[1], LV_STATE_DISABLED);
            }
        }
    }
    /*Loosen the brush*/
    else if(code == LV_EVENT_RELEASED)
    {
        last_x = -32768;
        last_y = -32768;
    }
    else if(code == LV_EVENT_DELETE)
    {
        if (sign_pad_buffer != NULL)
        {
            free(sign_pad_buffer);
            sign_pad_buffer = NULL;
        }
    }
}

void ui_create_signature() {
    char sz_feature_code[8+1] = {0};

    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    ui_lvgl_page_construct("Sale",touch_key_event_cb);
    //3prompt
    lv_obj_t * tip_lable = lv_label_create(Main_Panel);                                                                                                                                                                                                                                                                                                                         
    lv_label_set_text(tip_lable, "Please sign your name on the screen");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 150);
    lv_obj_set_style_text_color(tip_lable, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_24, 0);

    if (sign_pad_buffer != NULL)
    {
        free(sign_pad_buffer);
        sign_pad_buffer = NULL;
    }
    sign_pad_buffer = malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR(SIGNATURE_PAD_WIDTH, SIGNATURE_PAD_HEIGHT));
    memset(sign_pad_buffer,0x00,LV_CANVAS_BUF_SIZE_TRUE_COLOR(SIGNATURE_PAD_WIDTH, SIGNATURE_PAD_HEIGHT));
  
    // lv_obj_t *padBorder =lv_obj_create(Main_Panel); 
    // lv_obj_set_size(padBorder, SIGNATURE_PAD_WIDTH, SIGNATURE_PAD_HEIGHT);
    // lv_obj_align(padBorder, LV_ALIGN_TOP_MID, 0, 87);
    // lv_obj_set_style_border_color(padBorder, SIGNATURE_PAD_BG_COLOR, 0);
    // lv_obj_clear_flag(padBorder, LV_OBJ_FLAG_SCROLLABLE);
    // lv_obj_clear_flag(padBorder, LV_OBJ_FLAG_CLICKABLE);
    // lv_obj_set_style_border_width(padBorder, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_radius(padBorder, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(padBorder, SIGNATURE_PAD_BG_COLOR, LV_PART_MAIN | LV_STATE_DEFAULT);

    sign_pad_obj =lv_canvas_create(Main_Panel);
    lv_canvas_set_buffer(sign_pad_obj, sign_pad_buffer, SIGNATURE_PAD_WIDTH, SIGNATURE_PAD_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(sign_pad_obj, SIGNATURE_PAD_BG_COLOR, LV_OPA_COVER);
    lv_obj_align(sign_pad_obj, LV_ALIGN_TOP_MID,0,200);
    lv_obj_add_flag(sign_pad_obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_border_color(sign_pad_obj, lv_color_hex(0xA0B5CB), LV_PART_MAIN);
    lv_obj_set_style_border_width(sign_pad_obj, 1, LV_PART_MAIN);  
    lv_obj_set_style_pad_all(sign_pad_obj, 0, 0);
    lv_obj_clear_flag(sign_pad_obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(sign_pad_obj, 0, 0);

    lv_draw_label_dsc_init(&feature_label_obj);
    feature_label_obj.color = lv_color_hex(0xA0B5CB);
    feature_label_obj.font = &ali_middle_18;
    feature_label_obj.align = LV_TEXT_ALIGN_CENTER;
    get_feature_code(get_transaction_data()->sTransTime, get_transaction_data()->sTrace, sz_feature_code);
    OsLog(LOG_DEBUG,"get_feature_code %s\n",sz_feature_code);
    lv_canvas_draw_text(sign_pad_obj, 0, (SIGNATURE_PAD_HEIGHT-20)/2, SIGNATURE_PAD_WIDTH, &feature_label_obj, sz_feature_code);
    lv_obj_add_event_cb(sign_pad_obj, event_signature_draw, LV_EVENT_ALL, sign_pad_obj);

    lv_draw_line_dsc_init(&sign_line_obj);
    sign_line_obj.color =  lv_color_black();
    sign_line_obj.width = 2;
    sign_line_obj.round_end = 1;
    sign_line_obj.round_start = 1;

    lv_obj_t *sign_clear_button = lv_btn_create(Main_Panel);
    lv_obj_set_size(sign_clear_button, 200, 80);
    lv_obj_align(sign_clear_button, LV_ALIGN_BOTTOM_LEFT, 12, 4);
    lv_obj_set_style_bg_color(sign_clear_button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(sign_clear_button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(sign_clear_button,lv_color_hex(0xBCBCBC), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(sign_clear_button,1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(sign_clear_button, touch_key_event_cb, LV_EVENT_ALL, "14");
    button_list[0] = sign_clear_button;

    lv_obj_t * sign_clear_label =lv_label_create(sign_clear_button);
    lv_label_set_text(sign_clear_label, "Re-sign");
    lv_obj_align(sign_clear_label, LV_ALIGN_CENTER, 0, -3);
    lv_obj_set_style_text_color(sign_clear_label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(sign_clear_label, &ali_middle_24, 0);

    lv_obj_t *sign_comfirm_button = lv_btn_create(Main_Panel);
    lv_obj_set_size(sign_comfirm_button, 200, 80);
    lv_obj_align(sign_comfirm_button, LV_ALIGN_BOTTOM_RIGHT, -12, 4);
    lv_obj_set_style_bg_color(sign_comfirm_button, lv_color_hex(0xFF0039), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_state(sign_comfirm_button, LV_STATE_DISABLED);
    lv_obj_set_style_border_color(sign_clear_button,lv_color_hex(0xBCBCBC),LV_STATE_PRESSED);
    lv_obj_set_style_border_width(sign_clear_button,1, LV_STATE_PRESSED);
    lv_obj_add_event_cb(sign_comfirm_button, touch_key_event_cb, LV_EVENT_ALL, "28");
    button_list[1] = sign_comfirm_button;

    lv_obj_t * sign_comfirm_label =lv_label_create(sign_comfirm_button);
    lv_label_set_text(sign_comfirm_label, "Done");
    lv_obj_align(sign_comfirm_label, LV_ALIGN_CENTER, 0, -3);
    lv_obj_set_style_text_font(sign_comfirm_label, &ali_middle_24, 0);

    lv_timer_enable(true);
}