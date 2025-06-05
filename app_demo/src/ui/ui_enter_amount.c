#include "appinc.h"

// global variable
static lv_obj_t * amount;  // Label displaying amount
static char amount_str[32] = {0};  // Amount string buffer
static lv_obj_t * tax;

static void touch_key_event_cb(lv_event_t * e)
{
    int keyCode,nAmount;
    char szDisplayAmount[32] = {0};

    lv_event_code_t code = lv_event_get_code(e);
    char* index = (char*)lv_event_get_user_data(e);
    if( code == LV_EVENT_KEY){
        keyCode = lv_event_get_key(e);
        OsLog(LOG_DEBUG," keyCode = %04x",keyCode);
        switch(keyCode){
            case KB_KEY_ENTER:
                if(strlen(amount_str)> 0 && atoi(amount_str) > 0){
                    sprintf(get_transaction_data()->sAmount,"%012d",atol(amount_str));
                    event_ui_register(UI_SELECT_PAYTYPE);   
                }
                break;
            case KB_KEY_CANCEL://cancel
                OsLog(LOG_DEBUG," LV_EVENT_KEY KB_KEY_CANCEL===");
                event_ui_register(UI_IDLE);
                break;
        }  
    }else if (code == LV_EVENT_CLICKED){   
        keyCode = atoi(index);
        switch(keyCode){
            case 9:
                event_ui_register(UI_IDLE);
                break;
            case 1:
                if(strlen(amount_str)> 0 && atoi(amount_str) > 0){
                    sale_init();
                    sprintf(get_transaction_data()->sAmount,"%012d",atol(amount_str));
                    event_ui_register(UI_SELECT_PAYTYPE);
                }
                break;
            case 2:
                TranRecord_set_time_limit(0);
                event_ui_register(UI_WATERS);
                break;
            case 3:
                event_ui_register(UI_SETTING);
                break;
            case 4:
                event_ui_register(UI_ABOUT);
                break;    
        }
    }
}



// Number key callback function
static void num_key_event_cb(lv_event_t *e) {
    char szDisplayAmount[32] = {0};
    int nAmount;
    lv_event_code_t code = lv_event_get_code(e);
    const char *num = (char*)lv_event_get_user_data(e);

    OsLog(LOG_DEBUG,"num = %s",num);
    // // Processing digital inputs
    if(strcmp(num, "Backspace") == 0){
        size_t len = strlen(amount_str);
        if(len <= 1) {
            strcpy(amount_str, "0");
        } else {
            amount_str[len-1] = '\0';
        } 
    }else if(strcmp(amount_str, "0") == 0) {
        strcpy(amount_str, num);  // Replace the initial 0
    } else {
        if(strlen(amount_str) < 9){
            strcat(amount_str, num);
        }
    }
    nAmount = atol(amount_str);
    sprintf(szDisplayAmount,"$%ld.%02ld",nAmount/100,nAmount%100);
    lv_label_set_text(amount, szDisplayAmount);
    memset(szDisplayAmount,0x0,sizeof(szDisplayAmount));
    sprintf(szDisplayAmount,"+$0.00 tax = $%ld.%02ld ",nAmount/100,nAmount%100);
    lv_label_set_text(tax, szDisplayAmount);
}

#ifdef MAIN_UI_NEW
// Create a numeric keypad
void create_numpad(lv_obj_t *parent) {    
    // Create a numeric keypad panel
    lv_obj_t *numpad = lv_obj_create(parent);
    lv_obj_set_size(numpad, 480, 540);
    lv_obj_align(numpad, LV_ALIGN_BOTTOM_MID, 0, -100);
    lv_obj_set_style_bg_color(numpad, lv_color_hex(0xF3EFDD), 0);
    lv_obj_set_style_pad_all(numpad, 10, 0);
    lv_obj_set_flex_flow(numpad, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(numpad, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_border_width(numpad, 0, 0);
    lv_obj_set_style_radius(numpad, 20, 0);
    
    // Number keys 1-9
    const char *nums[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "00", "0", "Backspace"};
    for(int i = 0; i < 12; i++) {
        lv_obj_t *btn = lv_btn_create(numpad);
        lv_obj_set_size(btn, 140, 100);
        lv_obj_add_event_cb(btn, num_key_event_cb, LV_EVENT_CLICKED, (void*)nums[i]);
        
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, nums[i]);
        lv_obj_set_style_text_color(label, lv_color_hex(0x191E3B), 0);
        lv_obj_center(label);
        if(i == 11){
            lv_obj_set_style_text_font(label, &ali_middle_24, 0);  //28
        }else{
            lv_obj_set_style_text_font(label, &ali_middle_36, 0);  //28
        }
        lv_obj_set_style_bg_color(btn, lv_color_white(), 0);
    }
    
    lv_obj_t *btn = lv_btn_create(numpad);
    lv_obj_set_size(btn, 450, 80);
    lv_obj_add_event_cb(btn, touch_key_event_cb, LV_EVENT_CLICKED, "1");
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xFF0039), 0);
    lv_obj_set_style_radius(btn, 40, 0);
    
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Confirm");
    lv_obj_set_style_text_font(label, &ali_middle_24, 0);
    lv_obj_center(label);
}
#else

