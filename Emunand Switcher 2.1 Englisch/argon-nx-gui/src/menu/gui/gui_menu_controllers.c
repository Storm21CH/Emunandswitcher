// Copyright(c) 2019 Storm

#include "menu/gui/gui_menu_controllers.h"
#include "utils/util.h"
#include "menu/gui/gui_menu_pool.h"
#include "core/launcher.h"



#include <string.h>//hinzugefügt
#include "libs/fatfs/ff.h"//hinzugefügt

#include "power/bq24193.h"// Zeit Temp Akku Tool hekate
#include "rtc/max77620-rtc.h"// Zeit Temp Akku Tool hekate
#include "power/max77620.h"// Zeit Temp Akku Tool hekate
#include "thermal/tmp451.h"// Zeit Temp Akku Tool hekate
#include "soc/i2c.h"// Zeit Temp Akku Tool hekate

#include "power/max17050.h"//hinzugefügt Battery
#include "utils/fs_utils.h"//hinzugefügt
#include "mem/heap.h"//hinzugefügt s_printf string

#include "libs/lvgl/lvgl.h"//hinzugefügt Fenster win Funktion

//#include <stdio.h>//hinzugefügt
#include "gfx/di.h"//hinzugefügt Display Helligkeit
#include "soc/t210.h"//hinzugefügt Display Helligkeit

#include <stdlib.h>//hinzugefügt für Typumwandlung atoi, atol usw

const char* string = "";//Eintrag String Text



void ctrl_reboot_rcm(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        argon_ctxt_destroy(g_argon_ctxt);
        reboot_rcm();
    }
}

void ctrl_reboot_ofw(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        argon_ctxt_destroy(g_argon_ctxt);
        reboot_normal();
    }
}

void ctrl_power_off(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        argon_ctxt_destroy(g_argon_ctxt);
        power_off();
    }
}

void ctrl_lauch_payload(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_obj_t label = lv_obj_get_child(obj, NULL)[0];
        char* path = lv_label_get_text(&label);
        launch_payload(g_argon_ctxt, path);
    }
}


//Info Button Funktion
void ctrl_info(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;
		bg.text.font = &lv_font_montserrat_alternate_20;

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
		lv_mbox_set_recolor(mbox, true);
		lv_obj_set_width(mbox, LV_DPI * 5);
		lv_obj_set_top(mbox, true);
		lv_obj_set_auto_realign(mbox, true);
		lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 5, 5);
		lv_mbox_set_text(mbox, "Emunandswitcher Version 2.11 by Storm 2020\ncreated with Visual Studio and LittlevGL\npartially based on ArgonNX und Hekate\nIcon templates from mrdude\nThanks to the programmers!");
		lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
		lv_mbox_start_auto_close(mbox, 12000);
	}
}


//Theme del Button Funktion
void ctrl_themedel(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{

			if (f_stat("sxos/titles/0100000000001000", NULL)) {

				static lv_style_t bg;
				lv_style_copy(&bg, &lv_style_pretty);
				bg.text.color = LV_COLOR_WHITE;
				bg.body.opa = LV_OPA_0;
				lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 5, 5);
				lv_mbox_set_text(mbox, "No theme installed...");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

			}

			else { // if the directory exists - do this.

				f_unlink("sxos/titles/0100000000001000");
				f_unlink("sxos/titles/0100000000001007");
				f_unlink("sxos/titles/0100000000001013");

				static lv_style_t bg;
				lv_style_copy(&bg, &lv_style_pretty);
				bg.text.color = LV_COLOR_GREEN;
				bg.body.opa = LV_OPA_0;
				lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 5, 5);
				lv_mbox_set_text(mbox, "Theme folder deleted...");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

			}

	}
}


//Theme del Button Funktion ATM
void ctrl_themedelATM(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{

		if (f_stat("atmosphere/contents/0100000000001000", NULL)) {

			static lv_style_t bg;
			lv_style_copy(&bg, &lv_style_pretty);
			bg.text.color = LV_COLOR_WHITE;
			bg.body.opa = LV_OPA_0;
			lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 5, 5);
			lv_mbox_set_text(mbox, "No theme installed...");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

		}

		else { // if the directory exists - do this.

			f_unlink("atmosphere/contents/0100000000001000");
			f_unlink("atmosphere/contents/0100000000001007");
			f_unlink("atmosphere/contents/0100000000001013");

			static lv_style_t bg;
			lv_style_copy(&bg, &lv_style_pretty);
			bg.text.color = LV_COLOR_GREEN;
			bg.body.opa = LV_OPA_0;
			lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 5, 5);
			lv_mbox_set_text(mbox, "Theme folder deleted...");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

		}

	}
}


//Update Sx boot.dat Button Funktion
void ctrl_updsx(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{

		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;
		bg.text.font = &lv_font_montserrat_alternate_20;

		if (f_stat("emunandswitcher/updatesx/boot.dat", NULL)) {

			lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);//Grösse Fenster
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 5, 5);
			lv_mbox_set_text(mbox, "Boot.dat not found!\nPlease copy boot.dat update to\nfolder emunandswitcher/updatesx");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

	
		}

		else { // if the directory exists - do this.

			lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 5, 5);
			lv_mbox_set_text(mbox, "Boot.dat available! Update SXOS!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			f_unlink("boot.dat");

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 5, 35);
			lv_mbox_set_text(mbox, "Boot.dat SX deleted");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			f_rename("emunandswitcher/updatesx/boot.dat", "boot.dat");

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 5, 65);
			lv_mbox_set_text(mbox, "Copy boot.dat update!\nBoot.dat update deleted!\nSXOS update completed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

		}

	}
}





void ctrl_emun1(lv_obj_t* obj, lv_event_t event)//Emunand1 Button Funktion
{
	if (event == LV_EVENT_CLICKED)
	{
		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;

		bg.text.font = &lv_font_montserrat_alternate_20;//Test Font 20 Textbox

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);

		if (f_stat("sxos/emunand", NULL)) {

			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "No Emunand installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}

		if (f_stat("emunandswitcher/folders sx/emutmp00", NULL)) {

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Emunand 1 already installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders sx/emutmp01", NULL)) {

		}
		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);


			if (f_stat("emunandswitcher/folders sx/emutmp00/boot0.bin", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}
			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				if (f_stat("emunandswitcher/folders sx/emutmp00/boot1.bin", NULL)) {

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin not available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					return;//Test stop
				}
				else { // if the file exists - do this.

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					if (f_stat("emunandswitcher/folders sx/emutmp00/full.00.bin", NULL)) {

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin not available!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);

						return;//Test stop
					}

					else { // if the file exists - do this.

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin available! Move Emunand!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);


						f_rename("sxos/emunand", "emunandswitcher/folders sx/emutmp02");
						f_rename("emunandswitcher/folders sx/emutmp00", "sxos/emunand");
						f_rename("Emutendo", "emunandswitcher/folders sx/emutendo02");
						f_rename("emunandswitcher/folders sx/emutendo00", "Emutendo");
						f_rename("sxos/titles", "emunandswitcher/folders sx/titles02");
						f_rename("emunandswitcher/folders sx/titles00", "sxos/titles");

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 125);
						lv_mbox_set_text(mbox, "Emunand successfully moved...");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);


						return;//Test stop

					}
				}
			}


		}


		if (f_stat("emunandswitcher/folders sx/emutmp02", NULL)) {

		}
		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders sx/emutmp00/boot0.bin", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}
			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				if (f_stat("emunandswitcher/folders sx/emutmp00/boot1.bin", NULL)) {

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin not available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					return;//Test stop
				}
				else { // if the file exists - do this.

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					if (f_stat("emunandswitcher/folders sx/emutmp00/full.00.bin", NULL)) {

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin not available!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);

						return;//Test stop
					}

					else { // if the file exists - do this.

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin available! Move Emunand!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);


						f_rename("sxos/emunand", "emunandswitcher/folders sx/emutmp01");
						f_rename("emunandswitcher/folders sx/emutmp00", "sxos/emunand");
						f_rename("Emutendo", "emunandswitcher/folders sx/emutendo01");
						f_rename("emunandswitcher/folders sx/emutendo00", "Emutendo");
						f_rename("sxos/titles", "emunandswitcher/folders sx/titles01");
						f_rename("emunandswitcher/folders sx/titles00", "sxos/titles");

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 125);
						lv_mbox_set_text(mbox, "Emunand successfully moved...");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);


						return;//Test stop
					}
				}
			}
		}

		return;//Test stop
	}
}


