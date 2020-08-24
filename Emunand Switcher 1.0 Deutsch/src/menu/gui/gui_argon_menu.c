/*  
 * Copyright (c) 2018 Guillem96
 * Copyright (c) 2019 Storm
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

#include <string.h>
#include "menu/gui/gui_argon_menu.h"
#include "menu/gui/gui_menu.h"
#include "menu/gui/gui_menu_pool.h"

#include "gfx/gfx.h"

#include "utils/types.h"
#include "utils/fs_utils.h"
#include "utils/dirlist.h"
#include "utils/util.h"
#include "utils/touch.h"

#include "core/launcher.h"
#include "core/payloads.h"
#include "core/custom-gui.h"

#include "mem/heap.h"

#define COLUMNS 2
#define ROWS 1
#define ELEM_SIZE 230
#define MARGIN_TOP 110
/* #define MARGIN_LEFT 45 */

 /* String konfig für Text */
int MARGIN_LEFT = 1;
int text_scale = 2;
int y_height = 350;
const char* string = "";

/* static int tool_reboot_rcm(void* param); */
static int tool_power_off(void* param);

/* Neue int für neues Menu */
static int tool_emun1(void* param);
static int tool_emun2(void* param);
static int tool_emun3(void* param);
static int tool_about(void* param);

int tool_infoemuninstalled2();
/* int tool_test(); */

/* test Autostart funktioniert*/
/* Autostart int_ in gui_menu.c dazufügen unter static void gui_menu_draw_entries(gui_menu_t *menu)_else */
int tool_infoemun1();
int tool_infoemun2();
int tool_infoemun3();
int tool_infoemuninstalled();
int tool_theme();



/* Text configuration */
void text_centre() {
	int half_width = 640;
	int font_size = (((text_scale * 8) * strlen(string)) / 2);
	int x_loc = (half_width - font_size);
	gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
	g_gfx_con.scale = text_scale;
	gfx_con_setpos(&g_gfx_con, x_loc, y_height);
}


/* Generate entries dynamically, entfernt keine Warnung mehr beim builden und funktioniert */
/* static void generate_payloads_entries(char* payloads, gui_menu_t* menu)
{
    if (payloads == NULL)
    {
        g_gfx_con.scale = 4;
        gfx_con_setpos(&g_gfx_con, 140, 250);
        gfx_printf(&g_gfx_con, "Payloads directory is empty...\n");
        
        g_gfx_con.scale = 3;
        gfx_con_setpos(&g_gfx_con, 110, 370);
        gfx_printf(&g_gfx_con, "Place your payloads inside \"%s\"", PAYLOADS_DIR);

        return;
    }

    u32 i = 0; */
    /* For each payload generate its logo, its name and its path */
    /* while(payloads[i * 256])
    {
        char* payload_path = (char*)malloc(256);
        payload_full_path(&payloads[i * 256], payload_path);
        
        char payload_logo[256];
        payload_logo_path(&payloads[i * 256], payload_logo);

        u32 row = i / COLUMNS;
        u32 col = i % COLUMNS;
        u32 x = g_gfx_ctxt.width / COLUMNS * col + MARGIN_LEFT;
        u32 y = g_gfx_ctxt.height / ROWS * row + MARGIN_TOP + (row == 0 ? 30 : -60);

        const char* payload_wo_bin = str_replace(&payloads[i * 256], ".bin", "");
        gui_menu_append_entry(menu, 
            gui_create_menu_entry(payload_wo_bin,
                                    sd_file_read(payload_logo), 
                                    x, y,
                                    200, 200,
                                    (int (*)(void *))launch_payload, (void*)payload_path));
        i++;
    }
} */


