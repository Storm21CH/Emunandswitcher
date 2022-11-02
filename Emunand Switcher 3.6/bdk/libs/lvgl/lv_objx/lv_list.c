/*
 * Copyright (c) 2019 CTCaer
 * Copyright (c) 2020 Storm
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

/**
 * @file lv_list.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_list.h"
#if USE_LV_LIST != 0

#include "../lv_core/lv_group.h"
#include "../lv_themes/lv_theme.h"
#include "../lv_misc/lv_anim.h"
#include "../lv_misc/lv_math.h"

/*********************
 *      DEFINES
 *********************/
#define LV_LIST_LAYOUT_DEF  LV_LAYOUT_COL_M

#if USE_LV_ANIMATION
#  ifndef LV_LIST_FOCUS_TIME
#    define LV_LIST_FOCUS_TIME  100 /*Animation time of focusing to the a list element [ms] (0: no animation)  */
#  endif
#else
#  undef  LV_LIST_FOCUS_TIME
#  define LV_LIST_FOCUS_TIME    0   /*No animations*/
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_res_t lv_list_signal(lv_obj_t * list, lv_signal_t sign, void * param);
static lv_res_t lv_list_btn_signal(lv_obj_t * btn, lv_signal_t sign, void * param);
static void refr_btn_width(lv_obj_t * list);
static void lv_list_btn_single_selected(lv_obj_t *btn);
static bool lv_list_is_list_btn(lv_obj_t * list_btn);
static bool lv_list_is_list_img(lv_obj_t * list_btn);
static bool lv_list_is_list_label(lv_obj_t * list_btn);

/**********************
 *  STATIC VARIABLES
 **********************/
#if USE_LV_IMG
static lv_signal_func_t img_signal;
#endif
static lv_signal_func_t label_signal;
static lv_signal_func_t ancestor_page_signal;
static lv_signal_func_t ancestor_btn_signal;
#if USE_LV_GROUP
/*Used to make the last clicked button pressed (selected) when the list become focused and `click_focus == 1`*/
static lv_obj_t * last_clicked_btn;
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a list objects
 * @param par pointer to an object, it will be the parent of the new list
 * @param copy pointer to a list object, if not NULL then the new object will be copied from it
 * @return pointer to the created list
 */


//#include "../../../../gui/pl_gui/frontend/gui.h"//Test Symbolfarbe in gui.c definieren

lv_obj_t * lv_list_create(lv_obj_t * par, const lv_obj_t * copy)
{
    LV_LOG_TRACE("list create started");

    /*Create the ancestor basic object*/
    lv_obj_t * new_list = lv_page_create(par, copy);
    lv_mem_assert(new_list);
    if(new_list == NULL) return NULL;

    if(ancestor_page_signal == NULL) ancestor_page_signal = lv_obj_get_signal_func(new_list);

    lv_list_ext_t * ext = lv_obj_allocate_ext_attr(new_list, sizeof(lv_list_ext_t));
    lv_mem_assert(ext);
    if(ext == NULL) return NULL;

    //Test Symbolfarbe in gui.c definieren, hier deaktiviert!!!!!!!!!!!!!! geht nicht!!!
	// Important! Symbol definition f�r lv_list
    static lv_style_t img_btn_color;
    lv_style_copy( &img_btn_color, &lv_style_plain);
    img_btn_color.image.color = LV_COLOR_HEX(0xDDDDDD);//img_btn_color.image.color = LV_COLOR_YELLOW;//img_btn_color.image.color = LV_COLOR_HEX(0xDDDDDD);
    img_btn_color.image.intense = LV_OPA_50;

    img_btn_color.text.font = &interui_20;//Symbolgr�sse in Liste img_btn !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    ext->style_img = &img_btn_color;
    ext->styles_btn[LV_BTN_STATE_REL] = &lv_style_btn_rel;
    ext->styles_btn[LV_BTN_STATE_PR] = &lv_style_btn_pr;
    ext->styles_btn[LV_BTN_STATE_TGL_REL] = &lv_style_btn_tgl_rel;
    ext->styles_btn[LV_BTN_STATE_TGL_PR] = &lv_style_btn_tgl_pr;
    ext->styles_btn[LV_BTN_STATE_INA] = &lv_style_btn_ina;
    ext->anim_time = LV_LIST_FOCUS_TIME;
    ext->single_mode = false;
    ext->size = 0;

#if USE_LV_GROUP
    ext->last_sel = NULL;
    ext->selected_btn = NULL;
#endif

    lv_obj_set_signal_func(new_list, lv_list_signal);

    /*Init the new list object*/
    if(copy == NULL) {
        lv_obj_set_size(new_list, 2 * LV_DPI, 3 * LV_DPI);
        lv_page_set_scrl_layout(new_list, LV_LIST_LAYOUT_DEF);
        lv_list_set_sb_mode(new_list, LV_SB_MODE_DRAG);

        /*Set the default styles*/
        lv_theme_t * th = lv_theme_get_current();
        if(th) {
            lv_list_set_style(new_list, LV_LIST_STYLE_BG, th->list.bg);
            lv_list_set_style(new_list, LV_LIST_STYLE_SCRL, th->list.scrl);
            lv_list_set_style(new_list, LV_LIST_STYLE_SB, th->list.sb);
            lv_list_set_style(new_list, LV_LIST_STYLE_BTN_REL, th->list.btn.rel);
            lv_list_set_style(new_list, LV_LIST_STYLE_BTN_PR, th->list.btn.pr);
            lv_list_set_style(new_list, LV_LIST_STYLE_BTN_TGL_REL, th->list.btn.tgl_rel);
            lv_list_set_style(new_list, LV_LIST_STYLE_BTN_TGL_PR, th->list.btn.tgl_pr);
            lv_list_set_style(new_list, LV_LIST_STYLE_BTN_INA, th->list.btn.ina);
        } else {
            lv_list_set_style(new_list, LV_LIST_STYLE_BG, &lv_style_transp_fit);
            lv_list_set_style(new_list, LV_LIST_STYLE_SCRL, &lv_style_pretty);
        }
    } else {
        lv_list_ext_t * copy_ext = lv_obj_get_ext_attr(copy);

        lv_obj_t * copy_btn = lv_list_get_next_btn(copy, NULL);
        while(copy_btn) {
            const void * img_src = NULL;
#if USE_LV_IMG
            lv_obj_t * copy_img = lv_list_get_btn_img(copy_btn);
            if(copy_img) img_src = lv_img_get_src(copy_img);
#endif
            lv_list_add(new_list, img_src, lv_list_get_btn_text(copy_btn), lv_btn_get_action(copy_btn, LV_BTN_ACTION_CLICK));
            copy_btn = lv_list_get_next_btn(copy, copy_btn);
        }

        lv_list_set_style(new_list, LV_LIST_STYLE_BTN_REL, copy_ext->styles_btn[LV_BTN_STATE_REL]);
        lv_list_set_style(new_list, LV_LIST_STYLE_BTN_PR, copy_ext->styles_btn[LV_BTN_STATE_PR]);
        lv_list_set_style(new_list, LV_LIST_STYLE_BTN_TGL_REL, copy_ext->styles_btn[LV_BTN_STATE_TGL_REL]);
        lv_list_set_style(new_list, LV_LIST_STYLE_BTN_TGL_PR, copy_ext->styles_btn[LV_BTN_STATE_TGL_REL]);
        lv_list_set_style(new_list, LV_LIST_STYLE_BTN_INA, copy_ext->styles_btn[LV_BTN_STATE_INA]);

        /*Refresh the style with new signal function*/
        lv_obj_refresh_style(new_list);
    }


    LV_LOG_INFO("list created");


    return new_list;
}

