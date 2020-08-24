/*  
 * Copyright (c) 2018 Guillem96
 * Copyright (c) 2019 Storm
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "menu/gui/gui_menu.h"
#include "menu/gui/gui_menu_pool.h"
#include "menu/gui/gui_menu_controllers.h"
#include "power/battery.h"
#include "core/custom-gui.h"
#include "core/payloads.h"
#include "utils/util.h"
#include "utils/dirlist.h"
#include "gfx/gfx.h"
#include "gfx/lvgl_adapter.h"

#include <string.h>

#include "libs/fatfs/ff.h"//hinzugefügt

#include "power/bq24193.h"// Zeit Temp Akku Tool hekate
#include "rtc/max77620-rtc.h"// Zeit Temp Akku Tool hekate
#include "power/max77620.h"// Zeit Temp Akku Tool hekate
#include "thermal/tmp451.h"// Zeit Temp Akku Tool hekate
#include "libs/lvgl/lvgl.h"//hinzugefügt

#include "soc/i2c.h"// Zeit Temp Akku Tool hekate

#include "power/max17050.h"//hinzugefügt
#include "utils/fs_utils.h"//hinzugefügt
#include "mem/heap.h"//hinzugefügt

#include "gfx/di.h"//hinzugefügt Display Helligkeit
#include "soc/t210.h"//hinzugefügt Display Helligkeit

#include <stdlib.h>//hinzugefügt für Typumwandlung atoi, atol usw


const char* infotext = "";//Eintrag String Text

/* Render functions */
static bool render_title(argon_ctxt_t *);

static bool render_tabs(argon_ctxt_t *);

//Payloads Tab
static bool render_payloads_tab(lv_obj_t *, argon_ctxt_t *);
static bool render_single_payload_tab(lv_obj_t *, argon_ctxt_t *, char*, u32);
static bool render_payloads_entries(lv_obj_t *, argon_ctxt_t *, char*, u32);

static bool render_tools_tab(lv_obj_t *, argon_ctxt_t *);//Tools Tab

static bool render_switcherSX_tab(lv_obj_t*, argon_ctxt_t*); //Tab SX
static bool render_switcherATM_tab(lv_obj_t*, argon_ctxt_t*);//Tab ATM

static void update_status(lv_task_t* status);//Update Zeit Datum, Akku, installierter Emunand, Theme info

void gui_menu_draw(argon_ctxt_t *argon_ctxt)
{   
    custom_gui_t *cg = custom_gui_load();

    /* Deafult background is a beautiful gradient */
    lv_style_scr.body.main_color = GRAD_1;
    lv_style_scr.body.grad_color = GRAD_2;
       
    render_custom_background(cg, lv_scr_act());
    render_title(argon_ctxt);
    render_tabs(argon_ctxt);
}

void gui_menu_open(argon_ctxt_t *argon_ctxt)
{
    g_argon_ctxt = argon_ctxt;

    while (true)
    {
        lv_tick_inc(1);
        lv_task_handler();
        msleep(1);
    }
}

void gui_menu_destroy(argon_ctxt_t *argon_ctxt)
{
    argon_ctxt_destroy(argon_ctxt);
}

static bool render_tabs(argon_ctxt_t *argon_ctxt)
{
    /* Create tabview container */
    lv_obj_t *base_tabs = lv_tabview_create(lv_scr_act(), NULL);
    lv_obj_set_pos(base_tabs, 0, 0);
    lv_tabview_set_btns_pos(base_tabs, LV_TABVIEW_BTNS_POS_BOTTOM);
    lv_tabview_set_style(base_tabs, LV_TABVIEW_STYLE_BG, &lv_style_transp_tight);

    /* Disable animations */
    lv_tabview_set_anim_time(base_tabs, 0);
    lv_tabview_set_sliding(base_tabs, false);



	//Render all tabs Reihenfolge nacheinander einfügen

	render_switcherSX_tab(base_tabs, argon_ctxt); //Switcher SX Tab
	render_switcherATM_tab(base_tabs, argon_ctxt); //Switcher ATM Tab
	
	render_tools_tab(base_tabs, argon_ctxt);

	render_payloads_tab(base_tabs, argon_ctxt);

    return true;
}


static bool render_payloads_tab(lv_obj_t *par, argon_ctxt_t *ctxt)
{
	if (f_stat("argon/payloads", NULL)) {//Abfrage ob Verzeichnis existiert, wenn ja nur dann Payload Tab erstellen

	}

	else { // if the directory exists - do this.




		char* payloads = dirlist(PAYLOADS_DIR, "*.bin", false);
		u32 i = 0;
		u32 group = 0;

		while (payloads[i * 256])
		{
			if (i % 8 == 0)//Anzahl Payloads pro Tab
			{
				render_single_payload_tab(par, ctxt, payloads, group);
				group++;
			}
			i++;
		}

		if (group == 0)
			render_single_payload_tab(par, ctxt, payloads, group);

	}
		return true;
	
}

static bool render_single_payload_tab(lv_obj_t *par, argon_ctxt_t * ctxt, char* payloads, u32 group)
{

	if (f_stat("argon/payloads", NULL)) {//Abfrage ob Verzeichnis existiert, wenn ja nur dann Payload Tab erstellen

	}

	else { // if the directory exists - do this.

	//Setting scrollable view 
		lv_obj_t* payloads_tab = lv_tabview_add_tab(par,
			LV_SYMBOL_DIRECTORY " Payloads");
		lv_page_set_sb_mode(payloads_tab, LV_SB_MODE_OFF);

		lv_obj_t* page = lv_page_create(payloads_tab, NULL);
		lv_obj_set_size(page, lv_obj_get_width(payloads_tab), 700);//geändert 400 zu 700 höhe Page Payloads
		lv_obj_align(page, payloads_tab, LV_ALIGN_CENTER, 0, 0);//geändert 0,50
		lv_page_set_scrl_width(page, 0);

		//Horizontal grid layout
		lv_obj_t* cnr = lv_page_get_scrl(page);
		lv_cont_set_layout(cnr, LV_LAYOUT_PRETTY);
		lv_obj_set_size(cnr, LV_HOR_RES_MAX * .95, lv_obj_get_height(page));

		lv_cont_set_style(cnr,
			LV_CONT_STYLE_MAIN,
			lv_theme_get_argon()->style.panel);
		render_payloads_entries(cnr, ctxt, payloads, group);

		gui_menu_pool_push(ctxt->pool, page);
		gui_menu_pool_push(ctxt->pool, cnr);
		gui_menu_pool_push(ctxt->pool, payloads_tab);
	}
		return true;
	
}

static bool render_payloads_entries(lv_obj_t *par_tabview, argon_ctxt_t *argon_ctxt, char* payloads, u32 group)
{

	if (f_stat("argon/payloads", NULL)) {//Abfrage ob Verzeichnis existiert, wenn ja nur dann Payload Tab erstellen

	}

	else { // if the directory exists - do this.

		lv_obj_t* btn = NULL;
		lv_obj_t* label = NULL;
		lv_img_dsc_t* img = NULL;

		u32 i = 8 * group;//Anzahl Payloads pro Gruppe/Tab

		//Declare styles for payloads
		static lv_style_t style_pr;
		lv_style_copy(&style_pr, &lv_style_plain);
		style_pr.image.color = LV_COLOR_BLACK;
		style_pr.image.intense = LV_OPA_50;
		style_pr.text.color = lv_color_hex3(0xaaa);

		static lv_style_t rel_norm_btn;
		lv_style_copy(&rel_norm_btn, lv_theme_get_argon()->style.btn.rel);
		rel_norm_btn.body.padding.inner = 25;

		static lv_style_t pr_norm_btn;
		lv_style_copy(&pr_norm_btn, lv_theme_get_argon()->style.btn.pr);
		pr_norm_btn.body.padding.inner = 25;

		static lv_style_t inv_label;
		lv_style_copy(&inv_label, &lv_style_transp);
		inv_label.text.font = NULL;

		static lv_style_t no_img_label;
		lv_style_copy(&no_img_label, &lv_style_plain);
		no_img_label.text.font = &lv_font_montserrat_alternate_110;
		no_img_label.text.color = LV_COLOR_WHITE;

		while (payloads[i * 256] && i < 8 * (group + 1))//Orginal while (payloads[i * 256] && i < 4 * (group + 1)) geändert für 8 Symbole pro Tab
		{
			char payload_path[256];
			char payload_logo[256];

			payload_full_path(&payloads[i * 256], payload_path);
			payload_logo_path(&payloads[i * 256], payload_logo);

			//Try to get payload logo
			img = bmp_to_lvimg_obj((const char*)payload_logo);

			if (!img)
			{
				//If user has not defined a logo for the payload
				btn = lv_btn_create(par_tabview, NULL);
				lv_obj_set_size(btn, 280, 280);
				lv_btn_set_style(btn, LV_BTN_STYLE_PR, &pr_norm_btn);
				lv_btn_set_style(btn, LV_BTN_STYLE_REL, &rel_norm_btn);

				label = lv_label_create(btn, NULL);
				lv_obj_set_style(label, &no_img_label);
				lv_label_set_text(label, LV_SYMBOL_ROCKET);

				label = lv_label_create(btn, NULL);
				lv_label_set_text(label, &payloads[i * 256]);
			}
			else
			{
				//If user has defined a logo 
				btn = lv_imgbtn_create(par_tabview, NULL);
				lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
				lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
				lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
			}

			//Workaround: Payload path as invisible label so later we can get the path on the callback
			label = lv_label_create(btn, NULL);
			lv_label_set_text(label, payload_path);
			lv_obj_set_style(label, &inv_label);

			lv_obj_set_event_cb(btn, ctrl_lauch_payload);

			i++;
		}

		gui_menu_pool_push(argon_ctxt->pool, btn);
		gui_menu_pool_push(argon_ctxt->pool, label);
	}
		return true;
	
}