void ctrl_emun2(lv_obj_t* obj, lv_event_t event)//Emunand2 Button Funktion
{
	if (event == LV_EVENT_CLICKED)
	{
		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;

		bg.text.font = &lv_font_montserrat_alternate_20;//Test Font 20 Textbox

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);

		if (f_stat("sxos/emunand", NULL)) {

			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "No Emunand installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}

		if (f_stat("emunandswitcher/folders sx/emutmp01", NULL)) {

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Emunand 2 already installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders sx/emutmp00", NULL)) {

		}

		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders sx/emutmp01/boot0.bin", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}
			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				if (f_stat("emunandswitcher/folders sx/emutmp01/boot1.bin", NULL)) {

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin not available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					return;//Test stop
				}
				else { // if the file exists - do this.

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					if (f_stat("emunandswitcher/folders sx/emutmp01/full.00.bin", NULL)) {

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin not available!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);

						return;//Test stop
					}

					else { // if the file exists - do this.

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin available! Move Emunand!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);


						f_rename("sxos/emunand", "emunandswitcher/folders sx/emutmp02");
						f_rename("emunandswitcher/folders sx/emutmp01", "sxos/emunand");
						f_rename("Emutendo", "emunandswitcher/folders sx/emutendo02");
						f_rename("emunandswitcher/folders sx/emutendo01", "Emutendo");
						f_rename("sxos/titles", "emunandswitcher/folders sx/titles02");
						f_rename("emunandswitcher/folders sx/titles01", "sxos/titles");

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 125);
						lv_mbox_set_text(mbox, "Emunand successfully moved...");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);

						return;//Test stop
					}
				}
			}
		}

		if (f_stat("emunandswitcher/folders sx/emutmp02", NULL)) {

		}
		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders sx/emutmp01/boot0.bin", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}
			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				if (f_stat("emunandswitcher/folders sx/emutmp01/boot1.bin", NULL)) {

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin not available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					return;//Test stop
				}
				else { // if the file exists - do this.

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					if (f_stat("emunandswitcher/folders sx/emutmp01/full.00.bin", NULL)) {

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin not available!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);

						return;//Test stop
					}

					else { // if the file exists - do this.

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin available! Move Emunand!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);


						f_rename("sxos/emunand", "emunandswitcher/folders sx/emutmp00");
						f_rename("emunandswitcher/folders sx/emutmp01", "sxos/emunand");
						f_rename("Emutendo", "emunandswitcher/folders sx/emutendo00");
						f_rename("emunandswitcher/folders sx/emutendo01", "Emutendo");
						f_rename("sxos/titles", "emunandswitcher/folders sx/titles00");
						f_rename("emunandswitcher/folders sx/titles01", "sxos/titles");

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 125);
						lv_mbox_set_text(mbox, "Emunand successfully moved...");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);

						return;//Test stop
					}
				}
			}
		}

		return;//Test stop
	}
}





void ctrl_emun3(lv_obj_t* obj, lv_event_t event)//Emunand3 Button Funktion
{
	if (event == LV_EVENT_CLICKED)
	{
		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;

		bg.text.font = &lv_font_montserrat_alternate_20;//Test Font 20 Textbox

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);

		if (f_stat("sxos/emunand", NULL)) {


			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "No Emunand installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}

		if (f_stat("emunandswitcher/folders sx/emutmp02", NULL)) {

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Emunand 3 already installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders sx/emutmp00", NULL)) {

		}
		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders sx/emutmp02/boot0.bin", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}
			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				if (f_stat("emunandswitcher/folders sx/emutmp02/boot1.bin", NULL)) {

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin not available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					return;//Test stop
				}
				else { // if the file exists - do this.

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					if (f_stat("emunandswitcher/folders sx/emutmp02/full.00.bin", NULL)) {

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin not available!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);

						return;//Test stop
					}

					else { // if the file exists - do this.

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin available! Move Emunand!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);


						f_rename("sxos/emunand", "emunandswitcher/folders sx/emutmp01");
						f_rename("emunandswitcher/folders sx/emutmp02", "sxos/emunand");
						f_rename("Emutendo", "emunandswitcher/folders sx/emutendo01");
						f_rename("emunandswitcher/folders sx/emutendo02", "Emutendo");
						f_rename("sxos/titles", "emunandswitcher/folders sx/titles01");
						f_rename("emunandswitcher/folders sx/titles02", "sxos/titles");

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 125);
						lv_mbox_set_text(mbox, "Emunand successfully moved...");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);


						return;//Test stop
					}
				}
			}
		}

		if (f_stat("emunandswitcher/folders sx/emutmp01", NULL)) {

		}
		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders sx/emutmp02/boot0.bin", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}

			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "boot0.bin available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				if (f_stat("emunandswitcher/folders sx/emutmp02/boot1.bin", NULL)) {

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin not available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					return;//Test stop
				}

				else { // if the file exists - do this.

					mbox = lv_mbox_create(lv_layer_top(), NULL);
					lv_mbox_set_recolor(mbox, true);
					lv_obj_set_width(mbox, LV_DPI * 5);
					lv_obj_set_top(mbox, true);
					lv_obj_set_auto_realign(mbox, true);
					lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
					lv_mbox_set_text(mbox, "boot1.bin available!");
					lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
					lv_mbox_start_auto_close(mbox, 8000);

					if (f_stat("emunandswitcher/folders sx/emutmp02/full.00.bin", NULL)) {

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin not available!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);

						return;//Test stop
					}

					else { // if the file exists - do this.

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
						lv_mbox_set_text(mbox, "full.00.bin available! Move Emunand!");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);


						f_rename("sxos/emunand", "emunandswitcher/folders sx/emutmp00");
						f_rename("emunandswitcher/folders sx/emutmp02", "sxos/emunand");
						f_rename("Emutendo", "emunandswitcher/folders sx/emutendo00");
						f_rename("emunandswitcher/folders sx/emutendo02", "Emutendo");
						f_rename("sxos/titles", "emunandswitcher/folders sx/titles00");
						f_rename("emunandswitcher/folders sx/titles02", "sxos/titles");

						mbox = lv_mbox_create(lv_layer_top(), NULL);
						lv_mbox_set_recolor(mbox, true);
						lv_obj_set_width(mbox, LV_DPI * 5);
						lv_obj_set_top(mbox, true);
						lv_obj_set_auto_realign(mbox, true);
						lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 125);
						lv_mbox_set_text(mbox, "Emunand successfully moved...");
						lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
						lv_mbox_start_auto_close(mbox, 8000);


						return;//Test stop
					}
				}
			}
		}

		return;//Test stop
	}
}




void ctrl_emmc1(lv_obj_t* obj, lv_event_t event)//Emummc1 Button Funktion
{
	if (event == LV_EVENT_CLICKED)
	{

		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;

		bg.text.font = &lv_font_montserrat_alternate_20;//Test Font 20 Textbox

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);

		if (f_stat("emummc/SD00/eMMC", NULL)) {

			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "No Emunand installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}

		if (f_stat("emunandswitcher/folders atm/emutmp00", NULL)) {

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Emunand 1 already installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders atm/emutmp01", NULL)) {

		}
		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders atm/emutmp00/eMMC", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop

			}

			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
				lv_mbox_set_text(mbox, "Move Emunand!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				f_rename("emummc/SD00", "emunandswitcher/folders atm/emutmp02");
				f_rename("emunandswitcher/folders atm/emutmp00", "emummc/SD00");
				//f_rename("emummc/SD00/Nintendo", "emunandswitcher/folders atm/emutendo02");
				//f_rename("emunandswitcher/folders atm/emutendo00", "emummc/SD00/Nintendo");
				//f_rename("atmosphere/titles", "emunandswitcher/folders atm/titles02");
				//f_rename("emunandswitcher/folders atm/titles00", "atmosphere/titles");
				f_rename("atmosphere/contents", "emunandswitcher/folders atm/contents02");
				f_rename("emunandswitcher/folders atm/contents00", "atmosphere/contents");

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
				lv_mbox_set_text(mbox, "Emunand successfully moved...");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}

		}


		if (f_stat("emunandswitcher/folders atm/emutmp02", NULL)) {

		}
		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders atm/emutmp00/eMMC", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}

			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
				lv_mbox_set_text(mbox, "Move Emunand!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				f_rename("emummc/SD00", "emunandswitcher/folders atm/emutmp01");
				f_rename("emunandswitcher/folders atm/emutmp00", "emummc/SD00");
				//f_rename("emummc/SD00/Nintendo", "emunandswitcher/folders atm/emutendo01");
				//f_rename("emunandswitcher/folders atm/emutendo00", "emummc/SD00/Nintendo"); 
				f_rename("atmosphere/contents", "emunandswitcher/folders atm/contents01");
				f_rename("emunandswitcher/folders atm/contents00", "atmosphere/contents");

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
				lv_mbox_set_text(mbox, "Emunand successfully moved...");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);


				return;//Test stop
			}
		}

		return;//Test stop
	}
}


void ctrl_emmc2(lv_obj_t* obj, lv_event_t event)//Emummc2 Button Funktion
{
	if (event == LV_EVENT_CLICKED)
	{

		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;

		bg.text.font = &lv_font_montserrat_alternate_20;//Test Font 20 Textbox

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);

		if (f_stat("emummc/SD00/eMMC", NULL)) {

			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "No Emunand installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}

		if (f_stat("emunandswitcher/folders atm/emutmp01", NULL)) {

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Emunand 2 already installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders atm/emutmp00", NULL)) {

		}

		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders atm/emutmp01/eMMC", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}

			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
				lv_mbox_set_text(mbox, "Move Emunand!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				f_rename("emummc/SD00", "emunandswitcher/folders atm/emutmp02");
				f_rename("emunandswitcher/folders atm/emutmp01", "emummc/SD00");
				//f_rename("emummc/SD00/Nintendo", "emunandswitcher/folders atm/emutendo02");
				//f_rename("emunandswitcher/folders atm/emutendo01", "emummc/SD00/Nintendo");
				f_rename("atmosphere/contents", "emunandswitcher/folders atm/contents02");
				f_rename("emunandswitcher/folders atm/contents01", "atmosphere/contents");

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
				lv_mbox_set_text(mbox, "Emunand successfully moved...");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}


		}

		if (f_stat("emunandswitcher/folders atm/emutmp02", NULL)) {

		}
		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders atm/emutmp01/eMMC", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}

			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
				lv_mbox_set_text(mbox, "Move Emunand!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				f_rename("emummc/SD00", "emunandswitcher/folders atm/emutmp00");
				f_rename("emunandswitcher/folders atm/emutmp01", "emummc/SD00");
				//f_rename("emummc/SD00/Nintendo", "emunandswitcher/folders atm/emutendo00");
				//f_rename("emunandswitcher/folders atm/emutendo01", "emummc/SD00/Nintendo"); 
				f_rename("atmosphere/contents", "emunandswitcher/folders atm/contents00");
				f_rename("emunandswitcher/folders atm/contents01", "atmosphere/contents");

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
				lv_mbox_set_text(mbox, "Emunand successfully moved...");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}
		}

		return;//Test stop
	}
}