/* Init needed menus for ArgonNX */
void gui_init_argon_menu(void)
{
	
    /* Init pool for menu */
    gui_menu_pool_init();

    gui_menu_t* menu = gui_menu_create("ArgonNX");

	/* deaktiviert Payload Menü, aber gibt Warnung beim builden, definied but not used, funktioniert aber problemlos*/
    /* generate_payloads_entries(dirlist(PAYLOADS_DIR, "*.bin", false), menu); */

	 /* Einträge Menu */

	 gui_menu_append_entry(menu,
	 gui_create_menu_entry("SXOS starten...", sd_file_read("emunandswitcher/logos/sxos.bmp"), 540, 110, 200, 200, (int (*)(void*))launch_payload, (void*)"emunandswitcher/payloads/sxos.bin"));

	 gui_menu_append_entry(menu,
	 gui_create_menu_entry("Emunand 1", sd_file_read("emunandswitcher/logos/emunand1.bmp"), 110, 410, 200, 200, (int (*)(void*))tool_emun1, NULL));

	 gui_menu_append_entry(menu,
	 gui_create_menu_entry("Emunand 2", sd_file_read("emunandswitcher/logos/emunand2.bmp"), 540, 410, 200, 200, (int (*)(void*))tool_emun2, NULL));

	 gui_menu_append_entry(menu,
	gui_create_menu_entry_text("Emunand 3", sd_file_read("emunandswitcher/logos/emunand3.bmp"), 970, 410, 200, 200, (int (*)(void*))tool_emun3, NULL));

	/* gui_menu_append_entry(menu,
	gui_create_menu_entry_no_bitmap("about", 300, 680, 150, 100, tool_about, NULL)); */

	gui_menu_append_entry(menu,
	gui_create_menu_entry("Info", sd_file_read("emunandswitcher/logos/about.bmp"), 1150, 0, 100, 100, (int (*)(void*))tool_about, NULL));

	/* gui_menu_append_entry(menu,
	gui_create_menu_entry_no_bitmap("Screenshot", 700, 680, 150, 100, (int (*)(void*))screenshot, NULL)); */

	gui_menu_append_entry(menu,
	gui_create_menu_entry("Screenshot", sd_file_read("emunandswitcher/logos/screenshot.bmp"), 1020, 0, 100, 100, (int (*)(void*))screenshot, NULL));

	/* gui_menu_append_entry(menu,
	gui_create_menu_entry_no_bitmap("Info", 440, 680, 150, 100, tool_infoemun, NULL)); */

	/* gui_menu_append_entry(menu,
gui_create_menu_entry_no_bitmap("Test", 440, 680, 150, 100, tool_test, NULL)); */

	/* gui_menu_append_entry(menu,
	gui_create_menu_entry_no_bitmap("Welcher Emunand", 100, 680, 150, 100, tool_infoemuninstalled, NULL)); */

    /* Generate reboot rcm and shutdown entry */

   /* gui_menu_append_entry(menu, 
            gui_create_menu_entry_no_bitmap("Power off", 1100, 680, 150, 100, tool_power_off, NULL)); */

	gui_menu_append_entry(menu,
		gui_create_menu_entry("Power Off", sd_file_read("emunandswitcher/logos/power.bmp"), 30, 0, 100, 100, (int (*)(void*))tool_power_off, NULL));


  /*   gui_menu_append_entry(menu, 
            gui_create_menu_entry_no_bitmap("Reboot RCM", 1100, 680, 150, 100, tool_reboot_rcm, NULL)); */



    /* Start menu */
    gui_menu_open(menu);

    /* Clear all entries and menus */
    gui_menu_pool_cleanup();
	
}


static int tool_about(void* param)
{
	g_gfx_con.scale = 2;
	gfx_con_setpos(&g_gfx_con, 750, 160);
	gfx_printf(&g_gfx_con, "Emunandswitcher Version 1.00");
	gfx_con_setpos(&g_gfx_con, 750, 180);
	gfx_printf(&g_gfx_con, "made by Storm 2019");
	gfx_con_setpos(&g_gfx_con, 750, 200);
	gfx_printf(&g_gfx_con, "basiert auf argonNX und hekate");
	gfx_con_setpos(&g_gfx_con, 750, 220);
	gfx_printf(&g_gfx_con, "Icon templates von mrdude");
	gfx_con_setpos(&g_gfx_con, 750, 240);
	gfx_printf(&g_gfx_con, "Danke an die Programmierer!");
	gfx_con_setpos(&g_gfx_con, 750, 260);
	gfx_printf(&g_gfx_con, "Danke Muxi fuer den Betatest!");

	return 0;

}


/* Info Tool Theme prüfung*/
int tool_theme()
{
	if (f_stat("sxos/titles/0100000000001000", NULL)) {
		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 10, 160);
		gfx_printf(&g_gfx_con, "%kOK kein Theme installiert!%k", GREEN, WHITE);

	}

	else { // if the directory exists - do this.
		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 10, 160);
		gfx_printf(&g_gfx_con, "%kAchtung Theme installiert!%k", colour_red, WHITE); //erste rot, rest weiss
		gfx_con_setpos(&g_gfx_con, 10, 180);
		gfx_printf(&g_gfx_con, "Wird entfernt beim installieren");
		gfx_con_setpos(&g_gfx_con, 10, 200);
		gfx_printf(&g_gfx_con, "eines Emunandes, um den Start-");
		gfx_con_setpos(&g_gfx_con, 10, 220);
		gfx_printf(&g_gfx_con, "error zu vermeiden...");
		/* gfx_con_setpos(&g_gfx_con, 10, 240);
		gfx_printf(&g_gfx_con, "Theme Ordner wird umbenannt in");
		gfx_con_setpos(&g_gfx_con, 10, 260);
		gfx_printf(&g_gfx_con, "%k0100000000001000 theme temp%k", colour_red, WHITE); */
	
		return 0;
	}
	
	return 0;
}