/**
 * Delete all children of the scrl object, without deleting scrl child.
 * @param obj pointer to an object
 */
void lv_list_clean(lv_obj_t * obj)
{
    lv_obj_t * scrl = lv_page_get_scrl(obj);
    lv_obj_clean(scrl);
    lv_list_ext_t * ext = lv_obj_get_ext_attr(obj);
    ext->size = 0;
}

/*======================
 * Add/remove functions
 *=====================*/

/**
 * Add a list element to the list
 * @param list pointer to list object
 * @param img_fn file name of an image before the text (NULL if unused)
 * @param txt text of the list element (NULL if unused)
 * @param rel_action pointer to release action function (like with lv_btn)
 * @return pointer to the new list element which can be customized (a button)
 */
lv_obj_t * lv_list_add(lv_obj_t * list, const void * img_src, const char * txt, lv_action_t rel_action)
{
    lv_style_t * style = lv_obj_get_style(list);
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
    ext->size ++;
    /*Create a list element with the image an the text*/
    lv_obj_t * liste;
    liste = lv_btn_create(list, NULL);

    /*Save the original signal function because it will be required in `lv_list_btn_signal`*/
    if(ancestor_btn_signal == NULL) ancestor_btn_signal = lv_obj_get_signal_func(liste);

    /*Set the default styles*/
    lv_btn_set_style(liste, LV_BTN_STYLE_REL, ext->styles_btn[LV_BTN_STATE_REL]);
    lv_btn_set_style(liste, LV_BTN_STYLE_PR, ext->styles_btn[LV_BTN_STATE_PR]);
    lv_btn_set_style(liste, LV_BTN_STYLE_TGL_REL, ext->styles_btn[LV_BTN_STATE_TGL_REL]);
    lv_btn_set_style(liste, LV_BTN_STYLE_TGL_PR, ext->styles_btn[LV_BTN_STATE_TGL_PR]);
    lv_btn_set_style(liste, LV_BTN_STYLE_INA, ext->styles_btn[LV_BTN_STATE_INA]);

    lv_btn_set_action(liste, LV_BTN_ACTION_CLICK, rel_action);
    lv_page_glue_obj(liste, true);
    lv_btn_set_layout(liste, LV_LAYOUT_ROW_M);
    lv_btn_set_fit(liste, false, true);
    lv_obj_set_protect(liste, LV_PROTECT_PRESS_LOST);
    lv_obj_set_signal_func(liste, lv_list_btn_signal);

    /*Make the size adjustment*/
    lv_coord_t w = lv_obj_get_width(list);
    lv_style_t  * style_scrl = lv_obj_get_style(lv_page_get_scrl(list));
    lv_coord_t pad_hor_tot = style->body.padding.hor + style_scrl->body.padding.hor;
    w -= pad_hor_tot * 2;

    lv_obj_set_width(liste, w);
#if USE_LV_IMG != 0
    lv_obj_t * img = NULL;
    if(img_src) {
        img = lv_img_create(liste, NULL);
        lv_img_set_src(img, img_src);


        //Symbol Farbe definition f�r lv_list Ordner Symbol sonst Style von oben lv_list_create Orginal anwenden!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (strcmp(img_src, SYMBOL_DIRECTORY) == 0)//Char String vergleichen und je nach dem was machen, hier wenn gleich     
        {
            // Important! Symbol Farbe definition f�r lv_list Ordner Symbol
            static lv_style_t img_btn_gelb;
            lv_style_copy(&img_btn_gelb, &lv_style_plain);
            img_btn_gelb.image.color = LV_COLOR_YELLOW;//img_btn_color.image.color = LV_COLOR_HEX(0xDDDDDD);
            img_btn_gelb.image.intense = LV_OPA_30;//img_btn_gelb.image.intense = LV_OPA_50;
            img_btn_gelb.text.font = &interui_20;//Symbolgr�sse in Liste img_btn

            lv_obj_set_style(img, &img_btn_gelb);//Style gelb anwenden
        }

        //Symbol Farbe definition f�r lv_list Ordner Symbol sonst Style von oben lv_list_create Orginal anwenden!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /*if (img_src == SYMBOL_DIRECTORY)//if (img_src == SYMBOL_DIRECTORY) ok aber Warnung "\xE0"
        {
            // Important! Symbol Farbe definition f�r lv_list Ordner Symbol
            static lv_style_t img_btn_gelb;
            lv_style_copy(&img_btn_gelb, &lv_style_plain);
            img_btn_gelb.image.color = LV_COLOR_YELLOW;//img_btn_color.image.color = LV_COLOR_HEX(0xDDDDDD);
            img_btn_gelb.image.intense = LV_OPA_50;
            img_btn_gelb.text.font = &interui_20;//Symbolgr�sse in Liste img_btn

            lv_obj_set_style(img, &img_btn_gelb);//Style gelb anwenden
        }*/

        else 
        {
            lv_obj_set_style(img, ext->style_img);//lv_obj_set_style(img, ext->style_img);//Style von oben lv_list_create Orginal anwenden
        }

        //lv_obj_set_style(img, ext->style_img);//Orginal - Style von oben lv_list_create Orginal anwenden


        lv_obj_set_click(img, false);
        if(img_signal == NULL) img_signal = lv_obj_get_signal_func(img);
    }
#endif
    if(txt != NULL) {
        lv_coord_t btn_hor_pad = ext->styles_btn[LV_BTN_STYLE_REL]->body.padding.hor;
        lv_obj_t * label = lv_label_create(liste, NULL);
        lv_label_set_text(label, txt);
        lv_obj_set_click(label, false);
        lv_label_set_long_mode(label, LV_LABEL_LONG_ROLL);
        lv_obj_set_width(label, liste->coords.x2 - label->coords.x1 - btn_hor_pad);
        if(label_signal == NULL) label_signal = lv_obj_get_signal_func(label);
    }
#if USE_LV_GROUP
    /* If this is the first item to be added to the list and the list is
     * focussed, select it */
    {
        lv_group_t *g = lv_obj_get_group(list);
        if(ext->size == 1 && lv_group_get_focused(g) == list) {
            lv_list_set_btn_selected(list, liste);
        }
    }
#endif

    return liste;
}