//Typedefinition für Task refresh
typedef struct _gui_status_bar_ctx
{

	lv_obj_t* time_date;
	lv_obj_t* akkustand;
	lv_obj_t* akkusym;
	lv_obj_t* emuninst;
	lv_obj_t* emuninstsx;
	lv_obj_t* themeinfosx;
	lv_obj_t* themeinfoatm;
	lv_obj_t* battery_more;
	lv_obj_t* battery_more_volt;
	lv_obj_t* temperatur;

} gui_status_bar_ctx;

static gui_status_bar_ctx status_bar;



//Tasks refresh definitionen
static void update_status(lv_task_t* status)
{
	//Datum und Zeit auslesen und anzeigen
	char* zeit = (char*)malloc(64);

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
	//time.month = (i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_MONTH_REG) & 0xF) - 1;
	time.year = (i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_YEAR_REG) & 0x7F) + 2000;
	time.month = i2c_recv_byte(I2C_5, MAX77620_RTC_I2C_ADDR, MAX77620_RTC_MONTH_REG) & 0xF;


	//Parse time and use offset. Test eigen...

	// Um Datum zu stellen kann Ubuntu gestartet werden und die Zeitzone einstellen, wird über Internet synch und in RTC gespeichert

	//Zeitzone MEZ+1
	if (time.hour == 23) time.hour = 0;
	else time.hour += 1;

	//time.month = (time.month) - 8;

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



	// Set time funktioniert
	//s_printf(zeit, "%02d:%02d ",//LV_SYMBOL_MINUS" "LV_SYMBOL_GPS" %02d.%d",
	//	time.hour, time.min);

	// Set time und Date funktioniert
	s_printf(zeit, "%02d.%02d.%02d "" %02d:%02d:%02d",//LV_SYMBOL_MINUS" "LV_SYMBOL_GPS" %02d.%d",
		time.date, time.month, time.year, time.hour, time.min, time.sec);

	lv_label_set_array_text(status_bar.time_date, zeit, 64);

	free(zeit);





	//Akku auslesen und Symbol anzeigen je nach Füllstand
	u32 battPercent = 0;
	max17050_get_property(MAX17050_RepSOC, (int*)& battPercent);

	int per1 = (battPercent >> 8) & 0xFF;
	int per2 = (battPercent & 0xFF) / 25.5001; //exact to 4 decimal places
	if (per2 >= 0)
		per1 = per1 + 1; // keep value the same as the switch main screen

	if (per1 >= 101)// Korrektur akku 101%
		per1 = per1 - 1;


	//Akku Symbol
	if (per1 > 0) {
		lv_label_set_array_text(status_bar.akkusym, LV_SYMBOL_BATTERY_EMPTY, 64);
	}

	if (per1 > 17) {
		lv_label_set_array_text(status_bar.akkusym, LV_SYMBOL_BATTERY_1, 64);
	}

	if (per1 > 34) {
		lv_label_set_array_text(status_bar.akkusym, LV_SYMBOL_BATTERY_1, 64);
	}

	if (per1 > 51) {
		lv_label_set_array_text(status_bar.akkusym, LV_SYMBOL_BATTERY_2, 64);
	}

	if (per1 > 68) {
		lv_label_set_array_text(status_bar.akkusym, LV_SYMBOL_BATTERY_3, 64);
	}

	if (per1 > 84) {
		lv_label_set_array_text(status_bar.akkusym, LV_SYMBOL_BATTERY_FULL, 64);
	}

	if (per1 <= 5) {
		lv_label_set_array_text(status_bar.akkusym, LV_SYMBOL_BATTERY_EMPTY" Warning, battery almost empty! Please connect the charger!", 64);
	}


	//Info Text Akku Prozent anzeigen
	if (per1 < 20) {

		//char stringNum[20];//Beispiel int zu string (char) umwandeln
		//int num = 100;
		//sprintf(stringNum, "%d", num);

		//Wird benötigt!!!
		//#include "utils/fs_utils.h"//hinzugefügt
		//#include "mem/heap.h"//hinzugefügt

		static lv_style_t font20red_style;//Definition Schriftgrösse 20 rot
		lv_style_copy(&font20red_style, &lv_style_plain);
		font20red_style.text.color = LV_COLOR_RED; //Textfarbe
		font20red_style.text.font = &lv_font_montserrat_alternate_20;

		char* akku = (char*)malloc(0x1000);
		s_printf(akku, "%d %%", per1);

		lv_label_set_array_text(status_bar.akkustand, akku, 64);
		lv_label_set_style(status_bar.akkustand, LV_LABEL_STYLE_MAIN, &font20red_style);

		free(akku);

	}

	else {


		static lv_style_t font20_style;//Definition Schriftgrösse 20
		lv_style_copy(&font20_style, &lv_style_plain);
		font20_style.text.color = LV_COLOR_WHITE; //Textfarbe
		font20_style.text.font = &lv_font_montserrat_alternate_20;

		char* akku = (char*)malloc(0x1000);
		s_printf(akku, "%d %%", per1);

		lv_label_set_array_text(status_bar.akkustand, akku, 64);
		lv_label_set_style(status_bar.akkustand, LV_LABEL_STYLE_MAIN, &font20_style);

		free(akku);

	}

	
	//Stromverbrauch und Volt Anzeige
	char* amp = (char*)malloc(64);
	char* volt = (char*)malloc(64);
	int batt_volt;
	int batt_curr;

	// Get sensor data.
	max17050_get_property(MAX17050_VCELL, &batt_volt);
	max17050_get_property(MAX17050_Current, &batt_curr);

	// Set battery current draw
	if (batt_curr >= 0) {

		static lv_style_t font20green_style;//Definition Schriftgrösse 20 grün
		lv_style_copy(&font20green_style, &lv_style_plain);
		font20green_style.text.color = LV_COLOR_GREEN; //Textfarbe
		font20green_style.text.font = &lv_font_montserrat_alternate_20;

		s_printf(amp, "+%d mA", batt_curr / 1000);
		//s_printf(va + strlen(va), " mA (%d V)", batt_volt);

		lv_label_set_array_text(status_bar.battery_more, amp, 64);
		lv_label_set_style(status_bar.battery_more, LV_LABEL_STYLE_MAIN, &font20green_style);

		free(amp);
	}

	else {

		static lv_style_t font20red_style;//Definition Schriftgrösse 20 rot
		lv_style_copy(&font20red_style, &lv_style_plain);
		font20red_style.text.color = LV_COLOR_RED; //Textfarbe
		font20red_style.text.font = &lv_font_montserrat_alternate_20;

		s_printf(amp, "-%d mA", (~batt_curr + 1) / 1000);
		//s_printf(va + strlen(va), " mA (%d V)", batt_volt);

		lv_label_set_array_text(status_bar.battery_more, amp, 64);
		lv_label_set_style(status_bar.battery_more, LV_LABEL_STYLE_MAIN, &font20red_style);

		free(amp);
	}

	//mV in volt umwandeln und anzeigen
	static lv_style_t font20_style;//Definition Schriftgrösse 20
	lv_style_copy(&font20_style, &lv_style_plain);
	font20_style.text.color = LV_COLOR_WHITE; //Textfarbe
	font20_style.text.font = &lv_font_montserrat_alternate_20;

	char* mvolt = (char*)malloc(64);
	s_printf(volt, "%d V", batt_volt);
	s_printf(mvolt, "%d V", batt_volt);
	mvolt[0] = '.';//Ersetz erstes Zeichen mit .
	volt[strlen(volt) - 5] = '\0';//nur erste Stelle anzeigen, von hinten gerechnet, -5 Stellen von (4123 V) gibt 4
	strcat(volt, mvolt);//kopiert (.mvolt V) zu volt erstes Stelle

	lv_label_set_array_text(status_bar.battery_more_volt, volt, 64);
	lv_label_set_style(status_bar.battery_more_volt, LV_LABEL_STYLE_MAIN, &font20_style);

	free(volt);
	free(mvolt);


	//Temperatur auslesen und anzeigen
	char* temp = (char*)malloc(64);

	u8 val1;
	u16 soc_temp = 0;

	// Get sensor data. Temperatur
	val1 = i2c_recv_byte(I2C_1, TMP451_I2C_ADDR, TMP451_SOC_TEMP_REG);
	soc_temp = val1 << 8;
	val1 = i2c_recv_byte(I2C_1, TMP451_I2C_ADDR, TMP451_SOC_TMP_DEC_REG);
	soc_temp |= ((val1 >> 4) * 625) / 100;

	//SoC Temperatur label erstellen
	s_printf(temp, "CPU %02d.%d", soc_temp >> 8, (soc_temp & 0xFF) / 10);

	lv_label_set_array_text(status_bar.temperatur, temp, 64);
	lv_label_set_style(status_bar.temperatur, LV_LABEL_STYLE_MAIN, &font20_style);

	free(temp);





	//Emunand anzeigen
	#define MAXCHAR 100//Definition Text aus ini File lesen
	FIL fp;
	int linie = 0;
	char info[MAXCHAR];

	//Prüfung Config.ini vorhanden
	if (f_stat("emunandswitcher/config.ini", NULL)) {

	}

	else { // if the file exists - do this.


	//Installierter Emummc aus file lesen und aktualisieren
		if (f_stat("emunandswitcher/folders atm/emutmp00", NULL)) {

			f_open(&fp, "emunandswitcher/config.ini", FA_READ);
			while (f_gets(info, MAXCHAR, &fp)) {
				linie++;
				char* stelle;
				char* auswahl;
				auswahl = strstr(info, "Emuemmc 1 =");
				stelle = strchr(auswahl, (int)'=');
				infotext = str_replace(stelle, "= ", "");

				lv_label_set_array_text(status_bar.emuninst, infotext, 100);

			}
			f_close(&fp);

		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders atm/emutmp01", NULL)) {

			f_open(&fp, "emunandswitcher/config.ini", FA_READ);
			while (f_gets(info, MAXCHAR, &fp)) {
				linie++;
				char* stelle;
				char* auswahl;
				auswahl = strstr(info, "Emuemmc 2 =");
				stelle = strchr(auswahl, (int)'=');
				infotext = str_replace(stelle, "= ", "");

				lv_label_set_array_text(status_bar.emuninst, infotext, 100);

			}
			f_close(&fp);

		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders atm/emutmp02", NULL)) {

			f_open(&fp, "emunandswitcher/config.ini", FA_READ);
			while (f_gets(info, MAXCHAR, &fp)) {
				linie++;
				char* stelle;
				char* auswahl;
				auswahl = strstr(info, "Emuemmc 3 =");
				stelle = strchr(auswahl, (int)'=');
				infotext = str_replace(stelle, "= ", "");

				lv_label_set_array_text(status_bar.emuninst, infotext, 100);

			}
			f_close(&fp);

		}
		else { // if the directory exists - do this.

		}



		//Installierter Emunand aus file lesen und aktualisieren
		if (f_stat("emunandswitcher/folders sx/emutmp00", NULL)) {

			f_open(&fp, "emunandswitcher/config.ini", FA_READ);
			while (f_gets(info, MAXCHAR, &fp)) {
				linie++;
				char* stelle;
				char* auswahl;
				auswahl = strstr(info, "Emunand 1 =");
				stelle = strchr(auswahl, (int)'=');
				infotext = str_replace(stelle, "= ", "");

				lv_label_set_array_text(status_bar.emuninstsx, infotext, 100);

			}
			f_close(&fp);

		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders sx/emutmp01", NULL)) {

			f_open(&fp, "emunandswitcher/config.ini", FA_READ);
			while (f_gets(info, MAXCHAR, &fp)) {
				linie++;
				char* stelle;
				char* auswahl;
				auswahl = strstr(info, "Emunand 2 =");
				stelle = strchr(auswahl, (int)'=');
				infotext = str_replace(stelle, "= ", "");

				lv_label_set_array_text(status_bar.emuninstsx, infotext, 100);

			}
			f_close(&fp);

		}
		else { // if the directory exists - do this.

		}

		if (f_stat("emunandswitcher/folders sx/emutmp02", NULL)) {

			f_open(&fp, "emunandswitcher/config.ini", FA_READ);
			while (f_gets(info, MAXCHAR, &fp)) {
				linie++;
				char* stelle;
				char* auswahl;
				auswahl = strstr(info, "Emunand 3 =");
				stelle = strchr(auswahl, (int)'=');
				infotext = str_replace(stelle, "= ", "");

				lv_label_set_array_text(status_bar.emuninstsx, infotext, 100);

			}
			f_close(&fp);

		}
		else { // if the directory exists - do this.

		}

	}
	
	//Funktion Theme info SX
	if (f_stat("sxos/titles/0100000000001000", NULL)) {

		static lv_style_t bg;//Definiton Text Box
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_GREEN;
		bg.body.opa = LV_OPA_0;

		lv_mbox_set_text(status_bar.themeinfosx, "OK no Theme installed!");
		lv_mbox_set_style(status_bar.themeinfosx, LV_MBOX_STYLE_BG, &bg);
	}

	else { // if the directory exists - do this.

		static lv_style_t bg;//Definiton Text Box
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_RED;
		bg.body.opa = LV_OPA_0;
		bg.text.font = &lv_font_montserrat_alternate_20;//Test Font 20 Textbox


		lv_mbox_set_text(status_bar.themeinfosx, "Attention Theme installed!\nAfter a system software update\nof an emunand, the theme must\nbe removed to avoid the\nstartup error...");
		lv_mbox_set_style(status_bar.themeinfosx, LV_MBOX_STYLE_BG, &bg);
	}
	


	//Funktion Theme info ATM
	if (f_stat("atmosphere/titles/0100000000001000", NULL)) {

		static lv_style_t bg;//Definiton Text Box
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_GREEN;
		bg.body.opa = LV_OPA_0;
;
		lv_mbox_set_text(status_bar.themeinfoatm, "OK no Theme installed!");
		lv_mbox_set_style(status_bar.themeinfoatm, LV_MBOX_STYLE_BG, &bg);
	}

	else { // if the directory exists - do this.

		static lv_style_t bg;//Definiton Text Box
		lv_style_copy(&bg, &lv_style_pretty);
		bg.text.color = LV_COLOR_RED;
		bg.body.opa = LV_OPA_0;
		bg.text.font = &lv_font_montserrat_alternate_20;//Test Font 20 Textbox

		lv_mbox_set_text(status_bar.themeinfoatm, "Attention Theme installed!\nAfter a system software update\nof an emunand, the theme must\nbe removed to avoid the\nstartup error...");
		lv_mbox_set_style(status_bar.themeinfoatm, LV_MBOX_STYLE_BG, &bg);
	}



}