/* Info Tool Text Emunand 1 aus ini lesen */
int tool_infoemun1()
{
#define MAXCHAR 100
	FIL fp;
	int line = 0;
	TCHAR info[MAXCHAR];
	f_open(&fp, "emunandswitcher/config.ini", FA_READ);
	while (f_gets(info, MAXCHAR, &fp)) {
		line++;
		TCHAR* stelle;
		TCHAR* auswahl;
		auswahl = strstr(info, "Emunand 1 =");
		stelle = strchr(auswahl, (int)'=');
		string = str_replace(stelle, "= ", "");
		/*Text zentriert an Position half_width und Höhe 630*/
		int half_width = 220;
		int font_size = (((text_scale * 8) * strlen(string)) / 2);
		int x_loc = (half_width - font_size);
		gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
		g_gfx_con.scale = text_scale;
		gfx_con_setpos(&g_gfx_con, x_loc, 655);
		gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);



	}
	f_close(&fp);
	return 0;
}

/* Info Tool Text Emunand 2 aus ini lesen */
int tool_infoemun2()
{
#define MAXCHAR 100
	FIL fp;
	int line = 0;
	TCHAR info[MAXCHAR];
	f_open(&fp, "emunandswitcher/config.ini", FA_READ);
	while (f_gets(info, MAXCHAR, &fp)) {
		line++;
		TCHAR* stelle;
		TCHAR* auswahl;
		auswahl = strstr(info, "Emunand 2 =");
		stelle = strchr(auswahl, (int)'=');
		string = str_replace(stelle, "= ", "");
		/*Text zentriert an Position half_width und Höhe 650*/
		int half_width = 650;
		int font_size = (((text_scale * 8) * strlen(string)) / 2);
		int x_loc = (half_width - font_size);
		gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
		g_gfx_con.scale = text_scale;
		gfx_con_setpos(&g_gfx_con, x_loc, 655);
		gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
	}
	f_close(&fp);
	return 0;
}

/* Info Tool Text Emunand 3 aus ini lesen */
int tool_infoemun3()
{
#define MAXCHAR 100
	FIL fp;
	int line = 0;
	TCHAR info[MAXCHAR];
	f_open(&fp, "emunandswitcher/config.ini", FA_READ);
	while (f_gets(info, MAXCHAR, &fp)) {
		line++;
		TCHAR* stelle;
		TCHAR* auswahl;
		auswahl = strstr(info, "Emunand 3 =");
		stelle = strchr(auswahl, (int)'=');
		string = str_replace(stelle, "= ", "");
		/*Text zentriert an Position half_width und Höhe 630*/
		int half_width = 1080;
		int font_size = (((text_scale * 8) * strlen(string)) / 2);
		int x_loc = (half_width - font_size);
		gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
		g_gfx_con.scale = text_scale;
		gfx_con_setpos(&g_gfx_con, x_loc, 655);
		gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
	}
	f_close(&fp);
	return 0;
}

/*diveses Test*/
/* {
	if (f_stat("sxos/titles/0100000000001000", NULL)) {

	}

	else { // if the directory exists - do this.
		f_unlink("sxos/titles/0100000000001000");
		f_rename("sxos/titles/0100000000001000", "sxos/titles/0100000000001000 theme temp");
		gfx_con_setpos(&g_gfx_con, 750, 280);
		gfx_printf(&g_gfx_con, "Theme Ordner geloescht...");

	}
	return 0;
} 
 string = "____________________________";
text_centre();
gfx_printf(&g_gfx_con, "%k%s%k", BLACK, string, WHITE);
tool_infoemuninstalled();
 TRANSPARENT_COLOR

msleep(1500); //give some time to read the above text (1.5 seconds)

gui_init_argon_menu();

	return 0;

} */



