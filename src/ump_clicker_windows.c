#include "ump_clicker_windows.h"
#include "ump_clicker.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_24_bold;
static GFont s_res_gothic_28_bold;
static TextLayer *s_Ball_button;
static TextLayer *s_Out_button;
static TextLayer *s_HomeAway;
static TextLayer *s_Innings_const;
static TextLayer *s_Innings;
static TextLayer *s_Balls;
static TextLayer *s_Outs;
static TextLayer *s_Strikes;
static TextLayer *s_Strikes_button;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, false);
  
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  // s_Ball_button
  s_Ball_button = text_layer_create(GRect(102, 5, 40, 28));
  text_layer_set_text(s_Ball_button, "Ball");
  text_layer_set_text_alignment(s_Ball_button, GTextAlignmentCenter);
  text_layer_set_font(s_Ball_button, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_Ball_button);
  
  // s_Out_button
  s_Out_button = text_layer_create(GRect(102, 59, 40, 28));
  text_layer_set_text(s_Out_button, "Out");
  text_layer_set_text_alignment(s_Out_button, GTextAlignmentCenter);
  text_layer_set_font(s_Out_button, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_Out_button);
  
  // s_HomeAway
  s_HomeAway = text_layer_create(GRect(6, 5, 61, 34));
  text_layer_set_text(s_HomeAway, "Away");
  text_layer_set_text_alignment(s_HomeAway, GTextAlignmentCenter);
  text_layer_set_font(s_HomeAway, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_HomeAway);
  
  // s_Innings_const
  s_Innings_const = text_layer_create(GRect(6, 53, 26, 34));
  text_layer_set_text(s_Innings_const, "In:");
  text_layer_set_text_alignment(s_Innings_const, GTextAlignmentCenter);
  text_layer_set_font(s_Innings_const, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_Innings_const);
  
  // s_Innings
  s_Innings = text_layer_create(GRect(34, 53, 29, 34));
  text_layer_set_text(s_Innings, "1");
  text_layer_set_text_alignment(s_Innings, GTextAlignmentCenter);
  text_layer_set_font(s_Innings, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_Innings);
  
  // s_Balls
  s_Balls = text_layer_create(GRect(72, 5, 26, 28));
  text_layer_set_text(s_Balls, "0");
  text_layer_set_text_alignment(s_Balls, GTextAlignmentCenter);
  text_layer_set_font(s_Balls, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_Balls);
  
  // s_Outs
  s_Outs = text_layer_create(GRect(69, 59, 29, 28));
  text_layer_set_text(s_Outs, "0");
  text_layer_set_text_alignment(s_Outs, GTextAlignmentCenter);
  text_layer_set_font(s_Outs, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_Outs);
  
  // s_Strikes
  s_Strikes = text_layer_create(GRect(62, 117, 29, 28));
  text_layer_set_text(s_Strikes, "0");
  text_layer_set_text_alignment(s_Strikes, GTextAlignmentCenter);
  text_layer_set_font(s_Strikes, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_Strikes);
  
  // s_Strikes_button
  s_Strikes_button = text_layer_create(GRect(94, 117, 47, 28));
  text_layer_set_text(s_Strikes_button, "Strike");
  text_layer_set_text_alignment(s_Strikes_button, GTextAlignmentCenter);
  text_layer_set_font(s_Strikes_button, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_Strikes_button);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_Ball_button);
  text_layer_destroy(s_Out_button);
  text_layer_destroy(s_HomeAway);
  text_layer_destroy(s_Innings_const);
  text_layer_destroy(s_Innings);
  text_layer_destroy(s_Balls);
  text_layer_destroy(s_Outs);
  text_layer_destroy(s_Strikes);
  text_layer_destroy(s_Strikes_button);
}
// END AUTO-GENERATED UI CODE

static void save_state() {
	ump_LastEnd = ump_End;
	ump_LastAway = ump_Away;
	ump_LastOuts = ump_Outs;
	ump_LastBalls = ump_Balls;
	ump_LastStrikes = ump_Strikes;
	ump_LastInnings = ump_Innings;	
}