// Create a numeric keypad
void create_numpad(lv_obj_t *parent) {    
    // Create a numeric keypad panel
    lv_obj_t *numpad = lv_obj_create(parent);
    lv_obj_set_size(numpad, 480, 560);
    lv_obj_align(numpad, LV_ALIGN_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(numpad, lv_color_hex(0xF3EFDD), 0);
    lv_obj_set_style_pad_all(numpad, 10, 0);
    lv_obj_set_flex_flow(numpad, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(numpad, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_border_width(numpad, 0, 0);
    lv_obj_set_style_radius(numpad, 20, 0);
    
    // Number keys 1-9
    const char *nums[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "00", "0", "Backspace"};
    for(int i = 0; i < 12; i++) {
        lv_obj_t *btn = lv_btn_create(numpad);
        lv_obj_set_size(btn, 140, 100);
        lv_obj_add_event_cb(btn, num_key_event_cb, LV_EVENT_CLICKED, (void*)nums[i]);
        
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, nums[i]);
        lv_obj_set_style_text_color(label, lv_color_hex(0x191E3B), 0);
        lv_obj_center(label);
        if(i == 11){
            lv_obj_set_style_text_font(label, &ali_middle_24, 0);  //28
        }else{
            lv_obj_set_style_text_font(label, &ali_middle_36, 0);  //28
        }
        lv_obj_set_style_bg_color(btn, lv_color_white(), 0);
    }
    
    lv_obj_t *btn = lv_btn_create(numpad);
    lv_obj_set_size(btn, 450, 80);
    lv_obj_add_event_cb(btn, touch_key_event_cb, LV_EVENT_CLICKED, "1");
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xFF0039), 0);
    lv_obj_set_style_radius(btn, 40, 0);
    
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Confirm");
    lv_obj_set_style_text_font(label, &ali_middle_24, 0);
    lv_obj_center(label);
}
#endif

void ui_create_enter_amount() {
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    memset(amount_str,0x0,sizeof(amount_str));
    
    ui_lvgl_page_construct("Sale",touch_key_event_cb);
    // 3. Add the 'Enter Amount' label
    lv_obj_t * enter_amount = lv_label_create(Main_Panel);
    lv_obj_align(enter_amount, LV_ALIGN_TOP_MID, 0,100);
    lv_label_set_text(enter_amount, "Enter Amount");
    lv_obj_set_style_text_font(enter_amount, &ali_middle_24, 0);

    // 4. Add large amount display
    amount = lv_label_create(Main_Panel);
    lv_obj_align(amount, LV_ALIGN_TOP_MID, 0, 160);
    lv_label_set_text(amount, "$0.00");
    lv_obj_set_style_text_font(amount, &lv_font_montserrat_48, 0);

    // 5. Add a horizontal line (achieved through an object with a height of 1px)
    lv_obj_t * line = lv_obj_create(Main_Panel);
    lv_obj_set_size(line, 300, 2); // Width 200px, height 1px
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 220);
    lv_obj_set_style_bg_color(line, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);
 
     // 6. Add tax display
     tax = lv_label_create(Main_Panel);
     lv_obj_align(tax, LV_ALIGN_TOP_MID, 0, 230);
     lv_label_set_text(tax, "+$0.00 tax = $0.00");
     lv_obj_set_style_text_font(tax, &ali_middle_24, 0);
     lv_obj_set_style_text_color(tax, lv_palette_main(LV_PALETTE_GREY), 0);

    create_numpad(Main_Panel);
    lv_timer_enable(true);
}

