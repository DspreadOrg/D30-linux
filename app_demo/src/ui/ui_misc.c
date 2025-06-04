#include "appinc.h"

#define BAR_HEIGHT      20
#define MODAL_WIDTH     237
#define MODAL_HEIGHT    153
#define MODAL_MASK_CLOR lv_color_hex(0x000000)
#define MODAL_BG_CLOR   lv_color_hex(0xFFFFFF)
#define MODAL_BTN_CLOR  lv_color_hex(0x008CFF)
#define MODAL_CD_CLOR  lv_color_hex(0xEBEFF4)
#define MODAL_TITLE_FONT    (&ali_middle_18)
#define MODAL_CONTEN_FONT   (&ali_middle_14)
#define MODAL_CD_FONT       (&ali_middle_14)

static lv_obj_t *ui_element_modal = NULL;
static lv_timer_t *lv_elemelt_modal_timer = NULL;
static int modal_countdown = 0;
static lv_obj_t * cancel_button = NULL;
static lv_obj_t * confirm_button = NULL;
static em_modal_result modal_result = MODAL_RESULT_NULL;
static modal_cb modal_func_cb = NULL;


// Pop up timer callback cycle
#define MODAL_TIMER_CALLBACK_PERIOD 200

void ui_modal_delete(void)
{
    if (NULL != lv_elemelt_modal_timer)
    {
        lv_timer_pause(lv_elemelt_modal_timer);
        lv_timer_del(lv_elemelt_modal_timer);
        lv_elemelt_modal_timer = NULL;
    }
    
    if (NULL != ui_element_modal)
    {
        lv_obj_del(ui_element_modal);
        ui_element_modal = NULL;
    }
    
    cancel_button = NULL;
    confirm_button = NULL;
}

static void ui_modal_delete_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    
    if(event_code == LV_EVENT_DELETE)
    {
        if (NULL != lv_elemelt_modal_timer)
        {
            lv_timer_pause(lv_elemelt_modal_timer);
            lv_timer_del(lv_elemelt_modal_timer);
            lv_elemelt_modal_timer = NULL;
        }
        
        if (NULL != ui_element_modal)
        {
            ui_element_modal = NULL;
        }
        
        cancel_button = NULL;
        confirm_button = NULL;
    }

}

static void lv_event_modal_timer(struct _lv_timer_t *t)
{    
    lv_obj_t *input_label = (lv_obj_t *)t->user_data;

    if (NULL != input_label)     //Update Countdown
    {
        modal_countdown = modal_countdown - MODAL_TIMER_CALLBACK_PERIOD;
        if (modal_countdown > 0)
        {
            lv_label_set_text_fmt(input_label, "(%ds)", modal_countdown / 1000 + 1);
            lv_obj_set_style_text_color(input_label,lv_color_hex(0x7B7B7B),0);
        }
        if (modal_countdown <= 0)
        {
            modal_result = MODAL_RESULT_TIMEOUT;
            ui_modal_delete();
            if (NULL != modal_func_cb)
            {
                modal_func_cb(&modal_result);
            }
        }
    }
    else
    {
        //Waiting for the business to release the lock event;
        if(!event_thread_mutex_status())
        {
            modal_result = MODAL_RESULT_QUEUE;
            ui_modal_delete();
            if (NULL != modal_func_cb)
            {
                modal_func_cb(&modal_result);
            }
        }
    }
    
    return;
}

static void lv_event_modal_click(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    
    if(event_code == LV_EVENT_CLICKED)
    {
        modal_result = (em_modal_result)lv_event_get_user_data(e);
        ui_modal_delete();    
        if (modal_result != MODAL_RESULT_CANCEL && NULL != modal_func_cb)
        {
            modal_func_cb(&modal_result);
        }
        return;
    }
}

