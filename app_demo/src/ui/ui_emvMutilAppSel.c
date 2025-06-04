#include "appinc.h"

#define ITEM_PAD_HIGHT  80

struct App {
    int counter;
    char listIndex[ 5 ][ 3 ];
    char listName[ 5 ][ 20 ];
};
static struct App LocalMultiApp;

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode;

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
     OsLog(LOG_DEBUG," code = %d",code);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        OsLog(LOG_DEBUG," keyCode = %04x",keyCode);
        switch(keyCode){
            case KB_KEY_CANCEL://cancel
                get_transaction_data()->emv_multi_app_select_result = -1;
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9:
                get_transaction_data()->emv_multi_app_select_result = -1;
                break;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                get_transaction_data()->emv_multi_app_select_result = keyCode -1;
                OsLog(LOG_DEBUG," emv_multi_app_select_result = %d",get_transaction_data()->emv_multi_app_select_result);
                break;
        }
    }
}

void unpackAppsName( int counter, AidCandidate_t *pList ) 
{
    int i;
    memset(&LocalMultiApp, 0x00, sizeof( LocalMultiApp ) );
    LocalMultiApp.counter = counter;
    for(i =0;i<counter;i++)
    {
        if(i == 5)
            break;
        memcpy(LocalMultiApp.listName[i],pList[i]._lable,strlen(pList[i]._lable));
        sprintf(LocalMultiApp.listIndex[i],"%d",i+1);
    }
}

void ui_create_multiapp_sel() {

    unsigned char index[8];
    ui_lvgl_page_construct("Select App",touch_key_event_cb);
    
    // 1 .add pad
    lv_obj_t *menu_pad = lv_obj_create(Main_Panel);
    lv_obj_set_size(menu_pad,480,780);
    lv_obj_set_pos(menu_pad,0,70);
    lv_obj_set_style_radius(menu_pad, 0, 0);
    lv_obj_set_style_pad_all(menu_pad,0,0);
    lv_obj_set_style_border_width(menu_pad,0,0);
   // lv_obj_clear_flag(menu_pad, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    for (size_t i = 0; i < LocalMultiApp.counter; i++)
    {
        lv_obj_t *item_pad = lv_obj_create(menu_pad);
        lv_obj_set_size(item_pad,480,ITEM_PAD_HIGHT);
        lv_obj_set_pos(item_pad,0,i*ITEM_PAD_HIGHT);
        lv_obj_set_style_radius(item_pad, 0, 0);
        lv_obj_set_style_pad_all(item_pad,0,0);
        lv_obj_set_style_border_width(item_pad,0,0);
        lv_obj_add_flag(item_pad,LV_OBJ_FLAG_CLICKABLE);

        lv_obj_add_event_cb(item_pad,touch_key_event_cb,LV_EVENT_ALL,LocalMultiApp.listIndex[i]);

        lv_obj_t* index_label = lv_label_create(item_pad);
        lv_label_set_text(index_label, LocalMultiApp.listIndex[i]);
        lv_obj_set_size(index_label,30,30);
        lv_obj_set_pos(index_label,14,40);
        lv_obj_set_style_radius(index_label,30,0);
        // lv_obj_set_style_border_color(index_label,);
        lv_obj_set_style_border_width(index_label,1,0);
        lv_obj_set_style_pad_all(index_label,0,0);
        lv_obj_set_style_text_font(index_label, &ali_middle_24, 0);
        lv_obj_set_style_text_align(index_label, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_t* content_label = lv_label_create(item_pad);
        lv_label_set_text(content_label,  LocalMultiApp.listName[i]);
        lv_obj_set_height(content_label,30);
        lv_obj_set_pos(content_label,60,40);
        lv_obj_set_style_text_font(content_label, &ali_middle_24, 0);
        lv_obj_set_style_text_align(content_label, LV_TEXT_ALIGN_LEFT, 0);

        lv_obj_t *ui_item_separator = lv_obj_create(item_pad);
        lv_obj_set_size(ui_item_separator, 480, 2);
        lv_obj_align(ui_item_separator, LV_ALIGN_BOTTOM_MID, 0, -2);
        lv_obj_set_style_bg_color(ui_item_separator, lv_color_hex(0xDDDDDD), 0);
        lv_obj_set_style_border_width(ui_item_separator, 0, 0);
        lv_obj_set_style_pad_all(ui_item_separator, 0, 0);         
    }
    
    lv_timer_enable(true);
}