/**
 * Remove the index of the button in the list
 * @param list pointer to a list object
 * @param index pointer to a the button's index in the list, index must be 0 <= index < lv_list_ext_t.size
 * @return true: successfully deleted
 */
bool lv_list_remove(const lv_obj_t * list, uint32_t index)
{
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
    if(index >= ext->size) return false;
    uint32_t count = 0;
    lv_obj_t * e = lv_list_get_next_btn(list, NULL);
    while(e != NULL) {
        if(count == index) {
            lv_obj_del(e);
            ext->size --;
            return true;
        }
        e = lv_list_get_next_btn(list, e);
        count ++;
    }
    return false;
}

/*=====================
 * Setter functions
 *====================*/

/**
 * Set single button selected mode, only one button will be selected if enabled.
 * @param list pointer to the currently pressed list object
 * @param mode, enable(true)/disable(false) single selected mode.
 */
void lv_list_set_single_mode(lv_obj_t *list, bool mode)
{
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);

    ext->single_mode = mode;
}

#if USE_LV_GROUP

/**
 * Make a button selected
 * @param list pointer to a list object
 * @param btn pointer to a button to selectthe
 */
void lv_list_set_btn_selected(lv_obj_t * list, lv_obj_t * btn)
{
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);

    if(ext->selected_btn) {
        lv_btn_state_t s = lv_btn_get_state(ext->selected_btn);
        if(s == LV_BTN_STATE_PR) lv_btn_set_state(ext->selected_btn, LV_BTN_STATE_REL);
        else if(s == LV_BTN_STATE_TGL_PR) lv_btn_set_state(ext->selected_btn, LV_BTN_STATE_TGL_REL);
    }

    ext->selected_btn = btn;
    if( btn != NULL ) {
        ext->last_sel = btn;
    }

    if(ext->selected_btn) {
        lv_btn_state_t s = lv_btn_get_state(ext->selected_btn);
        if(s == LV_BTN_STATE_REL) lv_btn_set_state(ext->selected_btn, LV_BTN_STATE_PR);
        else if(s == LV_BTN_STATE_TGL_REL) lv_btn_set_state(ext->selected_btn, LV_BTN_STATE_TGL_PR);

        lv_page_focus(list, ext->selected_btn, ext->anim_time);
    }
}