/* int tool_test()
{
	f_rename("test/titles/0100000000001000", "test/0100000000001000 theme temp");

	return 0; 
} */

 int tool_infoemuninstalled2()
 {
	 /* const char* del = "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"; */
	 /*Text zentriert an Position half_width und Höhe 630*/
	 /* int half_width = 640;
	 int font_size = (((text_scale * 8) * strlen(del)) / 2);
	 int x_loc = (half_width - font_size);
	 gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
	 g_gfx_con.scale = text_scale;
	 gfx_con_setpos(&g_gfx_con, x_loc, 350);
	 gfx_printf(&g_gfx_con, "%k%s%k", WHITE, del, WHITE); */

	 if (f_stat("emunandswitcher/folders/emutmp00", NULL)) {
#define MAXCHAR 100
		 FIL fp;
		 int line = 0;
		 TCHAR info[MAXCHAR];
		 f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		 while (f_gets(info, MAXCHAR, &fp)) {
			 line++;
			 TCHAR* stelle;
			 TCHAR* auswahl;
			 /* char* ende; */
			 auswahl = strstr(info, "Emunand 1 =");
			 stelle = strchr(auswahl, (int)'=');
			 string = str_replace(stelle, "= ", "");




			 /*Text zentriert an Position half_width und Höhe 630*/
			 int half_width = 640;
			 int font_size = (((text_scale * 8) * strlen(string)) / 2);
			 int x_loc = (half_width - font_size);
			 gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
			 g_gfx_con.scale = text_scale;
			 gfx_con_setpos(&g_gfx_con, x_loc, 370);
			 gfx_printf(&g_gfx_con, "%k%s%k", GREEN, string, WHITE);



			 /* text_centre();
			 gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE); */

			 /* char string[MAXCHAR];
			 // init str to all spaces, NULL terminated with character as '\r'
			 strcpy(string, ende);       // copy my_string into str
			 string[strlen(ende)] = ' '; // erase null termination char
			 string[MAXCHAR - 1] = '\r'; */






			 /*	gfx_con_setpos(&g_gfx_con, 640, 350);
				 gfx_printf(&g_gfx_con, "                                                     "); */

				 /* g_gfx_con.scale = 2;
				 gfx_con_setpos(&g_gfx_con, 640, 350);
				 gfx_printf(&g_gfx_con, "                             "); */



				 /* gfx_printf(&g_gfx_con, "%k\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%k"); */




		 }
		 f_close(&fp);
		 return 0;
	 }
	 else { // if the directory exists - do this.

	 }

	 if (f_stat("emunandswitcher/folders/emutmp01", NULL)) {
#define MAXCHAR 100
		 FIL fp;
		 int line = 0;
		 TCHAR info[MAXCHAR];
		 f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		 while (f_gets(info, MAXCHAR, &fp)) {
			 line++;
			 TCHAR* stelle;
			 TCHAR* auswahl;
			 auswahl = strstr(info, "Emunand 2 =");
			 stelle = strchr(auswahl, (int)'=');
			 string = str_replace(stelle, "= ", "");
			 /*Text zentriert an Position half_width und Höhe 630*/
			 int half_width = 640;
			 int font_size = (((text_scale * 8) * strlen(string)) / 2);
			 int x_loc = (half_width - font_size);
			 gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
			 g_gfx_con.scale = text_scale;
			 gfx_con_setpos(&g_gfx_con, x_loc, 370);
			 gfx_printf(&g_gfx_con, "%k%s%k", GREEN, string, WHITE);

			 /* text_centre();
			 gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE); */
		 }
		 f_close(&fp);
		 return 0;
	 }
	 else { // if the directory exists - do this.

	 }

	 if (f_stat("emunandswitcher/folders/emutmp02", NULL)) {

#define MAXCHAR 100
		 FIL fp;
		 int line = 0;
		 TCHAR info[MAXCHAR];
		 f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		 while (f_gets(info, MAXCHAR, &fp)) {
			 line++;
			 TCHAR* stelle;
			 TCHAR* auswahl;
			 auswahl = strstr(info, "Emunand 3 =");
			 stelle = strchr(auswahl, (int)'=');
			 string = str_replace(stelle, "= ", "");
			 /*Text zentriert an Position half_width und Höhe 630*/
			 int half_width = 640;
			 int font_size = (((text_scale * 8) * strlen(string)) / 2);
			 int x_loc = (half_width - font_size);
			 gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
			 g_gfx_con.scale = text_scale;
			 gfx_con_setpos(&g_gfx_con, x_loc, 370);
			 gfx_printf(&g_gfx_con, "%k%s%k", GREEN, string, WHITE);

			 /* text_centre();
			 gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE); */

		 }
		 f_close(&fp);
		 return 0;
	 }
	 else { // if the directory exists - do this.

	 }

	 return 0;
 }



int tool_infoemuninstalled()
{

	if (f_stat("emunandswitcher/folders/emutmp00", NULL)) {
#define MAXCHAR 100
		FIL fp;
		int line = 0;
		TCHAR info[MAXCHAR];
		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			line++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emunand 1 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");
			/*Text zentriert an Position half_width und Höhe 350*/
			int half_width = 640;
			int font_size = (((text_scale * 8) * strlen(string)) / 2);
			int x_loc = (half_width - font_size);
			gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
			g_gfx_con.scale = text_scale;
			gfx_con_setpos(&g_gfx_con, x_loc, 350);
			gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
		}
		f_close(&fp);
		return 0;
	}
	else { // if the directory exists - do this.

	}

	if (f_stat("emunandswitcher/folders/emutmp01", NULL)) {
#define MAXCHAR 100
		FIL fp;
		int line = 0;
		TCHAR info[MAXCHAR];
		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			line++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emunand 2 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");
			/*Text zentriert an Position half_width und Höhe 350*/
			int half_width = 640;
			int font_size = (((text_scale * 8) * strlen(string)) / 2);
			int x_loc = (half_width - font_size);
			gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
			g_gfx_con.scale = text_scale;
			gfx_con_setpos(&g_gfx_con, x_loc, 350);
			gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
		}
		f_close(&fp);
		return 0;
	}
	else { // if the directory exists - do this.

	}

	if (f_stat("emunandswitcher/folders/emutmp02", NULL)) {
		
		#define MAXCHAR 100
		FIL fp;
		int line = 0;
		TCHAR info[MAXCHAR];
		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			line++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emunand 3 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");
			/*Text zentriert an Position half_width und Höhe 350*/
			int half_width = 640;
			int font_size = (((text_scale * 8) * strlen(string)) / 2);
			int x_loc = (half_width - font_size);
			gfx_con_init(&g_gfx_con, &g_gfx_ctxt);
			g_gfx_con.scale = text_scale;
			gfx_con_setpos(&g_gfx_con, x_loc, 350);
			gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
		}
		f_close(&fp);
		return 0;
	}
	else { // if the directory exists - do this.

	}

	return 0;
}




