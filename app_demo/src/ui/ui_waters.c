#include "appinc.h"

#define ITEM_PAD_HIGHT  100
#define LIST_DISP_PER_PAGE 100

static lv_obj_t *search_img = NULL;
static lv_obj_t *search_label = NULL;
static lv_obj_t *record_pad = NULL;
static lv_obj_t *water_list[LIST_DISP_PER_PAGE+1] = {0};
static lv_obj_t *content_pad = NULL;
static int waterindex = -1;     //The currently available focus objects
static int record_index = 0;    //Total flow object, total object+1=optional diagonal object
static int record_dest = -1;
static char trace_bumber_str[12] = {0};
static char* payment_org_icon[] = 
{
    "visa_small.png",
    "master_small.png",
    "unionpay_small.png",   //Change to UNIONPAY in the future
    "jcb_small.png",
    "amex_small.png",
    "dc_small.png",
    "QR_small.png",
};

typedef struct {
    lv_obj_t *target_obj;
    char *laber_name;
    char *event_value;
    long vulue;
} ST_LIMIT_ITEM;

ST_LIMIT_ITEM date_limit[3] =
{
    {NULL,"Today","11",24*60*60},
    {NULL,"3days","12",72*60*60},
    {NULL,"All","13",0}
};

ST_LIMIT_ITEM type_limit[3] =
{
    {NULL,"Paid","21"},
    {NULL,"voided","22"},
    {NULL,"All","23"},
};


static void filled_pad(void);
static void update_menu_state(void)
{
    for (size_t i = 0; i < sizeof(water_list)/sizeof(lv_obj_t*); i++)
    {
        if (i == waterindex)
        {
            if (water_list[i] !=  NULL)
            {
                lv_obj_add_state(water_list[i],LV_STATE_PRESSED);
                // Traverse all sub objects and add PRESSED status
                // if (i>0)
                // {
                //     uint32_t child_cnt = lv_obj_get_child_cnt(water_list[i]);
                //     // OsLog(LOG_DEBUG,"touch_key_event_cb LV_EVENT_PRESSED water_list[%d][0x%x]",i,water_list[i]);
                //     // OsLog(LOG_DEBUG,"touch_key_event_cb LV_EVENT_PRESSED child_cnt[%d]",child_cnt);
                //     for(size_t j = 0; j < child_cnt; j++) {
                //         lv_obj_t * child = lv_obj_get_child(water_list[i], j);
                //         lv_obj_add_state(child, LV_STATE_PRESSED);
                //     }
                // }
            }
            lv_obj_scroll_to_view_recursive(water_list[i],LV_ANIM_ON);
        }
        else
        {
            if (water_list[i] !=  NULL)
            {
                lv_obj_clear_state(water_list[i],LV_STATE_PRESSED);
                // Remove the PRESSED status of sub objects
                // if (i>0)
                // {
                //     uint32_t child_cnt = lv_obj_get_child_cnt(water_list[i]);
                //     // OsLog(LOG_DEBUG,"touch_key_event_cb LV_EVENT_PRESS_LOST water_list[%d][0x%x]",i,water_list[i]);
                //     // OsLog(LOG_DEBUG,"touch_key_event_cb LV_EVENT_PRESS_LOST child_cnt[%d]",child_cnt);
                //     for(size_t j = 0; j < child_cnt; j++) {
                //         lv_obj_t * child = lv_obj_get_child(water_list[i], j);
                //         lv_obj_clear_state(child, LV_STATE_PRESSED);
                //     }
                // }
            }
        }
    }
    return;
}

static void key_down(void)
{
    if (waterindex == -1)
    {
        waterindex = 1;
    }
    else
    {
        waterindex++;
        if (waterindex > record_index)
        {
            waterindex = record_index;
        }
        // if (waterindex > (sizeof(water_list)/sizeof(lv_obj_t*)-1))
        // {
        //     if (strlen(stWaterList[record_index].sOrderNo)>0)
        //     {
        //         filled_pad();
        //         waterindex = 1;
        //     }
        //     else
        //     {
        //         waterindex = (sizeof(water_list)/sizeof(lv_obj_t*)-1);
        //     }
        // }
        // else
        // {
        //     if (water_list[waterindex] == NULL)
        //     {
        //         waterindex--;
        //     }
        // }
    }
    update_menu_state();
    return;
}

static void key_up(void)
{
    if (waterindex == -1)
    {
        waterindex = record_index;
    }
    else
    {
        waterindex--;
        if (waterindex < 1)
        {
            // record_index=(record_index+1)/2*2;
            // if (record_index>=4)
            // {
            //     record_index = record_index - 4;
            //     filled_pad();
            //     waterindex = 2;
            // }
            // else
            {
                waterindex = 1;
            }
        }
    }
    update_menu_state();
    return;
}