#endif

/**
 * Set scroll animation duration on 'list_up()' 'list_down()' 'list_focus()'
 * @param list pointer to a list object
 * @param anim_time duration of animation [ms]
 */
void lv_list_set_anim_time(lv_obj_t * list, uint16_t anim_time)
{
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
#if USE_LV_ANIMATION == 0
    anim_time = 0;
#endif

    if(ext->anim_time == anim_time) return;
    ext->anim_time = anim_time;
}

/**
 * Set a style of a list
 * @param list pointer to a list object
 * @param type which style should be set
 * @param style pointer to a style
 */
void lv_list_set_style(lv_obj_t * list, lv_list_style_t type, lv_style_t * style)
{
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
    lv_btn_style_t btn_style_refr = LV_BTN_STYLE_REL;
    lv_obj_t * btn;

    switch(type) {
        case LV_LIST_STYLE_BG:
            lv_page_set_style(list, LV_PAGE_STYLE_BG, style);
            /*style change signal will call 'refr_btn_width' */
            break;
        case LV_LIST_STYLE_SCRL:
            lv_page_set_style(list, LV_PAGE_STYLE_SCRL, style);
            refr_btn_width(list);
            break;
        case LV_LIST_STYLE_SB:
            lv_page_set_style(list, LV_PAGE_STYLE_SB, style);
            break;
        case LV_LIST_STYLE_EDGE_FLASH:
            lv_page_set_style(list, LV_PAGE_STYLE_EDGE_FLASH, style);
            break;
        case LV_LIST_STYLE_BTN_REL:
            ext->styles_btn[LV_BTN_STATE_REL] = style;
            btn_style_refr = LV_BTN_STYLE_REL;
            break;
        case LV_LIST_STYLE_BTN_PR:
            ext->styles_btn[LV_BTN_STATE_PR] = style;
            btn_style_refr = LV_BTN_STYLE_PR;
            break;
        case LV_LIST_STYLE_BTN_TGL_REL:
            ext->styles_btn[LV_BTN_STATE_TGL_REL] = style;
            btn_style_refr = LV_BTN_STYLE_TGL_REL;
            break;
        case LV_LIST_STYLE_BTN_TGL_PR:
            ext->styles_btn[LV_BTN_STATE_TGL_PR] = style;
            btn_style_refr = LV_BTN_STYLE_TGL_PR;
            break;
        case LV_LIST_STYLE_BTN_INA:
            ext->styles_btn[LV_BTN_STATE_INA] = style;
            btn_style_refr = LV_BTN_STYLE_INA;
            break;
    }


    /*Refresh existing buttons' style*/
    if(type == LV_LIST_STYLE_BTN_PR || type == LV_LIST_STYLE_BTN_REL ||
            type == LV_LIST_STYLE_BTN_TGL_REL || type == LV_LIST_STYLE_BTN_TGL_PR ||
            type == LV_LIST_STYLE_BTN_INA) {
        btn = lv_list_get_prev_btn(list, NULL);
        while(btn != NULL) {
            lv_btn_set_style(btn, btn_style_refr, ext->styles_btn[btn_style_refr]);
            btn = lv_list_get_prev_btn(list, btn);
        }
    }
}

/*=====================
 * Getter functions
 *====================*/

/**
 * Get single button selected mode.
 * @param list pointer to the currently pressed list object.
 */
bool lv_list_get_single_mode(lv_obj_t *list)
{
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);

    return (ext->single_mode);
}

/**
 * Get the text of a list element
 * @param btn pointer to list element
 * @return pointer to the text
 */
const char * lv_list_get_btn_text(const lv_obj_t * btn)
{
    lv_obj_t * label = lv_list_get_btn_label(btn);
    if(label == NULL) return "";
    return lv_label_get_text(label);
}

/**
 * Get the label object from a list element
 * @param btn pointer to a list element (button)
 * @return pointer to the label from the list element or NULL if not found
 */