static bool render_title(argon_ctxt_t * ctxt)//Title und Akku, Zeit, Datum, Strom, Volt und Temperatur anzeige_Helligkeit auslesen aus hw.ini und setzen
{
	//Title Erstellen
    lv_obj_t* title = lv_label_create(lv_scr_act(), NULL);
    lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 15, 620);
	lv_label_set_text(title, "Emunand Switcher v2.1");
    lv_obj_set_auto_realign(title, true);
    
    static lv_style_t label_style;
    lv_style_copy(&label_style, &lv_style_plain);
    label_style.text.color = LV_COLOR_WHITE; //geändert in blau
    label_style.text.font = &lv_font_montserrat_alternate_20;
    lv_obj_set_style(title, &label_style);


	//Definition Styles, Tool Akku, Zeit und Datum auslesen und anzeigen
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


	//Label für Task aktualisieren erstellen Zeit und Datum
	lv_obj_t* lbl_time_temp = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_static_text(lbl_time_temp, "00.00.00 "" 00:00:0000"); //Zeit und Datum
	lv_obj_set_pos(lbl_time_temp, 430, 620);
	lv_label_set_style(lbl_time_temp, LV_LABEL_STYLE_MAIN, &font20_style);
	status_bar.time_date = lbl_time_temp;

	//Label für Task aktualisieren erstellen Akku %
	lv_obj_t* label_akku = lv_label_create(lv_scr_act(), NULL);
	lv_obj_set_pos(label_akku, 350, 620);
	status_bar.akkustand = label_akku;

	//Label für Task aktualisieren erstellen Akku Symbol
	lv_obj_t* symb_akku = lv_label_create(lv_scr_act(), NULL);
	lv_obj_set_pos(symb_akku, 300, 615);
	lv_label_set_style(symb_akku, LV_LABEL_STYLE_MAIN, &labels_style);
	status_bar.akkusym = symb_akku;

	//Label für Task aktualisieren erstellen Stromverbrauch und Volt
	lv_obj_t* label_strom = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_static_text(label_strom, "+0 mA");
	lv_obj_set_pos(label_strom, 1100, 620);
	status_bar.battery_more = label_strom;
	
	lv_obj_t* label_volt = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_static_text(label_volt, "0.000 V");
	lv_obj_set_pos(label_volt, 1200, 620);//1200
	status_bar.battery_more_volt = label_volt;

	//Label für Task aktualisieren erstellen Temperatur
	lv_obj_t* label_temp = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_static_text(label_temp, "CPU 00.0");
	lv_obj_set_pos(label_temp, 950, 620);
	status_bar.temperatur = label_temp;

	static lv_style_t font12_style;//Definition Schriftgrösse 12
	lv_style_copy(&font12_style, &lv_style_plain);
	font12_style.text.color = LV_COLOR_WHITE; //Textfarbe
	font12_style.text.font = &lv_font_montserrat_12;

	//Label Grad Zeichen ° und C
	lv_obj_t* label_grad = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label_grad, "o");
	lv_obj_set_pos(label_grad, 1050, 620);
	lv_label_set_style(label_grad, LV_LABEL_STYLE_MAIN, &font12_style);

	lv_obj_t* label_c = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label_c, "C");
	lv_obj_set_pos(label_c, 1060, 620);
	lv_label_set_style(label_c, LV_LABEL_STYLE_MAIN, &font20_style);



	//Task aktualisieren erstellen
	lv_task_t* status_bar = lv_task_create(update_status, 500, LV_TASK_PRIO_LOW, NULL);
	lv_task_ready(status_bar);


	
	//Hellikeit auslesen aus hw.ini und setzen
	#define MAXCHAR 100//Definition Text aus ini File lesen
	FIL fp;
	char info[MAXCHAR];

	//Prüfung hw.ini vorhanden, wenn nicht Grundhelligkeit setzen
	if (f_stat("emunandswitcher/sys/hw.ini", NULL)) {

		//Helligkeit setzen
		display_backlight_brightness(100, 1000);
	}

	else { // if the file exists - do this.

		f_open(&fp, "emunandswitcher/sys/hw.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			char* stelle;
			char* auswahl;
			auswahl = strstr(info, "Helligkeit =");
			stelle = strchr(auswahl, (int)'=');
			infotext = str_replace(stelle, "= ", "");

			//Zahlen umwandeln zu int, String zu int: atoi
			int hell = atoi(infotext);

			//Helligkeit setzen
			display_backlight_brightness(hell, 1000);
		}
		f_close(&fp);
	}


	
	gui_menu_pool_push(ctxt->pool, symb_akku);// Eintrag Akku Symbol Label 
	gui_menu_pool_push(ctxt->pool, label_akku);// Eintrag Akku % Label 
	gui_menu_pool_push(ctxt->pool, lbl_time_temp);// Eintrag Zeit und Datum Label
    gui_menu_pool_push(ctxt->pool, title);
    return true;
}


