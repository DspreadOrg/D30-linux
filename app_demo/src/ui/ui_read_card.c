#include "appinc.h"

static lv_obj_t * grad_pad;
static lv_obj_t * payment_img;
static int payment_index = 0;

static const char* payment_img_path[3] = {"insert2.png", "tap2.png", "swipe2.png"};

static void update_grad_disp(void)
{
    int i = 0;
    lv_obj_t *child = NULL;
    uint32_t childCount = lv_obj_get_child_cnt(grad_pad);

    for(i = 0; i < childCount; i ++)
    {
        child = lv_obj_get_child(grad_pad, i);

        if (i == payment_index)
        {
            lv_obj_add_state(child, LV_STATE_PRESSED);
        }
        else
        {
            lv_obj_clear_state(child, LV_STATE_PRESSED);
        }
    }
}


static void ui_event_close_timer(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_DELETE)
    {
        lv_timer_t* payment_img_timer = event->user_data;
        if(payment_img_timer != NULL)
        {
            lv_timer_del(payment_img_timer);
        }
    }
}


static void payment_img_timer_callback(lv_timer_t * timer)
{
    payment_index ++;
    if (payment_index >= sizeof(payment_img_path)/sizeof(char*))
    {
        payment_index = 0;
    }
    
    ui_lv_img_set_src(payment_img, (char*)payment_img_path[payment_index]);
    update_grad_disp();
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
                OsLog(LOG_DEBUG,"GET KEY CANCLE");
                stop_readcards();
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9: //cancel
                OsLog(LOG_DEBUG,"CLICKED CANCLE");
                stop_readcards();
                break;
        }
    }
}

void ui_read_ic_card(){
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    ui_modal_dialog_for_handle(NULL, "Reading card...",0,NULL);
    lv_timer_enable(true);
}

void ui_create_read_card() {
    char szDisplayAmount[16] = {0};
    int nAmount = atoi(get_transaction_data()->sAmount);
    sprintf(szDisplayAmount,"total $%ld.%02ld",nAmount/100,nAmount%100);

    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    ui_lvgl_page_construct("Sale",touch_key_event_cb);
     
    //3prompt
    lv_obj_t * amount_lable = lv_label_create(Main_Panel);
    lv_label_set_text(amount_lable, szDisplayAmount);
    lv_obj_set_style_text_font(amount_lable, &ali_middle_36, 0);
    lv_obj_align(amount_lable, LV_ALIGN_TOP_MID, 0, 120);
    lv_obj_set_style_text_color(amount_lable, lv_color_hex(0x191E3B), 0);

    #if 1
    payment_img = lv_img_create(Main_Panel);
    payment_index = 0;
    ui_lv_img_set_src(payment_img, (char*)payment_img_path[payment_index]);
    lv_obj_align(payment_img, LV_ALIGN_TOP_MID, 0, 200);
    // lv_obj_set_size(payment_img,96,82);
    lv_timer_t* payment_img_timer = lv_timer_create(payment_img_timer_callback, 1600, NULL);   // Responsible for updating card swiping prompts
    lv_obj_add_event_cb(payment_img, ui_event_close_timer, LV_EVENT_DELETE, payment_img_timer);   // Responsible for turning off the timer

    grad_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(grad_pad, 140, 5);
    lv_obj_align(grad_pad, LV_ALIGN_TOP_MID, 0, 400);
    lv_obj_set_style_border_width(grad_pad, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(grad_pad, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(grad_pad, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(grad_pad,LV_FLEX_ALIGN_SPACE_EVENLY, 0, 0);

    for (size_t i = 0; i < 3; i++)
    {
        lv_obj_t * line = lv_obj_create(grad_pad);
        lv_obj_set_size(line, 20, 2);
        lv_obj_set_style_radius(line, 0, 0);
        lv_obj_set_style_border_width(line, 0, 0);
        lv_obj_set_style_pad_all(line, 0, 0);
        lv_obj_set_style_bg_color(line, lv_color_hex(0x1B1B1B), 0);
        lv_obj_set_style_bg_color(line, lv_color_hex(0xFF0039), LV_STATE_PRESSED);
    }
    update_grad_disp();
    #else
    lv_obj_t * rfid_btn = lv_img_create(Main_Panel);
    lv_obj_align(rfid_btn, LV_ALIGN_TOP_MID, 0, 58);
    ui_lv_img_set_src(rfid_btn, (char*)"icon_NFC.png");

    for(int i = 0;i < 4;i++){
        lv_obj_t * line = lv_obj_create(Main_Panel);
        lv_obj_set_size(line, 10, 2); // Width 200px, height 1px
        lv_obj_align(line, LV_ALIGN_TOP_LEFT, 114+i*24, 150);
        lv_obj_set_style_bg_color(line, lv_color_hex(LV_PALETTE_GREY), 0);
        lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);
    }
    #endif

    lv_obj_t *tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Insert,tap or swipe");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 450);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_36, 0);

    event_trans_register(EVENT_READCARD);
    lv_timer_enable(true);
}

