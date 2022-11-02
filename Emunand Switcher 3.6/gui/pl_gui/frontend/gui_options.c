/*
 * Copyright (c) 2018-2021 CTCaer
 * Copyright (c) 2019-2022 Storm21
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

#include <stdlib.h>

#include <bdk.h>

#include "gui.h"
#include "../config.h"
#include <libs/lvgl/lvgl.h>

extern hekate_config h_cfg;
extern nyx_config n_cfg;

static lv_obj_t *autoboot_btn;
static bool autoboot_first_time = true;

static bool ini_changes_made = false;
static bool nyx_changes_made = false;

void nyx_options_clear_ini_changes_made()
{
	ini_changes_made = false;
}

bool nyx_options_get_ini_changes_made()
{
	return ini_changes_made;
}

static lv_res_t auto_hos_poweroff_toggle(lv_obj_t *btn)
{
	h_cfg.autohosoff = !h_cfg.autohosoff;
	ini_changes_made = true;

	if (!h_cfg.autohosoff)
		lv_btn_set_state(btn, LV_BTN_STATE_REL);
	else
		lv_btn_set_state(btn, LV_BTN_STATE_TGL_REL);

	nyx_generic_onoff_toggle(btn);

	return LV_RES_OK;
}

static lv_res_t auto_nogc_toggle(lv_obj_t *btn)
{
	h_cfg.autonogc = !h_cfg.autonogc;
	ini_changes_made = true;

	if (!h_cfg.autonogc)
		lv_btn_set_state(btn, LV_BTN_STATE_REL);
	else
		lv_btn_set_state(btn, LV_BTN_STATE_TGL_REL);

	nyx_generic_onoff_toggle(btn);

	return LV_RES_OK;
}

static lv_res_t _update_r2p_action(lv_obj_t *btn)
{
	h_cfg.updater2p = !h_cfg.updater2p;
	ini_changes_made = true;

	if (!h_cfg.updater2p)
		lv_btn_set_state(btn, LV_BTN_STATE_REL);
	else
		lv_btn_set_state(btn, LV_BTN_STATE_TGL_REL);

	nyx_generic_onoff_toggle(btn);

	return LV_RES_OK;
}

static lv_res_t _win_autoboot_close_action(lv_obj_t * btn)
{
	if (!h_cfg.autoboot)
		lv_btn_set_state(autoboot_btn, LV_BTN_STATE_REL);
	else
		lv_btn_set_state(autoboot_btn, LV_BTN_STATE_TGL_REL);

	nyx_generic_onoff_toggle(autoboot_btn);

	lv_obj_t * win = lv_win_get_from_btn(btn);

	lv_obj_del(win);

	close_btn = NULL;

	return LV_RES_INV;
}

lv_obj_t *create_window_autoboot(const char *win_title)
{
	static lv_style_t win_bg_style;

	lv_style_copy(&win_bg_style, &lv_style_plain);
	win_bg_style.body.main_color = LV_COLOR_HEX(0x2D2D2D);// TODO: COLOR_HOS_BG
	win_bg_style.body.grad_color = win_bg_style.body.main_color;

	lv_obj_t *win = lv_win_create(lv_scr_act(), NULL);
	lv_win_set_title(win, win_title);
	lv_win_set_style(win, LV_WIN_STYLE_BG, &win_bg_style);
	lv_obj_set_size(win, LV_HOR_RES, LV_VER_RES);

	close_btn = lv_win_add_btn(win, NULL, SYMBOL_CLOSE" Close", _win_autoboot_close_action);

	return win;
}

static lv_res_t _autoboot_disable_action(lv_obj_t *btn)
{
	h_cfg.autoboot = 0;
	h_cfg.autoboot_list = 0;
	ini_changes_made = true;

	lv_btn_set_state(autoboot_btn, LV_BTN_STATE_REL);
	nyx_generic_onoff_toggle(autoboot_btn);

	lv_obj_t * win = lv_win_get_from_btn(btn);

	lv_obj_del(win);

	close_btn = NULL;

	return LV_RES_OK;
}

lv_obj_t *auto_main_list;
lv_obj_t *auto_more_list;
static lv_res_t _autoboot_enable_main_action(lv_obj_t *btn)
{
	h_cfg.autoboot = lv_list_get_btn_index(auto_main_list, btn) + 1;
	h_cfg.autoboot_list = 0;
	ini_changes_made = true;

	lv_btn_set_state(autoboot_btn, LV_BTN_STATE_TGL_REL);
	nyx_generic_onoff_toggle(autoboot_btn);

	lv_obj_t *obj = lv_obj_get_parent(btn);
	for (int i = 0; i < 5; i++)
		obj = lv_obj_get_parent(obj);
	lv_obj_del(obj);

	close_btn = NULL;

	return LV_RES_INV;
}

static lv_res_t _autoboot_enable_more_action(lv_obj_t *btn)
{
	h_cfg.autoboot = lv_list_get_btn_index(auto_more_list, btn) + 1;
	h_cfg.autoboot_list = 1;
	ini_changes_made = true;

	lv_btn_set_state(autoboot_btn, LV_BTN_STATE_TGL_REL);
	nyx_generic_onoff_toggle(autoboot_btn);

	lv_obj_t *obj = lv_obj_get_parent(btn);
	for (int i = 0; i < 5; i++)
		obj = lv_obj_get_parent(obj);
	lv_obj_del(obj);

	close_btn = NULL;

	return LV_RES_INV;
}

static void _create_autoboot_window()
{
	lv_obj_t *win = create_window_autoboot(SYMBOL_GPS" Auto Boot");
	lv_win_add_btn(win, NULL, SYMBOL_POWER" Disable", _autoboot_disable_action);

	static lv_style_t h_style;
	lv_style_copy(&h_style, &lv_style_transp);
	h_style.body.padding.inner = 0;
	h_style.body.padding.hor = LV_DPI - (LV_DPI / 4);
	h_style.body.padding.ver = LV_DPI / 6;

	// Main configurations container.
	lv_obj_t *h1 = lv_cont_create(win, NULL);
	lv_cont_set_style(h1, &h_style);
	lv_cont_set_fit(h1, false, true);
	lv_obj_set_width(h1, (LV_HOR_RES / 9) * 4);
	lv_obj_set_click(h1, false);
	lv_cont_set_layout(h1, LV_LAYOUT_OFF);

	lv_obj_t *label_sep = lv_label_create(h1, NULL);
	lv_label_set_static_text(label_sep, "");

	lv_obj_t *label_txt = lv_label_create(h1, NULL);
	lv_label_set_static_text(label_txt, "Main configurations");
	lv_obj_set_style(label_txt, lv_theme_get_current()->label.prim);
	lv_obj_align(label_txt, label_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, -(LV_DPI / 4));

	lv_obj_t *line_sep = lv_line_create(h1, NULL);
	static const lv_point_t line_pp[] = { {0, 0}, { LV_HOR_RES - (LV_DPI - (LV_DPI / 4)) * 2, 0} };
	lv_line_set_points(line_sep, line_pp, 2);
	lv_line_set_style(line_sep, lv_theme_get_current()->line.decor);
	lv_obj_align(line_sep, label_txt, LV_ALIGN_OUT_BOTTOM_LEFT, -(LV_DPI / 4), LV_DPI / 8);

	// Create list and populate it with Main boot entries.
	lv_obj_t *list_main = lv_list_create(h1, NULL);
	auto_main_list = list_main;
	lv_obj_align(list_main, line_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, LV_DPI / 4);

	lv_obj_set_size(list_main, LV_HOR_RES * 4 / 10, LV_VER_RES * 4 / 7);
	lv_list_set_single_mode(list_main, true);

	sd_mount();

	LIST_INIT(ini_sections);
	if (ini_parse(&ini_sections, "bootloader/hekate_ipl.ini", false))
	{
		LIST_FOREACH_ENTRY(ini_sec_t, ini_sec, &ini_sections, link)
		{
			if (!strcmp(ini_sec->name, "config") || (ini_sec->type != INI_CHOICE))
				continue;

			lv_list_add(list_main, NULL, ini_sec->name, _autoboot_enable_main_action);
		}
	}

	// More configuration container.
	lv_obj_t *h2 = lv_cont_create(win, NULL);
	lv_cont_set_style(h2, &h_style);
	lv_cont_set_fit(h2, false, true);
	lv_obj_set_width(h2, (LV_HOR_RES / 9) * 4);
	lv_obj_set_click(h2, false);
	lv_cont_set_layout(h2, LV_LAYOUT_OFF);
	lv_obj_align(h2, h1, LV_ALIGN_OUT_RIGHT_TOP, LV_DPI * 17 / 29, 0);

	label_sep = lv_label_create(h2, NULL);
	lv_label_set_static_text(label_sep, "");

	lv_obj_t *label_txt3 = lv_label_create(h2, NULL);
	lv_label_set_static_text(label_txt3, "Ini folder configurations");
	lv_obj_set_style(label_txt3, lv_theme_get_current()->label.prim);
	lv_obj_align(label_txt3, label_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, -LV_DPI / 11);

	line_sep = lv_line_create(h2, line_sep);
	lv_obj_align(line_sep, label_txt3, LV_ALIGN_OUT_BOTTOM_LEFT, -(LV_DPI / 2), LV_DPI / 8);
	lv_line_set_style(line_sep, lv_theme_get_current()->line.decor);

	// Create list and populate it with more cfg boot entries.
	lv_obj_t *list_more_cfg = lv_list_create(h2, NULL);
	auto_more_list = list_more_cfg;
	lv_obj_align(list_more_cfg, line_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 2, LV_DPI / 4);

	lv_obj_set_size(list_more_cfg, LV_HOR_RES * 4 / 10, LV_VER_RES * 4 / 7);
	lv_list_set_single_mode(list_more_cfg, true);

	LIST_INIT(ini_list_sections);
	if (ini_parse(&ini_list_sections, "bootloader/ini", true))
	{
		LIST_FOREACH_ENTRY(ini_sec_t, ini_sec, &ini_list_sections, link)
		{
			if (!strcmp(ini_sec->name, "config") || (ini_sec->type != INI_CHOICE))
				continue;

			lv_list_add(list_more_cfg, NULL, ini_sec->name, _autoboot_enable_more_action);
		}
	}

	sd_unmount();
}

static lv_res_t _autoboot_hide_delay_action(lv_obj_t *btn)
{
	if (!autoboot_first_time)
		_create_autoboot_window();

	if (!h_cfg.autoboot && autoboot_first_time)
		lv_btn_set_state(btn, LV_BTN_STATE_REL);
	else
		lv_btn_set_state(btn, LV_BTN_STATE_TGL_REL);
	autoboot_first_time = false;

	nyx_generic_onoff_toggle(btn);

	return LV_RES_INV;
}

static lv_res_t _autoboot_delay_action(lv_obj_t *ddlist)
{
	u32 new_selection = lv_ddlist_get_selected(ddlist);
	if (h_cfg.bootwait != new_selection)
	{
		h_cfg.bootwait = new_selection;
		ini_changes_made = true;
	}

	return LV_RES_OK;
}

static lv_res_t _slider_brightness_action(lv_obj_t * slider)
{
	display_backlight_brightness(lv_slider_get_value(slider) - 20, 0);
	h_cfg.backlight = lv_slider_get_value(slider);
	ini_changes_made = true;

	return LV_RES_OK;
}

static lv_res_t _data_verification_action(lv_obj_t *ddlist)
{
	u32 new_selection = lv_ddlist_get_selected(ddlist);
	if (n_cfg.verification != new_selection)
	{
		n_cfg.verification = new_selection;
		nyx_changes_made = true;
	}

	return LV_RES_OK;
}

static lv_res_t _save_nyx_options_action(lv_obj_t *btn)
{
	static const char * mbox_btn_map[] = {"\211", "\222OK!", "\211", ""};
	lv_obj_t * mbox = lv_mbox_create(lv_scr_act(), NULL);
	lv_mbox_set_recolor_text(mbox, true);

	int res = !create_nyx_config_entry(true);

	nyx_changes_made = false;

	if (res)
		lv_mbox_set_text(mbox, "#FF8000 Nyx Configuration#\n\n#96FF00 The configuration was saved to sd card!#");
	else
		lv_mbox_set_text(mbox, "#FF8000 Nyx Configuration#\n\n#FFDD00 Failed to save the configuration#\n#FFDD00 to sd card!#");
	lv_mbox_add_btns(mbox, mbox_btn_map, NULL);
	lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_top(mbox, true);

	return LV_RES_OK;
}

void create_flat_button(lv_obj_t *parent, lv_obj_t *btn, lv_color_t color, lv_action_t action)
{
	lv_style_t *btn_onoff_rel_hos_style = malloc(sizeof(lv_style_t));
	lv_style_t *btn_onoff_pr_hos_style = malloc(sizeof(lv_style_t));
	lv_style_copy(btn_onoff_rel_hos_style, lv_theme_get_current()->btn.rel);
	btn_onoff_rel_hos_style->body.main_color = color;
	btn_onoff_rel_hos_style->body.grad_color = btn_onoff_rel_hos_style->body.main_color;
	btn_onoff_rel_hos_style->body.padding.hor = 0;
	btn_onoff_rel_hos_style->body.radius = 0;

	lv_style_copy(btn_onoff_pr_hos_style, lv_theme_get_current()->btn.pr);
	btn_onoff_pr_hos_style->body.main_color = color;
	btn_onoff_pr_hos_style->body.grad_color = btn_onoff_pr_hos_style->body.main_color;
	btn_onoff_pr_hos_style->body.padding.hor = 0;
	btn_onoff_pr_hos_style->body.border.color = LV_COLOR_GRAY;
	btn_onoff_pr_hos_style->body.border.width = 4;
	btn_onoff_pr_hos_style->body.radius = 0;

	lv_btn_set_style(btn, LV_BTN_STYLE_REL, btn_onoff_rel_hos_style);
	lv_btn_set_style(btn, LV_BTN_STYLE_PR, btn_onoff_pr_hos_style);
	lv_btn_set_style(btn, LV_BTN_STYLE_TGL_REL, btn_onoff_rel_hos_style);
	lv_btn_set_style(btn, LV_BTN_STYLE_TGL_PR, btn_onoff_pr_hos_style);

	lv_btn_set_fit(btn, false, true);
	lv_obj_set_width(btn, lv_obj_get_height(btn));
	lv_btn_set_toggle(btn, true);

	if (action)
		lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, action);
}

typedef struct _color_test_ctxt
{
	u16 hue;
	lv_obj_t *label;
	lv_obj_t *icons;
	lv_obj_t *slider;
	lv_obj_t *button;
	lv_obj_t *hue_slider;
	lv_obj_t *hue_label;
} color_test_ctxt;

color_test_ctxt color_test;

static lv_res_t _save_theme_color_action(lv_obj_t *btn)
{
	n_cfg.themecolor = color_test.hue;

	// Save nyx config.
	create_nyx_config_entry(true);

	reload_nyx();

	return LV_RES_OK;
}

static void _test_nyx_color(u16 hue)
{
	lv_color_t color = lv_color_hsv_to_rgb(hue, 100, 100);
	static lv_style_t btn_tgl_test;
	lv_style_copy(&btn_tgl_test, lv_btn_get_style(color_test.button, LV_BTN_STATE_TGL_PR));
	btn_tgl_test.body.border.color = color;
	btn_tgl_test.text.color = color;
	lv_btn_set_style(color_test.button, LV_BTN_STATE_TGL_PR, &btn_tgl_test);

	static lv_style_t txt_test;
	lv_style_copy(&txt_test, lv_label_get_style(color_test.label));
	txt_test.text.color = color;
	lv_obj_set_style(color_test.label, &txt_test);
	lv_obj_set_style(color_test.icons, &txt_test);

	static lv_style_t slider_test, slider_ind;
	lv_style_copy(&slider_test, lv_slider_get_style(color_test.slider, LV_SLIDER_STYLE_KNOB));
	lv_style_copy(&slider_ind, lv_slider_get_style(color_test.slider, LV_SLIDER_STYLE_INDIC));
	slider_test.body.main_color = color;
	slider_test.body.grad_color = slider_test.body.main_color;
	slider_ind.body.main_color = lv_color_hsv_to_rgb(hue, 100, 72);
	slider_ind.body.grad_color = slider_ind.body.main_color;
	lv_slider_set_style(color_test.slider, LV_SLIDER_STYLE_KNOB, &slider_test);
	lv_slider_set_style(color_test.slider, LV_SLIDER_STYLE_INDIC, &slider_ind);
}

static lv_res_t _slider_hue_action(lv_obj_t *slider)
{
	if (color_test.hue != lv_slider_get_value(slider))
	{
		color_test.hue = lv_slider_get_value(slider);
		_test_nyx_color(color_test.hue);
		char hue[8];
		s_printf(hue, "%03d", color_test.hue);
		lv_label_set_text(color_test.hue_label, hue);
	}

	return LV_RES_OK;
}

static lv_res_t _preset_hue_action(lv_obj_t *btn)
{
	lv_btn_ext_t *ext = lv_obj_get_ext_attr(btn);

	if (color_test.hue != ext->idx)
	{
		color_test.hue = ext->idx;
		_test_nyx_color(color_test.hue);
		char hue[8];
		s_printf(hue, "%03d", color_test.hue);
		lv_label_set_text(color_test.hue_label, hue);
		lv_bar_set_value(color_test.hue_slider, color_test.hue);
	}

	return LV_RES_OK;
}

const u16 theme_colors[17] = {
	4, 13, 23, 33, 43, 54, 66, 89, 124, 167, 187, 200, 208, 231, 261, 291, 341
};

static lv_res_t _create_window_nyx_colors(lv_obj_t *btn)
{
	lv_obj_t *win = nyx_create_standard_window(SYMBOL_COPY" Choose a Nyx Color Theme");
	lv_win_add_btn(win, NULL, SYMBOL_SAVE" Save & Reload", _save_theme_color_action);

	// Set current color.
	color_test.hue = n_cfg.themecolor;

	lv_obj_t *sep = lv_label_create(win, NULL);
	lv_label_set_static_text(sep, "");
	lv_obj_align(sep, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

	// Create container to keep content inside.
	lv_obj_t *h1 = lv_cont_create(win, NULL);
	lv_obj_set_size(h1, LV_HOR_RES - (LV_DPI * 8 / 10), LV_VER_RES / 7);

	// Create color preset buttons.
	lv_obj_t *color_btn = lv_btn_create(h1, NULL);
	lv_btn_ext_t *ext = lv_obj_get_ext_attr(color_btn);
	ext->idx = theme_colors[0];
	create_flat_button(h1, color_btn, lv_color_hsv_to_rgb(theme_colors[0], 100, 100), _preset_hue_action);
	lv_obj_t *color_btn2;

	for (u32 i = 1; i < 17; i++)
	{
		color_btn2 = lv_btn_create(h1, NULL);
		ext = lv_obj_get_ext_attr(color_btn2);
		ext->idx = theme_colors[i];
		create_flat_button(h1, color_btn2, lv_color_hsv_to_rgb(theme_colors[i], 100, 100), _preset_hue_action);
		lv_obj_align(color_btn2, color_btn, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
		color_btn = color_btn2;
	}

	lv_obj_align(h1, sep, LV_ALIGN_OUT_BOTTOM_MID, 0, LV_DPI / 4);

	// Create hue slider.
	lv_obj_t * slider = lv_slider_create(win, NULL);
	lv_obj_set_width(slider, 1070);
	lv_obj_set_height(slider, LV_DPI * 4 / 10);
	lv_bar_set_range(slider, 0, 359);
	lv_bar_set_value(slider, color_test.hue);
	lv_slider_set_action(slider, _slider_hue_action);
	lv_obj_align(slider, h1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
	color_test.hue_slider = slider;

	// Create hue label.
	lv_obj_t *hue_text_label = lv_label_create(win, NULL);
	lv_obj_align(hue_text_label, slider, LV_ALIGN_OUT_RIGHT_MID, LV_DPI * 24 / 100, 0);
	char hue[8];
	s_printf(hue, "%03d", color_test.hue);
	lv_label_set_text(hue_text_label, hue);
	color_test.hue_label = hue_text_label;

	// Create sample text.
	lv_obj_t *h2 = lv_cont_create(win, NULL);
	lv_obj_set_size(h2, LV_HOR_RES - (LV_DPI * 8 / 10), LV_VER_RES / 3);
	lv_obj_align(h2, slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI);

	lv_obj_t *lbl_sample = lv_label_create(h2, NULL);
	lv_label_set_static_text(lbl_sample, "Sample:");

	lv_obj_t *lbl_test = lv_label_create(h2, NULL);
	lv_label_set_long_mode(lbl_test, LV_LABEL_LONG_BREAK);
	lv_label_set_static_text(lbl_test,
		"Lorem ipsum dolor sit amet, consectetur adipisicing elit, "
		"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
		"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
		"nisi ut aliquip ex ea commodo consequat.");
	lv_obj_set_width(lbl_test, lv_obj_get_width(h2) - LV_DPI * 6 / 10);
	lv_obj_align(lbl_test, lbl_sample, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 5);
	color_test.label = lbl_test;

	// Create sample icons.
	lv_obj_t *lbl_icons = lv_label_create(h2, NULL);
	lv_label_set_static_text(lbl_icons,
		SYMBOL_BRIGHTNESS SYMBOL_CHARGE SYMBOL_FILE SYMBOL_DRIVE SYMBOL_FILE_CODE
		SYMBOL_EDIT SYMBOL_HINT SYMBOL_DRIVE SYMBOL_KEYBOARD SYMBOL_POWER);
	lv_obj_align(lbl_icons, lbl_test, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI * 2 / 5);
	color_test.icons = lbl_icons;

	// Create sample slider.
	lv_obj_t *slider_test = lv_slider_create(h2, NULL);
	lv_obj_align(slider_test, lbl_test, LV_ALIGN_OUT_BOTTOM_MID, 0, LV_DPI * 2 / 5);
	lv_obj_set_click(slider_test, false);
	lv_bar_set_value(slider_test, 60);
	color_test.slider = slider_test;

	// Create sample button.
	lv_obj_t *btn_test = lv_btn_create(h2, NULL);
	lv_btn_set_state(btn_test, LV_BTN_STATE_TGL_PR);
	lv_obj_align(btn_test, lbl_test, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, LV_DPI / 5);
	lv_label_create(btn_test, NULL);
	lv_obj_set_click(btn_test, false);
	color_test.button = btn_test;

	_test_nyx_color(color_test.hue);

	return LV_RES_OK;
}

typedef struct _time_edit_obj_t
{
	lv_obj_t *year;
	lv_obj_t *month;
	lv_obj_t *day;
	lv_obj_t *hour;
	lv_obj_t *min;
} time_edit_obj_t;

time_edit_obj_t clock_ctxt;

static lv_res_t _action_clock_edit(lv_obj_t *btns, const char * txt)
{
	int btn_idx = lv_btnm_get_pressed(btns);

	if (btn_idx == 1)
	{
		rtc_time_t time;
		max77620_rtc_get_time(&time);
		u32 epoch = max77620_rtc_date_to_epoch(&time);

		u32 year = lv_roller_get_selected(clock_ctxt.year);
		u32 month = lv_roller_get_selected(clock_ctxt.month) + 1;
		u32 day = lv_roller_get_selected(clock_ctxt.day) + 1;
		u32 hour = lv_roller_get_selected(clock_ctxt.hour);
		u32 min = lv_roller_get_selected(clock_ctxt.min);

		switch (month)
		{
		case 2:
			if (!(year % 4) && day > 29)
				day = 29;
			else if (day > 28)
				day = 28;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if (day > 30)
				day = 30;
			break;
		}

		time.year = year + 2020;
		time.month = month;
		time.day = day;
		time.hour = hour;
		time.min = min;

		u32 new_epoch = max77620_rtc_date_to_epoch(&time);

		n_cfg.timeoff = new_epoch - epoch;
		if (!n_cfg.timeoff)
			n_cfg.timeoff = 1;

		nyx_changes_made = true;
	}

	mbox_action(btns, txt);

	return LV_RES_INV;
}

static lv_res_t _action_clock_edit_save(lv_obj_t *btns, const char * txt)
{
	_action_clock_edit(btns, txt);

	// Save if changes were made.
	if (nyx_changes_made)
		_save_nyx_options_action(NULL);

	return LV_RES_INV;
}

static lv_res_t _create_mbox_clock_edit(lv_obj_t *btn)
{
	lv_obj_t *dark_bg = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_set_style(dark_bg, &mbox_darken);
	lv_obj_set_size(dark_bg, LV_HOR_RES, LV_VER_RES);

	static const char * mbox_btn_map[] = { "\211", "\222Done", "\222Cancel", "\211", "" };
	lv_obj_t *mbox = lv_mbox_create(dark_bg, NULL);
	lv_mbox_set_recolor_text(mbox, true);
	lv_obj_set_width(mbox, LV_HOR_RES / 9 * 6);

	lv_mbox_set_text(mbox, "Enter #C7EA46 Date# and #C7EA46 Time# for Nyx\nThis will not alter the actual HW clock!");

	rtc_time_t time;
	max77620_rtc_get_time(&time);
	if (n_cfg.timeoff)
	{
		u32 epoch = max77620_rtc_date_to_epoch(&time) + (s32)n_cfg.timeoff;
		max77620_rtc_epoch_to_date(epoch, &time);
	}
	if (time.year < 2020)
		time.year = 2020;
	else if (time.year > 2030)
		time.year = 2030;

	time.year -= 2020;

	lv_obj_t *h1 = lv_cont_create(mbox, NULL);
	lv_cont_set_fit(h1, true, true);

	lv_obj_t *roller_year = lv_roller_create(h1, NULL);
	lv_roller_set_options(roller_year,
		"2020\n"
		"2021\n"
		"2022\n"
		"2023\n"
		"2024\n"
		"2025\n"
		"2026\n"
		"2027\n"
		"2028\n"
		"2029\n"
		"2030");
	lv_roller_set_selected(roller_year, time.year, false);
	lv_roller_set_visible_row_count(roller_year, 3);
	clock_ctxt.year = roller_year;

	lv_obj_t *roller_month = lv_roller_create(h1, roller_year);
	lv_roller_set_options(roller_month,
		"January\n"
		"February\n"
		"March\n"
		"April\n"
		"May\n"
		"June\n"
		"July\n"
		"August\n"
		"September\n"
		"October\n"
		"November\n"
		"December");
	lv_roller_set_selected(roller_month, time.month - 1, false);
	lv_obj_align(roller_month, roller_year, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
	clock_ctxt.month = roller_month;

	static char days[256];
	days[0] = 0;
	for (u32 i = 1; i < 32; i++)
		s_printf(days + strlen(days), " %d \n", i);
	days[strlen(days) - 1] = 0;
	lv_obj_t *roller_day = lv_roller_create(h1, roller_year);
	lv_roller_set_options(roller_day, days);
	lv_roller_set_selected(roller_day, time.day - 1, false);
	lv_obj_align(roller_day, roller_month, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
	clock_ctxt.day = roller_day;

	static char hours[256];
	hours[0] = 0;
	for (u32 i = 0; i < 24; i++)
		s_printf(hours + strlen(hours), " %d \n", i);
	hours[strlen(hours) - 1] = 0;
	lv_obj_t *roller_hour = lv_roller_create(h1, roller_year);
	lv_roller_set_options(roller_hour, hours);
	lv_roller_set_selected(roller_hour, time.hour, false);
	lv_obj_align(roller_hour, roller_day, LV_ALIGN_OUT_RIGHT_MID, LV_DPI / 2, 0);
	clock_ctxt.hour = roller_hour;

	static char minutes[512];
	minutes[0] = 0;
	for (u32 i = 0; i < 60; i++)
		s_printf(minutes + strlen(minutes), " %02d \n", i);
	minutes[strlen(minutes) - 1] = 0;
	lv_obj_t *roller_minute = lv_roller_create(h1, roller_year);
	lv_roller_set_options(roller_minute, minutes);
	lv_roller_set_selected(roller_minute, time.min, false);
	lv_obj_align(roller_minute, roller_hour, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
	clock_ctxt.min = roller_minute;

	// If btn is empty, save options also because it was launched from boot.
	lv_mbox_add_btns(mbox, mbox_btn_map, btn ? _action_clock_edit : _action_clock_edit_save);

	lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_top(mbox, true);

	return LV_RES_OK;
}

void first_time_clock_edit(void *param)
{
	_create_mbox_clock_edit(NULL);
}

static lv_res_t _joycon_info_dump_action(lv_obj_t * btn)
{
	FIL fp;
	int error;
	bool is_l_hos = false;
	bool is_r_hos = false;
	jc_gamepad_rpt_t *jc_pad = jc_get_bt_pairing_info(&is_l_hos, &is_r_hos);

	char *data = (char *)malloc(SZ_16K);
	char *txt_buf = (char *)malloc(SZ_4K);

	if (!jc_pad)
	{
		error = 255;
		goto disabled;
	}

	// Count valid joycon.
	u32 joycon_found = jc_pad->bt_conn_l.type ? 1 : 0;
	if (jc_pad->bt_conn_r.type)
		joycon_found++;

	// Reset PC based for dumping.
	jc_pad->bt_conn_l.type = is_l_hos ? jc_pad->bt_conn_l.type : 0;
	jc_pad->bt_conn_r.type = is_r_hos ? jc_pad->bt_conn_r.type : 0;

	error = !sd_mount();

	if (!error)
	{
		// Save binary dump.
		memcpy(data, &jc_pad->bt_conn_l, sizeof(jc_bt_conn_t));
		memcpy(data + sizeof(jc_bt_conn_t), &jc_pad->bt_conn_r, sizeof(jc_bt_conn_t));

		f_mkdir("switchroot");
		error = sd_save_to_file((u8 *)data, sizeof(jc_bt_conn_t) * 2, "switchroot/joycon_mac.bin");

		// Save readable dump.
		jc_bt_conn_t *bt = &jc_pad->bt_conn_l;
		s_printf(data,
			"[joycon_00]\ntype=%d\nmac=%02X:%02X:%02X:%02X:%02X:%02X\n"
			"host=%02X:%02X:%02X:%02X:%02X:%02X\n"
			"ltk=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n\n",
			bt->type, bt->mac[0], bt->mac[1], bt->mac[2], bt->mac[3], bt->mac[4], bt->mac[5],
			bt->host_mac[0], bt->host_mac[1], bt->host_mac[2], bt->host_mac[3], bt->host_mac[4], bt->host_mac[5],
			bt->ltk[0], bt->ltk[1], bt->ltk[2], bt->ltk[3], bt->ltk[4], bt->ltk[5], bt->ltk[6], bt->ltk[7],
			bt->ltk[8], bt->ltk[9], bt->ltk[10], bt->ltk[11], bt->ltk[12], bt->ltk[13], bt->ltk[14], bt->ltk[15]);
		bt = &jc_pad->bt_conn_r;
		s_printf(data + strlen(data),
			"[joycon_01]\ntype=%d\nmac=%02X:%02X:%02X:%02X:%02X:%02X\n"
			"host=%02X:%02X:%02X:%02X:%02X:%02X\n"
			"ltk=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
			bt->type, bt->mac[0], bt->mac[1], bt->mac[2], bt->mac[3], bt->mac[4], bt->mac[5],
			bt->host_mac[0], bt->host_mac[1], bt->host_mac[2], bt->host_mac[3], bt->host_mac[4], bt->host_mac[5],
			bt->ltk[0], bt->ltk[1], bt->ltk[2], bt->ltk[3], bt->ltk[4], bt->ltk[5], bt->ltk[6], bt->ltk[7],
			bt->ltk[8], bt->ltk[9], bt->ltk[10], bt->ltk[11], bt->ltk[12], bt->ltk[13], bt->ltk[14], bt->ltk[15]);

		if (!error)
			error = f_open(&fp, "switchroot/joycon_mac.ini", FA_WRITE | FA_CREATE_ALWAYS);
		if (!error)
		{
			f_puts(data, &fp);
			f_close(&fp);
		}

		sd_unmount();
	}

disabled:;
	lv_obj_t *dark_bg = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_set_style(dark_bg, &mbox_darken);
	lv_obj_set_size(dark_bg, LV_HOR_RES, LV_VER_RES);

	static const char * mbox_btn_map[] = { "\211", "\222OK", "\211", "" };
	lv_obj_t * mbox = lv_mbox_create(dark_bg, NULL);
	lv_mbox_set_recolor_text(mbox, true);
	lv_obj_set_width(mbox, LV_HOR_RES / 9 * 5);

	if (!error)
	{
		s_printf(txt_buf,
			"Dumping to SD card finished!\n"
			"Saved to: #C7EA46 switchroot/joycon_mac.[bin/ini]#\n\n");

		bool success = true;

		// Check if pairing info was found.
		if (joycon_found == 2)
			strcat(txt_buf, "#C7EA46 Success!#\n#C7EA46 Found 2 out of 2 Joy-Con pairing data!#\n");
		else
		{
			s_printf(txt_buf + strlen(txt_buf), "#FF8000 Failed!#\n#FF8000 Warning:# Found #FFDD00 %d out of 2# pairing data!\n", joycon_found);
			success = false;
		}

		// Check if pairing was done in HOS.
		if (is_l_hos && is_r_hos)
			strcat(txt_buf, "#C7EA46 Both pairing data are HOS based!#");
		else if (!is_l_hos && is_r_hos)
		{
			strcat(txt_buf, "#FF8000 Warning:# #FFDD00 Left# pairing data is not HOS based!");
			success = false;
		}
		else if (is_l_hos && !is_r_hos)
		{
			strcat(txt_buf, "#FF8000 Warning:# #FFDD00 Right# pairing data is not HOS based!");
			success = false;
		}
		else
		{
			strcat(txt_buf, "#FF8000 Warning:# #FFDD00 No# pairing data is HOS based!");
			success = false;
		}

		if (!success)
			strcat(txt_buf,
				"\n\n#FFDD00 Make sure that both Joy-Con are connected,#\n"
				"#FFDD00 and that you paired them in HOS!#");
	}
	else
		s_printf(txt_buf, "#FFDD00 Failed to dump Joy-Con pairing info!#\n#FFDD00 Error: %d#", error);

	lv_mbox_set_text(mbox, txt_buf);

	lv_mbox_add_btns(mbox, mbox_btn_map, mbox_action); // Important. After set_text.

	lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_top(mbox, true);

	free(txt_buf);
	free(data);

	return LV_RES_OK;
}

static lv_res_t _home_screen_action(lv_obj_t *ddlist)
{
	u32 new_selection = lv_ddlist_get_selected(ddlist);
	if (n_cfg.home_screen != new_selection)
	{
		n_cfg.home_screen = new_selection;
		nyx_changes_made = true;
	}

	return LV_RES_OK;
}

static lv_res_t _action_nyx_options_save(lv_obj_t *btns, const char * txt)
{
	int btn_idx = lv_btnm_get_pressed(btns);

	mbox_action(btns, txt);

	if (!btn_idx)
		_save_nyx_options_action(NULL);

	return LV_RES_INV;
}

static void _check_nyx_changes()
{
	if (nyx_changes_made)
	{
		lv_obj_t *dark_bg = lv_obj_create(lv_scr_act(), NULL);
		lv_obj_set_style(dark_bg, &mbox_darken);
		lv_obj_set_size(dark_bg, LV_HOR_RES, LV_VER_RES);

		static const char * mbox_btn_map[] = { "\222Save", "\222Cancel", "" };
		lv_obj_t * mbox = lv_mbox_create(dark_bg, NULL);
		lv_mbox_set_recolor_text(mbox, true);

		lv_mbox_set_text(mbox,
			"#FF8000 Nyx configuration#\n\n"
			"You changed the configuration!\n\n"
			"Do you want to save it?");

		lv_mbox_add_btns(mbox, mbox_btn_map, _action_nyx_options_save);
		lv_obj_set_width(mbox, LV_HOR_RES / 9 * 5);
		lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 0);
		lv_obj_set_top(mbox, true);

		nyx_changes_made = false;
	}
}

static lv_res_t _action_win_nyx_options_close(lv_obj_t *btn)
{
	lv_win_close_action(btn);

	close_btn = NULL;

	_check_nyx_changes();

	return LV_RES_INV;
}

lv_res_t create_win_nyx_options(lv_obj_t *parrent_btn)
{
	lv_theme_t *th = lv_theme_get_current();

	lv_obj_t *win = nyx_create_window_custom_close_btn(SYMBOL_HOME" Nyx Options", _action_win_nyx_options_close);

	static lv_style_t h_style;
	lv_style_copy(&h_style, &lv_style_transp);
	h_style.body.padding.inner = 0;
	h_style.body.padding.hor = LV_DPI - (LV_DPI / 4);
	h_style.body.padding.ver = LV_DPI / 6;

	// Create containers to keep content inside.
	lv_obj_t * sw_h2 = lv_cont_create(win, NULL);
	lv_cont_set_style(sw_h2, &h_style);
	lv_cont_set_fit(sw_h2, false, true);
	lv_obj_set_width(sw_h2, (LV_HOR_RES / 9) * 4);
	lv_obj_set_click(sw_h2, false);
	lv_cont_set_layout(sw_h2, LV_LAYOUT_OFF);

	lv_obj_t * sw_h3 = lv_cont_create(win, NULL);
	lv_cont_set_style(sw_h3, &h_style);
	lv_cont_set_fit(sw_h3, false, true);
	lv_obj_set_width(sw_h3, (LV_HOR_RES / 9) * 4);
	lv_obj_set_click(sw_h3, false);
	lv_cont_set_layout(sw_h3, LV_LAYOUT_OFF);
	lv_obj_align(sw_h3, sw_h2, LV_ALIGN_OUT_RIGHT_TOP, LV_DPI * 11 / 25, 0);

	lv_obj_t * l_cont = lv_cont_create(sw_h2, NULL);
	lv_cont_set_style(l_cont, &lv_style_transp_tight);
	lv_cont_set_fit(l_cont, true, true);
	lv_obj_set_click(l_cont, false);
	lv_cont_set_layout(l_cont, LV_LAYOUT_OFF);
	lv_obj_set_opa_scale(l_cont, LV_OPA_40);

	lv_obj_t *label_sep = lv_label_create(sw_h2, NULL);
	lv_label_set_static_text(label_sep, "");

	lv_obj_t *btn = lv_btn_create(sw_h2, NULL);
	lv_obj_t *label_btn = lv_label_create(btn, NULL);
	lv_btn_set_fit(btn, true, true);
	lv_label_set_static_text(label_btn, SYMBOL_COPY" Color Theme");
	lv_obj_align(btn, label_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, -LV_DPI / 5 + 3);
	lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, _create_window_nyx_colors);

	lv_obj_t *label_txt2 = lv_label_create(sw_h2, NULL);
	lv_label_set_recolor(label_txt2, true);
	lv_label_set_static_text(label_txt2, "Select a color for all #00FFC8 highlights# in Nyx.\n");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, btn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 3 - 8);

	lv_obj_t *line_sep = lv_line_create(sw_h2, NULL);
	static const lv_point_t line_pp[] = { {0, 0}, { LV_HOR_RES - (LV_DPI - (LV_DPI / 4)) * 2, 0} };
	lv_line_set_points(line_sep, line_pp, 2);
	lv_line_set_style(line_sep, th->line.decor);
	lv_obj_align(line_sep, label_txt2, LV_ALIGN_OUT_BOTTOM_LEFT, -(LV_DPI / 4), LV_DPI / 4);

	lv_obj_t *label_txt = lv_label_create(l_cont, NULL);
	lv_label_set_static_text(label_txt, SYMBOL_HOME" Home Screen");
	lv_obj_set_style(label_txt, th->label.prim);
	lv_obj_align(label_txt, line_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, LV_DPI / 4);

	lv_obj_t *ddlist = lv_ddlist_create(l_cont, NULL);
	lv_obj_set_top(ddlist, true);
	lv_ddlist_set_draw_arrow(ddlist, true);
	lv_ddlist_set_options(ddlist,
		"Main menu       \n"
		"All Configs\n"
		"Launch\n"
		"More Configs");
	lv_ddlist_set_selected(ddlist, n_cfg.home_screen);
	lv_ddlist_set_action(ddlist, _home_screen_action);
	lv_obj_align(ddlist, label_txt, LV_ALIGN_OUT_RIGHT_MID, LV_DPI * 2 / 3, 0);

	label_txt2 = lv_label_create(l_cont, NULL);
	lv_label_set_recolor(label_txt2, true);
	lv_label_set_static_text(label_txt2,
		"Select what screen to show on Nyx boot.\n"
		"#FF8000 All Configs:# #C7EA46 Combines More configs into Launch empty slots.#\n"
		"#FF8000 Launch / More Configs:# #C7EA46 Uses the classic divided view.#");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, label_txt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 4);

	line_sep = lv_line_create(sw_h2, line_sep);
	lv_obj_align(line_sep, label_txt2, LV_ALIGN_OUT_BOTTOM_LEFT, -(LV_DPI / 4), LV_DPI / 4);

	lv_obj_t *btn2 = lv_btn_create(sw_h2, NULL);
	lv_obj_t *label_btn2 = lv_label_create(btn2, NULL);
	lv_btn_set_fit(btn2, true, true);
	lv_label_set_static_text(label_btn2, SYMBOL_CLOCK" Clock (Offset)");
	lv_obj_align(btn2, line_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, LV_DPI / 4);
	lv_btn_set_action(btn2, LV_BTN_ACTION_CLICK, _create_mbox_clock_edit);

	label_txt2 = lv_label_create(sw_h2, NULL);
	lv_label_set_recolor(label_txt2, true);
	lv_label_set_static_text(label_txt2,
		"Change clock offset manually.\n"
		"#C7EA46 The entered Date and Time will be converted to an offset#\n"
		"#C7EA46 automatically. This will be also used for FatFS operations.#");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, btn2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 4);

	label_sep = lv_label_create(sw_h3, NULL);
	lv_label_set_static_text(label_sep, "");

	// Create Dump Joy-Con BT button.
	lv_obj_t *btn3 = lv_btn_create(sw_h3, NULL);
	lv_obj_t *label_btn3 = lv_label_create(btn3, NULL);
	lv_btn_set_fit(btn3, true, true);
	lv_label_set_static_text(label_btn3, SYMBOL_DOWNLOAD" Dump Joy-Con BT");
	lv_obj_align(btn3, label_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, -LV_DPI / 3);
	lv_btn_set_action(btn3, LV_BTN_ACTION_CLICK, _joycon_info_dump_action);

	label_txt2 = lv_label_create(sw_h3, NULL);
	lv_label_set_recolor(label_txt2, true);
	lv_label_set_static_text(label_txt2,
		"Allows you to save the Switch and Joy-Con MAC addresses\n"
		"and the LTKs associated with them. For #C7EA46 Android# and #C7EA46 Linux#.");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, btn3, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 4);

	line_sep = lv_line_create(sw_h3, line_sep);
	lv_obj_align(line_sep, label_txt2, LV_ALIGN_OUT_BOTTOM_LEFT, -(LV_DPI / 4), LV_DPI / 4);

	// Create Backup/Restore Verification list.
	label_txt = lv_label_create(sw_h3, NULL);
	lv_label_set_static_text(label_txt, SYMBOL_MODULES_ALT" Data Verification");
	lv_obj_set_style(label_txt, th->label.prim);
	lv_obj_align(label_txt, line_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, LV_DPI / 4);

	lv_obj_t *ddlist2 = lv_ddlist_create(sw_h3, NULL);
	lv_obj_set_top(ddlist2, true);
	lv_ddlist_set_draw_arrow(ddlist2, true);
	lv_ddlist_set_options(ddlist2,
		"Off (Fastest)\n"
		"Sparse (Fast)    \n"
		"Full (Slow)\n"
		"Full (Hashes)");
	lv_ddlist_set_selected(ddlist2, n_cfg.verification);
	lv_obj_align(ddlist2, label_txt, LV_ALIGN_OUT_RIGHT_MID, LV_DPI * 3 / 8, 0);
	lv_ddlist_set_action(ddlist2, _data_verification_action);

	label_txt2 = lv_label_create(sw_h3, NULL);
	lv_label_set_static_text(label_txt2, "Set the type of data verification done for backup and restore.\n"
		"Can be canceled without losing the backup/restore.\n");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, label_txt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 4);

	line_sep = lv_line_create(sw_h3, line_sep);
	lv_obj_align(line_sep, label_txt2, LV_ALIGN_OUT_BOTTOM_LEFT, -(LV_DPI / 4), LV_DPI / 4);

	lv_obj_t *btn5 = lv_btn_create(sw_h3, NULL);
	lv_obj_t *label_btn5 = lv_label_create(btn5, NULL);
	lv_btn_set_fit(btn5, true, true);
	lv_label_set_static_text(label_btn5, SYMBOL_EDIT" Save Options");
	lv_obj_align(btn5, line_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI * 31 / 21, LV_DPI * 6 / 8);
	lv_btn_set_action(btn5, LV_BTN_ACTION_CLICK, _save_nyx_options_action);

	lv_obj_set_top(l_cont, true); // Set the ddlist container at top.
	lv_obj_set_parent(ddlist, l_cont); // Reorder ddlist.
	lv_obj_set_top(ddlist, true);
	lv_obj_set_top(ddlist2, true);

	return LV_RES_OK;
}

void create_tab_options(lv_theme_t *th, lv_obj_t *parent)
{
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY);

	static lv_style_t h_style;
	lv_style_copy(&h_style, &lv_style_transp);
	h_style.body.padding.inner = 0;
	h_style.body.padding.hor = LV_DPI - (LV_DPI / 4);
	h_style.body.padding.ver = LV_DPI / 6;

	// Create containers to keep content inside.
	lv_obj_t * sw_h2 = lv_cont_create(parent, NULL);
	lv_cont_set_style(sw_h2, &h_style);
	lv_cont_set_fit(sw_h2, false, true);
	lv_obj_set_width(sw_h2, (LV_HOR_RES / 9) * 4);
	lv_obj_set_click(sw_h2, false);
	lv_cont_set_layout(sw_h2, LV_LAYOUT_OFF);

	lv_obj_t * sw_h3 = lv_cont_create(parent, NULL);
	lv_cont_set_style(sw_h3, &h_style);
	lv_cont_set_fit(sw_h3, false, true);
	lv_obj_set_width(sw_h3, (LV_HOR_RES / 9) * 4);
	lv_obj_set_click(sw_h3, false);
	lv_cont_set_layout(sw_h3, LV_LAYOUT_OFF);

	lv_obj_t * l_cont = lv_cont_create(sw_h2, NULL);
	lv_cont_set_style(l_cont, &lv_style_transp_tight);
	lv_cont_set_fit(l_cont, true, true);
	lv_obj_set_click(l_cont, false);
	lv_cont_set_layout(l_cont, LV_LAYOUT_OFF);
	lv_obj_set_opa_scale(l_cont, LV_OPA_40);

	lv_obj_t *label_sep = lv_label_create(sw_h2, NULL);
	lv_label_set_static_text(label_sep, "");

	// Create Auto Boot button.
	lv_obj_t *btn = lv_btn_create(sw_h2, NULL);
	if (hekate_bg)
	{
		lv_btn_set_style(btn, LV_BTN_STYLE_REL, &btn_transp_rel);
		lv_btn_set_style(btn, LV_BTN_STYLE_PR, &btn_transp_pr);
	}
	lv_btn_set_layout(btn, LV_LAYOUT_OFF);
	lv_obj_t *label_btn = lv_label_create(btn, NULL);
	lv_label_set_recolor(label_btn, true);
	lv_btn_set_fit(btn, true, true);
	lv_btn_set_toggle(btn, true);
	lv_label_set_static_text(label_btn, SYMBOL_GPS" Auto Boot #00FFC9   ON #");
	lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, _autoboot_hide_delay_action);
	lv_obj_align(btn, label_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, -LV_DPI / 18 + 6);
	lv_btn_set_fit(btn, false, false);
	autoboot_btn = btn;

	lv_obj_t *label_txt2 = lv_label_create(sw_h2, NULL);
	lv_label_set_static_text(label_txt2, "Choose which boot entry or payload to automatically boot\nwhen injecting.");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, btn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 3 - 4);

	lv_obj_t *line_sep = lv_line_create(sw_h2, NULL);
	static const lv_point_t line_pp[] = { {0, 0}, { LV_HOR_RES - (LV_DPI - (LV_DPI / 4)) * 2, 0} };
	lv_line_set_points(line_sep, line_pp, 2);
	lv_line_set_style(line_sep, th->line.decor);
	lv_obj_align(line_sep, label_txt2, LV_ALIGN_OUT_BOTTOM_LEFT, -(LV_DPI / 4), LV_DPI / 4);

	// Create Boot time delay list.
	lv_obj_t *label_txt = lv_label_create(l_cont, NULL);
	lv_label_set_static_text(label_txt, SYMBOL_CLOCK" Boot Time Delay  ");
	lv_obj_set_style(label_txt, th->label.prim);
	lv_obj_align(label_txt, line_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, LV_DPI / 4);

	lv_obj_t *ddlist = lv_ddlist_create(l_cont, NULL);
	lv_obj_set_top(ddlist, true);
	lv_ddlist_set_draw_arrow(ddlist, true);
	lv_ddlist_set_options(ddlist,
		"No bootlogo    \n"
		"1 second\n"
		"2 seconds\n"
		"3 seconds\n"
		"4 seconds\n"
		"5 seconds\n"
		"6 seconds");
	lv_ddlist_set_selected(ddlist, 3);
	lv_obj_align(ddlist, label_txt, LV_ALIGN_OUT_RIGHT_MID, LV_DPI / 4, 0);
	lv_ddlist_set_action(ddlist, _autoboot_delay_action);
	lv_ddlist_set_selected(ddlist, h_cfg.bootwait);

	if (hekate_bg)
	{

	}

	label_txt2 = lv_label_create(l_cont, NULL);
	lv_label_set_recolor(label_txt2, true);
	lv_label_set_static_text(label_txt2,
		"Set how long to show bootlogo when autoboot is enabled.\n"
		"#C7EA46 You can press# #FF8000 VOL-# #C7EA46 during that time to enter hekate's menu.#\n");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, label_txt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 4);

	line_sep = lv_line_create(sw_h2, line_sep);
	lv_obj_align(line_sep, label_txt2, LV_ALIGN_OUT_BOTTOM_LEFT, -(LV_DPI / 4), LV_DPI / 4);

	// Create Auto NoGC button.
	lv_obj_t *btn2 = lv_btn_create(sw_h2, NULL);
	nyx_create_onoff_button(th, sw_h2, btn2, SYMBOL_SHRK" Auto NoGC", auto_nogc_toggle, true);
	lv_obj_align(btn2, line_sep, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 10);

	label_txt2 = lv_label_create(sw_h2, NULL);
	lv_label_set_recolor(label_txt2, true);
	lv_label_set_static_text(label_txt2,
		"It checks fuses and applies the patch automatically\n"
		"if higher firmware. It is now a global config and set\n"
		"at auto by default. (ON: Auto)\n\n\n");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, btn2, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, LV_DPI / 12);

	label_sep = lv_label_create(sw_h3, NULL);
	lv_label_set_static_text(label_sep, "");

	// Create Auto HOS Power Off button.
	lv_obj_t *btn3 = lv_btn_create(sw_h3, NULL);
	nyx_create_onoff_button(th, sw_h3, btn3, SYMBOL_POWER" Auto HOS Power Off", auto_hos_poweroff_toggle, true);
	lv_obj_align(btn3, label_sep, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	label_txt2 = lv_label_create(sw_h3, NULL);
	lv_label_set_static_text(label_txt2,
		"When Shutdown is used from HOS, the device wakes up after\n"
		"15s. Enable this to automatically power off on the next\npayload injection.");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, btn3, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, LV_DPI / 12);

	line_sep = lv_line_create(sw_h3, line_sep);
	lv_obj_align(line_sep, label_txt2, LV_ALIGN_OUT_BOTTOM_LEFT, -(LV_DPI / 4), LV_DPI / 4);

	// Create Backlight slider.
	label_txt = lv_label_create(sw_h3, NULL);
	lv_label_set_static_text(label_txt, SYMBOL_BRIGHTNESS" Backlight");
	lv_obj_set_style(label_txt, th->label.prim);
	lv_obj_align(label_txt, line_sep, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, LV_DPI / 4);

	lv_obj_t * slider = lv_slider_create(sw_h3, NULL);
	lv_obj_set_width(slider, LV_DPI * 80 / 34);
	//lv_obj_set_height(slider, LV_DPI * 4 / 10);
	lv_bar_set_range(slider, 30, 220);
	lv_bar_set_value(slider, h_cfg.backlight);
	lv_slider_set_action(slider, _slider_brightness_action);
	lv_obj_align(slider, label_txt, LV_ALIGN_OUT_RIGHT_MID, LV_DPI * 20 / 15, 0);

	label_txt2 = lv_label_create(sw_h3, NULL);
	lv_label_set_static_text(label_txt2, "Set backlight brightness.\n\n");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, label_txt, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 4);

	line_sep = lv_line_create(sw_h3, line_sep);
	lv_obj_align(line_sep, label_txt2, LV_ALIGN_OUT_BOTTOM_LEFT, -(LV_DPI / 4), LV_DPI / 4);

	// Create Update r2p button.
	lv_obj_t *btn4 = lv_btn_create(sw_h3, NULL);
	nyx_create_onoff_button(th, sw_h3, btn4, SYMBOL_REFRESH" Update Reboot 2 Payload", _update_r2p_action, true);
	lv_obj_align(btn4, line_sep, LV_ALIGN_OUT_BOTTOM_LEFT, 0, LV_DPI / 10);

	label_txt2 = lv_label_create(sw_h3, NULL);
	lv_label_set_recolor(label_txt2, true);
	lv_label_set_static_text(label_txt2,
		"If #FF8000 FSS0# is used in the selected boot entry, the reboot 2 payload\n"
		"binary will be checked and forced to be updated to hekate.\n\n\n\n");
	lv_obj_set_style(label_txt2, &hint_small_style);
	lv_obj_align(label_txt2, btn4, LV_ALIGN_OUT_BOTTOM_LEFT, LV_DPI / 4, LV_DPI / 12);

	// Set default loaded states.
	if (h_cfg.autohosoff)
		lv_btn_set_state(btn3, LV_BTN_STATE_TGL_REL);
	if (h_cfg.autonogc)
		lv_btn_set_state(btn2, LV_BTN_STATE_TGL_REL);
	if (h_cfg.updater2p)
		lv_btn_set_state(btn4, LV_BTN_STATE_TGL_REL);

	nyx_generic_onoff_toggle(btn2);
	nyx_generic_onoff_toggle(btn3);
	nyx_generic_onoff_toggle(btn4);
	_autoboot_hide_delay_action(btn);

	lv_obj_set_top(l_cont, true); // Set the ddlist container at top.
	lv_obj_set_parent(ddlist, l_cont); // Reorder ddlist.
	lv_obj_set_top(ddlist, true);
}