static void key_enter(void)
{
    OsLog(LOG_DEBUG," waterindex = %d",waterindex);
    if(waterindex == 0) {
        if(strlen(trace_bumber_str) != 0){
        //to do search sql and update pad
        }
    } 
    else{
        record_dest = waterindex-1;
        OsLog(LOG_DEBUG," record_dest = %d",record_dest);
        event_ui_register(UI_WATERS_DETAILS);
    }
    // else if (waterindex == sizeof(water_list)/sizeof(lv_obj_t*)-1) {   //last item
    //     //stWaterList[record_index-1].sTrace
    //     record_dest = record_index-1;
    //     OsLog(LOG_DEBUG," record_dest = %d",record_dest);
    //     event_ui_register(UI_WATERS_DETAILS);
    // }
    // else if(waterindex == sizeof(water_list)/sizeof(lv_obj_t*)-2) {     //first item
    //     if (water_list[waterindex+1] != NULL) { 
    //         //stWaterList[record_index-2].sTrace
    //         record_dest = record_index-2;
    //     }
    //     else {
    //         //stWaterList[record_index-1].sTrace
    //         record_dest = record_index-1;
    //     }
    //     OsLog(LOG_DEBUG," record_dest = %d",record_dest);
    //     event_ui_register(UI_WATERS_DETAILS);
    // }
    return;
}