/* Menu Emunand 1 bis 3, mit prüfung Verzeichnis und Dateien, Anzeige welcher Emunand unter SXOX starten...*/
static int tool_emun1(void* param)
{
	

	if (f_stat("sxos/emunand", NULL)) {
		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Kein Emunand installiert!");
		return 0;
	}

	if (f_stat("emunandswitcher/folders/emutmp00", NULL)) {
		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Emunand 1 schon installiert!");



		/*Zeigt installierter Emunand unter SXOS starten... zentriert*/
		/* #define MAXCHAR 100
		FIL fp;
		int line = 0;
		TCHAR info[MAXCHAR];
		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			line++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emunand 1 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");
			text_centre();
			gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
		}
		f_close(&fp); */

		return 0;
	}
	else { // if the directory exists - do this.

	}

	if (f_stat("emunandswitcher/folders/emutmp01", NULL)) {

	}
	else { // if the directory exists - do this.

		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Suche Dateien!");

		if (f_stat("emunandswitcher/folders/emutmp00/boot0.bin", NULL)) {
			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin nicht vorhanden!");
			return 0;
		}
		else { // if the file exists - do this.

			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin vorhanden!");

			if (f_stat("emunandswitcher/folders/emutmp00/boot1.bin", NULL)) {
				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin nicht vorhanden!");
				return 0;
			}
			else { // if the file exists - do this.

				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin vorhanden!");

				if (f_stat("emunandswitcher/folders/emutmp00/full.00.bin", NULL)) {
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin nicht vorhanden!");
					return 0;
				}

				else { // if the file exists - do this.

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin vorhanden!");

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 240);
					gfx_printf(&g_gfx_con, "Verschiebe Emunand!");
					f_rename("sxos/emunand", "emunandswitcher/folders/emutmp02");
					f_rename("emunandswitcher/folders/emutmp00", "sxos/emunand");
					f_rename("Emutendo", "emunandswitcher/folders/emutendo02");
					f_rename("emunandswitcher/folders/emutendo00", "Emutendo");
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 260);
					gfx_printf(&g_gfx_con, "Emunand erfolgreich verschoben...");
					
					/* tool_infoemuninstalled(); */
					tool_infoemuninstalled2();

					if (f_stat("sxos/titles/0100000000001000", NULL)) {

					}

					else { // if the directory exists - do this.

						f_unlink("sxos/titles/0100000000001000");
						f_unlink("sxos/titles/0100000000001007");
						f_unlink("sxos/titles/0100000000001013");
						/* f_rename("sxos/titles/0100000000001000", "sxos/titles/0100000000001000 theme temp"); */
						gfx_con_setpos(&g_gfx_con, 750, 280);
						gfx_printf(&g_gfx_con, "Theme Ordner entfernt...");

					}

					

					/*Zeigt installierter Emunand unter SXOS starten... zentriert*/
					/*#define MAXCHAR 100
					FIL fp;
					int line = 0;
					TCHAR info[MAXCHAR];
					f_open(&fp, "emunandswitcher/config.ini", FA_READ);
					while (f_gets(info, MAXCHAR, &fp)) {
						line++;
						TCHAR* stelle;
						TCHAR* auswahl;
						auswahl = strstr(info, "Emunand 1 =");
						stelle = strchr(auswahl, (int)'=');
						string = str_replace(stelle, "= ", "");
						text_centre();
						gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
					}
					f_close(&fp); */

					return 0;
				}
			}
		}


	}
	

	if (f_stat("emunandswitcher/folders/emutmp02", NULL)) {

	}
	else { // if the directory exists - do this.

		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Suche Dateien!");

		if (f_stat("emunandswitcher/folders/emutmp00/boot0.bin", NULL)) {
			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin nicht vorhanden!");
			return 0;
		}
		else { // if the file exists - do this.

			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin vorhanden!");

			if (f_stat("emunandswitcher/folders/emutmp00/boot1.bin", NULL)) {
				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin nicht vorhanden!");
				return 0;
			}
			else { // if the file exists - do this.

				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin vorhanden!");

				if (f_stat("emunandswitcher/folders/emutmp00/full.00.bin", NULL)) {
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin nicht vorhanden!");
					return 0;
				}

				else { // if the file exists - do this.

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin vorhanden!");

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 240);
					gfx_printf(&g_gfx_con, "Verschiebe Emunand!");
					f_rename("sxos/emunand", "emunandswitcher/folders/emutmp01");
					f_rename("emunandswitcher/folders/emutmp00", "sxos/emunand");
					f_rename("Emutendo", "emunandswitcher/folders/emutendo01");
					f_rename("emunandswitcher/folders/emutendo00", "Emutendo");
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 260);
					gfx_printf(&g_gfx_con, "Emunand erfolgreich verschoben...");

					tool_infoemuninstalled2();
					/* tool_infoemuninstalled(); */

					if (f_stat("sxos/titles/0100000000001000", NULL)) {

					}

					else { // if the directory exists - do this.

						f_unlink("sxos/titles/0100000000001000");
						f_unlink("sxos/titles/0100000000001007");
						f_unlink("sxos/titles/0100000000001013");
						/* f_rename("sxos/titles/0100000000001000", "sxos/titles/0100000000001000 theme temp"); */
						gfx_con_setpos(&g_gfx_con, 750, 280);
						gfx_printf(&g_gfx_con, "Theme Ordner entfernt...");

					}
					
			

					/*Zeigt installierter Emunand unter SXOS starten... zentriert*/
					/* #define MAXCHAR 100
					FIL fp;
					int line = 0;
					TCHAR info[MAXCHAR];
					f_open(&fp, "emunandswitcher/config.ini", FA_READ);
					while (f_gets(info, MAXCHAR, &fp)) {
						line++;
						TCHAR* stelle;
						TCHAR* auswahl;
						auswahl = strstr(info, "Emunand 1 =");
						stelle = strchr(auswahl, (int)'=');
						string = str_replace(stelle, "= ", "");
						text_centre();
						gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
					}
					f_close(&fp); */

					return 0;
				}
			}
		}
	}

	return 0;
}