lv_obj_t * lv_list_get_btn_label(const lv_obj_t * btn)
{
    lv_obj_t * label = lv_obj_get_child(btn, NULL);
    if(label == NULL) return NULL;

    while(lv_list_is_list_label(label) == false) {
        label = lv_obj_get_child(btn, label);
        if(label == NULL) break;
    }

    return label;
}

/**
 * Get the image object from a list element
 * @param btn pointer to a list element (button)
 * @return pointer to the image from the list element or NULL if not found
 */
lv_obj_t * lv_list_get_btn_img(const lv_obj_t * btn)
{
#if USE_LV_IMG != 0
    lv_obj_t * img = lv_obj_get_child(btn, NULL);
    if(img == NULL) return NULL;

    while(lv_list_is_list_img(img) == false) {
        img = lv_obj_get_child(btn, img);
        if(img == NULL) break;
    }

    return img;
#else
    return NULL;
#endif
}

/**
 * Get the previous button from list. (Starts from the top button)
 * @param list pointer to a list object
 * @param prev_btn pointer to button. Search the previous before it.
 * @return pointer to the previous button or NULL when no more buttons
 */
lv_obj_t * lv_list_get_prev_btn(const lv_obj_t * list, lv_obj_t * prev_btn)
{
    /* Not a good practice but user can add/create objects to the lists manually.
     * When getting the next button try to be sure that it is at least a button */

    lv_obj_t * btn ;
    lv_obj_t * scrl = lv_page_get_scrl(list);

    btn = lv_obj_get_child(scrl, prev_btn);
    if(btn == NULL) return NULL;

    while(lv_list_is_list_btn(btn) == false) {
        btn = lv_obj_get_child(scrl, btn);
        if(btn == NULL) break;
    }

    return btn;
}



 /**
 * Get the next button from list. (Starts from the bottom button)
 * @param list pointer to a list object
 * @param prev_btn pointer to button. Search the next after it.
 * @return pointer to the next button or NULL when no more buttons
 */
lv_obj_t * lv_list_get_next_btn(const lv_obj_t * list, lv_obj_t * prev_btn)
{
    /* Not a good practice but user can add/create objects to the lists manually.
     * When getting the next button try to be sure that it is at least a button */

    lv_obj_t * btn ;
    lv_obj_t * scrl = lv_page_get_scrl(list);

    btn = lv_obj_get_child_back(scrl, prev_btn);
    if(btn == NULL) return NULL;

    while(lv_list_is_list_btn(btn) == false) {
        btn = lv_obj_get_child_back(scrl, btn);
        if(btn == NULL) break;
    }

    return btn;
}

/**
 * Get the index of the button in the list
 * @param list pointer to a list object. If NULL, assumes btn is part of a list.
 * @param btn pointer to a list element (button)
 * @return the index of the button in the list, or -1 of the button not in this list
 */
int32_t lv_list_get_btn_index(const lv_obj_t * list, const lv_obj_t * btn)
{
    int index = 0;
    if( list == NULL ){
        /* no list provided, assuming btn is part of a list */
        list = lv_obj_get_parent(lv_obj_get_parent(btn));
    }
    lv_obj_t * e = lv_list_get_next_btn(list, NULL);
    while(e != NULL) {
        if(e == btn) {
            return index;
        }
        index ++;
        e = lv_list_get_next_btn(list, e);
    }
    return -1;
}

/**
 * Get the number of buttons in the list
 * @param list pointer to a list object
 * @return the number of buttons in the list
 */
uint32_t lv_list_get_size(const lv_obj_t * list)
{
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
    return ext->size;
}

#if USE_LV_GROUP
/**
 * Get the currently selected button
 * @param list pointer to a list object
 * @return pointer to the selected button
 */
lv_obj_t * lv_list_get_btn_selected(const lv_obj_t * list)
{
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
    return ext->selected_btn;
}

#endif

/**
 * Get scroll animation duration
 * @param list pointer to a list object
 * @return duration of animation [ms]
 */
uint16_t lv_list_get_anim_time(const lv_obj_t * list)
{
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
    return ext->anim_time;
}

/**
 * Get a style of a list
 * @param list pointer to a list object
 * @param type which style should be get
 * @return style pointer to a style
 *  */
lv_style_t * lv_list_get_style(const lv_obj_t * list, lv_list_style_t type)
{
    lv_style_t * style = NULL;
    lv_list_ext_t * ext = lv_obj_get_ext_attr(list);

    switch(type) {
        case LV_LIST_STYLE_BG:
            style = lv_page_get_style(list, LV_PAGE_STYLE_BG);
            break;
        case LV_LIST_STYLE_SCRL:
            style = lv_page_get_style(list, LV_PAGE_STYLE_SB);
            break;
        case LV_LIST_STYLE_SB:
            style = lv_page_get_style(list, LV_PAGE_STYLE_SCRL);
            break;
        case LV_LIST_STYLE_EDGE_FLASH:
            style = lv_page_get_style(list, LV_PAGE_STYLE_EDGE_FLASH);
            break;
        case LV_LIST_STYLE_BTN_REL:
            style = ext->styles_btn[LV_BTN_STATE_REL];
            break;
        case LV_LIST_STYLE_BTN_PR:
            style = ext->styles_btn[LV_BTN_STATE_PR];
            break;
        case LV_LIST_STYLE_BTN_TGL_REL:
            style = ext->styles_btn[LV_BTN_STATE_TGL_REL];
            break;
        case LV_LIST_STYLE_BTN_TGL_PR:
            style = ext->styles_btn[LV_BTN_STATE_TGL_PR];
            break;
        case LV_LIST_STYLE_BTN_INA:
            style = ext->styles_btn[LV_BTN_STATE_INA];
            break;
        default:
            style = NULL;
            break;
    }

    return style;
}
/*=====================
 * Other functions
 *====================*/

