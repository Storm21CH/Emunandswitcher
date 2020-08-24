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


#include "power/max17050.h"//hinzugefügt
#include "utils/fs_utils.h"//hinzugefügt
#include "mem/heap.h"//hinzugefügt

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
		lv_mbox_set_text(mbox, "Emunandswitcher Version 2.0\nmade by Storm 2019\nbased on argonNX and hekate\nIcon templates from mrdude\nThanks to the programmers!");
		lv_mbox_set_style(mbox, LV_MBOX_STYLE_BG, &bg);
		lv_mbox_start_auto_close(mbox, 8000);
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

		if (f_stat("atmosphere/titles/0100000000001000", NULL)) {

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

			f_unlink("atmosphere/titles/0100000000001000");
			f_unlink("atmosphere/titles/0100000000001007");
			f_unlink("atmosphere/titles/0100000000001013");

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
				f_rename("atmosphere/titles", "emunandswitcher/folders atm/titles02");
				f_rename("emunandswitcher/folders atm/titles00", "atmosphere/titles");

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
				f_rename("atmosphere/titles", "emunandswitcher/folders atm/titles01");
				f_rename("emunandswitcher/folders atm/titles00", "atmosphere/titles");

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
				f_rename("atmosphere/titles", "emunandswitcher/folders atm/titles02");
				f_rename("emunandswitcher/folders atm/titles01", "atmosphere/titles");

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
				f_rename("atmosphere/titles", "emunandswitcher/folders atm/titles00");
				f_rename("emunandswitcher/folders atm/titles01", "atmosphere/titles");

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
				f_rename("atmosphere/titles", "emunandswitcher/folders atm/titles01");
				f_rename("emunandswitcher/folders atm/titles02", "atmosphere/titles");

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
				f_rename("atmosphere/titles", "emunandswitcher/folders atm/titles00");
				f_rename("emunandswitcher/folders atm/titles02", "atmosphere/titles");

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