static int tool_emun2(void* param)
{
	

	if (f_stat("sxos/emunand", NULL)) {
		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Kein Emunand installiert!");
		return 0;
	}

	if (f_stat("emunandswitcher/folders/emutmp01", NULL)) {
		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Emunand 2 schon installiert!");

		/*Zeigt installierter Emunand unter SXOS starten... zentriert*/
		/* #define MAXCHAR 100
		FIL fp;
		int line = 0;
		TCHAR info[MAXCHAR];
		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			line++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emunand 2 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");
			text_centre();
			gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
		}
		f_close(&fp); */

		return 0;
	}
	else { // if the directory exists - do this.

	}

	if (f_stat("emunandswitcher/folders/emutmp00", NULL)) {

	}
	
	else { // if the directory exists - do this.

		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Suche Dateien!");

		if (f_stat("emunandswitcher/folders/emutmp01/boot0.bin", NULL)) {
			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin nicht vorhanden!");
			return 0;
		}
		else { // if the file exists - do this.

			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin vorhanden!");

			if (f_stat("emunandswitcher/folders/emutmp01/boot1.bin", NULL)) {
				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin nicht vorhanden!");
				return 0;
			}
			else { // if the file exists - do this.

				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin vorhanden!");

				if (f_stat("emunandswitcher/folders/emutmp01/full.00.bin", NULL)) {
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin nicht vorhanden!");
					return 0;
				}

				else { // if the file exists - do this.

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin vorhanden!");

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 240);
					gfx_printf(&g_gfx_con, "Verschiebe Emunand!");
					f_rename("sxos/emunand", "emunandswitcher/folders/emutmp02");
					f_rename("emunandswitcher/folders/emutmp01", "sxos/emunand");
					f_rename("Emutendo", "emunandswitcher/folders/emutendo02");
					f_rename("emunandswitcher/folders/emutendo01", "Emutendo");
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 260);
					gfx_printf(&g_gfx_con, "Emunand erfolgreich verschoben...");

					tool_infoemuninstalled2();
					/* tool_infoemuninstalled(); */

					if (f_stat("sxos/titles/0100000000001000", NULL)) {

					}

					else { // if the directory exists - do this.

						f_unlink("sxos/titles/0100000000001000");
						f_unlink("sxos/titles/0100000000001007");
						f_unlink("sxos/titles/0100000000001013");
						/* f_rename("sxos/titles/0100000000001000", "sxos/titles/0100000000001000 theme temp"); */
						gfx_con_setpos(&g_gfx_con, 750, 280);
						gfx_printf(&g_gfx_con, "Theme Ordner entfernt...");

					}

					/*Zeigt installierter Emunand unter SXOS starten... zentriert*/
					/* #define MAXCHAR 100
					FIL fp;
					int line = 0;
					TCHAR info[MAXCHAR];
					f_open(&fp, "emunandswitcher/config.ini", FA_READ);
					while (f_gets(info, MAXCHAR, &fp)) {
						line++;
						TCHAR* stelle;
						TCHAR* auswahl;
						auswahl = strstr(info, "Emunand 2 =");
						stelle = strchr(auswahl, (int)'=');
						string = str_replace(stelle, "= ", "");
						text_centre();
						gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
					}
					f_close(&fp); */

					return 0;
				}
			}
		}
	}

	if (f_stat("emunandswitcher/folders/emutmp02", NULL)) {

	}
	else { // if the directory exists - do this.

		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Suche Dateien!");

		if (f_stat("emunandswitcher/folders/emutmp01/boot0.bin", NULL)) {
			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin nicht vorhanden!");
			return 0;
		}
		else { // if the file exists - do this.

			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin vorhanden!");

			if (f_stat("emunandswitcher/folders/emutmp01/boot1.bin", NULL)) {
				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin nicht vorhanden!");
				return 0;
			}
			else { // if the file exists - do this.

				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin vorhanden!");

				if (f_stat("emunandswitcher/folders/emutmp01/full.00.bin", NULL)) {
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin nicht vorhanden!");
					return 0;
				}

				else { // if the file exists - do this.

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin vorhanden!");

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 240);
					gfx_printf(&g_gfx_con, "Verschiebe Emunand!");
					f_rename("sxos/emunand", "emunandswitcher/folders/emutmp00");
					f_rename("emunandswitcher/folders/emutmp01", "sxos/emunand");
					f_rename("Emutendo", "emunandswitcher/folders/emutendo00");
					f_rename("emunandswitcher/folders/emutendo01", "Emutendo");
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 260);
					gfx_printf(&g_gfx_con, "Emunand erfolgreich verschoben...");

					tool_infoemuninstalled2();
					/* tool_infoemuninstalled(); */

					if (f_stat("sxos/titles/0100000000001000", NULL)) {

					}

					else { // if the directory exists - do this.

						f_unlink("sxos/titles/0100000000001000");
						f_unlink("sxos/titles/0100000000001007");
						f_unlink("sxos/titles/0100000000001013");
						/* f_rename("sxos/titles/0100000000001000", "sxos/titles/0100000000001000 theme temp"); */
						gfx_con_setpos(&g_gfx_con, 750, 280);
						gfx_printf(&g_gfx_con, "Theme Ordner entfernt...");

					}

					/*Zeigt installierter Emunand unter SXOS starten... zentriert*/
					/* #define MAXCHAR 100
					FIL fp;
					int line = 0;
					TCHAR info[MAXCHAR];
					f_open(&fp, "emunandswitcher/config.ini", FA_READ);
					while (f_gets(info, MAXCHAR, &fp)) {
						line++;
						TCHAR* stelle;
						TCHAR* auswahl;
						auswahl = strstr(info, "Emunand 2 =");
						stelle = strchr(auswahl, (int)'=');
						string = str_replace(stelle, "= ", "");
						text_centre();
						gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
					}
					f_close(&fp); */

					return 0;
				}
			}
		}
	}

	return 0;
}