/**
 * Move the list elements up by one
 * @param list pointer a to list object
 */
void lv_list_up(const lv_obj_t * list)
{
    /*Search the first list element which 'y' coordinate is below the parent
     * and position the list to show this element on the bottom*/
    lv_obj_t * scrl = lv_page_get_scrl(list);
    lv_obj_t * e;
    lv_obj_t * e_prev = NULL;
    e = lv_list_get_prev_btn(list, NULL);
    while(e != NULL) {
        if(e->coords.y2 <= list->coords.y2) {
            if(e_prev != NULL) {
                lv_coord_t new_y = lv_obj_get_height(list) - (lv_obj_get_y(e_prev) + lv_obj_get_height(e_prev));
                lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
                if(ext->anim_time == 0) {
                    lv_obj_set_y(scrl, new_y);
                } else {
#if USE_LV_ANIMATION
                    lv_anim_t a;
                    a.var = scrl;
                    a.start = lv_obj_get_y(scrl);
                    a.end = new_y;
                    a.fp = (lv_anim_fp_t)lv_obj_set_y;
                    a.path = lv_anim_path_linear;
                    a.end_cb = NULL;
                    a.act_time = 0;
                    a.time = LV_LIST_FOCUS_TIME;
                    a.playback = 0;
                    a.playback_pause = 0;
                    a.repeat = 0;
                    a.repeat_pause = 0;
                    lv_anim_create(&a);
#endif
                }
            }
            break;
        }
        e_prev = e;
        e = lv_list_get_prev_btn(list, e);
    }
}

/**
 * Move the list elements down by one
 * @param list pointer to a list object
 */
void lv_list_down(const lv_obj_t * list)
{
    /*Search the first list element which 'y' coordinate is above the parent
     * and position the list to show this element on the top*/
    lv_obj_t * scrl = lv_page_get_scrl(list);
    lv_obj_t * e;
    e = lv_list_get_prev_btn(list, NULL);
    while(e != NULL) {
        if(e->coords.y1 < list->coords.y1) {
            lv_coord_t new_y = -lv_obj_get_y(e);
            lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
            if(ext->anim_time == 0) {
                lv_obj_set_y(scrl, new_y);
            } else {
#if USE_LV_ANIMATION
                lv_anim_t a;
                a.var = scrl;
                a.start = lv_obj_get_y(scrl);
                a.end = new_y;
                a.fp = (lv_anim_fp_t)lv_obj_set_y;
                a.path = lv_anim_path_linear;
                a.end_cb = NULL;
                a.act_time = 0;
                a.time = LV_LIST_FOCUS_TIME;
                a.playback = 0;
                a.playback_pause = 0;
                a.repeat = 0;
                a.repeat_pause = 0;
                lv_anim_create(&a);

#endif
            }
            break;
        }
        e = lv_list_get_prev_btn(list, e);
    }
}

/**
 * Focus on a list button. It ensures that the button will be visible on the list.
 * @param btn pointer to a list button to focus
 * @param anim_en true: scroll with animation, false: without animation
 */