static bool render_switcherSX_tab(lv_obj_t* par, argon_ctxt_t* ctxt) //Eigener Tab SwitcherSX
{
	static lv_style_t labels_style;

	lv_obj_t* emunsx_tab = lv_tabview_add_tab(par,
		LV_SYMBOL_LOOP " Switcher SX"); //Menü Eintrag und Symbol Tab

	lv_style_copy(&labels_style, lv_theme_get_current()->style.label.prim);
	labels_style.text.color = LV_COLOR_WHITE;

	u32 labels_y = 140;


	static lv_style_t inv_label;//Definition Transparentes Label Jeweils bei Label hinzufügen: lv_obj_set_style(label, &inv_label);
	lv_style_copy(&inv_label, &lv_style_transp);
	inv_label.text.font = NULL;

	static lv_style_t font20_style;//Definition Schriftgrösse 20
	lv_style_copy(&font20_style, &lv_style_plain);
	font20_style.text.color = LV_COLOR_WHITE; //Textfarbe
	font20_style.text.font = &lv_font_montserrat_alternate_20;




	/*Text ausserhalb Button*/
	/*label = lv_label_create(lv_scr_act(), NULL);
	lv_obj_align(label, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 500, 500);
	lv_label_set_text(label, "Test");*/

	lv_obj_t* title_emun = lv_label_create(emunsx_tab, NULL); //Menu Eintrag rechts
	lv_label_set_text(title_emun, LV_SYMBOL_WARNING" Choose Emunand");
	lv_obj_set_pos(title_emun, LV_HOR_RES_MAX / 2 + 80, labels_y);
	lv_label_set_style(title_emun, LV_LABEL_STYLE_MAIN, &labels_style);



	/*Emunand Buttons*/
	lv_obj_t* btn_emunand = lv_cont_create(emunsx_tab, NULL);
	lv_obj_set_pos(btn_emunand, LV_HOR_RES_MAX / 2 + 30, labels_y);//possition Buttons
	lv_obj_set_size(btn_emunand, LV_HOR_RES / 2.5, 490);
	lv_cont_set_layout(btn_emunand, LV_LAYOUT_COL_L);//lv_cont_set_layout(btn_test, LV_LAYOUT_CENTER);//Alle Buttons ausrichtung



	/*Erster Button muss immer das hier am Anfang haben*/
	/*lv_obj_t* btn = lv_btn_create(btn_test, NULL);
	lv_obj_t* label = lv_label_create(btn, NULL);*/

	/*Button mit Bild und Text zentirert im Bild*/
	lv_img_dsc_t* img = NULL;//Bild Pointer
	/* Declare styles for Bilder Buttons */
	static lv_style_t style_pr;
	lv_style_copy(&style_pr, &lv_style_plain);
	style_pr.image.color = LV_COLOR_BLACK;
	style_pr.image.intense = LV_OPA_50;
	style_pr.text.color = LV_COLOR_WHITE;//lv_color_hex3(0xaaa);

	/* Try to get logo */
	img = bmp_to_lvimg_obj("emunandswitcher/logos/emunand1.bmp");

	lv_obj_t* btn = lv_imgbtn_create(btn_emunand, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 130, 130);
	lv_obj_set_event_cb(btn, ctrl_emun1);



	#define MAXCHAR 100//Definition Text aus ini File lesen
	FIL fp;
	int linie = 0;
	char info[MAXCHAR];

	lv_obj_t* label = lv_label_create(emunsx_tab, NULL);

	//Prüfung Config.ini vorhanden
	if (f_stat("emunandswitcher/config.ini", NULL)) {

		lv_label_set_text(label, "Config.ini not available\nor incorrect!");
		lv_obj_set_pos(label, 870, 240);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);
	}

	else { // if the file exists - do this.

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);//Label Emunand1 aus ini lesen und auf Button anzeigen
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			char* stelle;
			char* auswahl;
			auswahl = strstr(info, "Emunand 1 =");
			stelle = strchr(auswahl, (int)'=');
			infotext = str_replace(stelle, "= ", "");

			lv_label_set_text(label, infotext);
			lv_obj_set_pos(label, 870, 240);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

		}
		f_close(&fp);
	}

	/* Try to get logo */
	img = bmp_to_lvimg_obj("emunandswitcher/logos/emunand2.bmp");

	btn = lv_imgbtn_create(btn_emunand, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 130, 130);
	lv_obj_set_event_cb(btn, ctrl_emun2);

	label = lv_label_create(emunsx_tab, NULL);

	//Prüfung Config.ini vorhanden
	if (f_stat("emunandswitcher/config.ini", NULL)) {

		lv_label_set_text(label, "Config.ini not available\nor incorrect!");
		lv_obj_set_pos(label, 870, 385);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);
	}

	else { // if the file exists - do this.

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			char* stelle;
			char* auswahl;
			auswahl = strstr(info, "Emunand 2 =");
			stelle = strchr(auswahl, (int)'=');
			infotext = str_replace(stelle, "= ", "");

			lv_label_set_text(label, infotext);
			lv_obj_set_pos(label, 870, 385);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

		}
		f_close(&fp);
	}





	/* Try to get logo */
	img = bmp_to_lvimg_obj("emunandswitcher/logos/emunand3.bmp");

	btn = lv_imgbtn_create(btn_emunand, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 130, 130);
	lv_obj_set_event_cb(btn, ctrl_emun3);

	label = lv_label_create(emunsx_tab, NULL);

	//Prüfung Config.ini vorhanden
	if (f_stat("emunandswitcher/config.ini", NULL)) {

		lv_label_set_text(label, "Config.ini not available\nor incorrect!");
		lv_obj_set_pos(label, 870, 530);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);
	}

	else { // if the file exists - do this.

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			char* stelle;
			char* auswahl;
			auswahl = strstr(info, "Emunand 3 =");
			stelle = strchr(auswahl, (int)'=');
			infotext = str_replace(stelle, "= ", "");

			lv_label_set_text(label, infotext);
			lv_obj_set_pos(label, 870, 530);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

		}
		f_close(&fp);
	}


	// CFW start Spalte
	lv_obj_t* title_CFW = lv_label_create(emunsx_tab, NULL);
	lv_label_set_text(title_CFW, LV_SYMBOL_PLAY" Start CFW");
	lv_obj_set_pos(title_CFW, 245, labels_y);
	lv_label_set_style(title_CFW, LV_LABEL_STYLE_MAIN, &labels_style);

	lv_obj_t* btn_sx = lv_cont_create(emunsx_tab, NULL);
	lv_obj_set_pos(btn_sx, 80, 170);// labels_y + 20 entfehrt = Zentriert links
	lv_obj_set_size(btn_sx, LV_HOR_RES / 2.5, 400);
	lv_cont_set_layout(btn_sx, LV_LAYOUT_CENTER);

	
	
	/* Try to get logo */
	img = bmp_to_lvimg_obj("emunandswitcher/logos/sxos.bmp");

	btn = lv_imgbtn_create(btn_sx, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 200, 200);
	lv_obj_set_event_cb(btn, ctrl_lauch_payload);

	label = lv_label_create(btn, NULL);
	lv_label_set_text(label, "emunandswitcher/payloads/sxos.bin");
	lv_obj_set_style(label, &inv_label);//Unsichtbares Label
	
	
	//Label für Task aktualisieren erstellen Installierter EmunandSX
	lv_obj_t* emuninstall_sx = lv_label_create(btn_sx, NULL);
	lv_obj_set_pos(emuninstall_sx, 10, 100);
	lv_label_set_style(emuninstall_sx, LV_LABEL_STYLE_MAIN, &labels_style);
	status_bar.emuninstsx = emuninstall_sx;

	//mbox für Task aktualisieren Theme info
	lv_obj_t* themesx = lv_mbox_create(emunsx_tab, NULL);
	lv_mbox_set_recolor(themesx, true);
	lv_obj_set_width(themesx, LV_DPI * 4.5);
	lv_obj_set_top(themesx, true);
	lv_obj_set_auto_realign(themesx, true);
	lv_obj_align(themesx, NULL, LV_ALIGN_IN_TOP_LEFT, 100, 5);
	status_bar.themeinfosx = themesx;


	/*//Label für Task aktualisieren Theme info
	lv_obj_t* themesx = lv_label_create(emunsx_tab, NULL);
	lv_obj_set_pos(themesx, 100, 5);
	status_bar.themeinfosx = themesx;*/



	lv_obj_t* line = lv_line_create(emunsx_tab, NULL);

	static lv_point_t line_points[] = { {LV_HOR_RES_MAX / 2., 20}, {LV_HOR_RES_MAX / 2., LV_VER_RES_MAX - 100} };
	lv_line_set_points(line, line_points, 2);
	lv_line_set_style(line, LV_LINE_STYLE_MAIN, lv_theme_get_current()->style.line.decor);

	gui_menu_pool_push(ctxt->pool, emunsx_tab);
	gui_menu_pool_push(ctxt->pool, title_CFW);
	gui_menu_pool_push(ctxt->pool, btn_sx);
	gui_menu_pool_push(ctxt->pool, btn);
	gui_menu_pool_push(ctxt->pool, label);
	gui_menu_pool_push(ctxt->pool, line);

	gui_menu_pool_push(ctxt->pool, btn_emunand);//Eigene Buttons
	gui_menu_pool_push(ctxt->pool, emuninstall_sx);// Eintrag Installierter Emunanand Label 
	//gui_menu_pool_push(ctxt->pool, themesx);// Eintrag Theme info SX-->Wenn aktiv blockiert!!! Funktioniert auch ohne bool!!!



	return true;
}