static void click_enter(void)
{
    OsLog(LOG_DEBUG," waterindex = %d",waterindex);
    if(waterindex == 0) {
    } 
    else{
        record_dest = waterindex-1;
        OsLog(LOG_DEBUG," record_dest = %d",record_dest);
        event_ui_register(UI_WATERS_DETAILS);
    }
    return;
}

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;
    char szDisplayAmount[32] = {0};

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    // OsLog(LOG_DEBUG," code = %d",code);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        OsLog(LOG_DEBUG," keyCode = %04x",keyCode);
        switch(keyCode){
            case KB_KEY_0:
            case KB_KEY_1:
            case KB_KEY_2:
            case KB_KEY_3:
            case KB_KEY_4:
            case KB_KEY_5:
            case KB_KEY_6:
            case KB_KEY_7:
            case KB_KEY_8:
            case KB_KEY_9:
                if(strlen(trace_bumber_str) > 7 ){  //max lenth
                    break;
                }
                if(strlen(trace_bumber_str) == 0){
                    lv_obj_set_style_text_color(search_label,lv_color_hex(0x000000),0);
                }
                trace_bumber_str[strlen(trace_bumber_str)] = keyCode;
                lv_label_set_text(search_label, trace_bumber_str);
                waterindex = 0;
                update_menu_state();
                break;
            case KB_KEY_CLEAR:
                remove_last_byte(trace_bumber_str);
                if(strlen(trace_bumber_str) == 0){
                    lv_label_set_text(search_label, "Search by number or ID");
                    lv_obj_set_style_text_color(search_label,lv_color_hex(0xBCBCBC),0);
                }else{
                    lv_label_set_text(search_label, trace_bumber_str);
                }
                waterindex = 0;
                update_menu_state();
                break;
            case KB_KEY_ENTER:
                key_enter();
                break;
            case KB_KEY_CANCEL://cancel
                event_ui_register(UI_IDLE);
                break;
            case KB_KEY_UP:
                key_up();
                break;
            case KB_KEY_DOWN:
                key_down();
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9:
                event_ui_register(UI_IDLE);
                break;
            case 28:
                {
                    OsLog(LOG_DEBUG," touch_key_event_cb record_dest = %d",e->target);
                    for (size_t i = 0; i < record_index+1; i++)
                    {
                        if (water_list[i] == e->target)
                        {
                            waterindex = i;
                            update_menu_state();
                            click_enter();
                        }
                        else
                        {
                            uint32_t child_cnt = lv_obj_get_child_cnt(water_list[i]);
                            for(size_t j = 0; j < child_cnt; j++) {
                                lv_obj_t * child = lv_obj_get_child(water_list[i], j);
                                if (child == e->target) {
                                    waterindex = i;
                                    update_menu_state();
                                    click_enter();
                                }
                                else {
                                    uint32_t child_cnt2 = lv_obj_get_child_cnt(child);
                                    for(size_t k = 0; k < child_cnt2; k++) {
                                        lv_obj_t * child2 = lv_obj_get_child(child, k);
                                        if (child2 == e->target) {
                                            waterindex = i;
                                            update_menu_state();
                                            click_enter();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            case 414:
                event_ui_register(UI_WATERS_FILTER);
                break;
        }
    }
}

static void touch_key_event_details_cb(lv_event_t * e)
{
    int keyCode;
    char szDisplayAmount[32] = {0};

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        OsLog(LOG_DEBUG," keyCode = %04x",keyCode);
        switch(keyCode){
            case KB_KEY_ENTER:
                {
                    event_trans_register(EVENT_PRINT_RECORD);
                    ui_modal_dialog_for_handle(NULL,"Processing....",0,0);
                }
                break;
            case KB_KEY_CANCEL://cancel
                TranRecord_set_time_limit(0);
                event_ui_register(UI_WATERS);
                break;
            case KB_KEY_UP:
                // lv_obj_scroll_to_y(content_pad,0,LV_ANIM_ON);
                lv_obj_scroll_by_bounded(content_pad,0,50,LV_ANIM_ON);
                break;
            case KB_KEY_DOWN:
                // lv_obj_scroll_to_y(content_pad,50,LV_ANIM_ON);
                lv_obj_scroll_by_bounded(content_pad,0,-50,LV_ANIM_ON);
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9:
                TranRecord_set_time_limit(0);
                event_ui_register(UI_WATERS);
                break;
            case 28:
                {
                    event_trans_register(EVENT_PRINT_RECORD);
                    ui_modal_dialog_for_handle(NULL,"Processing....",0,0);
                }
                break;
        }
    }
}

static void touch_key_event_filter_cb(lv_event_t * e)
{
    int keyCode;
    char szDisplayAmount[32] = {0};

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    // OsLog(LOG_DEBUG," code = %d",code);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        OsLog(LOG_DEBUG," keyCode = %04x",keyCode);
        switch(keyCode){
            case KB_KEY_ENTER:
                event_ui_register(UI_WATERS);
                break;
            case KB_KEY_CANCEL://cancel
                TranRecord_set_time_limit(0);
                TranRecord_set_type_limit(0xFF);
                event_ui_register(UI_WATERS);
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9:
                TranRecord_set_time_limit(0);
                TranRecord_set_type_limit(0xFF);
                event_ui_register(UI_WATERS);
                break;
            case 28:
                event_ui_register(UI_WATERS);
                break;
            case 11:
            case 12:
            case 13:
                for (size_t i = 0; i < 3; i++)
                {
                    if (keyCode%11 == i)
                    {
                        lv_obj_add_state(date_limit[i].target_obj,LV_STATE_PRESSED);
                        lv_obj_t * child = lv_obj_get_child(date_limit[i].target_obj, 0);
                        if(NULL!=child ) lv_obj_add_state(child, LV_STATE_PRESSED);
                        TranRecord_set_time_limit(date_limit[i].vulue);
                    }
                    else
                    {
                        lv_obj_clear_state(date_limit[i].target_obj,LV_STATE_PRESSED);
                        lv_obj_t * child1 = lv_obj_get_child(date_limit[i].target_obj, 0);
                        if(NULL!=child1 ) lv_obj_clear_state(child1, LV_STATE_PRESSED);
                    }
                }
                break;
            case 21:
            case 22:
            case 23:
                for (size_t i = 0; i < 3; i++)
                {
                    if (keyCode%21 == i)
                    {
                        lv_obj_add_state(type_limit[i].target_obj,LV_STATE_PRESSED);
                        lv_obj_t * child = lv_obj_get_child(type_limit[i].target_obj, 0);
                        if(NULL!=child ) lv_obj_add_state(child, LV_STATE_PRESSED);
                    }
                    else
                    {
                        lv_obj_clear_state(type_limit[i].target_obj,LV_STATE_PRESSED);
                        lv_obj_t * child1 = lv_obj_get_child(type_limit[i].target_obj, 0);
                        if(NULL!=child1 ) lv_obj_clear_state(child1, LV_STATE_PRESSED);
                    }
                }
                break;
        }
    }
}

static void filled_pad(void)
{
    if(lv_obj_is_valid(record_pad))
    {
        lv_obj_clean(record_pad);
        memset(&water_list[1],0x00,sizeof(water_list)-sizeof(lv_obj_t*));
        // 6.1. Add detailed content
        for (size_t i = 0; i < LIST_DISP_PER_PAGE; i++)
        {
            if (strlen(stWaterList[record_index].sOrderNo)<=0)
            {
                break;
            }
            lv_obj_t *item_pad = lv_obj_create(record_pad);
            lv_obj_set_size(item_pad,460,ITEM_PAD_HIGHT);
            lv_obj_set_pos(item_pad,0,0+i*ITEM_PAD_HIGHT);                
            lv_obj_set_style_radius(item_pad, 0, 0);
            lv_obj_set_style_pad_all(item_pad,0,0);
            lv_obj_set_style_border_width(item_pad,0,0);
            lv_obj_add_flag(item_pad,LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(item_pad, LV_OBJ_FLAG_PRESS_LOCK);
            lv_obj_add_event_cb(item_pad,touch_key_event_cb,LV_EVENT_ALL,"28"); //todo trace number
            lv_obj_set_style_bg_color(item_pad, lv_color_hex(0xE7F5FF),LV_STATE_PRESSED); // Background blue
            water_list[i+1] = item_pad;

            lv_obj_t* date_label = lv_label_create(item_pad);
            lv_label_set_text(date_label, stWaterList[record_index].sYear);
            lv_obj_set_height(date_label,30);
            lv_obj_set_pos(date_label,14,2);
            lv_obj_set_style_text_font(date_label, &ali_middle_24, 0);
            lv_obj_set_style_text_align(date_label, LV_TEXT_ALIGN_LEFT, 0);
            lv_obj_set_style_text_color(date_label, lv_color_hex(0x1B1B1B), 0);
            lv_obj_add_flag(date_label, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);

            lv_obj_t* amount_label = lv_label_create(item_pad);
            lv_label_set_text(amount_label, stWaterList[record_index].sAmt);
            lv_obj_set_height(amount_label,30);
            lv_obj_set_pos(amount_label,14,30);
            lv_obj_set_style_text_font(amount_label, &ali_middle_24, 0);
            lv_obj_set_style_text_align(amount_label, LV_TEXT_ALIGN_LEFT, 0);
            lv_obj_set_style_text_color(amount_label, lv_color_hex(0x1B1B1B), 0);
            lv_obj_add_flag(amount_label, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);

            lv_obj_t *org_pad = lv_obj_create(item_pad);
            lv_obj_set_size(org_pad,22,20);
            lv_obj_set_pos(org_pad,14,60);
            lv_obj_set_style_radius(org_pad, 0, 0);
            lv_obj_set_style_border_width(org_pad,0,0);
            lv_obj_set_style_pad_all(org_pad,0,0);
            lv_obj_clear_flag(org_pad, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_opa(org_pad, LV_OPA_0, 0);
            lv_obj_t* org_img = lv_img_create(org_pad);
            lv_obj_set_align(org_img, LV_ALIGN_LEFT_MID);
            ui_lv_img_set_src(org_img, (char*)payment_org_icon[stWaterList[record_index].szPaymentOrg]);
            lv_obj_add_flag(org_img, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);

            lv_obj_t *info_pad = lv_obj_create(item_pad);
            lv_obj_set_size(info_pad,400,20);
            lv_obj_set_pos(info_pad,40,60);
            lv_obj_set_style_radius(info_pad, 0, 0);
            lv_obj_set_style_border_width(info_pad,0,0);
            lv_obj_set_style_pad_all(info_pad,0,0);
            lv_obj_clear_flag(info_pad, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_opa(info_pad, LV_OPA_0, 0);
            lv_obj_t* info_label = lv_label_create(info_pad);
            lv_label_set_text(info_label, stWaterList[record_index].szTransDesc);
            lv_obj_set_align(info_label,LV_ALIGN_LEFT_MID);
            lv_obj_set_style_text_font(info_label, &ali_middle_18, 0);
            lv_obj_set_style_text_color(info_label, lv_color_hex(0x6B6B6B), 0);
            lv_obj_add_flag(info_label, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);

            lv_obj_t *type_pad = lv_obj_create(item_pad);
            lv_obj_set_size(type_pad,80,35);
            lv_obj_set_pos(type_pad,360,6);
            lv_obj_set_style_radius(type_pad,13, 0);
            lv_obj_set_style_border_width(type_pad,0,0);
            lv_obj_set_style_pad_all(type_pad,0,0);
            lv_obj_set_style_bg_color(type_pad, lv_color_hex(0xD0E2F5),0);
            lv_obj_add_flag(type_pad, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);

            lv_obj_t* type_label = lv_label_create(type_pad);
            lv_label_set_text(type_label, "Paid");
            lv_obj_set_align(type_label,LV_ALIGN_CENTER);
            lv_obj_set_style_text_font(type_label, &ali_middle_18, 0);
            lv_obj_set_style_text_color(type_label, lv_color_hex(0x446383), 0);
            lv_obj_add_flag(type_label, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);

            if (strlen(stWaterList[record_index+1].sOrderNo)>0)
            {
                lv_obj_t *ui_item_separator = lv_obj_create(item_pad);
                lv_obj_set_size(ui_item_separator, 440, 2);
                lv_obj_set_pos(ui_item_separator, 14, 95);
                lv_obj_set_style_bg_color(ui_item_separator, lv_color_hex(0xDDDDDD), 0);
                lv_obj_set_style_border_width(ui_item_separator, 0, 0);
                lv_obj_set_style_pad_all(ui_item_separator, 0, 0); 
            }
            record_index++;
        }
    }
}

static void ui_update_waters(void) {

    lv_timer_enable(false);
    filled_pad();
    lv_timer_enable(true);
    return;
}

static void return_update(void* pra)
{
    ui_update_waters();
    return;
};

void ui_create_waters() {

    ui_lvgl_page_construct("Transactions",touch_key_event_cb);
    memset(trace_bumber_str,0x00,sizeof(trace_bumber_str));
    waterindex = -1;
    record_index = 0;
    
    // 4. Add pad
    lv_obj_t *search_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(search_pad,400,36);
    lv_obj_set_pos(search_pad,10,70);
    lv_obj_set_style_radius(search_pad, 6, 0);
    lv_obj_set_style_pad_all(search_pad,0,0);
    lv_obj_set_style_border_width(search_pad,1,0);
    lv_obj_set_style_border_color(search_pad,lv_color_hex(0xBCBCBC),0);
    lv_obj_clear_flag(search_pad, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(search_pad, lv_color_hex(0xE7F5FF),LV_STATE_PRESSED); // Background blue
    lv_obj_set_style_border_color(search_pad, lv_color_hex(0x1596FF),LV_STATE_PRESSED); // Background blue
    lv_obj_add_flag(search_pad,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(search_pad,touch_key_event_cb,LV_EVENT_ALL,"28"); //todo trace number
    water_list[0] = search_pad;

    // 4.2.Add magnifying glass
    search_img = lv_img_create(search_pad);
    lv_obj_set_pos(search_img,9,13);
    ui_lv_img_set_src(search_img, (char*)"icon_search.png");
    lv_obj_add_flag(search_img, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);

    // 4.1. Add search label
    search_label = lv_label_create(search_pad);
    lv_label_set_text(search_label, "Search by number or ID");
    lv_obj_set_style_text_color(search_label, lv_color_hex(0xBCBCBC), 0);
    lv_obj_set_size(search_label,222,20);
    lv_obj_set_pos(search_label,28,9);
    lv_obj_set_style_text_font(search_label, &ali_regular_14, 0);
    lv_obj_set_style_text_align(search_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_add_flag(search_label, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);

    // 5 .Add pad
    lv_obj_t *filter_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(filter_pad,36,36);
    lv_obj_set_pos(filter_pad,420,70);
    lv_obj_set_style_radius(filter_pad, 6, 0);
    lv_obj_set_style_border_width(filter_pad,1,0);
    lv_obj_set_style_pad_all(filter_pad,0,0);
    lv_obj_set_style_border_color(filter_pad,lv_color_hex(0xBCBCBC),0);
    lv_obj_clear_flag(filter_pad, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(filter_pad, lv_color_hex(0xE7F5FF),LV_STATE_PRESSED); // Background blue
    lv_obj_set_style_border_color(filter_pad, lv_color_hex(0x1596FF),LV_STATE_PRESSED); // Background blue
    lv_obj_add_flag(filter_pad, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(filter_pad,touch_key_event_cb,LV_EVENT_ALL,"414"); //todo trace number

    // 5.1.Add filter icon
    lv_obj_t* filter_img = lv_img_create(filter_pad);
    lv_obj_set_align(filter_img, LV_ALIGN_CENTER);
    ui_lv_img_set_src(filter_img, (char*)"icon_filter.png");
    lv_obj_add_flag(filter_img, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);

    // 6. Add detailed pad
    record_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(record_pad,480,720);
    lv_obj_set_pos(record_pad,0,120);
    lv_obj_set_style_radius(record_pad, 0, 0);
    lv_obj_set_style_pad_all(record_pad,0,0);
    lv_obj_set_style_border_width(record_pad,0,0);
    lv_obj_add_flag(record_pad, LV_OBJ_FLAG_SCROLLABLE);

    // 6.1. Add detailed content
    // filled_pad();
    lv_timer_enable(true);
    event_trans_register(EVENT_RECORD_TOTAL);
    ui_modal_dialog_for_handle(NULL, "waiting...",0,return_update);
    return;
}
#if 0
void ui_create_details() {
    int int_time[6] = {0};  //[0]-yy,[1]-mm,....
    char time_buf[24] = {0};
    char *info_details[5] = {0};

    ui_lvgl_page_construct("Transactions Details",touch_key_event_details_cb);
    memset(&stTransactionRecord,0x00,sizeof(stTransactionRecord));
    TranRecord_ReadTradeByOrder(stWaterList[record_dest].sOrderNo,&stTransactionRecord);
	OsLog(LOG_ERROR,"stTransactionRecord.sTransTime = %s\n", stTransactionRecord.sTransTime);
    for (size_t i = 0; i < 6; i++) {
        int_time[i] = (stTransactionRecord.sTransTime[2+i*2]-'0')*10 +  stTransactionRecord.sTransTime[3+i*2]-'0';
    }
    if(int_time[3]>12)
    {
        sprintf(time_buf,"%02d/%02d/%02d-%02d:%02d pm",int_time[0],int_time[1],int_time[2],int_time[3]-12,int_time[4]);
    }
    else
    {
        sprintf(time_buf,"%02d/%02d/%02d-%02d:%02d am",int_time[0],int_time[1],int_time[2],int_time[3],int_time[4]);
    }
    info_details[0] = "SALE";
    info_details[1] = stWaterList[record_dest].sAmt;
    info_details[2] = stTransactionRecord.sCardNo;
    info_details[3] = stTransactionRecord.sTrace;
    info_details[4] = stTransactionRecord.sOrderNo;
    record_dest = -1;
    
    // 1 .add pad
    lv_obj_t *title_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(title_pad,320,35);
    lv_obj_set_pos(title_pad,0,34);
    lv_obj_set_style_radius(title_pad, 0, 0);
    lv_obj_set_style_pad_all(title_pad,0,0);
    lv_obj_set_style_border_width(title_pad,0,0);
    lv_obj_set_style_bg_color(title_pad,lv_color_hex(0xEFEFEF),0);
    lv_obj_clear_flag(title_pad, LV_OBJ_FLAG_SCROLLABLE);
    // 1.1. 添加label1+label2
    lv_obj_t *time_label = lv_label_create(title_pad);
    lv_label_set_text(time_label, time_buf);
    lv_obj_align(time_label,LV_ALIGN_LEFT_MID,12,0);
    // lv_obj_set_style_text_color(search_label, lv_color_hex(0xBCBCBC), 0);
    // lv_obj_set_style_text_font(time_label, &lv_font_montserrat_14, 0);

    lv_obj_t *time_labe2 = lv_label_create(title_pad);
    lv_label_set_text(time_labe2, "Reissue receipt");
    lv_obj_align(time_labe2,LV_ALIGN_RIGHT_MID,-9,0);
    lv_obj_set_style_text_color(time_labe2, lv_color_hex(0x008CFF), 0);
    // lv_obj_set_style_text_font(search_label, &lv_font_montserrat_14, 0);
    // lv_obj_set_style_text_align(search_label, LV_TEXT_ALIGN_LEFT, 0);

    // 2. add pad
    content_pad = lv_obj_create(Main_Panel);
    lv_obj_set_width(content_pad,320);
    lv_obj_set_pos(content_pad,0,70);
    lv_obj_set_style_radius(content_pad, 0, 0);
    lv_obj_set_style_pad_all(content_pad,0,0);
    lv_obj_set_style_border_width(content_pad,0,0);
    lv_obj_add_flag(content_pad, LV_OBJ_FLAG_SCROLLABLE);
    for (size_t i = 0; i < sizeof(title_details)/sizeof(char*); i++)
    {
        lv_obj_t* tips_label = lv_label_create(content_pad);
        lv_label_set_text(tips_label, title_details[i]);
        lv_obj_align(tips_label,LV_ALIGN_TOP_LEFT,12,12+32*i);
        lv_obj_set_style_text_font(tips_label, &lv_font_montserrat_14, 0);

        lv_obj_t* info_label = lv_label_create(content_pad);
        lv_label_set_text(info_label, info_details[i]);
        lv_obj_align(info_label,LV_ALIGN_TOP_LEFT,128,12+32*i); //168 at lanhu
        lv_obj_set_style_text_font(info_label, &lv_font_montserrat_14, 0);
    }
    lv_timer_enable(true);

    return;
}
#else
void ui_create_details() {
    int int_time[6] = {0};  //[0]-yy,[1]-mm,....
    char time_buf[24] = {0};
    char pan_mask_buf[24] = {0};
    char *info_details[6] = {0};
    char* title_details[6] = 
    {
        "Txn Type",
        "Pay Type",
        "PAN",   //后续改UNIONPAY
        "Batch NO",
        "Trace NO",
        "DEVICE ID"
    };

    ui_lvgl_page_construct("Details",touch_key_event_details_cb);
    memset(&stTransactionRecord,0x00,sizeof(stTransactionRecord));
    TranRecord_ReadTradeByOrder(stWaterList[record_dest].sOrderNo,&stTransactionRecord);
	OsLog(LOG_ERROR,"stTransactionRecord.sTransTime = %s\n", stTransactionRecord.sTransTime);
    for (size_t i = 0; i < 6; i++) {
        int_time[i] = (stTransactionRecord.sTransTime[2+i*2]-'0')*10 +  stTransactionRecord.sTransTime[3+i*2]-'0';
    }
    if(int_time[3]>12)
    {
        sprintf(time_buf,"%02d/%02d/%02d-%02d:%02d pm",int_time[0],int_time[1],int_time[2],int_time[3]-12,int_time[4]);
    }
    else
    {
        sprintf(time_buf,"%02d/%02d/%02d-%02d:%02d am",int_time[0],int_time[1],int_time[2],int_time[3],int_time[4]);
    }
    info_details[0] = "Sale";
    if (stTransactionRecord.nPosEntryMode == INPUT_QRCODE || stTransactionRecord.nPosEntryMode == INPUT_SCANCODE)
    {
        info_details[1] = "Generate";
        title_details[2] = "Order NO";
        info_details[2] = stTransactionRecord.sOrderNo;
    }
    else
    {
        info_details[1] = "Card";
        strncpy(pan_mask_buf,stTransactionRecord.sCardNo,6);
        strcat(pan_mask_buf,"****");
        strcat(pan_mask_buf,stTransactionRecord.sCardNo+strlen(stTransactionRecord.sCardNo)-4);
        info_details[2] = pan_mask_buf;
    }
    
    info_details[3] = stTransactionRecord.sBatch;
    info_details[4] = stTransactionRecord.sTrace;
    info_details[5] = "00007902123456";
    
    // 1. Add pad
    content_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(content_pad, 480, 780);
    lv_obj_set_pos(content_pad, 0, 80);
    lv_obj_set_style_radius(content_pad, 0, 0);
    lv_obj_set_style_pad_all(content_pad, 0, 0);
    lv_obj_set_style_border_width(content_pad, 0, 0);
    lv_obj_add_flag(content_pad, LV_OBJ_FLAG_SCROLLABLE);

    // 1.1 Create and center the total label
    lv_obj_t* total_label = lv_label_create(content_pad);
    lv_label_set_text(total_label, "Total");
    lv_obj_set_style_text_font(total_label, &ali_middle_36, 0);
    lv_obj_set_width(total_label, LV_PCT(100));  // Set the width to 100% of the parent object
    lv_obj_set_style_text_align(total_label, LV_TEXT_ALIGN_CENTER, 0);  // Text centered
    lv_obj_align(total_label, LV_ALIGN_TOP_MID, 0, 8);  // Top centered, offset 8px downwards
    
    // 1.2 Create and center an amont_label
    lv_obj_t* amount_label = lv_label_create(content_pad);
    lv_label_set_text(amount_label, stWaterList[record_dest].sAmt);
    lv_obj_set_style_text_font(amount_label, &lv_font_montserrat_44, 0);
    lv_obj_set_width(amount_label, LV_PCT(100));  // Set the width to 100% of the parent object
    lv_obj_set_style_text_align(amount_label, LV_TEXT_ALIGN_CENTER, 0);  // Text centered
    lv_obj_align(amount_label, LV_ALIGN_TOP_MID, 0, 50);  // Top centered, offset 50px downwards
    
    lv_obj_t *type_pad = lv_obj_create(content_pad);
    lv_obj_set_size(type_pad,80,30);
    lv_obj_align(type_pad,LV_ALIGN_TOP_MID,0,120);
    lv_obj_set_style_radius(type_pad,13, 0);
    lv_obj_set_style_border_width(type_pad,0,0);
    lv_obj_set_style_pad_all(type_pad,0,0);
    lv_obj_set_style_bg_color(type_pad, lv_color_hex(0xD0E2F5),0);

    lv_obj_t* type_label = lv_label_create(type_pad);
    lv_label_set_text(type_label, "Paid");
    lv_obj_set_align(type_label,LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(type_label, &ali_bold_14, 0);
    lv_obj_set_style_text_color(type_label, lv_color_hex(0x446383), 0);

    // 1.1 .Add pad
    lv_obj_t *title_pad = lv_obj_create(content_pad);
    lv_obj_set_size(title_pad,480,60);
    lv_obj_align(title_pad,LV_ALIGN_TOP_MID,0,180);
    lv_obj_set_style_radius(title_pad, 0, 0);
    lv_obj_set_style_pad_all(title_pad,0,0);
    lv_obj_set_style_border_width(title_pad,0,0);
    lv_obj_set_style_bg_color(title_pad,lv_color_hex(0xEFEFEF),0);
    lv_obj_clear_flag(title_pad, LV_OBJ_FLAG_SCROLLABLE);
    // 1.1. Add label1+label2
    lv_obj_t *time_label = lv_label_create(title_pad);
    lv_label_set_text(time_label, time_buf);
    lv_obj_align(time_label,LV_ALIGN_LEFT_MID,12,0);
    lv_obj_set_style_text_font(time_label, &ali_middle_24, 0);

    lv_obj_t *time_labe2 = lv_label_create(title_pad);
    lv_label_set_text(time_labe2, "Reissue receipt");
    lv_obj_align(time_labe2,LV_ALIGN_RIGHT_MID,-12,0);
    lv_obj_set_style_text_color(time_labe2, lv_color_hex(0x008CFF), 0);
    lv_obj_set_style_text_font(time_labe2, &ali_middle_24, 0);
    lv_obj_add_flag(time_labe2,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(time_labe2,touch_key_event_details_cb,LV_EVENT_ALL,"28"); //todo trace number

    for (size_t i = 0; i < sizeof(title_details)/sizeof(char*); i++)
    {
        lv_obj_t* tips_label = lv_label_create(content_pad);
        lv_label_set_text(tips_label, title_details[i]);
        lv_obj_align(tips_label,LV_ALIGN_TOP_LEFT,20,260+40*i);
        lv_obj_set_style_text_font(tips_label, &ali_middle_24, 0);

        lv_obj_t* info_label = lv_label_create(content_pad);
        lv_label_set_text(info_label, info_details[i]);
        lv_obj_align(info_label,LV_ALIGN_TOP_LEFT,200,260+40*i); //168 at lanhu
        lv_obj_set_style_text_font(info_label, &ali_middle_24, 0);
    }
    record_dest = -1;
    lv_timer_enable(true);

    return;
}
#endif

void ui_create_filter() {

    ui_lvgl_page_construct("Filter",touch_key_event_filter_cb);
    memset(trace_bumber_str,0x00,sizeof(trace_bumber_str));
    
    // 1. 添加 Transaction Date label
    search_label = lv_label_create(Main_Panel);
    lv_label_set_text(search_label, "Transaction Date");
    lv_obj_set_style_text_color(search_label, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_size(search_label,300,40);
    lv_obj_set_pos(search_label,14,70);
    lv_obj_set_style_text_font(search_label, &ali_middle_24, 0);
    lv_obj_set_style_text_align(search_label, LV_TEXT_ALIGN_LEFT, 0);

    for (size_t i = 0; i < 3; i++)
    {
        lv_obj_t *item_pad = lv_obj_create(Main_Panel);
        lv_obj_set_size(item_pad,120,60);
        lv_obj_set_pos(item_pad,14+i*140,120);                
        lv_obj_set_style_radius(item_pad, 6, 0);
        lv_obj_set_style_pad_all(item_pad,0,0);
        lv_obj_set_style_border_width(item_pad,1,0);
        lv_obj_add_flag(item_pad,LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item_pad,touch_key_event_filter_cb,LV_EVENT_ALL,date_limit[i].event_value); //todo trace number
        lv_obj_set_style_border_color(item_pad, lv_color_hex(0xBCBCBC),0); // Background blue
        lv_obj_set_style_border_color(item_pad, lv_color_hex(0x1596FF),LV_STATE_PRESSED); // Background blue
        lv_obj_set_style_bg_color(item_pad, lv_color_hex(0xE7F5FF),LV_STATE_PRESSED); // Background blue
        date_limit[i].target_obj = item_pad;
        
        lv_obj_t* item_label = lv_label_create(item_pad);
        lv_label_set_text(item_label, date_limit[i].laber_name);
        lv_obj_set_align(item_label,LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(item_label, &ali_middle_24, 0);
        lv_obj_set_style_text_align(item_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(item_label, lv_color_hex(0x1596FF),LV_STATE_PRESSED);
        lv_obj_add_flag(item_label, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);
    }
    // 2. Add Transaction Type label
    search_label = lv_label_create(Main_Panel);
    lv_label_set_text(search_label, "Transaction Type");
    lv_obj_set_style_text_color(search_label, lv_color_hex(0x1B1B1B), 0);
    lv_obj_set_size(search_label,300,40);
    lv_obj_set_pos(search_label,14,230);
    lv_obj_set_style_text_font(search_label, &ali_middle_24, 0);
    lv_obj_set_style_text_align(search_label, LV_TEXT_ALIGN_LEFT, 0);

    for (size_t i = 0; i < 3; i++)
    {
        lv_obj_t *item_pad = lv_obj_create(Main_Panel);
        lv_obj_set_size(item_pad,120,60);
        lv_obj_set_pos(item_pad,14+i*140,280);                
        lv_obj_set_style_radius(item_pad, 6, 0);
        lv_obj_set_style_pad_all(item_pad,0,0);
        lv_obj_set_style_border_width(item_pad,1,0);
        lv_obj_add_flag(item_pad,LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(item_pad,touch_key_event_filter_cb,LV_EVENT_ALL,type_limit[i].event_value); //todo trace number
        lv_obj_set_style_border_color(item_pad, lv_color_hex(0xBCBCBC),0); // Background blue
        lv_obj_set_style_border_color(item_pad, lv_color_hex(0x1596FF),LV_STATE_PRESSED); // Background blue
        lv_obj_set_style_bg_color(item_pad, lv_color_hex(0xE7F5FF),LV_STATE_PRESSED); // Background blue
        type_limit[i].target_obj = item_pad;
        
        lv_obj_t* item_label = lv_label_create(item_pad);
        lv_label_set_text(item_label, type_limit[i].laber_name);
        lv_obj_set_align(item_label,LV_ALIGN_CENTER);
        lv_obj_set_style_text_font(item_label, &ali_middle_24, 0);
        lv_obj_set_style_text_align(item_label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(item_label, lv_color_hex(0x1596FF),LV_STATE_PRESSED);
        lv_obj_add_flag(item_label, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);
    }
    lv_obj_add_state(date_limit[0].target_obj,LV_STATE_PRESSED);
    lv_obj_t * child1 = lv_obj_get_child(date_limit[0].target_obj, 0);
    if(NULL!=child1 ) lv_obj_add_state(child1, LV_STATE_PRESSED);
    TranRecord_set_time_limit(date_limit[0].vulue);

    lv_obj_add_state(type_limit[0].target_obj,LV_STATE_PRESSED);
    lv_obj_t * child2 = lv_obj_get_child(type_limit[0].target_obj, 0);
    if(NULL!=child2 ) lv_obj_add_state(child2, LV_STATE_PRESSED);


    lv_obj_t *btn = lv_btn_create(Main_Panel);
    lv_obj_set_size(btn, 400, 80);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 650);
    lv_obj_add_event_cb(btn, touch_key_event_filter_cb, LV_EVENT_CLICKED, "28");
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x1596FF), 0); 
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);  // Ensure opacity
    lv_obj_set_style_radius(btn, 0, 0);  // Remove rounded corners

    // Create tags and set red font
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Done");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);  
    lv_obj_set_style_text_font(label, &ali_middle_24, 0);
    lv_obj_center(label);

    // 6.1. Add detailed content
    lv_timer_enable(true);
    return;
}