void lv_list_focus(const lv_obj_t * btn, bool anim_en)
{

#if USE_LV_ANIMATION == 0
    anim_en = false;
#endif

    lv_obj_t * list = lv_obj_get_parent(lv_obj_get_parent(btn));

    lv_page_focus(list, btn, anim_en == false ? 0 : lv_list_get_anim_time(list));
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Signal function of the list
 * @param list pointer to a list object
 * @param sign a signal type from lv_signal_t enum
 * @param param pointer to a signal specific variable
 * @return LV_RES_OK: the object is not deleted in the function; LV_RES_INV: the object is deleted
 */
static lv_res_t lv_list_signal(lv_obj_t * list, lv_signal_t sign, void * param)
{
    lv_res_t res;

    /* Include the ancient signal function */
    res = ancestor_page_signal(list, sign, param);
    if(res != LV_RES_OK) return res;

    if(sign == LV_SIGNAL_CORD_CHG) {
        /*Be sure the width of the buttons are correct*/
        lv_coord_t w = lv_obj_get_width(list);
        if(w != lv_area_get_width(param)) {   /*Width changed*/
            refr_btn_width(list);
        }
    } else if(sign == LV_SIGNAL_STYLE_CHG) {
        /*Because of the possible change of horizontal and vertical padding refresh buttons width */
        refr_btn_width(list);
    } else if(sign == LV_SIGNAL_FOCUS) {

#if USE_LV_GROUP
        lv_hal_indev_type_t indev_type = lv_indev_get_type(lv_indev_get_act());
        /*With ENCODER select the first button only in edit mode*/
        if(indev_type == LV_INDEV_TYPE_ENCODER) {
            lv_group_t * g = lv_obj_get_group(list);
            if(lv_group_get_editing(g)) {
                lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
                if(ext->last_sel) {
                    /* Select the    last used button */
                    lv_list_set_btn_selected(list, ext->last_sel);
                }
                else {
                    /*Get the first button and mark it as selected*/
                    lv_list_set_btn_selected(list, lv_list_get_next_btn(list, NULL));
                }
            } else {
                lv_list_set_btn_selected(list, NULL);
            }
        }
        /*Else select the clicked button*/
        else {
            /*Mark the last clicked button (if any) as selected because it triggered the focus*/
            if(last_clicked_btn) {
                lv_list_set_btn_selected(list, last_clicked_btn);
            } else {
                lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
                if(ext->last_sel) {
                    /* Select the last used button */
                    lv_list_set_btn_selected(list, ext->last_sel);
                }
                else {
                    /*Get the first button and mark it as selected*/
                    lv_list_set_btn_selected(list, lv_list_get_next_btn(list, NULL));
                }
            }
        }
#endif
    } else if(sign == LV_SIGNAL_DEFOCUS) {

#if USE_LV_GROUP
        /*De-select the selected btn*/
        lv_list_set_btn_selected(list, NULL);
        last_clicked_btn = NULL;        /*button click will be set if click happens before focus*/
        lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
        ext->selected_btn = NULL;
#endif
    } else if(sign == LV_SIGNAL_GET_EDITABLE) {
        bool * editable = (bool *)param;
        *editable = true;
    } else if(sign == LV_SIGNAL_CONTROLL) {

#if USE_LV_GROUP
        char c = *((char *)param);
        if(c == LV_GROUP_KEY_RIGHT || c == LV_GROUP_KEY_DOWN) {
            lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
            /*If there is a valid selected button the make the previous selected*/
            if(ext->selected_btn) {
                lv_obj_t * btn_prev = lv_list_get_next_btn(list, ext->selected_btn);
                if(btn_prev) lv_list_set_btn_selected(list, btn_prev);
            }
            /*If there is no selected button the make the first selected*/
            else {
                lv_obj_t * btn = lv_list_get_next_btn(list, NULL);
                if(btn) lv_list_set_btn_selected(list, btn);    /*If there are no buttons on the list then there is no first button*/
            }
        } else if(c == LV_GROUP_KEY_LEFT || c == LV_GROUP_KEY_UP) {
            lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
            /*If there is a valid selected button the make the next selected*/
            if(ext->selected_btn != NULL) {
                lv_obj_t * btn_next = lv_list_get_prev_btn(list, ext->selected_btn);
                if(btn_next) lv_list_set_btn_selected(list, btn_next);
            }
            /*If there is no selected button the make the first selected*/
            else {
                lv_obj_t * btn = lv_list_get_next_btn(list, NULL);
                if(btn) lv_list_set_btn_selected(list, btn);
            }
        } else if(c == LV_GROUP_KEY_ENTER) {
            /*Get the 'pressed' button*/
            lv_obj_t * btn = NULL;
            btn = lv_list_get_prev_btn(list, btn);
            while(btn != NULL) {
                if(lv_btn_get_state(btn) == LV_BTN_STATE_PR) break;
                btn = lv_list_get_prev_btn(list, btn);
            }

            if(btn != NULL) {
                lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
                ext->last_sel = btn;
                lv_action_t rel_action;
                rel_action = lv_btn_get_action(btn, LV_BTN_ACTION_CLICK);
                if(rel_action != NULL) rel_action(btn);
            }
        }
#endif
    } else if(sign == LV_SIGNAL_GET_TYPE) {
        lv_obj_type_t * buf = param;
        uint8_t i;
        for(i = 0; i < LV_MAX_ANCESTOR_NUM - 1; i++) {  /*Find the last set data*/
            if(buf->type[i] == NULL) break;
        }
        buf->type[i] = "lv_list";
    }
    return res;
}


/**
 * Signal function of the list buttons
 * @param btn pointer to a button on the list
 * @param sign a signal type from lv_signal_t enum
 * @param param pointer to a signal specific variable
 * @return LV_RES_OK: the object is not deleted in the function; LV_RES_INV: the object is deleted
 */
static lv_res_t lv_list_btn_signal(lv_obj_t * btn, lv_signal_t sign, void * param)
{
    lv_res_t res;

    /* Include the ancient signal function */
    res = ancestor_btn_signal(btn, sign, param);
    if(res != LV_RES_OK) return res;

    if(sign == LV_SIGNAL_RELEASED) {
        lv_obj_t * list = lv_obj_get_parent(lv_obj_get_parent(btn));
        lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
        ext->page.scroll_prop_ip = 0;

#if USE_LV_GROUP
        lv_group_t * g = lv_obj_get_group(list);
        if(lv_group_get_focused(g) == list && lv_indev_is_dragging(lv_indev_get_act()) == false) {
            /* Is the list is focused then be sure only the button being released
             * has a pressed state to indicate the selected state on the list*/
            lv_obj_t * btn_i = lv_list_get_prev_btn(list, NULL);
            while(btn_i) {
                lv_btn_state_t s = lv_btn_get_state(btn_i);
                if(s == LV_BTN_STATE_PR) lv_btn_set_state(btn_i, LV_BTN_STATE_REL);
                else if(s == LV_BTN_STATE_TGL_PR) lv_btn_set_state(btn_i, LV_BTN_STATE_TGL_REL);
                btn_i = lv_list_get_prev_btn(list, btn_i);
            }

            /*Make the released button "selected"*/
            lv_list_set_btn_selected(list, btn);
        }

        /* If `click_focus == 1` then LV_SIGNAL_FOCUS need to know which button triggered the focus
         * to mark it as selected (pressed state)*/
        last_clicked_btn = btn;
#endif
        if(lv_indev_is_dragging(lv_indev_get_act()) == false && ext->single_mode)
        {
            lv_list_btn_single_selected(btn);
        }
    }
    else if(sign == LV_SIGNAL_PRESS_LOST) {
        lv_obj_t * list = lv_obj_get_parent(lv_obj_get_parent(btn));
        lv_list_ext_t * ext = lv_obj_get_ext_attr(list);
        ext->page.scroll_prop_ip = 0;
    }
    else if(sign == LV_SIGNAL_CLEANUP) {

#if USE_LV_GROUP
        lv_obj_t * list = lv_obj_get_parent(lv_obj_get_parent(btn));
        lv_obj_t * sel = lv_list_get_btn_selected(list);
        if(sel == btn) lv_list_set_btn_selected(list, lv_list_get_next_btn(list, btn));
#endif
    }


    return res;
}

static void refr_btn_width(lv_obj_t * list)
{
    lv_style_t * style = lv_list_get_style(list, LV_LIST_STYLE_BG);
    lv_style_t * style_scrl = lv_obj_get_style(lv_page_get_scrl(list));
    lv_coord_t w = lv_obj_get_width(list);
    lv_coord_t btn_w = w - (style->body.padding.hor + style_scrl->body.padding.hor) * 2;

    lv_obj_t * btn = lv_list_get_prev_btn(list, NULL);
    while(btn) {
        /*Make the size adjustment for each buttons*/
        if(lv_obj_get_width(btn) != btn_w) {
            lv_obj_set_width(btn, btn_w);
            /*Set the label size to roll its text*/
            lv_obj_t * label = lv_list_get_btn_label(btn);
            lv_obj_set_width(label, btn->coords.x2 - label->coords.x1);
            lv_label_set_text(label, NULL);
        }
        btn = lv_list_get_prev_btn(list, btn);
    }
}

/**
 * Make a single button selected in the list, deselect others, should be called in list btns call back.
 * @param btn pointer to the currently pressed list btn object
 */
static void lv_list_btn_single_selected(lv_obj_t *btn)
{
    lv_obj_t *list = lv_obj_get_parent(lv_obj_get_parent(btn));

    lv_obj_t * e = lv_list_get_next_btn(list, NULL);
    do
    {
        if(e == btn)
        {
            lv_btn_set_state(e, LV_BTN_STATE_TGL_REL);
        }
        else
        {
            lv_btn_set_state(e, LV_BTN_STATE_REL);
        }
        e = lv_list_get_next_btn(list, e);
    } while (e != NULL);
}

/**
 * Check if this is really a list button or another object.
 * @param list_btn List button
 */
static bool lv_list_is_list_btn(lv_obj_t * list_btn)
{
    lv_obj_type_t type;

    lv_obj_get_type(list_btn, &type);
    uint8_t cnt;
    for(cnt = 0; cnt < LV_MAX_ANCESTOR_NUM; cnt++) {
        if(type.type[cnt] == NULL) break;
        if(!strcmp(type.type[cnt], "lv_btn"))
            return true;
    }
    return false;
}

/**
 * Check if this is really a list label or another object.
 * @param list_label List label
 */
static bool lv_list_is_list_label(lv_obj_t * list_label)
{
    lv_obj_type_t type;

    lv_obj_get_type(list_label, &type);
    uint8_t cnt;
    for(cnt = 0; cnt < LV_MAX_ANCESTOR_NUM; cnt++) {
        if(type.type[cnt] == NULL) break;
        if(!strcmp(type.type[cnt], "lv_label"))
            return true;
    }
    return false;
}

/**
 * Check if this is really a list image or another object.
 * @param list_image List image
 */
static bool lv_list_is_list_img(lv_obj_t * list_img)
{
    lv_obj_type_t type;

    lv_obj_get_type(list_img, &type);
    uint8_t cnt;
    for(cnt = 0; cnt < LV_MAX_ANCESTOR_NUM; cnt++) {
        if(type.type[cnt] == NULL) break;
        if(!strcmp(type.type[cnt], "lv_img"))
            return true;
    }
    return false;
}

#endif