static bool render_switcherATM_tab(lv_obj_t* par, argon_ctxt_t* ctxt) //Eigener Tab SwitcherATM
{
	static lv_style_t labels_style;

	lv_obj_t* emunatm_tab = lv_tabview_add_tab(par,
		LV_SYMBOL_LOOP " Switcher ATM"); //Menü Eintrag und Symbol Tab

	lv_style_copy(&labels_style, lv_theme_get_current()->style.label.prim);
	labels_style.text.color = LV_COLOR_WHITE;

	u32 labels_y = 140;

	static lv_style_t inv_label;//Definition Transparentes Label Jeweil bei Label hinzuf�gen: lv_obj_set_style(label, &inv_label);
	lv_style_copy(&inv_label, &lv_style_transp);
	inv_label.text.font = NULL;

	static lv_style_t font20_style;//Definition Schriftgr�sse 20
	lv_style_copy(&font20_style, &lv_style_plain);
	font20_style.text.color = LV_COLOR_WHITE; //Textfarbe
	font20_style.text.font = &lv_font_montserrat_alternate_20;

	lv_obj_t* title_emunatm = lv_label_create(emunatm_tab, NULL); //Menu Eintrag rechts
	lv_label_set_text(title_emunatm, LV_SYMBOL_WARNING" Choose EmuMMC");
	lv_obj_set_pos(title_emunatm, LV_HOR_RES_MAX / 2 + 80, labels_y);
	lv_label_set_style(title_emunatm, LV_LABEL_STYLE_MAIN, &labels_style);

	//Emunand Buttons ATM
	lv_obj_t* btn_emuatm = lv_cont_create(emunatm_tab, NULL);
	lv_obj_set_pos(btn_emuatm, LV_HOR_RES_MAX / 2 + 30, labels_y);//possition Buttons
	lv_obj_set_size(btn_emuatm, LV_HOR_RES / 2.5, 490);
	lv_cont_set_layout(btn_emuatm, LV_LAYOUT_COL_L);//Alle Buttons im Container ausrichtung

	lv_img_dsc_t* img = NULL;//Bild Pointer

	static lv_style_t style_pr;
	lv_style_copy(&style_pr, &lv_style_plain);
	style_pr.image.color = LV_COLOR_BLACK;
	style_pr.image.intense = LV_OPA_50;
	style_pr.text.color = LV_COLOR_WHITE;//lv_color_hex3(0xaaa);


	img = bmp_to_lvimg_obj("emunandswitcher/logos/emummc1.bmp");

	lv_obj_t* btn = lv_imgbtn_create(btn_emuatm, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 130, 130);
	lv_obj_set_event_cb(btn, ctrl_emmc1);

	#define MAXCHAR 100//Definition Text aus ini File lesen
	FIL fp;
	int linie = 0;
	char info[MAXCHAR];

	lv_obj_t* label = lv_label_create(emunatm_tab, NULL);

	//Prüfung Config.ini vorhanden
	if (f_stat("emunandswitcher/config.ini", NULL)) {

		lv_label_set_text(label, "Config.ini not available\nor incorrect!");
		lv_obj_set_pos(label, 870, 240);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);
	}

	else { // if the file exists - do this.

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);//Label Emunand1 aus ini lesen und auf Button anzeigen
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			char* stelle;
			char* auswahl;
			auswahl = strstr(info, "Emuemmc 1 =");
			stelle = strchr(auswahl, (int)'=');
			infotext = str_replace(stelle, "= ", "");

			lv_label_set_text(label, infotext);
			lv_obj_set_pos(label, 870, 240);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

		}
		f_close(&fp);
	}


	img = bmp_to_lvimg_obj("emunandswitcher/logos/emummc2.bmp");

	btn = lv_imgbtn_create(btn_emuatm, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 130, 130);
	lv_obj_set_event_cb(btn, ctrl_emmc2);

	label = lv_label_create(emunatm_tab, NULL);

	//Prüfung Config.ini vorhanden
	if (f_stat("emunandswitcher/config.ini", NULL)) {

		lv_label_set_text(label, "Config.ini not available\nor incorrect!");
		lv_obj_set_pos(label, 870, 385);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);
	}

	else { // if the file exists - do this.

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			char* stelle;
			char* auswahl;
			auswahl = strstr(info, "Emuemmc 2 =");
			stelle = strchr(auswahl, (int)'=');
			infotext = str_replace(stelle, "= ", "");

			lv_label_set_text(label, infotext);
			lv_obj_set_pos(label, 870, 385);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

		}
		f_close(&fp);
	}

	img = bmp_to_lvimg_obj("emunandswitcher/logos/emummc3.bmp");

	btn = lv_imgbtn_create(btn_emuatm, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 130, 130);
	lv_obj_set_event_cb(btn, ctrl_emmc3);

	label = lv_label_create(emunatm_tab, NULL);

	//Prüfung Config.ini vorhanden
	if (f_stat("emunandswitcher/config.ini", NULL)) {

		lv_label_set_text(label, "Config.ini not available\nor incorrect!");
		lv_obj_set_pos(label, 870, 530);
		lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);
	}

	else { // if the file exists - do this.

		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			linie++;
			char* stelle;
			char* auswahl;
			auswahl = strstr(info, "Emuemmc 3 =");
			stelle = strchr(auswahl, (int)'=');
			infotext = str_replace(stelle, "= ", "");

			lv_label_set_text(label, infotext);
			lv_obj_set_pos(label, 870, 530);
			lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

		}
		f_close(&fp);
	}




	//CFW start Spalte ATM
	lv_obj_t* title_CFWatm = lv_label_create(emunatm_tab, NULL);
	lv_label_set_text(title_CFWatm, LV_SYMBOL_PLAY" Start CFW");
	lv_obj_set_pos(title_CFWatm, 245, labels_y);
	lv_label_set_style(title_CFWatm, LV_LABEL_STYLE_MAIN, &labels_style);

	lv_obj_t* btn_atm = lv_cont_create(emunatm_tab, NULL);
	lv_obj_set_pos(btn_atm, 80, 170);// labels_y + 20 entfehrt = Zentriert links
	lv_obj_set_size(btn_atm, LV_HOR_RES / 2.5, 400);
	lv_cont_set_layout(btn_atm, LV_LAYOUT_CENTER);




	img = bmp_to_lvimg_obj("emunandswitcher/logos/atmosphere.bmp");

	btn = lv_imgbtn_create(btn_atm, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 200, 200);
	lv_obj_set_event_cb(btn, ctrl_lauch_payload);

	label = lv_label_create(btn, NULL);
	lv_label_set_text(label, "emunandswitcher/payloads/fusee-primary.bin");
	lv_obj_set_style(label, &inv_label);//Unsichtbares Label


	//Label für Task aktualisieren erstellen Installierter EmunandATM
	lv_obj_t* emuninstall_atm = lv_label_create(btn_atm, NULL);
	lv_obj_set_pos(emuninstall_atm, 10, 10);
	lv_label_set_style(emuninstall_atm, LV_LABEL_STYLE_MAIN, &labels_style);
	status_bar.emuninst = emuninstall_atm;


	//mbox für Task aktualisieren Theme info
	lv_obj_t* themeatm = lv_mbox_create(emunatm_tab, NULL);
	lv_mbox_set_recolor(themeatm, true);
	lv_obj_set_width(themeatm, LV_DPI * 4.5);
	lv_obj_set_top(themeatm, true);
	lv_obj_set_auto_realign(themeatm, true);
	lv_obj_align(themeatm, NULL, LV_ALIGN_IN_TOP_LEFT, 100, 5);
	status_bar.themeinfoatm = themeatm;

	/*//Label für Task aktualisieren Theme info
	lv_obj_t* themeatm = lv_label_create(emunatm_tab, NULL);
	lv_obj_set_pos(themeatm, 100, 5);
	status_bar.themeinfoatm = themeatm;*/




	lv_obj_t* line = lv_line_create(emunatm_tab, NULL);

	static lv_point_t line_points[] = { {LV_HOR_RES_MAX / 2., 20}, {LV_HOR_RES_MAX / 2., LV_VER_RES_MAX - 100} };
	lv_line_set_points(line, line_points, 2);
	lv_line_set_style(line, LV_LINE_STYLE_MAIN, lv_theme_get_current()->style.line.decor);

	gui_menu_pool_push(ctxt->pool, emunatm_tab);
	gui_menu_pool_push(ctxt->pool, title_CFWatm);
	gui_menu_pool_push(ctxt->pool, btn_atm);
	gui_menu_pool_push(ctxt->pool, btn);
	gui_menu_pool_push(ctxt->pool, label);
	gui_menu_pool_push(ctxt->pool, line);

	gui_menu_pool_push(ctxt->pool, btn_emuatm);//Eigene Buttons
	gui_menu_pool_push(ctxt->pool, emuninstall_atm);// Eintrag Installierter Emunanand Label 

	return true;
}



