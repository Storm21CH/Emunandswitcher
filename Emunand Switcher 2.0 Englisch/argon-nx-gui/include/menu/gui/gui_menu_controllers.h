// Copyright(c) 2019 Storm

#ifndef _GUI_MENU_CTRL_
#define _GUI_MENU_CTRL_

#include "libs/lvgl/lvgl.h"
#include "core/argon-ctxt.h"

argon_ctxt_t* g_argon_ctxt;

void ctrl_reboot_rcm(lv_obj_t *obj, lv_event_t event);
void ctrl_reboot_ofw(lv_obj_t *obj, lv_event_t event);
void ctrl_power_off(lv_obj_t *obj, lv_event_t event);
void ctrl_lauch_payload(lv_obj_t *obj, lv_event_t event);


//void ctrl_test(lv_obj_t* obj, lv_event_t event);//Eintrag Button Test Funktion
void ctrl_info(lv_obj_t* obj, lv_event_t event);//Eintrag Button Info Funktion
void ctrl_themedel(lv_obj_t* obj, lv_event_t event);//Eintrag Button Theme del Funktion
void ctrl_themedelATM(lv_obj_t* obj, lv_event_t event);//Eintrag Button Theme del Funktion ATM
void ctrl_updsx(lv_obj_t* obj, lv_event_t event);//Update Sx boot.dat Button Funktion
void ctrl_emun1(lv_obj_t* obj, lv_event_t event);//Eintrag Emunand1 Button Funktion
void ctrl_emun2(lv_obj_t* obj, lv_event_t event);//Eintrag Emunand2 Button Funktion
void ctrl_emun3(lv_obj_t* obj, lv_event_t event);//Eintrag Emunand3 Button Funktion
void ctrl_emmc1(lv_obj_t* obj, lv_event_t event);//Eintrag Emmummc1 Button Funktion
void ctrl_emmc2(lv_obj_t* obj, lv_event_t event);//Eintrag Emmummc1 Button Funktion
void ctrl_emmc3(lv_obj_t* obj, lv_event_t event);//Eintrag Emmummc1 Button Funktion

//int tool_infoemuninstalled();
//int tool_infoemuninstalledATM();

#endif