static lv_obj_t * ui_dialog_basic_construct(em_modal_type type, char* title, char* content, bool new_pad, char cIsHidden)
{
    lv_obj_t *return_obj = NULL;
    lv_obj_t *licon_obj = NULL;

    if (ui_element_modal != NULL)
    {
        ui_modal_delete();
    }
    modal_result = MODAL_RESULT_NULL;
    ui_element_modal = lv_obj_create(Main_Panel);    
    lv_obj_set_size(ui_element_modal, 480, 854);
    lv_obj_set_pos(ui_element_modal, 0, 0);
    lv_obj_set_style_bg_color(ui_element_modal, MODAL_MASK_CLOR, 0);
    lv_obj_set_style_bg_opa(ui_element_modal, LV_OPA_80, 0);
    lv_obj_set_style_radius(ui_element_modal,0,0);
    lv_obj_set_style_border_width(ui_element_modal,0,0);
    lv_obj_add_event_cb(ui_element_modal, ui_modal_delete_cb, LV_EVENT_DELETE, NULL);
    if(cIsHidden)
    {
        lv_obj_add_flag(ui_element_modal, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(ui_element_modal, LV_OBJ_FLAG_HIDDEN);
    }
    return_obj = ui_element_modal;
    if (new_pad)
    {
        lv_obj_t * msg_pad = lv_obj_create(ui_element_modal);
        lv_obj_set_size(msg_pad, MODAL_WIDTH, MODAL_HEIGHT);
        lv_obj_align(msg_pad, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_bg_color(msg_pad,MODAL_BG_CLOR,0);
        lv_obj_clear_flag(msg_pad, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(msg_pad, 6, 0);
        lv_obj_set_style_pad_all(msg_pad,0,0);
        return_obj = msg_pad;
    }
    
    if (title != NULL)
    {
        lv_obj_t *titleLabel = lv_label_create(return_obj);
        lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 200);
        lv_label_set_text(titleLabel, title);
        lv_obj_set_width(titleLabel, LV_PCT(100));
        lv_label_set_long_mode(titleLabel, LV_LABEL_LONG_WRAP);
        lv_obj_set_style_text_align(titleLabel, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(titleLabel, &ali_middle_24, 0);
        if (!new_pad)
        {
            lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 200);
            lv_obj_set_style_text_color(titleLabel,MODAL_BG_CLOR,0);
        }
    }

    if (content != NULL)
    {
        lv_obj_t *contentLabel = lv_label_create(return_obj);
        lv_obj_align(contentLabel, LV_ALIGN_TOP_MID, 0, 360);
        lv_label_set_text(contentLabel, content);
        lv_obj_set_width(contentLabel, LV_PCT(100));
        lv_label_set_long_mode(contentLabel, LV_LABEL_LONG_WRAP);
        lv_obj_set_style_text_align(contentLabel, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(contentLabel, &ali_middle_24, 0);
        if (!new_pad)
        {
            lv_obj_align(contentLabel, LV_ALIGN_TOP_MID, 0, 360);
            lv_obj_set_style_text_color(contentLabel,MODAL_BG_CLOR,0);
        }
    }

    if (type == MODAL_TYPE_DEFAULT)
    {
        return return_obj;
    }
    if (type == MODAL_TYPE_SUCCESS)
    {
        licon_obj = lv_img_create(return_obj);
        ui_lv_img_set_src(licon_obj, (char*)"success.png");
    }
    else if (type == MODAL_TYPE_WAITING)
    {
        licon_obj = lv_img_create(return_obj);
        ui_lv_img_set_src(licon_obj, (char*)"wait_white.png");
    }
    else if (type == MODAL_TYPE_ERROR)
    {
        licon_obj = lv_img_create(return_obj);
        ui_lv_img_set_src(licon_obj, (char*)"fail.png");
    }
    else if (type == MODAL_TYPE_WARNING)
    {
        licon_obj = lv_img_create(return_obj);
        ui_lv_img_set_src(licon_obj, (char*)"fail.png");
    }

    lv_obj_align(licon_obj, LV_ALIGN_TOP_MID, 0, 240);
    if (!new_pad)
    {
        lv_obj_align(licon_obj, LV_ALIGN_TOP_MID, 0, 240);
    }
    // if (title == NULL || strcmp(title, " ") == 0)
    // {
    //     // lv_obj_align_to(contentLabel, licon_obj, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    // }
    // else
    // {
    //     lv_obj_align_to(licon_obj, titleLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    //     lv_obj_align_to(contentLabel, licon_obj, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    // }

    return return_obj;
}

// Blocking dialog box, return button
static em_modal_result ui_modal_dialog(em_modal_type type, char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback)
{
    lv_obj_t *msg_pad = ui_dialog_basic_construct(type, title, content, false, 0);    
    // lv_obj_t *label_pad = lv_obj_get_child(msg_pad, 0);

    modal_func_cb = callback;
    if(str_cancel == NULL)
    {
        if (str_confirm != NULL)
        {
            confirm_button = lv_btn_create(msg_pad);
            lv_obj_set_size(confirm_button, LV_PCT(100), 35);
            lv_obj_set_style_radius(confirm_button, 0, 0);
            lv_obj_align(confirm_button, LV_ALIGN_BOTTOM_MID, 0, 0);
            lv_obj_set_style_bg_color(confirm_button, MODAL_BTN_CLOR, 0);
            // lv_obj_set_style_border_width(confirm_button,2,0);
            // lv_obj_set_style_border_color(confirm_button,lv_color_hex(0xBCBCBC),0);
            lv_obj_add_event_cb(confirm_button, lv_event_modal_click, LV_EVENT_ALL, (void*)MODAL_RESULT_CONFIRM);

            lv_obj_t *confirmLabel = lv_label_create(confirm_button);
            lv_label_set_text(confirmLabel, str_confirm);
            lv_obj_align(confirmLabel, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_align(confirmLabel, LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_add_flag(confirmLabel, LV_OBJ_FLAG_EVENT_BUBBLE);

        }
    }
    else if (str_cancel != NULL && str_confirm != NULL)
    {
        cancel_button = lv_btn_create(msg_pad);
        lv_obj_set_size(cancel_button, LV_PCT(50), 35);
        lv_obj_set_style_radius(cancel_button, 0, 0);
        lv_obj_align(cancel_button, LV_ALIGN_BOTTOM_LEFT, 0, 0);
        lv_obj_set_style_bg_opa(cancel_button, 0, 0);
        lv_obj_set_style_bg_opa(cancel_button, 125, LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(cancel_button, MODAL_BTN_CLOR, LV_STATE_PRESSED);
        lv_obj_add_event_cb(cancel_button, lv_event_modal_click, LV_EVENT_ALL, (void*)MODAL_RESULT_CANCEL);

        lv_obj_t *cancelLabel = lv_label_create(cancel_button);
        lv_label_set_text(cancelLabel, str_cancel);
        lv_obj_set_style_text_align(cancelLabel, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(cancelLabel, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(cancelLabel, lv_event_modal_click, LV_EVENT_ALL, (void*)MODAL_RESULT_CANCEL);

        confirm_button = lv_btn_create(msg_pad);
        lv_obj_move_to_index(confirm_button, 0);
        lv_obj_set_size(confirm_button, LV_PCT(50), 35);
        lv_obj_set_style_radius(confirm_button, 0, 0);
        lv_obj_align(confirm_button, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
        lv_obj_set_style_bg_opa(confirm_button, 0, 0);
        lv_obj_set_style_bg_opa(confirm_button, 125, LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(confirm_button, MODAL_BTN_CLOR, LV_STATE_PRESSED);
        lv_obj_add_event_cb(confirm_button, lv_event_modal_click, LV_EVENT_ALL, (void*)MODAL_RESULT_CONFIRM);

        lv_obj_t *confirmLabel = lv_label_create(confirm_button);
        lv_obj_set_style_text_align(confirmLabel, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(confirmLabel, LV_ALIGN_CENTER, 0, 0);
        lv_label_set_text(confirmLabel, str_confirm);
        lv_obj_set_style_text_opa(confirmLabel, LV_OPA_100, 0);
        lv_obj_add_flag(confirmLabel, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_add_event_cb(confirmLabel, lv_event_modal_click, LV_EVENT_ALL, (void*)MODAL_RESULT_CONFIRM);


        lv_obj_t * seperator = lv_obj_create(msg_pad);
        lv_obj_set_size(seperator, LV_PCT(100), 1);
        lv_obj_align(seperator, LV_ALIGN_BOTTOM_MID, 0, -36);
        // lv_obj_set_style_bg_color(seperator, COLOR_GREY, 0);
        seperator = lv_obj_create(msg_pad);
        lv_obj_set_size(seperator, 1, 35);
        lv_obj_align(seperator, LV_ALIGN_BOTTOM_MID, 0, 0);
        // lv_obj_set_style_bg_color(seperator, COLOR_GREY, 0);
    }

    if (time_out > 0)
    {
        modal_countdown = time_out * 1000;
        lv_obj_t *obj_countdown_label = lv_label_create(msg_pad);
        lv_obj_set_style_text_font(obj_countdown_label, MODAL_CD_FONT, 0);
        lv_obj_set_style_text_color(obj_countdown_label, MODAL_CD_CLOR, 0);
        lv_label_set_text_fmt(obj_countdown_label, "%d", time_out);
        lv_obj_align(obj_countdown_label, LV_ALIGN_TOP_RIGHT, -10, 10);
        lv_elemelt_modal_timer = lv_timer_create(lv_event_modal_timer, MODAL_TIMER_CALLBACK_PERIOD, (void*)obj_countdown_label);
        lv_timer_set_repeat_count(lv_elemelt_modal_timer, modal_countdown);
    }

    return modal_result;
}

em_modal_result ui_modal_result(void)
{
    return modal_result;
}

void ui_modal_close(void)
{
    ui_modal_delete();
}

bool ui_madal_is_run(void)
{
    if (ui_element_modal == NULL)
    {
        return false;
    }
    return true;
    
}
// Blocking dialog box, return button
em_modal_result ui_modal_dialog_for_handle(char* title, char* content, int time_out, modal_cb callback)
{


    lv_obj_t *msg_pad = ui_dialog_basic_construct(MODAL_TYPE_WAITING, title, content, false, 0);
    modal_func_cb = callback;
    if (time_out > 0)
    {
        modal_countdown = time_out * 1000;
        lv_obj_t *obj_countdown_label = lv_label_create(msg_pad);
        lv_obj_set_style_text_font(obj_countdown_label, MODAL_CD_FONT, 0);
        lv_obj_set_style_text_color(obj_countdown_label, MODAL_CD_CLOR, 0);
        lv_label_set_text_fmt(obj_countdown_label, "%d", time_out);
        lv_obj_align(obj_countdown_label, LV_ALIGN_TOP_RIGHT, -10, 10);
        lv_elemelt_modal_timer = lv_timer_create(lv_event_modal_timer, MODAL_TIMER_CALLBACK_PERIOD, (void*)obj_countdown_label);
        lv_timer_set_repeat_count(lv_elemelt_modal_timer, modal_countdown);
    }
    else
    {
        lv_elemelt_modal_timer = lv_timer_create(lv_event_modal_timer, MODAL_TIMER_CALLBACK_PERIOD, NULL);
        lv_timer_set_repeat_count(lv_elemelt_modal_timer, -1);
    }
    
    return modal_result;
}

em_modal_result ui_modal_dialog_by_type(char* title, em_modal_type type,char* content, int time_out, modal_cb callback)
{
    
    lv_obj_t *msg_pad = ui_dialog_basic_construct(type, title, content, false, 0);
    modal_func_cb = callback;
    if (time_out > 0)
    {
        modal_countdown = time_out * 1000;
        lv_obj_t *obj_countdown_label = lv_label_create(msg_pad);
        lv_obj_set_style_text_font(obj_countdown_label, MODAL_CD_FONT, 0);
        lv_obj_set_style_text_color(obj_countdown_label, MODAL_CD_CLOR, 0);
        lv_label_set_text_fmt(obj_countdown_label, "%d", time_out);
        lv_obj_align(obj_countdown_label, LV_ALIGN_TOP_RIGHT, -10, 10);
        lv_elemelt_modal_timer = lv_timer_create(lv_event_modal_timer, MODAL_TIMER_CALLBACK_PERIOD, (void*)obj_countdown_label);
        lv_timer_set_repeat_count(lv_elemelt_modal_timer, modal_countdown);
    }
    else
    {
        lv_elemelt_modal_timer = lv_timer_create(lv_event_modal_timer, MODAL_TIMER_CALLBACK_PERIOD, NULL);
        lv_timer_set_repeat_count(lv_elemelt_modal_timer, -1);
    }

    return modal_result;
}

em_modal_result ui_modal_dialog_for_comm_hidden(char* title, char* content, int time_out, modal_cb callback)
{
    if(!event_thread_mutex_status())    //锁已经释放直接关闭退出
    {
        return MODAL_RESULT_NULL;   
    }

    lv_obj_t *msg_pad = ui_dialog_basic_construct(MODAL_TYPE_WAITING, title, content, false, 1);
    modal_func_cb = callback;
    lv_obj_add_flag(msg_pad, LV_OBJ_FLAG_HIDDEN);
    if (time_out > 0)
    {
        modal_countdown = time_out * 1000;
        lv_obj_t *obj_countdown_label = lv_label_create(msg_pad);
        lv_obj_set_style_text_font(obj_countdown_label, MODAL_CD_FONT, 0);
        lv_obj_set_style_text_color(obj_countdown_label, MODAL_CD_CLOR, 0);
        lv_label_set_text_fmt(obj_countdown_label, "%d", time_out);
        lv_obj_align(obj_countdown_label, LV_ALIGN_TOP_RIGHT, -10, 10);
        lv_elemelt_modal_timer = lv_timer_create(lv_event_modal_timer, MODAL_TIMER_CALLBACK_PERIOD, (void*)obj_countdown_label);
        lv_timer_set_repeat_count(lv_elemelt_modal_timer, modal_countdown);
    }
    else
    {
        lv_elemelt_modal_timer = lv_timer_create(lv_event_modal_timer, MODAL_TIMER_CALLBACK_PERIOD, NULL);
        lv_timer_set_repeat_count(lv_elemelt_modal_timer, -1);
    }

    return modal_result;    
}

em_modal_result ui_modal_dialog_nomal(char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback)
{
    return ui_modal_dialog(MODAL_TYPE_DEFAULT, title, content, time_out, str_cancel, str_confirm, callback);
}

em_modal_result ui_modal_dialog_error(char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback)
{
    return ui_modal_dialog(MODAL_TYPE_ERROR, title, content, time_out, str_cancel, str_confirm, callback);
}

em_modal_result ui_modal_dialog_success(char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback)
{
    return ui_modal_dialog(MODAL_TYPE_SUCCESS, title, content, time_out, str_cancel, str_confirm, callback);
}

em_modal_result ui_modal_dialog_waiting(char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback)
{
    return ui_modal_dialog(MODAL_TYPE_WAITING, title, content, time_out, str_cancel, str_confirm, callback);
}

em_modal_result ui_modal_dialog_warning(char* title, char* content, int time_out, char* str_cancel, char* str_confirm, modal_cb callback)
{
    return ui_modal_dialog(MODAL_TYPE_WARNING, title, content, time_out, str_cancel, str_confirm, callback);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

// BMP 文件头和信息头结构体
typedef struct {
    char bfType[2];        // 文件类型，必须是 "BM"
    char bfSize[4];        // 文件大小,小端模式
    char bfReserved1[2];   // 保留字段
    char bfReserved2[2];   // 保留字段
    char bfOffBits[4];     // 像素数据偏移量,小端模式
} BMPFileHeader;

typedef struct {
    char biSize[4];        // 信息头大小
    char biWidth[4];       // 图像宽度
    char biHeight[4];      // 图像高度
    char biPlanes[2];      // 颜色平面数（必须为 1）
    char biBitCount[2];    // 每像素位数
    char biCompression[4]; // 压缩类型
    char biSizeImage[4];   // 图像数据大小
    char biXPelsPerMeter[4]; // 水平分辨率
    char biYPelsPerMeter[4]; // 垂直分辨率
    char biClrUsed[4];     // 使用的颜色数
    char biClrImportant[4]; // 重要颜色数
} BMPInfoHeader;

// 调色板（单色 BMP 需要调色板）
static uint32_t palette[2] = {
    0x00000000, // 黑色
    0x00FFFFFF  // 白色
};

#include <stdio.h>

static void int_to_little_endian_char_array(int value, unsigned char *buffer, int buffer_len) {
    
    for (size_t i = 0; i < buffer_len; i++)
    {
        buffer[i] = (value >> i*8) & 0xFF;  // 最低字节
    }
    return;
}

// 将 lv_canvas 保存为单色 BMP 文件
int lv_canvas_save_as_mono_bmp(sign_buf_t *bitmap, const char *filename) {

    // 计算 BMP 文件大小
    int row_size = (bitmap->w + 31) / 32 * 4; // 每行字节数（按 4 字节对齐）
    int image_size = row_size * bitmap->h;    // 图像数据大小
    int file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(palette) + image_size;

    // 创建 BMP 文件头
    BMPFileHeader file_header = {
        .bfType = "BM", // "BM"
    };
    int_to_little_endian_char_array(file_size,file_header.bfSize,sizeof(file_header.bfSize));
    int_to_little_endian_char_array(sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(palette),file_header.bfOffBits,sizeof(file_header.bfOffBits));
    // 创建 BMP 信息头
    BMPInfoHeader info_header = {0};
    
    int_to_little_endian_char_array(sizeof(BMPInfoHeader),info_header.biSize,sizeof(info_header.biSize));
    int_to_little_endian_char_array(bitmap->w,info_header.biWidth,sizeof(info_header.biWidth));
    int_to_little_endian_char_array(bitmap->h,info_header.biHeight,sizeof(info_header.biHeight));
    int_to_little_endian_char_array(1,info_header.biPlanes,sizeof(info_header.biPlanes));
    int_to_little_endian_char_array(1,info_header.biBitCount,sizeof(info_header.biBitCount));
    int_to_little_endian_char_array(0,info_header.biCompression,sizeof(info_header.biCompression));
    int_to_little_endian_char_array(file_size-(sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(palette)),info_header.biSizeImage,sizeof(info_header.biSizeImage));
    int_to_little_endian_char_array(2835,info_header.biXPelsPerMeter,sizeof(info_header.biXPelsPerMeter));
    int_to_little_endian_char_array(2835,info_header.biYPelsPerMeter,sizeof(info_header.biYPelsPerMeter));
    int_to_little_endian_char_array(0,info_header.biClrUsed,sizeof(info_header.biClrUsed));
    int_to_little_endian_char_array(0,info_header.biClrImportant,sizeof(info_header.biClrImportant));

    // 打开文件
    FILE *file = fopen(filename, "wb");
    OsLog(LOG_DEBUG,"%s filename = %s\n",__func__,filename);
    if (!file) {
        OsLog(LOG_ERROR,"Failed to open file: %d\n",file);
        return -1;
    }

    // 写入文件头和信息头
    fwrite(&file_header, sizeof(BMPFileHeader), 1, file);
    fwrite(&info_header, sizeof(BMPInfoHeader), 1, file);

    // 写入调色板
    fwrite(palette, sizeof(palette), 1, file);

    // 写入像素数据
    fwrite(bitmap->buf, row_size*bitmap->h, 1, file);
    fclose(file);
    OsLog(LOG_DEBUG,"Mono BMP file saved\n");
    return 0;
}
// 小端模式转换辅助函数
uint32_t readU32(const char* bytes) {
    return (uint32_t)((unsigned char)bytes[0]) |
           ((uint32_t)((unsigned char)bytes[1]) << 8) |
           ((uint32_t)((unsigned char)bytes[2]) << 16) |
           ((uint32_t)((unsigned char)bytes[3]) << 24);
}

uint16_t readU16(const char* bytes) {
    return (uint16_t)((unsigned char)bytes[0]) |
           ((uint16_t)((unsigned char)bytes[1]) << 8);
}

void writeU32(char* bytes, uint32_t value) {
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;
}

void writeU16(char* bytes, uint16_t value) {
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
}

// 检查颜色是否为白色（RGB值均为255）
int is_white(uint8_t r, uint8_t g, uint8_t b) {
    return (r == 255 && g == 255 && b == 255);
}

int process_bmp(const char* input_file, const char* output_file, int target_width) {
    FILE* fp_in = fopen(input_file, "rb");
    if (!fp_in) {
        OsLog(LOG_DEBUG,"OPen File Failed");
        return -1;
    }

    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    
    fread(&file_header, sizeof(BMPFileHeader), 1, fp_in);
    fread(&info_header, sizeof(BMPInfoHeader), 1, fp_in);

    // 检查是否为BMP文件
    if (file_header.bfType[0] != 'B' || file_header.bfType[1] != 'M') {
        OsLog(LOG_DEBUG,"BMP File Err\n");
        fclose(fp_in);
        return -1;
    }

    // 检查是否为24位BMP或1位BMP
    uint16_t bit_count = readU16(info_header.biBitCount);
    if (bit_count != 1) {
        OsLog(LOG_DEBUG,"Only Support 1bit BMP\n");
        fclose(fp_in);
        return -1;
    }

    // 读取调色板（仅1位位图需要）
    uint32_t palette[2] = {0xFFFFFFFF, 0x00000000}; // 默认白黑调色板
    if (bit_count == 1) {
        fread(palette, sizeof(uint32_t), 2, fp_in);
    }

    // 计算原始图像参数
    uint32_t original_width = readU32(info_header.biWidth);
    uint32_t original_height = readU32(info_header.biHeight);
    if (original_height & 0x80000000) {
        original_height = -original_height; // 处理负高度(从上到下存储)
    }
    
    uint32_t row_padded = ((original_width * bit_count + 31) / 32) * 4;
    
    // 分配内存读取图像数据
    uint8_t* image_data = (uint8_t*)malloc(row_padded * original_height);
    uint32_t data_offset = readU32(file_header.bfOffBits);
    fseek(fp_in, data_offset, SEEK_SET);
    fread(image_data, 1, row_padded * original_height, fp_in);
    fclose(fp_in);

    // 寻找上下非白边边界
    uint32_t top = 0, bottom = original_height - 1;
    
    // 寻找上边界
    for (; top < original_height; top++) {
        int is_row_white = 1;
        for (uint32_t x = 0; x < original_width; x++) {
            // 1位位图处理
            uint32_t byte_offset = top * row_padded + x / 8;
            uint8_t bit_offset = 7 - (x % 8);
            uint8_t pixel = (image_data[byte_offset] >> bit_offset) & 1;
            uint32_t color = palette[pixel];
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;
            
            if (!is_white(r, g, b)) {
                is_row_white = 0;
                break;
            }
    
        }
        if (!is_row_white) break;
    }

    // 寻找下边界
    for (; bottom >= top; bottom--) {
        int is_row_white = 1;
        for (uint32_t x = 0; x < original_width; x++) {
            // 1位位图处理
            uint32_t byte_offset = bottom * row_padded + x / 8;
            uint8_t bit_offset = 7 - (x % 8);
            uint8_t pixel = (image_data[byte_offset] >> bit_offset) & 1;
            uint32_t color = palette[pixel];
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;
            
            if (!is_white(r, g, b)) {
                is_row_white = 0;
                break;
            }
        }
        if (!is_row_white) break;
    }

    // 计算新高度
    uint32_t new_height = bottom - top + 1;
    if (new_height <= 0) {
        OsLog(LOG_DEBUG, "Bmp is Empty\n");
        free(image_data);
        return -1;
    }

    // 计算宽度缩放比例
    float scale = (float)target_width / original_width;
    uint32_t new_width = target_width;
    
    // 计算新图像每行字节数
    uint32_t new_row_padded = ((new_width * bit_count + 31) / 32) * 4;
    
    // 分配内存存储新图像数据
    uint8_t* new_image_data = (uint8_t*)calloc(new_row_padded * new_height, 1);

    // 图像缩放处理
    if (bit_count == 24) {
        // 24位色图缩放
        for (uint32_t y = 0; y < new_height; y++) {
            uint32_t orig_y = top + y;
            for (uint32_t x = 0; x < new_width; x++) {
                uint32_t orig_x = (uint32_t)(x / scale);
                if (orig_x >= original_width) orig_x = original_width - 1;
                
                uint32_t orig_offset = orig_y * row_padded + orig_x * 3;
                uint32_t new_offset = y * new_row_padded + x * 3;
                
                new_image_data[new_offset] = image_data[orig_offset];     // B
                new_image_data[new_offset + 1] = image_data[orig_offset + 1]; // G
                new_image_data[new_offset + 2] = image_data[orig_offset + 2]; // R
            }
        }
    } else {
        // 1位位图缩放
        for (uint32_t y = 0; y < new_height; y++) {
            uint32_t orig_y = top + y;
            for (uint32_t x = 0; x < new_width; x++) {
                uint32_t orig_x = (uint32_t)(x / scale);
                if (orig_x >= original_width) orig_x = original_width - 1;
                
                // 获取原始像素
                uint32_t orig_byte_offset = orig_y * row_padded + orig_x / 8;
                uint8_t orig_bit_offset = 7 - (orig_x % 8);
                uint8_t orig_pixel = (image_data[orig_byte_offset] >> orig_bit_offset) & 1;
                
                // 设置新像素
                uint32_t new_byte_offset = y * new_row_padded + x / 8;
                uint8_t new_bit_offset = 7 - (x % 8);
                if (orig_pixel) {
                    new_image_data[new_byte_offset] |= (1 << new_bit_offset);
                } else {
                    new_image_data[new_byte_offset] &= ~(1 << new_bit_offset);
                }
            }
        }
    }

    // 更新文件头和信息头
    writeU32(info_header.biWidth, new_width);
    writeU32(info_header.biHeight, new_height);
    uint32_t new_size_image = new_row_padded * new_height;
    writeU32(info_header.biSizeImage, new_size_image);
    
    uint32_t new_file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + 
                            (bit_count == 1 ? 8 : 0) + new_size_image;
    writeU32(file_header.bfSize, new_file_size);
    
    // 写入新文件
    FILE* fp_out = fopen(output_file, "wb");
    if (!fp_out) {
        OsLog(LOG_DEBUG,"Create New Bmp Failed");
        free(image_data);
        free(new_image_data);
        return -1;
    }
    
    fwrite(&file_header, sizeof(BMPFileHeader), 1, fp_out);
    fwrite(&info_header, sizeof(BMPInfoHeader), 1, fp_out);
    
    // 写入调色板（仅1位位图需要）
    if (bit_count == 1) {
        fwrite(palette, sizeof(uint32_t), 2, fp_out);
    }
    
    fwrite(new_image_data, 1, new_size_image, fp_out);
    fclose(fp_out);
    
    free(image_data);
    free(new_image_data);
    return 0;
}