//Statische Funktionen Helligkeit
void ctrl_helligkeit(lv_obj_t* obj, lv_event_t event);//Eintrag Helligkeit einstellen Funktion
static lv_obj_t* slider;


static bool render_tools_tab(lv_obj_t* par, argon_ctxt_t* ctxt)
{
	static lv_style_t labels_style;

	lv_obj_t* settings_tab = lv_tabview_add_tab(par,
		LV_SYMBOL_SETTINGS " Tools");

	lv_style_copy(&labels_style, lv_theme_get_current()->style.label.prim);
	labels_style.text.color = LV_COLOR_WHITE;


	static lv_style_t font20_style;//Definition Schriftgrösse 20
	lv_style_copy(&font20_style, &lv_style_plain);
	font20_style.text.color = LV_COLOR_WHITE; //Textfarbe
	font20_style.text.font = &lv_font_montserrat_alternate_20;


	u32 labels_y = 140;

	//System Tools
	lv_obj_t* more_to_come = lv_label_create(settings_tab, NULL); //Menu Eintrag rechts
	lv_label_set_text(more_to_come, LV_SYMBOL_SETTINGS" System Tools");
	lv_obj_set_pos(more_to_come, 420, labels_y);
	lv_label_set_style(more_to_come, LV_LABEL_STYLE_MAIN, &labels_style);

	lv_obj_t* btn_test = lv_cont_create(settings_tab, NULL);
	//lv_obj_set_pos(btn_test, 370, labels_y + 20);//possition Buttons
	lv_obj_set_pos(btn_test, LV_HOR_RES_MAX - 910, labels_y + 20);//possition Buttons
	lv_obj_set_size(btn_test, 400, 450);
	lv_cont_set_layout(btn_test, LV_LAYOUT_COL_L);//lv_cont_set_layout(btn_test, LV_LAYOUT_CENTER);//Alle Buttons ausrichtung


	//Erster Button muss immer das hier am Anfang haben
	/*lv_obj_t* btn = lv_btn_create(btn_test, NULL);
	lv_obj_t* label = lv_label_create(btn, NULL);*/

	//Button mit Bild
	lv_img_dsc_t* img = NULL;//Bild Pointer

	// Declare styles for Bilder Buttons 
	static lv_style_t style_pr;
	lv_style_copy(&style_pr, &lv_style_plain);
	style_pr.image.color = LV_COLOR_BLACK;
	style_pr.image.intense = LV_OPA_50;
	style_pr.text.color = LV_COLOR_WHITE;//lv_color_hex3(0xaaa);

	// Try to get logo Button
	img = bmp_to_lvimg_obj("emunandswitcher/logos/theme.bmp");

	lv_obj_t* btn = lv_imgbtn_create(btn_test, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 100, 100);
	lv_obj_set_event_cb(btn, ctrl_themedel);

	lv_obj_t* label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "Delete Theme SXOS");
	lv_obj_set_pos(label, 540, 245);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

	// Try to get logo Button
	img = bmp_to_lvimg_obj("emunandswitcher/logos/theme.bmp");

	btn = lv_imgbtn_create(btn_test, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 100, 100);
	lv_obj_set_event_cb(btn, ctrl_themedelATM);

	label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "Delete Theme ATM");
	lv_obj_set_pos(label, 540, 360);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

	// Try to get logo Button
	img = bmp_to_lvimg_obj("emunandswitcher/logos/update.bmp");

	btn = lv_imgbtn_create(btn_test, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 100, 100);
	lv_obj_set_event_cb(btn, ctrl_updsx);

	label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "Update SXOS");
	lv_obj_set_pos(label, 540, 475);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);




	// Power off tools
	lv_obj_t* power_label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(power_label, LV_SYMBOL_POWER" Power Tools");
	lv_obj_set_pos(power_label, 60, labels_y);
	lv_label_set_style(power_label, LV_LABEL_STYLE_MAIN, &labels_style);

	lv_obj_t* btn_cont = lv_cont_create(settings_tab, NULL);
	//lv_obj_set_pos(btn_cont, 0, labels_y + 20);
	lv_obj_set_pos(btn_cont, 10, labels_y + 20);
	lv_obj_set_size(btn_cont, 350, 450);
	lv_cont_set_layout(btn_cont, LV_LAYOUT_COL_L);

	// Try to get logo Button
	img = bmp_to_lvimg_obj("emunandswitcher/logos/power.bmp");

	btn = lv_imgbtn_create(btn_cont, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 100, 100);
	lv_obj_set_event_cb(btn, ctrl_reboot_rcm);

	label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "Reboot RCM");
	lv_obj_set_pos(label, 180, 245);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

	// Try to get logo Button
	img = bmp_to_lvimg_obj("emunandswitcher/logos/power.bmp");

	btn = lv_imgbtn_create(btn_cont, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 100, 100);
	lv_obj_set_event_cb(btn, ctrl_power_off);

	label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "Power off");
	lv_obj_set_pos(label, 180, 360);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

	// Try to get logo Button
	img = bmp_to_lvimg_obj("emunandswitcher/logos/power.bmp");

	btn = lv_imgbtn_create(btn_cont, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 100, 100);
	lv_obj_set_event_cb(btn, ctrl_reboot_ofw);

	label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "Reboot OFW");
	lv_obj_set_pos(label, 180, 475);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);




	//Konfigurations tools
	lv_obj_t* sonst_label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(sonst_label, LV_SYMBOL_SETTINGS" Configuration Tools");
	lv_obj_set_pos(sonst_label, 850, labels_y);
	lv_label_set_style(sonst_label, LV_LABEL_STYLE_MAIN, &labels_style);

	lv_obj_t* btn_sonst = lv_cont_create(settings_tab, NULL);
	lv_obj_set_pos(btn_sonst, 800, labels_y + 20);
	lv_obj_set_size(btn_sonst, 400, 450);
	lv_cont_set_layout(btn_sonst, LV_LAYOUT_COL_L);

	// Try to get logo Button Config.ini bearbeiten
	img = bmp_to_lvimg_obj("emunandswitcher/logos/confini.bmp");

	btn = lv_imgbtn_create(btn_sonst, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 100, 100);
	lv_obj_set_event_cb(btn, ctrl_confini);

	label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "Edit Config.ini");
	lv_obj_set_pos(label, 970, 245);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

	// Try to get logo RTC Button
	img = bmp_to_lvimg_obj("emunandswitcher/logos/rtc.bmp");

	btn = lv_imgbtn_create(btn_sonst, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 100, 100);
	lv_obj_set_event_cb(btn, ctrl_rtctime);

	label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "RTC Time and Date");
	lv_obj_set_pos(label, 970, 360);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);

	// Try to get logo Info Button
	img = bmp_to_lvimg_obj("emunandswitcher/logos/about.bmp");

	btn = lv_imgbtn_create(btn_sonst, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 100, 100);
	lv_obj_set_event_cb(btn, ctrl_info);

	label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "Info");
	lv_obj_set_pos(label, 970, 475);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);


	/*// Try to get logo Test Button
	img = bmp_to_lvimg_obj("emunandswitcher/logos/about.bmp");

	btn = lv_imgbtn_create(settings_tab, NULL);
	lv_imgbtn_set_style(btn, LV_BTN_STATE_PR, &style_pr);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_REL, img);
	lv_imgbtn_set_src(btn, LV_BTN_STATE_PR, img);
	lv_obj_set_size(btn, 100, 100);
	lv_obj_set_pos(btn, 500, 20);
	lv_obj_set_event_cb(btn, ctrl_test);

	label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "Test");
	lv_obj_set_pos(label, 620, 70);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);*/



	//Slider für Helligkeit

	//Auslesen aktuelle Helligkeit
	//char* hellakt = (char*)malloc(64);
	u32 slider_value = (PWM(PWM_CONTROLLER_PWM_CSR_0) >> 16) & 0xFF;
	//s_printf(hellakt, "Helligkeit %d", slider_value);

	//Create styles
	static lv_style_t style_bg;
	lv_style_copy(&style_bg, &lv_style_pretty);
	style_bg.body.main_color = LV_COLOR_BLACK;
	style_bg.body.grad_color = LV_COLOR_GRAY;
	style_bg.body.radius = LV_RADIUS_CIRCLE;
	style_bg.body.border.color = LV_COLOR_WHITE;

	static lv_style_t style_indic;
	lv_style_copy(&style_indic, &lv_style_pretty_color);
	style_indic.body.radius = LV_RADIUS_CIRCLE;
	style_indic.body.shadow.width = 8;
	style_indic.body.shadow.color = style_indic.body.main_color;
	style_indic.body.padding.left = 3;
	style_indic.body.padding.right = 3;
	style_indic.body.padding.top = 3;
	style_indic.body.padding.bottom = 3;

	static lv_style_t style_knob;
	lv_style_copy(&style_knob, &lv_style_pretty);
	style_knob.body.radius = LV_RADIUS_CIRCLE;
	style_knob.body.opa = LV_OPA_70;
	style_knob.body.padding.top = 10;
	style_knob.body.padding.bottom = 10;

	//Create a slider
	slider = lv_slider_create(settings_tab, NULL);
	lv_slider_set_style(slider, LV_SLIDER_STYLE_BG, &style_bg);
	lv_slider_set_style(slider, LV_SLIDER_STYLE_INDIC, &style_indic);
	lv_slider_set_style(slider, LV_SLIDER_STYLE_KNOB, &style_knob);
	lv_obj_set_pos(slider, 80, 20);
	//lv_obj_set_size(slider, 100, 100);
	//lv_obj_set_width(slider, 300);
	
	//Minimale und Maximale Werte des Sliders und slider_value von oben setzen
	lv_slider_set_range(slider, 10, 200);
	lv_slider_set_value(slider, slider_value, LV_ANIM_OFF);

	//lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_event_cb(slider, ctrl_helligkeit);

	label = lv_label_create(settings_tab, NULL);
	lv_label_set_text(label, "Display brightness");
	lv_obj_set_pos(label, 90, 60);
	lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &font20_style);


	//Linie erzeugen
	lv_obj_t* line = lv_line_create(settings_tab, NULL);
	static lv_point_t line_points[] = { {360, 20}, {360, LV_VER_RES_MAX - 120} };
	lv_line_set_points(line, line_points, 2);
	lv_line_set_style(line, LV_LINE_STYLE_MAIN, lv_theme_get_current()->style.line.decor);

	lv_obj_t* line2 = lv_line_create(settings_tab, NULL);
	static lv_point_t line2_points[] = { {790, 20}, {790, LV_VER_RES_MAX - 120} };
	lv_line_set_points(line2, line2_points, 2);
	lv_line_set_style(line2, LV_LINE_STYLE_MAIN, lv_theme_get_current()->style.line.decor);


	gui_menu_pool_push(ctxt->pool, settings_tab);
	gui_menu_pool_push(ctxt->pool, power_label);
	gui_menu_pool_push(ctxt->pool, btn_cont);
	gui_menu_pool_push(ctxt->pool, btn);
	gui_menu_pool_push(ctxt->pool, label);
	gui_menu_pool_push(ctxt->pool, line);
	gui_menu_pool_push(ctxt->pool, btn_test);//Eigene Buttons


	return true;
}