static int tool_emun3(void* param)
{
	

	if (f_stat("sxos/emunand", NULL)) {
		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Kein Emunand installiert!");
		return 0;
	}

	if (f_stat("emunandswitcher/folders/emutmp02", NULL)) {
		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Emunand 3 schon installiert!");

		/*Zeigt installierter Emunand unter SXOS starten... zentriert*/
		/* #define MAXCHAR 100
		FIL fp;
		int line = 0;
		TCHAR info[MAXCHAR];
		f_open(&fp, "emunandswitcher/config.ini", FA_READ);
		while (f_gets(info, MAXCHAR, &fp)) {
			line++;
			TCHAR* stelle;
			TCHAR* auswahl;
			auswahl = strstr(info, "Emunand 3 =");
			stelle = strchr(auswahl, (int)'=');
			string = str_replace(stelle, "= ", "");
			text_centre();
			gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
		}
		f_close(&fp); */

		return 0;
	}
	else { // if the directory exists - do this.

	}

	if (f_stat("emunandswitcher/folders/emutmp00", NULL)) {

	}
	else { // if the directory exists - do this.

		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Suche Dateien!");

		if (f_stat("emunandswitcher/folders/emutmp02/boot0.bin", NULL)) {
			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin nicht vorhanden!");
			return 0;
		}
		else { // if the file exists - do this.

			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin vorhanden!");

			if (f_stat("emunandswitcher/folders/emutmp02/boot1.bin", NULL)) {
				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin nicht vorhanden!");
				return 0;
			}
			else { // if the file exists - do this.

				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin vorhanden!");

				if (f_stat("emunandswitcher/folders/emutmp02/full.00.bin", NULL)) {
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin nicht vorhanden!");
					return 0;
				}

				else { // if the file exists - do this.

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin vorhanden!");

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 240);
					gfx_printf(&g_gfx_con, "Verschiebe Emunand!");
					f_rename("sxos/emunand", "emunandswitcher/folders/emutmp01");
					f_rename("emunandswitcher/folders/emutmp02", "sxos/emunand");
					f_rename("Emutendo", "emunandswitcher/folders/emutendo01");
					f_rename("emunandswitcher/folders/emutendo02", "Emutendo");
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 260);
					gfx_printf(&g_gfx_con, "Emunand erfolgreich verschoben...");

					tool_infoemuninstalled2();
					/* tool_infoemuninstalled(); */

					if (f_stat("sxos/titles/0100000000001000", NULL)) {

					}

					else { // if the directory exists - do this.

						f_unlink("sxos/titles/0100000000001000");
						f_unlink("sxos/titles/0100000000001007");
						f_unlink("sxos/titles/0100000000001013");
						/* f_rename("sxos/titles/0100000000001000", "sxos/titles/0100000000001000 theme temp"); */
						gfx_con_setpos(&g_gfx_con, 750, 280);
						gfx_printf(&g_gfx_con, "Theme Ordner entfernt...");

					}

					/*Zeigt installierter Emunand unter SXOS starten... zentriert*/
					/* #define MAXCHAR 100
					FIL fp;
					int line = 0;
					TCHAR info[MAXCHAR];
					f_open(&fp, "emunandswitcher/config.ini", FA_READ);
					while (f_gets(info, MAXCHAR, &fp)) {
						line++;
						TCHAR* stelle;
						TCHAR* auswahl;
						auswahl = strstr(info, "Emunand 3 =");
						stelle = strchr(auswahl, (int)'=');
						string = str_replace(stelle, "= ", "");
						text_centre();
						gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
					}
					f_close(&fp); */

					return 0;
				}
			}
		}
	}

	if (f_stat("emunandswitcher/folders/emutmp01", NULL)) {

	}
	else { // if the directory exists - do this.

		g_gfx_con.scale = 2;
		gfx_con_setpos(&g_gfx_con, 750, 160);
		gfx_printf(&g_gfx_con, "Suche Dateien!");

		if (f_stat("emunandswitcher/folders/emutmp02/boot0.bin", NULL)) {
			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin nicht vorhanden!");
			return 0;
		}
		else { // if the file exists - do this.

			g_gfx_con.scale = 2;
			gfx_con_setpos(&g_gfx_con, 750, 180);
			gfx_printf(&g_gfx_con, "boot0.bin vorhanden!");

			if (f_stat("emunandswitcher/folders/emutmp02/boot1.bin", NULL)) {
				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin nicht vorhanden!");
				return 0;
			}
			else { // if the file exists - do this.

				g_gfx_con.scale = 2;
				gfx_con_setpos(&g_gfx_con, 750, 200);
				gfx_printf(&g_gfx_con, "boot1.bin vorhanden!");

				if (f_stat("emunandswitcher/folders/emutmp02/full.00.bin", NULL)) {
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin nicht vorhanden!");
					return 0;
				}

				else { // if the file exists - do this.

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 220);
					gfx_printf(&g_gfx_con, "full.00.bin vorhanden!");

					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 240);
					gfx_printf(&g_gfx_con, "Verschiebe Emunand!");
					f_rename("sxos/emunand", "emunandswitcher/folders/emutmp00");
					f_rename("emunandswitcher/folders/emutmp02", "sxos/emunand");
					f_rename("Emutendo", "emunandswitcher/folders/emutendo00");
					f_rename("emunandswitcher/folders/emutendo02", "Emutendo");
					g_gfx_con.scale = 2;
					gfx_con_setpos(&g_gfx_con, 750, 260);
					gfx_printf(&g_gfx_con, "Emunand erfolgreich verschoben...");

					tool_infoemuninstalled2();
					/* tool_infoemuninstalled(); */
					
					if (f_stat("sxos/titles/0100000000001000", NULL)) {

					}

					else { // if the directory exists - do this.

						f_unlink("sxos/titles/0100000000001000");
						f_unlink("sxos/titles/0100000000001007");
						f_unlink("sxos/titles/0100000000001013");
						/* f_rename("sxos/titles/0100000000001000", "sxos/titles/0100000000001000 theme temp"); */
						gfx_con_setpos(&g_gfx_con, 750, 280);
						gfx_printf(&g_gfx_con, "Theme Ordner entfernt...");

					}

					/*Zeigt installierter Emunand unter SXOS starten... zentriert*/
					/* #define MAXCHAR 100
					FIL fp;
					int line = 0;
					TCHAR info[MAXCHAR];
					f_open(&fp, "emunandswitcher/config.ini", FA_READ);
					while (f_gets(info, MAXCHAR, &fp)) {
						line++;
						TCHAR* stelle;
						TCHAR* auswahl;
						auswahl = strstr(info, "Emunand 3 =");
						stelle = strchr(auswahl, (int)'=');
						string = str_replace(stelle, "= ", "");
						text_centre();
						gfx_printf(&g_gfx_con, "%k%s%k", WHITE, string, WHITE);
					}
					f_close(&fp); */

					return 0;
				}
			}
		}
	}

	return 0;
}



/* static int tool_reboot_rcm(void* param)
{
    gui_menu_pool_cleanup();
    reboot_rcm();
    return 0;
} */

static int tool_power_off(void* param)
{
    gui_menu_pool_cleanup();
    power_off();
    return 0;
}