void ctrl_emmc3(lv_obj_t* obj, lv_event_t event)//Emummc3 Button Funktion
{
	if (event == LV_EVENT_CLICKED)
	{
		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;

		bg.text.font = &lv_font_montserrat_alternate_20;//Test Font 20 Textbox

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);

		if (f_stat("emummc/SD00/eMMC", NULL)) {

			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "No Emunand installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}

		if (f_stat("emunandswitcher/folders atm/emutmp02", NULL)) {

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Emunand 3 already installed!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			return;//Test stop
		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders atm/emutmp00", NULL)) {

		}
		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders atm/emutmp02/eMMC", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}

			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
				lv_mbox_set_text(mbox, "Move Emunand!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				f_rename("emummc/SD00", "emunandswitcher/folders atm/emutmp01");
				f_rename("emunandswitcher/folders atm/emutmp02", "emummc/SD00");
				//f_rename("emummc/SD00/Nintendo", "emunandswitcher/folders atm/emutendo01");
				//f_rename("emunandswitcher/folders atm/emutendo02", "emummc/SD00/Nintendo");
				f_rename("atmosphere/contents", "emunandswitcher/folders atm/contents01");
				f_rename("emunandswitcher/folders atm/contents02", "atmosphere/contents");

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
				lv_mbox_set_text(mbox, "Emunand successfully moved...");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}
		}

		if (f_stat("emunandswitcher/folders atm/emutmp01", NULL)) {

		}
		else { // if the directory exists - do this.

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 5);
			lv_mbox_set_text(mbox, "Searching files!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

			if (f_stat("emunandswitcher/folders atm/emutmp02/eMMC", NULL)) {

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC not available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}

			else { // if the file exists - do this.

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
				lv_mbox_set_text(mbox, "eMMC available!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 65);
				lv_mbox_set_text(mbox, "Move Emunand!");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				f_rename("emummc/SD00", "emunandswitcher/folders atm/emutmp00");
				f_rename("emunandswitcher/folders atm/emutmp02", "emummc/SD00");
				//f_rename("emummc/SD00/Nintendo", "emunandswitcher/folders atm/emutendo00");
				//f_rename("emunandswitcher/folders atm/emutendo02", "emummc/SD00/Nintendo");
				f_rename("atmosphere/contents", "emunandswitcher/folders atm/contents00");
				f_rename("emunandswitcher/folders atm/contents02", "atmosphere/contents");

				mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 5);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 95);
				lv_mbox_set_text(mbox, "Emunand successfully moved...");
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				lv_mbox_start_auto_close(mbox, 8000);

				return;//Test stop
			}
		}

		return;//Test stop
	}
}




//definition mehrere Texfelder Tastatur springen
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void kb_event_cb(lv_obj_t* event_kb, lv_event_t event);
static void ta_event_cb(lv_obj_t* ta, lv_event_t event);

/**********************
 *   STATIC FUNCTIONS
 **********************/

 //Definition statische Tastatur um in mehreren Funtkionen zu benutzen
static lv_obj_t* kb;

//Definition statische Texfelder um in mehreren Funtkionen zu benutzen
static lv_obj_t* tastd;
static lv_obj_t* tamin;
static lv_obj_t* tatag;
static lv_obj_t* tamon;
static lv_obj_t* tajahr;

static lv_obj_t* taemun1;
static lv_obj_t* taemun2;
static lv_obj_t* taemun3;
static lv_obj_t* taemmc1;
static lv_obj_t* taemmc2;
static lv_obj_t* taemmc3;

static void kb_event_cb(lv_obj_t* event_kb, lv_event_t event)
{
	// Just call the regular event handler
	lv_kb_def_event_cb(event_kb, event);

}

static void ta_event_cb(lv_obj_t* ta, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		// Focus on the clicked text area
		if (kb != NULL)
			lv_kb_set_ta(kb, ta);
	}

}



void ctrl_confini(lv_obj_t* obj, lv_event_t event)//Button Funktion edit Config.ini
{
	if (event == LV_EVENT_CLICKED)
	{
		//Header Style Konfiguration
		static lv_style_t header_style;
		lv_style_copy(&header_style, &lv_style_pretty);
		header_style.text.color = LV_COLOR_WHITE; //geändert in blau
		header_style.text.font = &lv_font_montserrat_alternate_30;
		header_style.body.opa = LV_OPA_50;
		
		//Fenster Style konfiguration
		static lv_style_t win_style;
		lv_style_copy(&win_style, &lv_style_btn_rel);
		win_style.body.padding.left = LV_DPI / 6;
		win_style.body.padding.right = LV_DPI / 6;
		win_style.body.padding.top = 0;
		win_style.body.padding.bottom = 0;
		win_style.body.padding.inner = LV_DPI / 6;
		win_style.body.opa = LV_OPA_80;

		//Definition Transparentes Label jeweils bei Label hinzufügen: lv_obj_set_style(label, &inv_label);
		static lv_style_t inv_label;
		lv_style_copy(&inv_label, &lv_style_transp);
		inv_label.text.font = NULL;
		
		//Create a window to hold all the objects, bei lv_layer_top() ist das Fenster nicht volle Bildgrösse, mit lv_scr_act() ok
		lv_obj_t* win = lv_win_create(lv_scr_act(), NULL);//lv_obj_t* win = lv_win_create(lv_disp_get_scr_act(NULL), NULL);//In lc_conf.h aktivieren	
		lv_win_set_title(win, "Edit Config.ini");
		lv_page_set_scrl_layout(lv_win_get_content(win), LV_LAYOUT_OFF);
		lv_win_set_style(win, LV_WIN_STYLE_CONTENT, &win_style);
		lv_win_set_style(win, LV_WIN_STYLE_HEADER, &header_style);//Header Style ausführen
		
		//Add control button to the header
		lv_obj_t* close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);//Add close button and use built-in close action
		lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);
		lv_obj_set_style(close_btn, LV_LABEL_STYLE_MAIN);//Style Farbe close button

		lv_obj_t* reload_btn = lv_win_add_btn(win, LV_SYMBOL_REFRESH);//Reload button im Header nur Symbol möglich
		lv_obj_set_event_cb(reload_btn, ctrl_lauch_payload);
		lv_obj_set_style(reload_btn, LV_LABEL_STYLE_MAIN);//Style Farbe Reload button

		lv_obj_t* label = lv_label_create(reload_btn, NULL);//Fake Label unsichtbar für Payload
		lv_label_set_text(label, "emunandswitcher/emunsw.bin");
		lv_obj_set_style(label, &inv_label);//Unsichtbares Label

		lv_obj_t* save_btn = lv_win_add_btn(win, LV_SYMBOL_SAVE);//Save button im Header nur Symbol möglich
		lv_obj_set_event_cb(save_btn, ctrl_confinisave);
		lv_obj_set_style(save_btn, LV_LABEL_STYLE_MAIN);//Style Farbe Save button


		//Text aus ini lesen
		#define MAXCHAR 100
		FIL fp;
		int linien = 0;
		char info[MAXCHAR];

		//Texfenster definieren
		taemun1 = lv_ta_create(win, NULL);
		taemun2 = lv_ta_create(win, NULL);
		taemun3 = lv_ta_create(win, NULL);
		taemmc1 = lv_ta_create(win, NULL);
		taemmc2 = lv_ta_create(win, NULL);
		taemmc3 = lv_ta_create(win, NULL);

		if (f_stat("emunandswitcher/config.ini", NULL)) {

			//Definition Textbox Style
			static lv_style_t bg;
			lv_style_copy(&bg, &lv_style_pretty);
			bg.text.color = LV_COLOR_RED;
			bg.body.opa = LV_OPA_0;
			bg.text.font = &lv_font_montserrat_alternate_20;

			lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
			lv_mbox_set_text(mbox, "Config.ini not found!\nPlease copy to folder emunandswitcher...");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 8000);

		}

		else { // if the file exists - do this.

			f_open(&fp, "emunandswitcher/config.ini", FA_READ);
			while (f_gets(info, MAXCHAR, &fp)) {
				linien++;

				char* emun1;
				char* emun2;
				char* emun3;
				char* emmc1;
				char* emmc2;
				char* emmc3;

				emun1 = strstr(info, "Emunand 1 =");
				emun2 = strstr(info, "Emunand 2 =");
				emun3 = strstr(info, "Emunand 3 =");
				emmc1 = strstr(info, "Emuemmc 1 =");
				emmc2 = strstr(info, "Emuemmc 2 =");
				emmc3 = strstr(info, "Emuemmc 3 =");
				
				emun1[strlen(emun1) - 1] = '\0';// Stelle von hinten gerechnet abschneiden
				emun2[strlen(emun2) - 1] = '\0';// Stelle von hinten gerechnet abschneiden
				emun3[strlen(emun3) - 1] = '\0';// Stelle von hinten gerechnet abschneiden
				emmc1[strlen(emmc1) - 1] = '\0';// Stelle von hinten gerechnet abschneiden
				emmc2[strlen(emmc2) - 1] = '\0';// Stelle von hinten gerechnet abschneiden
				emmc3[strlen(emmc3) - 0] = '\0';// Stelle von hinten gerechnet abschneiden

				//Texfenster definieren und anzeigen, Hauptdefinition oben ausserhalb Funktion statisch
				lv_ta_set_cursor_type(taemun1, LV_CURSOR_BLOCK);//Erster aktiver Cursor setzen
				lv_obj_set_size(taemun1, 900, 80);
				lv_ta_set_text(taemun1, emun1);    //Set an initial text
				//lv_ta_set_max_length(taemun1, 100);//Definition max 100 Zeichen !!!Freeze bei btn betätigung??? 101 ev???
				lv_obj_set_pos(taemun1, 190, 20);
				lv_obj_set_event_cb(taemun1, ta_event_cb);//Event Fensterwechsel

				lv_ta_set_cursor_type(taemun2, LV_CURSOR_BLOCK | LV_CURSOR_HIDDEN);//Cursor definieren aber verbergen
				lv_obj_set_size(taemun2, 900, 80);
				lv_ta_set_text(taemun2, emun2);    //Set an initial text
				lv_obj_set_pos(taemun2, 190, 70);
				lv_obj_set_event_cb(taemun2, ta_event_cb);//Event Fensterwechsel

				lv_ta_set_cursor_type(taemun3, LV_CURSOR_BLOCK | LV_CURSOR_HIDDEN);//Cursor definieren aber verbergen
				lv_obj_set_size(taemun3, 900, 80);
				lv_ta_set_text(taemun3, emun3);    //Set an initial text
				lv_obj_set_pos(taemun3, 190, 120);
				lv_obj_set_event_cb(taemun3, ta_event_cb);//Event Fensterwechsel

				lv_ta_set_cursor_type(taemmc1, LV_CURSOR_BLOCK | LV_CURSOR_HIDDEN);//Cursor definieren aber verbergen
				lv_obj_set_size(taemmc1, 900, 80);
				lv_ta_set_text(taemmc1, emmc1);    //Set an initial text
				lv_obj_set_pos(taemmc1, 190, 170);
				lv_obj_set_event_cb(taemmc1, ta_event_cb);//Event Fensterwechsel

				lv_ta_set_cursor_type(taemmc2, LV_CURSOR_BLOCK | LV_CURSOR_HIDDEN);//Cursor definieren aber verbergen
				lv_obj_set_size(taemmc2, 900, 80);
				lv_ta_set_text(taemmc2, emmc2);    //Set an initial text
				lv_obj_set_pos(taemmc2, 190, 220);
				lv_obj_set_event_cb(taemmc2, ta_event_cb);//Event Fensterwechsel

				lv_ta_set_cursor_type(taemmc3, LV_CURSOR_BLOCK | LV_CURSOR_HIDDEN);//Cursor definieren aber verbergen
				lv_obj_set_size(taemmc3, 900, 80);
				lv_ta_set_text(taemmc3, emmc3);    //Set an initial text
				lv_obj_set_pos(taemmc3, 190, 270);
				lv_obj_set_event_cb(taemmc3, ta_event_cb);//Event Fensterwechsel

				free(emun1);
				free(emun2);
				free(emun3);
				free(emmc1);
				free(emmc2);
				free(emmc3);
			}
		}
		f_close(&fp);


		//Style für Tastatur
		static lv_style_t style_kb;
		lv_style_copy(&style_kb, &lv_style_plain);
		style_kb.body.opa = LV_OPA_70;
		style_kb.body.main_color = lv_color_hex3(0x333);
		style_kb.body.grad_color = lv_color_hex3(0x333);
		style_kb.body.padding.left = 0;
		style_kb.body.padding.right = 0;
		style_kb.body.padding.top = 0;
		style_kb.body.padding.bottom = 0;
		style_kb.body.padding.inner = 0;

		static lv_style_t style_kb_rel;
		lv_style_copy(&style_kb_rel, &lv_style_plain);
		style_kb_rel.body.opa = LV_OPA_TRANSP;
		style_kb_rel.body.radius = 0;
		style_kb_rel.body.border.width = 1;
		style_kb_rel.body.border.color = LV_COLOR_SILVER;
		style_kb_rel.body.border.opa = LV_OPA_50;
		style_kb_rel.body.main_color = lv_color_hex3(0x333);    //Recommended if LV_VDB_SIZE == 0 and bpp > 1 fonts are used
		style_kb_rel.body.grad_color = lv_color_hex3(0x333);
		style_kb_rel.text.color = LV_COLOR_WHITE;

		static lv_style_t style_kb_pr;
		lv_style_copy(&style_kb_pr, &lv_style_plain);
		style_kb_pr.body.radius = 0;
		style_kb_pr.body.opa = LV_OPA_50;
		style_kb_pr.body.main_color = LV_COLOR_WHITE;
		style_kb_pr.body.grad_color = LV_COLOR_WHITE;
		style_kb_pr.body.border.width = 1;
		style_kb_pr.body.border.color = LV_COLOR_SILVER;

		//Tastatur erstellen neu mit Fenster wechsel
		kb = lv_kb_create(win, NULL);
		lv_obj_set_size(kb, 1080, 300);
		lv_obj_set_pos(kb, 100, 340);
		lv_obj_set_event_cb(kb, kb_event_cb); // Setting a custom event handler stops the keyboard from closing automatically
		lv_kb_set_ta(kb, taemun1);// Focus it on one of the text areas to start
		lv_kb_set_cursor_manage(kb, true);// Automatically show/hide cursors on text areas

		//Style für Tastatur übernehmen
		//lv_kb_set_style(kb, LV_KB_STYLE_BG, &style_kb);
		lv_kb_set_style(kb, LV_KB_STYLE_BTN_REL, &style_kb_rel);
		lv_kb_set_style(kb, LV_KB_STYLE_BTN_PR, &style_kb_pr);

	}
}