//Helligkeit Slider Funktion
void ctrl_helligkeit(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_VALUE_CHANGED)
	{
		//Helligkeit setzen
		int slider_hell = lv_slider_get_value(slider);
		display_backlight_brightness(slider_hell, 1000);

		//Auslesen Text von slider für hw.ini
		char* hellakt = (char*)malloc(64);
		s_printf(hellakt, "Helligkeit = %d", lv_slider_get_value(slider));

		//Text in ini speichern
		FIL fp;

		//alte hw.ini löschen, sonst komischer Fehler letzter String beim speichern immer MM am ende
		f_unlink("emunandswitcher/sys/hw.ini");

		//Prüfung Config.ini vorhanden sonst erstellen
		if (f_stat("emunandswitcher/sys/hw.ini", NULL)) {

			f_open(&fp, "emunandswitcher/sys/hw.ini", FA_CREATE_NEW);//FA_WRITE, FA_CREATE_NEW
			f_close(&fp);

		}

		//Datei öffnen und Text speichern
		f_open(&fp, "emunandswitcher/sys/hw.ini", FA_WRITE);//FA_WRITE, FA_CREATE_NEW

		f_puts(hellakt, &fp);

		f_close(&fp);

	}
}



/*
typedef struct _system_maintenance_tasks_t
{
	union
	{
		lv_task_t* tasks[2];
		struct
		{
			lv_task_t* status_bar;
			lv_task_t* dram_periodic_comp;
		} task;
	};
} system_maintenance_tasks_t;

static system_maintenance_tasks_t system_tasks;

void manual_system_maintenance(bool refresh)
{
	for (u32 task_idx = 0; task_idx < (sizeof(system_maintenance_tasks_t) / sizeof(lv_task_t*)); task_idx++)
	{
		lv_task_t* task = system_tasks.tasks[task_idx];
		if (task && (lv_tick_elaps(task->last_run) >= task->period))
		{
			task->last_run = lv_tick_get();
			task->task(task->param);
		}
	}
	if (refresh)
		lv_refr_now();
}*/






//hekate nyx status bar
/*typedef struct _gui_status_bar_ctx
{
	lv_obj_t* mid;
	lv_obj_t* time_temp;
	lv_obj_t* temp_symbol;
	lv_obj_t* temp_degrees;
	lv_obj_t* battery;
	lv_obj_t* battery_more;
} gui_status_bar_ctx;

static gui_status_bar_ctx status_bar;*/




/*static void _update_status_bar(void* params)
{





	char* label = (char*)malloc(64);

	u16 soc_temp;
	u32 batt_percent;
	int charge_status;
	int batt_volt;
	int batt_curr;
	rtc_time_t time;

	// Get sensor data.
	max77620_rtc_get_time(&time);
	soc_temp = tmp451_get_soc_temp(false);
	bq24193_get_property(BQ24193_ChargeStatus, &charge_status);
	max17050_get_property(MAX17050_RepSOC, (int*)& batt_percent);
	max17050_get_property(MAX17050_VCELL, &batt_volt);
	max17050_get_property(MAX17050_Current, &batt_curr);

	//! TODO: Parse time and use offset.
	// Set time and SoC temperature.
	s_printf(label, "%02d:%02d "LV_SYMBOL_MINUS" "LV_SYMBOL_GPS" %02d.%d",
		time.hour, time.min, soc_temp >> 8, (soc_temp & 0xFF) / 10);

	lv_label_set_array_text(status_bar.time_temp, label, 64);

	lv_obj_realign(status_bar.temp_symbol);
	lv_obj_realign(status_bar.temp_degrees);

	// Set battery percent and charging symbol.
	s_printf(label, " "LV_SYMBOL_MINUS" %d.%d%% ", (batt_percent >> 8) & 0xFF, (batt_percent & 0xFF) / 26);

	u8 batt_level = (batt_percent >> 8) & 0xFF;
	if (batt_level > 80)
		s_printf(label + strlen(label), LV_SYMBOL_BATTERY_FULL);
	else if (batt_level > 60)
		s_printf(label + strlen(label), LV_SYMBOL_BATTERY_3);
	else if (batt_level > 40)
		s_printf(label + strlen(label), LV_SYMBOL_BATTERY_2);
	else if (batt_level > 15)
		s_printf(label + strlen(label), LV_SYMBOL_BATTERY_1);
	else
		s_printf(label + strlen(label), "#FF3C28 "LV_SYMBOL_BATTERY_EMPTY"#");

	if (charge_status)
		s_printf(label + strlen(label), " #FFDD00 "LV_SYMBOL_CHARGE"#");

	lv_label_set_array_text(status_bar.battery, label, 64);
	lv_obj_realign(status_bar.battery);

	// Set battery current draw and voltage.
	if (batt_curr >= 0)
		s_printf(label, "#96FF00 +%d", batt_curr / 1000);
	else
		s_printf(label, "#FF3C28 -%d", (~batt_curr + 1) / 1000);

	s_printf(label + strlen(label), " mA# (%d mV)", batt_volt);

	lv_label_set_array_text(status_bar.battery_more, label, 64);
	lv_obj_realign(status_bar.battery_more);

	free(label);
}*/