static void restore_state() {
	ump_End = ump_LastEnd;
	ump_Away = ump_LastAway;
	ump_Outs = ump_LastOuts;
	ump_Balls = ump_LastBalls;
	ump_Strikes = ump_LastStrikes;
	ump_Innings = ump_LastInnings;
	if (ump_Away) {
		text_layer_set_text(s_HomeAway, "Away");
	} else {
		text_layer_set_text(s_HomeAway, "Home");
	}
	snprintf(ump_T_Innings, 2, "%d", ump_Innings);
	text_layer_set_text(s_Innings, ump_T_Innings);
	snprintf(ump_T_Outs, 2, "%d", ump_Outs);
	text_layer_set_text(s_Outs, ump_T_Outs);
	snprintf(ump_T_Balls, 2, "%d", ump_Balls);
	text_layer_set_text(s_Balls, ump_T_Balls);
	snprintf(ump_T_Strikes, 2, "%d", ump_Strikes);
	text_layer_set_text(s_Strikes, ump_T_Strikes);
}

static void check_outs(int outs){
	ump_Balls = 0;
	ump_Strikes = 0;
	text_layer_set_text(s_Balls, "0");
	text_layer_set_text(s_Strikes, "0");
	if (outs == 3){
		ump_Outs = 0;
		text_layer_set_text(s_Outs, "0");
		if (ump_Away){
			ump_Away = false;
			text_layer_set_text(s_HomeAway, "Home");
		} else {
			ump_Away = true;
			text_layer_set_text(s_HomeAway, "Away");
			ump_Innings++;
			snprintf(ump_T_Innings, 2, "%d", ump_Innings);
			text_layer_set_text(s_Innings, ump_T_Innings);
			if (ump_Innings > MAX_INNINGS){
				ump_End = true;
				text_layer_set_text(s_HomeAway, "TIE?");
			}
		}
	}
}


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (!ump_End){
		save_state();
		ump_Outs++;
		if (ump_Away){
			text_layer_set_text(s_HomeAway, "Away");
		} else {
			text_layer_set_text(s_HomeAway, "Home");
		}
		snprintf(ump_T_Outs, 2, "%d", ump_Outs);
		text_layer_set_text(s_Outs, ump_T_Outs);
		check_outs(ump_Outs);
	} else {
		ump_End = false;
		text_layer_set_text(s_HomeAway, "Away");
		ump_Away = true;
	}
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	restore_state();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	if(!ump_End){
		save_state();
		if (ump_Away){
			text_layer_set_text(s_HomeAway, "Away");
		} else {
			text_layer_set_text(s_HomeAway, "Home");
		}
		ump_Balls++;
		if (ump_Balls == 4){
			text_layer_set_text(s_HomeAway, "WALK");
			ump_Balls = 0;
			ump_Strikes = 0;
			text_layer_set_text(s_Balls, "0");
			text_layer_set_text(s_Strikes, "0");
		} else {
			snprintf(ump_T_Balls, 2, "%d", ump_Balls);
			text_layer_set_text(s_Balls, ump_T_Balls);
			if ((ump_Balls == 3) && (ump_Strikes == 2)){
				text_layer_set_text(s_HomeAway, "FULL");
			}
		}
	}
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (!ump_End){
		save_state();
		if (ump_Away){
			text_layer_set_text(s_HomeAway, "Away");
		} else {
			text_layer_set_text(s_HomeAway, "Home");
		}
		ump_Strikes++;
		if (ump_Strikes == 3){
			text_layer_set_text(s_HomeAway, "OUT");
			ump_Strikes = 0;
			text_layer_set_text(s_Strikes, "0");
			ump_Balls = 0;
			text_layer_set_text(s_Balls, "0");
			ump_Outs++;
			snprintf(ump_T_Outs, 2, "%d", ump_Outs);
			text_layer_set_text(s_Outs,ump_T_Outs);
			check_outs(ump_Outs);
		} else {
			snprintf(ump_T_Strikes, 2, "%d", ump_Strikes);
			text_layer_set_text(s_Strikes, ump_T_Strikes);
			if ((ump_Balls == 3) && (ump_Strikes == 2)){
				text_layer_set_text(s_HomeAway, "FULL");
			}
		}
	}
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_ump_clicker_windows(void) {
	initialise_ui();
	window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
	window_set_click_config_provider(s_window, click_config_provider);
	window_stack_push(s_window, true);
}

void hide_ump_clicker_windows(void) {
  window_stack_remove(s_window, true);
}