//Button Funktion Config.ini einstellen SAVE
void ctrl_confinisave(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		//Text in ini speichern
		FIL fp;
		
		//const char* str = lv_ta_get_text(const lv_obj_t * ta); Definition Text zurück aus Textfeld
		const char* emun1save = lv_ta_get_text(taemun1);
		const char* emun2save = lv_ta_get_text(taemun2);
		const char* emun3save = lv_ta_get_text(taemun3);
		const char* emmc1save = lv_ta_get_text(taemmc1);
		const char* emmc2save = lv_ta_get_text(taemmc2);
		const char* emmc3save = lv_ta_get_text(taemmc3);

		//alte Config.ini löschen, sonst komischer Fehler letzter String emmc3save beim speichern immer MM am ende
		f_unlink("emunandswitcher/config.ini");

		//Prüfung Config.ini vorhanden sonst erstellen
		if (f_stat("emunandswitcher/config.ini", NULL)) {

			f_open(&fp, "emunandswitcher/config.ini", FA_CREATE_NEW);//FA_WRITE, FA_CREATE_NEW
			f_close(&fp);

		}

		//Datei öffnen und Text speichern
		f_open(&fp, "emunandswitcher/config.ini", FA_WRITE);//FA_WRITE, FA_CREATE_NEW

		f_puts(emun1save, &fp);
		f_puts("\n", &fp);
		f_puts(emun2save, &fp);
		f_puts("\n", &fp);
		f_puts(emun3save, &fp);
		f_puts("\n", &fp);
		f_puts(emmc1save, &fp);
		f_puts("\n", &fp);
		f_puts(emmc2save, &fp);
		f_puts("\n", &fp);
		f_puts(emmc3save, &fp);
		//f_puts("\0", &fp);

		f_close(&fp);

		//Definition Textbox Style
		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;
		bg.text.font = &lv_font_montserrat_alternate_20;

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
		lv_mbox_set_recolor(mbox, true);
		lv_obj_set_width(mbox, LV_DPI * 5);
		lv_obj_set_top(mbox, true);
		lv_obj_set_auto_realign(mbox, true);
		lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
		lv_mbox_set_text(mbox, "Config.ini saved!");
		//lv_mbox_set_text(mbox, emun1save);//Test anzeige Text Emunand 1
		lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
		lv_mbox_start_auto_close(mbox, 8000);

	}
}