/*static void _create_status_bar(lv_theme_t* th)
{

	static lv_style_t hint_small_style_white;
	lv_style_copy(&hint_small_style_white, lv_theme_get_current()->style.label.prim);
	hint_small_style_white.text.color = LV_COLOR_WHITE;
	hint_small_style_white.text.letter_space = 1;
	//hint_small_style_white.text.font = &interui_20;





	static lv_obj_t* status_bar_bg;
	status_bar_bg = lv_cont_create(lv_layer_top(), NULL);

	static lv_style_t status_bar_style;
	lv_style_copy(&status_bar_style, &lv_style_plain_color);
	status_bar_style.body.opa = LV_OPA_0;
	status_bar_style.body.shadow.width = 0;

	lv_obj_set_style(status_bar_bg, &status_bar_style);
	lv_obj_set_size(status_bar_bg, LV_HOR_RES, LV_DPI * 9 / 14);
	lv_obj_align(status_bar_bg, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

	// Battery percentages.
	lv_obj_t* lbl_battery = lv_label_create(status_bar_bg, NULL);
	lv_label_set_recolor(lbl_battery, true);
	lv_label_set_static_text(lbl_battery, " "LV_SYMBOL_MINUS" 00.0% "LV_SYMBOL_BATTERY_1" #FFDD00 "LV_SYMBOL_CHARGE"#");
	lv_obj_align(lbl_battery, NULL, LV_ALIGN_IN_RIGHT_MID, -LV_DPI * 6 / 11, 0);
	status_bar.battery = lbl_battery;

	// Amperages, voltages.
	lbl_battery = lv_label_create(status_bar_bg, lbl_battery);
	lv_obj_set_style(lbl_battery, &hint_small_style_white);
	lv_label_set_static_text(lbl_battery, "#96FF00 +0 mA# (0 mV)");
	lv_obj_align(lbl_battery, status_bar.battery, LV_ALIGN_OUT_LEFT_MID, -LV_DPI / 25, -1);
	status_bar.battery_more = lbl_battery;

	lv_obj_t* lbl_left = lv_label_create(status_bar_bg, NULL);
	lv_label_set_static_text(lbl_left, LV_SYMBOL_BELL" ");
	lv_obj_align(lbl_left, NULL, LV_ALIGN_IN_LEFT_MID, LV_DPI * 6 / 11, 0);

	// Time, temperature.
	lv_obj_t* lbl_time_temp = lv_label_create(status_bar_bg, NULL);
	lv_label_set_static_text(lbl_time_temp, "00:00 "LV_SYMBOL_MINUS" "LV_SYMBOL_GPS" 00.0");
	lv_obj_align(lbl_time_temp, lbl_left, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
	status_bar.time_temp = lbl_time_temp;

	lbl_left = lv_label_create(status_bar_bg, NULL);
	lv_label_set_static_text(lbl_left, " "LV_SYMBOL_MINUS);
	lv_obj_align(lbl_left, lbl_time_temp, LV_ALIGN_OUT_RIGHT_MID, 0, -LV_DPI / 14);
	status_bar.temp_symbol = lbl_left;

	lv_obj_t* lbl_degrees = lv_label_create(status_bar_bg, NULL);
	lv_label_set_static_text(lbl_degrees, "C");
	lv_obj_align(lbl_degrees, lbl_left, LV_ALIGN_OUT_RIGHT_MID, LV_DPI / 50, LV_DPI / 14);
	status_bar.temp_degrees = lbl_degrees;


}*/














/************************************************************************/
/*                  */
/*      Convert 32 bit to date & time      */
/*      e.g. from RTC chip DS1994      */
/*                  */
/*    Author: Peter Dannegger          */
/*           danni@specs.de          */
/*                  */
/************************************************************************/

// 4294967295 sec = 0136 y + 1 m + 6 d + 6 h + 28 min + 15 sec


/*#define FIRSTYEAR  2000      // start year
#define FIRSTDAY  6      // 0 = Sunday


#ifndef Test
#define code
#define idata
#endif


//typedef unsigned char  u8;
//typedef   signed char  s8;
//typedef unsigned short u16;
//typedef   signed short s16;
//typedef unsigned long  u32;
//typedef   signed long  s32;


struct time {
	u8  second;
	u8  minute;
	u8  hour;
	u8  day;
	u8  month;
	u16 year;
	u8  wday;
};


u8 code DayOfMonth[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

void summertime(struct time idata* t)
{
	u8 hour, day, wday, month;      // locals for faster access

	hour = t->hour;
	day = t->day;
	wday = t->wday;
	month = t->month;

	if (month < 3 || month > 10)      // month 1, 2, 11, 12
		return;          // -> Winter

	if (day - wday >= 25 && (wday || hour >= 2)) { // after last Sunday 2:00
		if (month == 10)        // October -> Winter
			return;
	}
	else {          // before last Sunday 2:00
		if (month == 3)        // March -> Winter
			return;
	}
	hour++;          // add one hour
	if (hour == 24) {        // next day
		hour = 0;
		wday++;          // next weekday
		if (wday == 7)
			wday = 0;
		if (day == DayOfMonth[month - 1]) {    // next month
			day = 0;
			month++;
		}
		day++;
	}
	t->month = month;
	t->hour = hour;
	t->day = day;
	t->wday = wday;
}


void gettime(u32 sec, struct time idata* t)
{
	u16 day;
	u8 year;
	u16 dayofyear;
	u8 leap400;
	u8 month;

	t->second = sec % 60;
	sec /= 60;
	t->minute = sec % 60;
	sec /= 60;
	t->hour = sec % 24;
	day = sec / 24;

	t->wday = (day + FIRSTDAY) % 7;    // weekday

	year = FIRSTYEAR % 100;      // 0..99
	leap400 = 4 - ((FIRSTYEAR - 1) / 100 & 3);  // 4, 3, 2, 1

	for (;;) {
		dayofyear = 365;
		if ((year & 3) == 0) {
			dayofyear = 366;          // leap year
			if (year == 0 || year == 100 || year == 200)  // 100 year exception
				if (--leap400)          // 400 year exception
					dayofyear = 365;
		}
		if (day < dayofyear)
			break;
		day -= dayofyear;
		year++;          // 00..136 / 99..235
	}
	t->year = year + FIRSTYEAR / 100 * 100;  // + century

	if (dayofyear & 1 && day > 58)    // no leap year and after 28.2.
		day++;          // skip 29.2.

	for (month = 1; day >= DayOfMonth[month - 1]; month++)
		day -= DayOfMonth[month - 1];

	t->month = month;        // 1..12
	t->day = day + 1;        // 1..31
}


int main(void)
{
	u32 sec = 0;
	struct time idata current_time;
	struct time idata summer_time;

	gettime(4294967295U, &current_time);
	for (;;) {
		gettime(sec, &current_time);
		summer_time = current_time;
		summertime(&summer_time);
		sec += 3600U;
	}
}*/




/*void display_backlight_brightness(u32 brightness, u32 step_delay)
{
	u32 old_value = (PWM(PWM_CONTROLLER_PWM_CSR_0) >> 16) & 0xFF;
	if (brightness == old_value)
		return;

	if (brightness > 255)
		brightness = 255;

	if (old_value < brightness)
	{
		for (u32 i = old_value; i < brightness + 1; i++)
		{
			PWM(PWM_CONTROLLER_PWM_CSR_0) = (1 << 31) | (i << 16); // Enable PWM
			usleep(step_delay);
		}
	}
	else
	{
		for (u32 i = old_value; i > brightness; i--)
		{
			PWM(PWM_CONTROLLER_PWM_CSR_0) = (1 << 31) | (i << 16); // Enable PWM
			usleep(step_delay);
		}
	}
	if (!brightness)
		PWM(PWM_CONTROLLER_PWM_CSR_0) = 0;
}*/



/*Create a button*/
/*lv_obj_t* btn1 = lv_btn_create(lv_disp_get_scr_act(NULL), NULL);         //Create a button on the currently loaded screen
lv_obj_set_event_cb(btn1, btn_event_cb);                                  //Set function to be called when the button is released
lv_obj_align(btn1, label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);               //Align below the label */

/*Create a label on the button (the 'label' variable can be reused)*/
/*label = lv_label_create(btn1, NULL);
lv_label_set_text(label, "Button 1");*/