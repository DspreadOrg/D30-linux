#include "ui_emv_disp.h"

void ui_create_see_phone()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    lv_obj_t *tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Plese check your phone");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 450);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_36, 0);

    lv_timer_enable(true);
}

void ui_create_nfc_retap()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling


    lv_obj_t *nfc_img = lv_img_create(Main_Panel);
    ui_lv_img_set_src(nfc_img, (char*)"tap2.png");
    lv_obj_align(nfc_img, LV_ALIGN_TOP_MID, 0, 200);


    lv_obj_t *tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Plese retap card");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 450);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_36, 0);

    lv_timer_enable(true);
}

void ui_create_remove_card()
{
    lv_timer_enable(false);
    lv_obj_clean(Main_Panel);
    lv_group_remove_all_objs(group_keypad_indev);
    lv_obj_clear_flag(Main_Panel, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    lv_obj_t *tip_lable = lv_label_create(Main_Panel);
    lv_label_set_text(tip_lable, "Plese remove card");
    lv_obj_align(tip_lable, LV_ALIGN_TOP_MID, 0, 450);
    lv_obj_set_style_text_font(tip_lable, &ali_middle_36, 0);

    lv_timer_enable(true);
}