void ctrl_rtctime(lv_obj_t* obj, lv_event_t event)//Button Funktion Zeit und Datum RTC einstellen
{
	if (event == LV_EVENT_CLICKED)
	{
		//Header Style Konfiguration
		static lv_style_t header_style;
		lv_style_copy(&header_style, &lv_style_pretty);
		header_style.text.color = LV_COLOR_WHITE;
		header_style.text.font = &lv_font_montserrat_alternate_30;
		header_style.body.opa = LV_OPA_50;
	
		//Fenster Style Konfiguration
		static lv_style_t win_style;
		lv_style_copy(&win_style, &lv_style_btn_rel);
		win_style.body.padding.left = LV_DPI / 6;
		win_style.body.padding.right = LV_DPI / 6;
		win_style.body.padding.top = 0;
		win_style.body.padding.bottom = 0;
		win_style.body.padding.inner = LV_DPI / 6;
		win_style.body.opa = LV_OPA_80;

		//Textfenster Style Konfiguration Schrift 110px
		static lv_style_t tafont110_style;
		lv_style_copy(&tafont110_style, &lv_style_pretty);
		tafont110_style.text.color = LV_COLOR_WHITE; //Textfarbe
		tafont110_style.text.font = &lv_font_montserrat_alternate_110;
		tafont110_style.body.opa = LV_OPA_20;

		//Create a window to hold all the objects, bei lv_layer_top() ist das Fenster nicht volle Bildgrösse, mit lv_scr_act() ok
		lv_obj_t* win = lv_win_create(lv_scr_act(), NULL);//lv_obj_t* win = lv_win_create(lv_disp_get_scr_act(NULL), NULL);//win = Fenster, in lc_conf.h aktivieren		
		lv_win_set_title(win, "RTC Time and Date");
		lv_page_set_scrl_layout(lv_win_get_content(win), LV_LAYOUT_OFF);
		lv_win_set_style(win, LV_WIN_STYLE_CONTENT, &win_style);
		lv_win_set_style(win, LV_WIN_STYLE_HEADER, &header_style);//Header Style ausführen

		//Add control button to the header
		lv_obj_t* close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);//Add close button and use built-in close action
		lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);
		lv_obj_set_style(close_btn, LV_LABEL_STYLE_MAIN);//Style Farbe close button

		lv_obj_t* save_btn = lv_win_add_btn(win, LV_SYMBOL_SAVE);//Save button im Header nur Symbol möglich
		lv_obj_set_event_cb(save_btn, ctrl_rtctimesave);
		lv_obj_set_style(save_btn, LV_LABEL_STYLE_MAIN);//Style Farbe Save button


		//RTC auslesen...
		rtc_time_t time;

		// Get sensor data Clock
		u8 val = 0;

		// Update RTC regs from RTC clock.
		i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_UPDATE0_REG, MAX77620_RTC_READ_UPDATE);

		// Get control reg config.
		val = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_CONTROL_REG);

		// Get time.
		time.sec = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_SEC_REG) & 0x7F;
		time.min = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_MIN_REG) & 0x7F;

		time.hour = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_HOUR_REG) & 0x1F;

		if (!(val & MAX77620_RTC_24H) && time.hour & MAX77620_RTC_HOUR_PM_MASK)
			time.hour = (time.hour & 0xF) + 12;

		// Get day of week. 1: Monday to 7: Sunday.
		time.weekday = 0;
		val = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_WEEKDAY_REG);
		for (int i = 0; i < 8; i++)
		{
			time.weekday++;
			if (val & 1)
				break;
			val >>= 1;
		}

		// Get date.
		time.date = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_DATE_REG) & 0x1f;
		time.year = (i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_YEAR_REG) & 0x7F) + 2000;
		time.month = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_MONTH_REG) & 0xF;

		
		//Parse time and use offset. Test eigen...

		//Zeitzone MEZ+1
		if (time.hour == 23) time.hour = 0;
		else time.hour += 1;

		//time.month = (time.month) - 8;//Test Sommerzeit

		//Sommerzeit
		int MEZ = 0;

		if (time.month > 3 && time.month < 10)// Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
		{
			if (time.hour == 23) time.hour = 0;
			else time.hour += 1;
		}

		//Neu test
		if (time.month == 3 && (time.hour + 24 * time.date) >= (1 + MEZ + 24 * (31 - (5 * time.year / 4 + 4) % 7)))
		{
			if (time.hour == 23) time.hour = 0;
			else time.hour += 1;
		}

		if (time.month == 10 && (time.hour + 24 * time.date) < (1 + MEZ + 24 * (31 - (5 * time.year / 4 + 1) % 7)))
		{
			if (time.hour == 23) time.hour = 0;
			else time.hour += 1;
		}

		//char* zeit = (char*)malloc(64);
		char* std = (char*)malloc(64);
		char* min = (char*)malloc(64);
		char* tag = (char*)malloc(64);
		char* mon = (char*)malloc(64);
		char* jahr = (char*)malloc(64);

		s_printf(std, "%02d", time.hour);
		s_printf(min, "%02d", time.min);
		s_printf(tag, "%02d", time.date);
		s_printf(mon, "%02d", time.month);
		s_printf(jahr, "%02d", time.year);

		//Texfenster definieren und anzeigen, Hauptdefinition oben ausserhalb Funktion statisch
		tastd = lv_ta_create(win, NULL);//ta = Textarea, in lc_conf.h aktivieren
		lv_ta_set_cursor_type(tastd, LV_CURSOR_BLOCK);//Erster aktiver Cursor setzen
		lv_obj_set_size(tastd, 190, 140);
		lv_obj_set_pos(tastd, 25, 50);
		lv_ta_set_text(tastd, std);    //Set an initial text
		lv_ta_set_max_length(tastd, 2);//Definition max zwei Zeichen
		lv_ta_set_accepted_chars(tastd, "0123456789");//Definition nur diese Zeichen erlauben
		lv_ta_set_style(tastd, LV_LABEL_STYLE_MAIN, &tafont110_style);//Textgrösse 110 und Style Textfenster ausführen
		lv_obj_set_event_cb(tastd, ta_event_cb);//Event Fensterwechsel

		//Label Datum und Zeit Textbox
		lv_obj_t* label = lv_label_create(win, NULL);
		lv_label_set_text(label, "Hour");
		lv_obj_align(label, tastd, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &header_style);

		tamin = lv_ta_create(win, NULL);
		lv_ta_set_cursor_type(tamin, LV_CURSOR_BLOCK | LV_CURSOR_HIDDEN);//Cursor definieren aber verbergen
		lv_obj_set_size(tamin, 190, 140);
		lv_obj_set_pos(tamin, 235, 50);
		lv_ta_set_text(tamin, min);    //Set an initial text
		lv_ta_set_max_length(tamin, 2);//Definition max zwei Zeichen
		lv_ta_set_accepted_chars(tamin, "0123456789");//Definition nur diese Zeichen erlauben
		lv_ta_set_style(tamin, LV_LABEL_STYLE_MAIN, &tafont110_style);//Textgrösse 110 und Style Textfenster ausführen
		lv_obj_set_event_cb(tamin, ta_event_cb);//Event Fensterwechsel

		//Label Datum und Zeit Textbox
		label = lv_label_create(win, NULL);
		lv_label_set_text(label, "Minute");
		lv_obj_align(label, tamin, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &header_style);

		tatag = lv_ta_create(win, NULL);
		lv_ta_set_cursor_type(tatag, LV_CURSOR_BLOCK | LV_CURSOR_HIDDEN);//Cursor definieren aber verbergen
		lv_obj_set_size(tatag, 190, 140);
		lv_obj_set_pos(tatag, 495, 50);
		lv_ta_set_text(tatag, tag);    //Set an initial text
		lv_ta_set_max_length(tatag, 2);//Definition max zwei Zeichen
		lv_ta_set_accepted_chars(tatag, "0123456789");//Definition nur diese Zeichen erlauben
		lv_ta_set_style(tatag, LV_LABEL_STYLE_MAIN, &tafont110_style);//Textgrösse 110 und Style Textfenster ausführen
		lv_obj_set_event_cb(tatag, ta_event_cb);//Event Fensterwechsel

		//Label Datum und Zeit Textbox
		label = lv_label_create(win, NULL);
		lv_label_set_text(label, "Day");
		lv_obj_align(label, tatag, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &header_style);

		tamon = lv_ta_create(win, NULL);
		lv_ta_set_cursor_type(tamon, LV_CURSOR_BLOCK | LV_CURSOR_HIDDEN);//Cursor definieren aber verbergen
		lv_obj_set_size(tamon, 190, 140);
		lv_obj_set_pos(tamon, 705, 50);
		lv_ta_set_text(tamon, mon);    //Set an initial text
		lv_ta_set_max_length(tamon, 2);//Definition max zwei Zeichen
		lv_ta_set_accepted_chars(tamon, "0123456789");//Definition nur diese Zeichen erlauben
		lv_ta_set_style(tamon, LV_LABEL_STYLE_MAIN, &tafont110_style);//Textgrösse 110 und Style Textfenster ausführen
		lv_obj_set_event_cb(tamon, ta_event_cb);//Event Fensterwechsel

		//Label Datum und Zeit Textbox
		label = lv_label_create(win, NULL);
		lv_label_set_text(label, "Month");
		lv_obj_align(label, tamon, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &header_style);

		tajahr = lv_ta_create(win, NULL);
		lv_ta_set_cursor_type(tajahr, LV_CURSOR_BLOCK | LV_CURSOR_HIDDEN);//Cursor definieren aber verbergen
		lv_obj_set_size(tajahr, 330, 140);
		lv_obj_set_pos(tajahr, 915, 50);
		lv_ta_set_text(tajahr, jahr);    //Set an initial text
		lv_ta_set_max_length(tajahr, 4);//Definition max vier Zeichen
		lv_ta_set_accepted_chars(tajahr, "0123456789");//Definition nur diese Zeichen erlauben
		lv_ta_set_style(tajahr, LV_LABEL_STYLE_MAIN, &tafont110_style);//Textgrösse 110 und Style Textfenster ausführen
		lv_obj_set_event_cb(tajahr, ta_event_cb);//Event Fensterwechsel

		//Label Datum und Zeit Textbox
		label = lv_label_create(win, NULL);
		lv_label_set_text(label, "Year");
		lv_obj_align(label, tajahr, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &header_style);


		//Label Datum und Zeit Punkte Textbox
		lv_obj_t* label_punkte = lv_label_create(win, NULL);
		lv_label_set_text(label_punkte, ":");
		lv_obj_set_pos(label_punkte, 213, 50);
		lv_label_set_style(label_punkte, LV_LABEL_STYLE_MAIN, &tafont110_style);

		lv_obj_t* label_punkt1 = lv_label_create(win, NULL);
		lv_label_set_text(label_punkt1, ".");
		lv_obj_set_pos(label_punkt1, 683, 50);
		lv_label_set_style(label_punkt1, LV_LABEL_STYLE_MAIN, &tafont110_style);

		lv_obj_t* label_punkt2 = lv_label_create(win, NULL);
		lv_label_set_text(label_punkt2, ".");
		lv_obj_set_pos(label_punkt2, 893, 50);
		lv_label_set_style(label_punkt2, LV_LABEL_STYLE_MAIN, &tafont110_style);

		free(std);
		free(min);
		free(tag);
		free(mon);
		free(jahr);


		//Style für Tastatur
		static lv_style_t style_kb;
		lv_style_copy(&style_kb, &lv_style_plain);
		style_kb.body.opa = LV_OPA_70;
		style_kb.body.main_color = lv_color_hex3(0x333);
		style_kb.body.grad_color = lv_color_hex3(0x333);
		style_kb.body.padding.left = 0;
		style_kb.body.padding.right = 0;
		style_kb.body.padding.top = 0;
		style_kb.body.padding.bottom = 0;
		style_kb.body.padding.inner = 0;

		static lv_style_t style_kb_rel;
		lv_style_copy(&style_kb_rel, &lv_style_plain);
		style_kb_rel.body.opa = LV_OPA_TRANSP;
		style_kb_rel.body.radius = 0;
		style_kb_rel.body.border.width = 1;
		style_kb_rel.body.border.color = LV_COLOR_SILVER;
		style_kb_rel.body.border.opa = LV_OPA_50;
		style_kb_rel.body.main_color = lv_color_hex3(0x333);    //Recommended if LV_VDB_SIZE == 0 and bpp > 1 fonts are used
		style_kb_rel.body.grad_color = lv_color_hex3(0x333);
		style_kb_rel.text.color = LV_COLOR_WHITE;

		static lv_style_t style_kb_pr;
		lv_style_copy(&style_kb_pr, &lv_style_plain);
		style_kb_pr.body.radius = 0;
		style_kb_pr.body.opa = LV_OPA_50;
		style_kb_pr.body.main_color = LV_COLOR_WHITE;
		style_kb_pr.body.grad_color = LV_COLOR_WHITE;
		style_kb_pr.body.border.width = 1;
		style_kb_pr.body.border.color = LV_COLOR_SILVER;

		//Tastatur erstellen neu mit Fenster wechsel
		kb = lv_kb_create(win, NULL);
		lv_obj_set_size(kb, 1080, 300);
		lv_obj_set_pos(kb, 100, 300);
		lv_obj_set_event_cb(kb, kb_event_cb); // Setting a custom event handler stops the keyboard from closing automatically
		lv_kb_set_ta(kb, tastd);// Focus it on one of the text areas to start
		lv_kb_set_cursor_manage(kb, true);// Automatically show/hide cursors on text areas

		//Style für Tastatur übernehmen
		//lv_kb_set_style(kb, LV_KB_STYLE_BG, &style_kb);
		lv_kb_set_style(kb, LV_KB_STYLE_BTN_REL, &style_kb_rel);
		lv_kb_set_style(kb, LV_KB_STYLE_BTN_PR, &style_kb_pr);

	}
}