#ifdef MAIN_UI_NEW
void lvgl_MainMenu()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    memset(amount_str,0x0,sizeof(amount_str));
    
    lv_obj_t * title = lv_label_create(Main_Panel);
    lv_label_set_text(title, "SALE");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);
    lv_obj_set_style_text_font(title, &ali_middle_36, 0);

    // 4. Add large amount display
    amount = lv_label_create(Main_Panel);
    lv_obj_align(amount, LV_ALIGN_TOP_MID, 0, 80);
    lv_label_set_text(amount, "$0.00");
    lv_obj_set_style_text_font(amount, &lv_font_montserrat_48, 0);

    // 5. Add a horizontal line (achieved through an object with a height of 1px)
    lv_obj_t * line = lv_obj_create(Main_Panel);
    lv_obj_set_size(line, 300, 2); // Width 200px, height 1px
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 140);
    lv_obj_set_style_bg_color(line, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);
 
     // 6. Add tax display
     tax = lv_label_create(Main_Panel);
     lv_obj_align(tax, LV_ALIGN_TOP_MID, 0, 150);
     lv_label_set_text(tax, "+$0.00 tax = $0.00");
     lv_obj_set_style_text_font(tax, &ali_middle_24, 0);
     lv_obj_set_style_text_color(tax, lv_palette_main(LV_PALETTE_GREY), 0);

    create_numpad(Main_Panel);


     // Add Trade Card Content
     // Create Left Trade Card (purple) - Align with the top of the Right Setting Card
    lv_obj_t *trade_card = lv_obj_create(Main_Panel);
    lv_obj_set_size(trade_card, 150, 80);
    lv_obj_align(trade_card, LV_ALIGN_BOTTOM_LEFT, 5, -12);  // The Y coordinate is the same as the Setting card at 240+200+20
    lv_obj_set_style_bg_color(trade_card, lv_color_hex(0xF3EFDD), 0);  // Purple background
    lv_obj_set_style_radius(trade_card, 0, 0);  //Remove rounded corners
    lv_obj_add_event_cb(trade_card, touch_key_event_cb, LV_EVENT_ALL, "2");
    lv_obj_clear_flag(trade_card, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_pad_all(trade_card, 10, 0);
    lv_obj_set_flex_flow(trade_card, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(trade_card, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_border_width(trade_card, 0, 0);
    lv_obj_set_style_radius(trade_card, 20, 0);


    lv_obj_t *trade_img = lv_img_create(trade_card);
    ui_lv_img_set_src(trade_img,"icon_Transactions2.png");
    lv_obj_align(trade_img, LV_ALIGN_TOP_MID, 0, 5);

    lv_obj_t *trade_label = lv_label_create(trade_card);
    lv_label_set_text(trade_label, "Transactions");
    lv_obj_set_style_text_font(trade_label, &ali_middle_24, 0);
    lv_obj_set_style_text_color(trade_label,lv_color_hex(0x9933ff), 0);  // White font
    lv_obj_align_to(trade_label, trade_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    // Create a Setting card on the right side (light blue)
    lv_obj_t *setting_card = lv_obj_create(Main_Panel);
    lv_obj_set_size(setting_card, 150, 80);
    lv_obj_align(setting_card, LV_ALIGN_BOTTOM_MID, 0,-12);  // Align the Y coordinate 460 with the Trade card
    lv_obj_set_style_bg_color(setting_card, lv_color_hex(0xF3EFDD), 0);  // Light blue background
    lv_obj_set_style_radius(setting_card, 0, 0);  // Remove rounded corners
    lv_obj_add_event_cb(setting_card, touch_key_event_cb, LV_EVENT_ALL, "3");
    lv_obj_clear_flag(setting_card, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_pad_all(setting_card, 10, 0);
    lv_obj_set_flex_flow(setting_card, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(setting_card, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_border_width(setting_card, 0, 0);
    lv_obj_set_style_radius(setting_card, 20, 0);

    // Add Setting Card Content
    lv_obj_t *setting_img = lv_img_create(setting_card);
    ui_lv_img_set_src(setting_img,"icon_setting2.png");
    lv_obj_align(setting_img, LV_ALIGN_TOP_MID, 0, 5);

    lv_obj_t *setting_label = lv_label_create(setting_card);
    lv_label_set_text(setting_label, "  Setting  ");
    lv_obj_set_style_text_font(setting_label, &ali_middle_24, 0);
    lv_obj_set_style_text_color(setting_label, lv_color_hex(0x66ccff), 0);  // White font
    lv_obj_align_to(setting_label, setting_img, LV_ALIGN_OUT_BOTTOM_MID, 0,0);
    // lv_obj_align(setting_label, LV_ALIGN_BOTTOM_MID, 0,0);

    // Create the About card on the right (dark blue)
    lv_obj_t *about_card = lv_obj_create(Main_Panel);
    lv_obj_set_size(about_card, 150, 80);
    lv_obj_align(about_card, LV_ALIGN_BOTTOM_RIGHT, -5, -12); 
    lv_obj_set_style_bg_color(about_card,lv_color_hex(0xF3EFDD), 0);  // Light blue background
    lv_obj_set_style_radius(about_card, 0, 0);  // Remove rounded corners
    lv_obj_add_event_cb(about_card, touch_key_event_cb, LV_EVENT_ALL, "4");
    lv_obj_clear_flag(about_card, LV_OBJ_FLAG_SCROLLABLE);
    
    lv_obj_set_style_pad_all(about_card, 10, 0);
    lv_obj_set_flex_flow(about_card, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(about_card, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_border_width(about_card, 0, 0);
    lv_obj_set_style_radius(about_card, 20, 0);

    // Add About Card Content
    lv_obj_t *about_img = lv_img_create(about_card);
    ui_lv_img_set_src(about_img,"icon_about2.png");
    lv_obj_align(about_img, LV_ALIGN_TOP_MID, 0, 5);

    lv_obj_t *about_label = lv_label_create(about_card);
    lv_label_set_text(about_label, "  About  ");
    lv_obj_set_style_text_font(about_label, &ali_middle_24, 0);
    lv_obj_set_style_text_color(about_label, lv_color_hex(0x0066cc), 0);  // White font
    lv_obj_align_to(about_label, about_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_timer_enable(true);
}

#endif