//Button Funktion Zeit und Datum RTC einstellen SAVE
void ctrl_rtctimesave(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		//RTC Zeifelder definition...
		rtc_time_t time;

		//const char* str = lv_ta_get_text(const lv_obj_t * ta); Definition Text zurück aus Textfeld
		const char* getstd = lv_ta_get_text(tastd);
		const char* getmin = lv_ta_get_text(tamin);
		const char* gettag = lv_ta_get_text(tatag);
		const char* getmonth = lv_ta_get_text(tamon);
		const char* getjahr = lv_ta_get_text(tajahr);

		//Zahlen umwandeln zu int, String zu int: atoi
		int stdaus = atoi(getstd);
		int minaus = atoi(getmin);
		int tagaus = atoi(gettag);
		int monthaus = atoi(getmonth);
		int jahraus = atoi(getjahr);

		//int in time felder einfügen und korrigieren
		//time.month += 0;//Test manuel
		time.hour = stdaus;
		time.min = minaus;
		time.date = tagaus;
		time.month = monthaus;
		time.year = jahraus;

		//Zeitzone MEZ+1 Rückgängig
		if (time.hour == 00) time.hour = 23;
		else time.hour -= 1;

		//Sommerzeit Rückgängig
		int MEZ = 0;

		if (time.month > 3 && time.month < 10)// Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
		{
			if (time.hour == 00) time.hour = 23;
			else time.hour -= 1;
		}

		if (time.month == 3 && (time.hour + 24 * time.date) >= (1 + MEZ + 24 * (31 - (5 * time.year / 4 + 4) % 7)))
		{
			if (time.hour == 00) time.hour = 23;
			else time.hour -= 1;
		}

		if (time.month == 10 && (time.hour + 24 * time.date) < (1 + MEZ + 24 * (31 - (5 * time.year / 4 + 1) % 7)))
		{
			if (time.hour == 00) time.hour = 23;
			else time.hour -= 1;
		}

		time.year -= 2000;//Jahr Vorzahl 20 weg zum speichern

		//Set RTC Time
		i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_HOUR_REG, time.hour);
		i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_MIN_REG, time.min);
		i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_DATE_REG, time.date);
		i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_MONTH_REG, time.month);
		i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_YEAR_REG, time.year);
		
		//Update RTC clock from RTC regs.
		i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_UPDATE0_REG, MAX77620_RTC_WRITE_UPDATE);

		//Definition Textbox Style
		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;
		bg.text.font = &lv_font_montserrat_alternate_20;

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
		lv_mbox_set_recolor(mbox, true);
		lv_obj_set_width(mbox, LV_DPI * 5);
		lv_obj_set_top(mbox, true);
		lv_obj_set_auto_realign(mbox, true);
		lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
		lv_mbox_set_text(mbox, "RTC Time and Date saved!");
		lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
		lv_mbox_start_auto_close(mbox, 8000);

	}
}






/*//Test Button Funktion
void ctrl_test(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;
		bg.text.font = &lv_font_montserrat_alternate_20;

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
		lv_mbox_set_recolor(mbox, true);
		lv_obj_set_width(mbox, LV_DPI * 5);
		lv_obj_set_top(mbox, true);
		lv_obj_set_auto_realign(mbox, true);
		lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 5, 5);
		lv_mbox_set_text(mbox, "Test Helligkeit.....");
		lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
		lv_mbox_start_auto_close(mbox, 8000);

		display_backlight_brightness(50, 1000);

	}
}*/




/*int tool_infoemuninstalled()
{
	#define MAXCHAR 100
	FIL fp;
	int linie = 0;
	char info[MAXCHAR];

	static lv_style_t bg;//Definition Textbox
	lv_style_copy(&bg, &lv_style_pretty);
	bg.text.color = LV_COLOR_WHITE;
	bg.body.opa = LV_OPA_0;

	static lv_style_t labels_style;//Definiton Label
	lv_style_copy(&labels_style, lv_theme_get_current()->style.label.prim);
	labels_style.text.color = LV_COLOR_GREEN;



	if (f_stat("emunandswitcher/folders sx/emutmp00", NULL)) {//Installierter Emunand aus file lesen

		lv_obj_t* label = lv_label_create(lv_layer_top(), NULL);

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emunand 1 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");

			lv_label_set_text(label, string);
			lv_obj_set_pos(label, 150, 500);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);




		}
		f_close(&fp);

	}
	else { // if the directory exists - do this.

	}

	if (f_stat("emunandswitcher/folders sx/emutmp01", NULL)) {

		lv_obj_t* label = lv_label_create(lv_layer_top(), NULL);

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emunand 2 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");

			lv_label_set_text(label, string);
			lv_obj_set_pos(label, 150, 500);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}
		f_close(&fp);

	}
	else { // if the directory exists - do this.

	}

	if (f_stat("emunandswitcher/folders sx/emutmp02", NULL)) {

		lv_obj_t* label = lv_label_create(lv_layer_top(), NULL);

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emunand 3 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");

			lv_label_set_text(label, string);
			lv_obj_set_pos(label, 150, 500);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}
		f_close(&fp);

	}
	else { // if the directory exists - do this.

	}

	return 0;
}*/


/*int tool_infoemuninstalledATM()
{
#define MAXCHAR 100
	FIL fp;
	int linie = 0;
	char info[MAXCHAR];

	static lv_style_t bg;//Definition Textbox
	lv_style_copy(&bg, &lv_style_pretty);
	bg.text.color = LV_COLOR_WHITE;
	bg.body.opa = LV_OPA_0;

	static lv_style_t labels_style;//Definiton Label
	lv_style_copy(&labels_style, lv_theme_get_current()->style.label.prim);
	labels_style.text.color = LV_COLOR_GREEN;



	if (f_stat("emunandswitcher/folders sx/emutmp00", NULL)) {//Installierter Emunand aus file lesen

		lv_obj_t* label = lv_label_create(lv_layer_top(), NULL);

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emuemmc 1 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");

			lv_label_set_text(label, string);
			lv_obj_set_pos(label, 150, 500);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);




		}
		f_close(&fp);

	}
	else { // if the directory exists - do this.

	}

	if (f_stat("emunandswitcher/folders sx/emutmp01", NULL)) {

		lv_obj_t* label = lv_label_create(lv_layer_top(), NULL);

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emuemmc 2 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");

			lv_label_set_text(label, string);
			lv_obj_set_pos(label, 150, 500);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}
		f_close(&fp);

	}
	else { // if the directory exists - do this.

	}

	if (f_stat("emunandswitcher/folders sx/emutmp02", NULL)) {

		lv_obj_t* label = lv_label_create(lv_layer_top(), NULL);

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emuemmc 3 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");

			lv_label_set_text(label, string);
			lv_obj_set_pos(label, 150, 500);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}
		f_close(&fp);

	}
	else { // if the directory exists - do this.

	}

	return 0;
}*/

/*
void ctrl_test(lv_obj_t* obj, lv_event_t event)//Test Button Funktion Zeit und Datum
{
	if (event == LV_EVENT_CLICKED)
	{

		char* zeit = (char*)malloc(64);

		rtc_time_t time;

		// Get sensor data Clock
		u8 val = 0;

		// Update RTC regs from RTC clock.
		i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_UPDATE0_REG, MAX77620_RTC_READ_UPDATE);

		// Get control reg config.
		val = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_CONTROL_REG);
		// TODO: Check for binary format also?

		// Get time.
		time.sec = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_SEC_REG) & 0x7F;
		time.min = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_MIN_REG) & 0x7F;

		time.hour = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_HOUR_REG) & 0x1F;

		if (!(val & MAX77620_RTC_24H) && time.hour & MAX77620_RTC_HOUR_PM_MASK)
			time.hour = (time.hour & 0xF) + 12;

		// Get day of week. 1: Monday to 7: Sunday.
		time.weekday = 0;
		val = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_WEEKDAY_REG);
		for (int i = 0; i < 8; i++)
		{
			time.weekday++;
			if (val & 1)
				break;
			val >>= 1;
		}

		// Get date.
		time.date = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_DATE_REG) & 0x1f;
		time.month = (i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_MONTH_REG) & 0xF) - 1;
		time.year = (i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_YEAR_REG) & 0x7F) + 2000;




		//! TODO: Parse time and use offset. Test eigen... Vermutlich nur auf meiner Switch korrekt
		time.hour = (time.hour) + 4;
		time.min = (time.min) + 13;

		time.date = (time.date) + 6;
		time.month = (time.month) + 3;
		time.year = (time.year) + 17;

		// Set time funktioniert
		//s_printf(zeit, "%02d:%02d ",//LV_SYMBOL_MINUS" "LV_SYMBOL_GPS" %02d.%d",
		//	time.hour, time.min);

		// Set time und Date funktioniert
		s_printf(zeit, "%02d:%02d "" %02d.%02d.%02d",//LV_SYMBOL_MINUS" "LV_SYMBOL_GPS" %02d.%d",
			time.hour, time.min, time.date, time.month, time.year);


		//Zeit und Datum anzeigen
		static lv_style_t labels_style;//Definiton Label
		lv_style_copy(&labels_style, lv_theme_get_current()->style.label.prim);
		labels_style.text.color = LV_COLOR_WHITE;

		static lv_style_t font20_style;//Definition Schriftgrösse 20
		lv_style_copy(&font20_style, &lv_style_plain);
		font20_style.text.color = LV_COLOR_WHITE; //Textfarbe
		font20_style.text.font = &lv_font_montserrat_alternate_20;



		//	lv_obj_t* emunsx_tab = lv_tabview_add_tab(par,
		//LV_SYMBOL_WARNING " Switcher SX"); //Menü Eintrag und Symbol Tab

		lv_obj_t* lbl_time_temp = lv_label_create(lv_layer_top(), NULL);//lv_tabview_get_tab_act(const lv_obj_t *tabview)
		//lv_obj_t* lbl_time_temp = lv_label_create(lv_layer_top(), NULL);

		lv_label_set_array_text(lbl_time_temp, zeit, 64);

		//lv_label_set_static_text(lbl_time_temp, "00:00 "); //nur Zeit
		lv_label_set_static_text(lbl_time_temp, "00:00 "" 00.00.0000"); //Zeit und Datum

		lv_label_set_text(lbl_time_temp, zeit);
		lv_obj_set_pos(lbl_time_temp, 400, 620);
		lv_label_set_style(lbl_time_temp, LV_LABEL_STYLE_MAIN, &font20_style);



		free(zeit);


	}
}*/


/*void ctrl_themeinfoSX(lv_obj_t* obj, lv_event_t event)//Theme Info Funktion
{
	if (event == LV_EVENT_CLICKED)
	{

		if (f_stat("sxos/titles/0100000000001000", NULL)) {

			static lv_style_t bg;
			lv_style_copy(&bg, &lv_style_pretty);
			bg.text.color = LV_COLOR_GREEN;
			bg.body.opa = LV_OPA_0;
			lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 50);
			lv_mbox_set_text(mbox, "OK kein Theme installiert!");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 4000);

		}

		else { // if the directory exists - do this.



			static lv_style_t bg;
			lv_style_copy(&bg, &lv_style_pretty);
			bg.text.color = LV_COLOR_RED;
			bg.body.opa = LV_OPA_0;
			lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 50);
			lv_mbox_set_text(mbox, "Achtung Theme installiert!\nNach einem Systemsoftware Update\neines Emunandes, muss das Theme\nentfernt werden um den Starterror\nzu vermeiden...");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 4000);

		}

	}
}*/



/*void ctrl_test(lv_obj_t* obj, lv_event_t event)//Test Button Funktion
{
	if (event == LV_EVENT_CLICKED)
	{

#define MAXCHAR 100
		FIL fp;
		int line = 0;
		char info[MAXCHAR];

		static lv_style_t bg;//Definition Textbox
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;


		if (f_stat("emunandswitcher/folders sx/emutmp00", NULL)) {//Installierter Emunand aus file lesen

			f_open(&fp, "emunandswitcher/config.ini", FA_READ);
			while (f_gets(info, MAXCHAR, &fp)) {
				line++;
				TCHAR* stelle;
				TCHAR* auswahl;
				auswahl = strstr(info, "Emunand 1 =");
				stelle = strchr(auswahl, (int)'=');
				string = str_replace(stelle, "= ", "");




				lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 4);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 600);
				lv_mbox_set_text(mbox, string);
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				//lv_mbox_start_auto_close(mbox, 4000);


			}
			f_close(&fp);

		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders sx/emutmp01", NULL)) {

			f_open(&fp, "emunandswitcher/config.ini", FA_READ);
			while (f_gets(info, MAXCHAR, &fp)) {
				line++;
				TCHAR* stelle;
				TCHAR* auswahl;
				auswahl = strstr(info, "Emunand 2 =");
				stelle = strchr(auswahl, (int)'=');
				string = str_replace(stelle, "= ", "");

				lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 4);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 600);
				lv_mbox_set_text(mbox, string);
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				//lv_mbox_start_auto_close(mbox, 4000);



			}
			f_close(&fp);

		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders sx/emutmp02", NULL)) {

			f_open(&fp, "emunandswitcher/config.ini", FA_READ);
			while (f_gets(info, MAXCHAR, &fp)) {
				line++;
				TCHAR* stelle;
				TCHAR* auswahl;
				auswahl = strstr(info, "Emunand 3 =");
				stelle = strchr(auswahl, (int)'=');
				string = str_replace(stelle, "= ", "");

				lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
				lv_mbox_set_recolor(mbox, true);
				lv_obj_set_width(mbox, LV_DPI * 4);
				lv_obj_set_top(mbox, true);
				lv_obj_set_auto_realign(mbox, true);
				lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 600);
				lv_mbox_set_text(mbox, string);
				lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
				//lv_mbox_start_auto_close(mbox, 4000);



			}
			f_close(&fp);

		}
		else { // if the directory exists - do this.

		}












	}
}*/



/*void ctrl_test(lv_obj_t* obj, lv_event_t event)//Test Button Funktion Akku Anzeige IO
{
	if (event == LV_EVENT_CLICKED)
	{

		static lv_style_t labels_style;//Definiton Label
		lv_style_copy(&labels_style, lv_theme_get_current()->style.label.prim);
		labels_style.text.color = LV_COLOR_WHITE;

		static lv_style_t font20_style;//Definition Schriftgrösse 20
		lv_style_copy(&font20_style, &lv_style_plain);
		font20_style.text.color = LV_COLOR_WHITE; //Textfarbe
		font20_style.text.font = &lv_font_montserrat_alternate_20;

		static lv_style_t font20red_style;//Definition Schriftgrösse 20 rot
		lv_style_copy(&font20red_style, &lv_style_plain);
		font20red_style.text.color = LV_COLOR_RED; //Textfarbe
		font20red_style.text.font = &lv_font_montserrat_alternate_20;

		//Akku auslesen und Symbol anzeigen je nach Füllstand
		u32 battPercent = 0;
		max17050_get_property(MAX17050_RepSOC, (int*)& battPercent);

		lv_obj_t* label = lv_label_create(lv_layer_top(), NULL);

		int per1 = (battPercent >> 8) & 0xFF;
		int per2 = (battPercent & 0xFF) / 25.5001; //exact to 4 decimal places
		if (per2 >= 0)
			per1 = per1 + 1; // keep value the same as the switch main screen

		if (per1 > 0) {

			lv_label_set_text(label, LV_SYMBOL_BATTERY_EMPTY);
			lv_obj_set_pos(label, 280, 615);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}

		if (per1 > 17) {

			label = lv_label_create(lv_layer_top(), NULL);
			lv_label_set_text(label, LV_SYMBOL_BATTERY_1);
			lv_obj_set_pos(label, 280, 615);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}

		if (per1 > 34) {

			label = lv_label_create(lv_layer_top(), NULL);
			lv_label_set_text(label, LV_SYMBOL_BATTERY_1);
			lv_obj_set_pos(label, 280, 615);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}

		if (per1 > 51) {

			label = lv_label_create(lv_layer_top(), NULL);
			lv_label_set_text(label, LV_SYMBOL_BATTERY_2);
			lv_obj_set_pos(label, 280, 615);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}

		if (per1 > 68) {

			label = lv_label_create(lv_layer_top(), NULL);
			lv_label_set_text(label, LV_SYMBOL_BATTERY_3);
			lv_obj_set_pos(label, 280, 615);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}

		if (per1 > 84) {

			label = lv_label_create(lv_layer_top(), NULL);
			lv_label_set_text(label, LV_SYMBOL_BATTERY_FULL);
			lv_obj_set_pos(label, 280, 615);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}

		if (per1 <= 5) {

			label = lv_label_create(lv_layer_top(), NULL);
			lv_label_set_text(label, LV_SYMBOL_BATTERY_EMPTY"Warnung, Akku fast leer! Bitte Ladegerät anschliessen!");
			lv_obj_set_pos(label, 280, 615);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &labels_style);

		}


		//Info Text Akku Prozent anzeigen
		if (per1 < 20) {

			//char stringNum[20];//Beispiel int zu string umwandeln
			//int num = 100;
			//sprintf(stringNum, "%d", num);

			//Wird benötigt!!!
			//#include "utils/fs_utils.h"//hinzugefügt
			//#include "mem/heap.h"//hinzugefügt

			char* akku = (char*)malloc(0x1000);
			s_printf(akku, "%d %%", per1);

			label = lv_label_create(lv_layer_top(), NULL);
			lv_label_set_text(label, akku);
			lv_obj_set_pos(label, 330, 620);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20red_style);

			free(akku);

		}
		else {

			char* akku = (char*)malloc(0x1000);
			s_printf(akku, "%d %%", per1);

			label = lv_label_create(lv_layer_top(), NULL);
			lv_label_set_text(label, akku);
			lv_obj_set_pos(label, 330, 620);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

			free(akku);

		}


	}
}*/


/*void ctrl_test(lv_obj_t* obj, lv_event_t event)//Test Button Funktion
{
	if (event == LV_EVENT_CLICKED)
	{

		static lv_style_t bg;
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;

		if (f_stat("sxos/titles/0100000000001000", NULL)) {


			lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 50);
			lv_mbox_set_text(mbox, "Kein Theme installiert...");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 4000);

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 80);
			lv_mbox_set_text(mbox, "Zweite Zeile Test...");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 4000);




		}

		else { // if the directory exists - do this.


			lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 50);
			lv_mbox_set_text(mbox, "Info Ordner exisitert...");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 4000);

			return;//Test stop

			mbox = lv_mbox_create(lv_layer_top(), NULL);
			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 5);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 80);
			lv_mbox_set_text(mbox, "Zweite Zeile Test...");
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 4000);

		}
	}
}*/


/*void ctrl_test(lv_obj_t* obj, lv_event_t event)//Test Button Funktion
{
	if (event == LV_EVENT_CLICKED)
	{

#define MAXCHAR 100
		FIL fp;
		int linie = 0;
		char info[MAXCHAR];

		static lv_style_t bg;//Definition Textbox
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_WHITE;
		bg.body.opa = LV_OPA_0;

		lv_obj_t* mbox = lv_mbox_create(lv_layer_top(), NULL);


		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			char* stelle;
			char* auswahl;
			auswahl = strstr(info, "Emunand 1 =");
			stelle = strchr(auswahl, (int)'=');


			string = str_replace(stelle, "= ", "");







			lv_mbox_set_recolor(mbox, true);
			lv_obj_set_width(mbox, LV_DPI * 4);
			lv_obj_set_top(mbox, true);
			lv_obj_set_auto_realign(mbox, true);
			lv_obj_align(mbox, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 50);
			lv_mbox_set_text(mbox, string);
			lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
			lv_mbox_start_auto_close(mbox, 4000);

		}

		f_close(&fp);

	}
}*/




//stelle = strchr(auswahl, (int)'=');
//string = str_replace(stelle, "= ", ""); */

//char* textaus;
//strcat(emun1, emun2);

//char textaus[1000];
//textaus = emun1, emun2;
//s_printf(textaus, "%d", emun1);//, emun2, emun3, emmc1, emmc2, emmc3);



/* Open a text file */
//fr = f_open(&fil, "message.txt", FA_READ);
//if (fr) return (int)fr;

/* Read every line and display it */
/*while (f_gets(line, sizeof line, &fp)) {

	//char* temp = (char*)malloc(0x1000);
	//temp[linie] = line;
	linie++;
	//printf(line);

	//char* temp = (char*)malloc(0x1000);
	//s_printf(temp, "%d\n", linie);
	//f_rewind(fp);
	lv_ta_set_cursor_type(ta, LV_CURSOR_BLOCK);
	lv_obj_set_size(ta, 900, 320);
	lv_ta_set_text(ta, line);    //Set an initial text
}*/

/*FILE* file = fopen("emunandswitcher/config.ini", "r");
char  c;
char buffer[10000];
while (fscanf(file, "%c", &c) == 1)
{
	buffer[linie] = c;
	linie++;




	buffer[linie] = 0;
}*/


/*FILE* file = fopen(fileName, "r");
if (file == NULL)
{
	puts("File not found");
	exit(1);
}
char  c;
int idx = 0;
while (fscanf(file, "%c", &c) == 1)
{
	buffer[idx] = c;
	idx++;
}
buffer[idx] = 0;*/




/*// Create a keyboard and make it fill the width of the above text areas
kb = lv_kb_create(lv_scr_act(), NULL);
lv_obj_set_pos(kb, 5, 90);
lv_obj_set_event_cb(kb, kb_event_cb); // Setting a custom event handler stops the keyboard from closing automatically
lv_obj_set_size(kb, LV_HOR_RES - 10, 140);
lv_kb_set_ta(kb, pwd_ta); // Focus it on one of the text areas to start
lv_kb_set_cursor_manage(kb, true); // Automatically show/hide cursors on text areas*/

/*//Textfester erstellen
lv_obj_t* ta = lv_ta_create(win, NULL);
lv_ta_set_cursor_type(ta, LV_CURSOR_BLOCK);
lv_obj_set_size(ta, 900, 320);
//lv_obj_align(ta, NULL, LV_ALIGN_CENTER, 0, -50);
//lv_ta_set_cursor_type(ta, LV_CURSOR_BLOCK);
//lv_ta_set_text(ta, "A\ntext\nin\na\nText\nArea");    //Set an initial text
lv_ta_set_text(ta, string);    //Set an initial text*/
//lv_obj_set_event_cb(ta, general_event_handler);
//lv_group_add_obj(g, ta);


//Test Text zurück aus fenster funktioniert
/*lv_obj_t* label_aus = lv_label_create(win, NULL);
lv_label_set_text(label_aus, lv_ta_get_text(tastd));
lv_obj_set_pos(label_aus, 0, 200);
lv_label_set_style(label_aus, LV_LABEL_STYLE_MAIN, &tafont110_style);*/


/*//Set RTC Time ok
time.month += 0;
i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_MONTH_REG, time.month); //& 0xF;

// Update RTC clock from RTC regs.
i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_UPDATE0_REG, MAX77620_RTC_WRITE_UPDATE);*/


/*//Tastatur erstellen kb = Keyboard, in lc_conf.h aktivieren - Ein Textfenster
lv_obj_t* kb = lv_kb_create(win, NULL);
lv_obj_set_size(kb, 1080, 300);
lv_kb_set_ta(kb, tastd);
lv_kb_set_cursor_manage(kb, true);*/


/*//Test Text zurück aus fenster funktioniert
lv_obj_t* label_aus = lv_label_create(win, NULL);
lv_label_set_text(label_aus, lv_ta_get_text(tastd));
lv_obj_set_pos(label_aus, 0, 200);
lv_label_set_style(label_aus, LV_LABEL_STYLE_MAIN, &tafont110_style);*/





/*//hekate set rtc alarm
//u8 val = 0;

// Update RTC regs from RTC clock.
i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_UPDATE0_REG, MAX77620_RTC_READ_UPDATE);

// Stop alarm for both ALARM1 and ALARM2. Horizon uses ALARM2.
for (int i = 0; i < (MAX77620_RTC_NR_TIME_REGS * 2); i++)
{
	val = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_ALARM1_SEC_REG + i);
	val &= ~MAX77620_RTC_ALARM_EN_MASK;
	i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_ALARM1_SEC_REG + i, val);
}

// Update RTC clock from RTC regs.
i2c_send_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_UPDATE0_REG, MAX77620_RTC_WRITE_UPDATE);*/






/* Open a text file */
//fr = f_open(&fil, "message.txt", FA_READ);
//if (fr) return (int)fr;

/* Read every line and display it */
/*while (f_gets(line, sizeof line, &fp)) {

	//char* temp = (char*)malloc(0x1000);
	//temp[linie] = line;
	linie++;
	//printf(line);

	//char* temp = (char*)malloc(0x1000);
	//s_printf(temp, "%d\n", linie);
	//f_rewind(fp);
	lv_ta_set_cursor_type(ta, LV_CURSOR_BLOCK);
	lv_obj_set_size(ta, 900, 320);
	lv_ta_set_text(ta, line);    //Set an initial text
}*/

/*FILE* file = fopen("emunandswitcher/config.ini", "r");
char  c;
char buffer[10000];
while (fscanf(file, "%c", &c) == 1)
{
	buffer[linie] = c;
	linie++;




	buffer[linie] = 0;
}*/


/*FILE* file = fopen(fileName, "r");
if (file == NULL)
{
	puts("File not found");
	exit(1);
}
char  c;
int idx = 0;
while (fscanf(file, "%c", &c) == 1)
{
	buffer[idx] = c;
	idx++;
}
buffer[idx] = 0;*/





/*//Textfester erstellen
lv_obj_t* ta = lv_ta_create(win, NULL);
lv_ta_set_cursor_type(ta, LV_CURSOR_BLOCK);
lv_obj_set_size(ta, 900, 320);
//lv_obj_align(ta, NULL, LV_ALIGN_CENTER, 0, -50);
//lv_ta_set_cursor_type(ta, LV_CURSOR_BLOCK);

//lv_ta_set_text(ta, "A\ntext\nin\na\nText\nArea");    //Set an initial text
lv_ta_set_text(ta, string);    //Set an initial text*/

//lv_obj_set_event_cb(ta, general_event_handler);
//lv_group_add_obj(g, ta);




/*static void ta_event_cb(lv_obj_t* ta, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		// Focus on the clicked text area
		if (kb != NULL)
			lv_kb_set_ta(kb, ta);
	}

	//neu für Textfenster Textänderung, nötig?
	if (event == LV_EVENT_VALUE_CHANGED) {
		s_printf("Value: %c\n", lv_ta_get_text(ta));
	}

	else if (event == LV_EVENT_INSERT) {
		const char* str = lv_event_get_data();
		if (str[0] == '\n') {
			printf("Ready\n");//printf mit aktiver #include <stdio.h> fehler beim builden
		}
	}
}*/