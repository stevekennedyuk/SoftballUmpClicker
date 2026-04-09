#include "ump_clicker_windows.h"
#include "ump_clicker.h"
#include <pebble.h>

/* ═══════════════════════════════════════════════════════════
   HELPERS – shared across windows
   ═══════════════════════════════════════════════════════════ */

static void save_state(void) {
  ump_LastState.innings         = ump_Innings;
  ump_LastState.away            = ump_Away;
  ump_LastState.balls           = ump_Balls;
  ump_LastState.strikes         = ump_Strikes;
  ump_LastState.outs            = ump_Outs;
  ump_LastState.gameEnd         = ump_GameEnd;
  ump_LastState.awayTotal       = ump_AwayTotal;
  ump_LastState.homeTotal       = ump_HomeTotal;
  ump_LastState.inningsRecorded = ump_InningsRecorded;
  ump_LastState.screen          = ump_Screen;
  for (int i = 0; i < MAX_INNINGS_BASEBALL + MAX_EXTRA_INNINGS; i++) {
    ump_LastState.awayInning[i] = ump_AwayInning[i];
    ump_LastState.homeInning[i] = ump_HomeInning[i];
  }
  ump_HasLastState = true;
}

static void restore_state(void) {
  if (!ump_HasLastState) return;
  ump_Innings         = ump_LastState.innings;
  ump_Away            = ump_LastState.away;
  ump_Balls           = ump_LastState.balls;
  ump_Strikes         = ump_LastState.strikes;
  ump_Outs            = ump_LastState.outs;
  ump_GameEnd         = ump_LastState.gameEnd;
  ump_AwayTotal       = ump_LastState.awayTotal;
  ump_HomeTotal       = ump_LastState.homeTotal;
  ump_InningsRecorded = ump_LastState.inningsRecorded;
  for (int i = 0; i < MAX_INNINGS_BASEBALL + MAX_EXTRA_INNINGS; i++) {
    ump_AwayInning[i] = ump_LastState.awayInning[i];
    ump_HomeInning[i] = ump_LastState.homeInning[i];
  }
  /* Navigate back to the correct screen */
  AppScreen prev = ump_LastState.screen;
  if (ump_Screen != prev) {
    /* pop whatever is on top and re-push the right one */
    switch (prev) {
      case SCREEN_GAME:     hide_score_window(); game_window_update(); break;
      case SCREEN_SCORE:    /* already there or re-show */              break;
      case SCREEN_GAMEOVER: hide_gameover_window(); game_window_update(); break;
      default: break;
    }
    ump_Screen = prev;
  }
  game_window_update();
}

/* Reset pitch count (new batter) */
static void reset_pitch(void) {
  ump_Balls   = 0;
  ump_Strikes = 0;
}

/* ═══════════════════════════════════════════════════════════
   SCREEN 1 – SETUP WINDOW
   ═══════════════════════════════════════════════════════════ */

static Window     *s_setup_window;
static TextLayer  *s_setup_title;
static TextLayer  *s_setup_type;    /* "Softball 7" / "Baseball 9" */
static TextLayer  *s_setup_hint;

static void setup_update_display(void) {
  if (ump_Softball) {
    text_layer_set_text(s_setup_type, "Softball\n  7 inn.");
  } else {
    text_layer_set_text(s_setup_type, "Baseball\n  9 inn.");
  }
}

/* UP / DOWN toggle game type; SELECT starts game */
static void setup_up_click(ClickRecognizerRef recognizer, void *context) {
  ump_Softball = true;
  ump_MaxInnings = MAX_INNINGS_SOFTBALL;
  setup_update_display();
}

static void setup_down_click(ClickRecognizerRef recognizer, void *context) {
  ump_Softball = false;
  ump_MaxInnings = MAX_INNINGS_BASEBALL;
  setup_update_display();
}

static void setup_select_click(ClickRecognizerRef recognizer, void *context) {
  /* Reset all game state */
  ump_Innings         = 1;
  ump_Away            = true;
  ump_Balls           = 0;
  ump_Strikes         = 0;
  ump_Outs            = 0;
  ump_GameEnd         = false;
  ump_AwayTotal       = 0;
  ump_HomeTotal       = 0;
  ump_InningsRecorded = 0;
  ump_HasLastState    = false;
  ump_ScoreAway       = 0;
  ump_ScoreHome       = 0;
  for (int i = 0; i < MAX_INNINGS_BASEBALL + MAX_EXTRA_INNINGS; i++) {
    ump_AwayInning[i] = -1;
    ump_HomeInning[i] = -1;
  }
  ump_Screen = SCREEN_GAME;
  show_game_window();
}

static void setup_click_config(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP,     setup_up_click);
  window_single_click_subscribe(BUTTON_ID_DOWN,   setup_down_click);
  window_single_click_subscribe(BUTTON_ID_SELECT, setup_select_click);
}

static void setup_window_load(Window *window) {
  Layer *root = window_get_root_layer(window);
  GFont font24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  GFont font18 = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  GFont font14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);

  s_setup_title = text_layer_create(GRect(0, 8, 144, 28));
  text_layer_set_text(s_setup_title, "UMP CLICKER");
  text_layer_set_text_alignment(s_setup_title, GTextAlignmentCenter);
  text_layer_set_font(s_setup_title, font24);
  layer_add_child(root, (Layer *)s_setup_title);

  s_setup_type = text_layer_create(GRect(12, 48, 120, 60));
  text_layer_set_text_alignment(s_setup_type, GTextAlignmentCenter);
  text_layer_set_font(s_setup_type, font18);
  layer_add_child(root, (Layer *)s_setup_type);

  s_setup_hint = text_layer_create(GRect(4, 130, 136, 34));
  text_layer_set_text(s_setup_hint, "UP/DN: type\nSELECT: start");
  text_layer_set_text_alignment(s_setup_hint, GTextAlignmentCenter);
  text_layer_set_font(s_setup_hint, font14);
  layer_add_child(root, (Layer *)s_setup_hint);

  setup_update_display();
}

static void setup_window_unload(Window *window) {
  text_layer_destroy(s_setup_title);
  text_layer_destroy(s_setup_type);
  text_layer_destroy(s_setup_hint);
  window_destroy(s_setup_window);
  s_setup_window = NULL;
}

void show_setup_window(void) {
  ump_Screen = SCREEN_SETUP;
  s_setup_window = window_create();
  window_set_background_color(s_setup_window, GColorBlack);
  window_set_window_handlers(s_setup_window, (WindowHandlers){
    .load   = setup_window_load,
    .unload = setup_window_unload,
  });
  window_set_click_config_provider(s_setup_window, setup_click_config);
  window_stack_push(s_setup_window, true);
}

void hide_setup_window(void) {
  if (s_setup_window) window_stack_remove(s_setup_window, true);
}

/* ═══════════════════════════════════════════════════════════
   SCREEN 2 – GAME WINDOW  (pitch counter)
   ═══════════════════════════════════════════════════════════ */

static Window    *s_game_window;
static TextLayer *s_g_status;    /* top centre: "T3" / "B3" / "WALK" etc. */
static TextLayer *s_g_inning_lbl;
static TextLayer *s_g_score;     /* "A:2  H:1" */
static TextLayer *s_g_balls_lbl;
static TextLayer *s_g_balls_val;
static TextLayer *s_g_outs_lbl;
static TextLayer *s_g_outs_val;
static TextLayer *s_g_strikes_lbl;
static TextLayer *s_g_strikes_val;
static TextLayer *s_g_hint;

/* forward declarations for score window navigation */
static void enter_score_phase(void);

static void game_refresh_labels(void) {
  /* Inning label: "T3" top / "B3" bottom, or "E1" for extra */
  if (ump_Innings > ump_MaxInnings) {
    snprintf(ump_T_Innings, sizeof(ump_T_Innings), "E%d%s",
             ump_Innings - ump_MaxInnings,
             ump_Away ? "T" : "B");
  } else {
    snprintf(ump_T_Innings, sizeof(ump_T_Innings), "%s%d",
             ump_Away ? "T" : "B", ump_Innings);
  }
  text_layer_set_text(s_g_inning_lbl, ump_T_Innings);

  snprintf(ump_T_Score, sizeof(ump_T_Score), "A:%d  H:%d",
           ump_AwayTotal, ump_HomeTotal);
  text_layer_set_text(s_g_score, ump_T_Score);

  snprintf(ump_T_Balls,   sizeof(ump_T_Balls),   "%d", ump_Balls);
  snprintf(ump_T_Outs,    sizeof(ump_T_Outs),    "%d", ump_Outs);
  snprintf(ump_T_Strikes, sizeof(ump_T_Strikes), "%d", ump_Strikes);
  text_layer_set_text(s_g_balls_val,   ump_T_Balls);
  text_layer_set_text(s_g_outs_val,    ump_T_Outs);
  text_layer_set_text(s_g_strikes_val, ump_T_Strikes);

  /* restore batting team label */
  text_layer_set_text(s_g_status, ump_Away ? "Away bat" : "Home bat");
}

void game_window_update(void) {
  if (s_game_window) game_refresh_labels();
}

/* ── called when 3 outs are recorded ── */
static void handle_three_outs(void) {
  reset_pitch();
  ump_Outs = 0;

  if (ump_Away) {
    /* top half done → flip to bottom */
    ump_Away = false;
    text_layer_set_text(s_g_status, "Home bat");
    game_refresh_labels();
  } else {
    /* bottom half done → full inning complete → score entry */
    ump_Away = true;
    enter_score_phase();
  }
}

/* ── check for walk / strikeout side-effects ── */
static void check_walk(void) {
  if (ump_Balls >= 4) {
    reset_pitch();
    text_layer_set_text(s_g_status, "WALK");
    game_refresh_labels();
  }
}

static void check_strikeout(void) {
  if (ump_Strikes >= 3) {
    reset_pitch();
    ump_Outs++;
    snprintf(ump_T_Outs, sizeof(ump_T_Outs), "%d", ump_Outs);
    text_layer_set_text(s_g_outs_val, ump_T_Outs);
    if (ump_Outs >= 3) {
      text_layer_set_text(s_g_status, "3 OUTS!");
      handle_three_outs();
    } else {
      text_layer_set_text(s_g_status, "OUT");
      game_refresh_labels();
    }
  }
}

static void check_full_count(void) {
  if (ump_Balls == 3 && ump_Strikes == 2) {
    text_layer_set_text(s_g_status, "FULL");
  }
}

/* ── button handlers ── */
static void game_up_click(ClickRecognizerRef recognizer, void *context) {
  /* Ball */
  save_state();
  ump_Balls++;
  game_refresh_labels();
  check_full_count();
  check_walk();
}

static void game_select_click(ClickRecognizerRef recognizer, void *context) {
  /* Out (fielded) */
  save_state();
  reset_pitch();
  ump_Outs++;
  game_refresh_labels();
  if (ump_Outs >= 3) {
    text_layer_set_text(s_g_status, "3 OUTS!");
    handle_three_outs();
  } else {
    text_layer_set_text(s_g_status, "OUT");
  }
}

static void game_down_click(ClickRecognizerRef recognizer, void *context) {
  /* Strike */
  save_state();
  ump_Strikes++;
  game_refresh_labels();
  check_full_count();
  check_strikeout();
}

static void game_back_click(ClickRecognizerRef recognizer, void *context) {
  restore_state();
}

static void game_click_config(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP,     game_up_click);
  window_single_click_subscribe(BUTTON_ID_SELECT, game_select_click);
  window_single_click_subscribe(BUTTON_ID_DOWN,   game_down_click);
  window_single_click_subscribe(BUTTON_ID_BACK,   game_back_click);
}

static void game_window_load(Window *window) {
  Layer *root = window_get_root_layer(window);
  GFont font28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  GFont font24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  GFont font18 = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  GFont font14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);

  /* Status / batting team – top row */
  s_g_status = text_layer_create(GRect(0, 2, 144, 24));
  text_layer_set_text_alignment(s_g_status, GTextAlignmentCenter);
  text_layer_set_font(s_g_status, font18);
  layer_add_child(root, (Layer *)s_g_status);

  /* Inning label – top right */
  s_g_inning_lbl = text_layer_create(GRect(100, 2, 44, 24));
  text_layer_set_text_alignment(s_g_inning_lbl, GTextAlignmentRight);
  text_layer_set_font(s_g_inning_lbl, font18);
  layer_add_child(root, (Layer *)s_g_inning_lbl);

  /* Score line */
  s_g_score = text_layer_create(GRect(0, 26, 144, 24));
  text_layer_set_text_alignment(s_g_score, GTextAlignmentCenter);
  text_layer_set_font(s_g_score, font18);
  layer_add_child(root, (Layer *)s_g_score);

  /* ── BALLS ── */
  s_g_balls_lbl = text_layer_create(GRect(4, 58, 40, 20));
  text_layer_set_text(s_g_balls_lbl, "BALL");
  text_layer_set_font(s_g_balls_lbl, font14);
  text_layer_set_text_alignment(s_g_balls_lbl, GTextAlignmentCenter);
  layer_add_child(root, (Layer *)s_g_balls_lbl);

  s_g_balls_val = text_layer_create(GRect(4, 74, 40, 36));
  text_layer_set_text_alignment(s_g_balls_val, GTextAlignmentCenter);
  text_layer_set_font(s_g_balls_val, font28);
  layer_add_child(root, (Layer *)s_g_balls_val);

  /* ── OUTS ── */
  s_g_outs_lbl = text_layer_create(GRect(52, 58, 40, 20));
  text_layer_set_text(s_g_outs_lbl, "OUT");
  text_layer_set_font(s_g_outs_lbl, font14);
  text_layer_set_text_alignment(s_g_outs_lbl, GTextAlignmentCenter);
  layer_add_child(root, (Layer *)s_g_outs_lbl);

  s_g_outs_val = text_layer_create(GRect(52, 74, 40, 36));
  text_layer_set_text_alignment(s_g_outs_val, GTextAlignmentCenter);
  text_layer_set_font(s_g_outs_val, font28);
  layer_add_child(root, (Layer *)s_g_outs_val);

  /* ── STRIKES ── */
  s_g_strikes_lbl = text_layer_create(GRect(100, 58, 40, 20));
  text_layer_set_text(s_g_strikes_lbl, "STRK");
  text_layer_set_font(s_g_strikes_lbl, font14);
  text_layer_set_text_alignment(s_g_strikes_lbl, GTextAlignmentCenter);
  layer_add_child(root, (Layer *)s_g_strikes_lbl);

  s_g_strikes_val = text_layer_create(GRect(100, 74, 40, 36));
  text_layer_set_text_alignment(s_g_strikes_val, GTextAlignmentCenter);
  text_layer_set_font(s_g_strikes_val, font28);
  layer_add_child(root, (Layer *)s_g_strikes_val);

  /* Hint */
  s_g_hint = text_layer_create(GRect(0, 140, 144, 26));
  text_layer_set_text(s_g_hint, "UP=Ball SEL=Out DN=Strk");
  text_layer_set_font(s_g_hint, font14);
  text_layer_set_text_alignment(s_g_hint, GTextAlignmentCenter);
  layer_add_child(root, (Layer *)s_g_hint);

  game_refresh_labels();
}

static void game_window_unload(Window *window) {
  text_layer_destroy(s_g_status);
  text_layer_destroy(s_g_inning_lbl);
  text_layer_destroy(s_g_score);
  text_layer_destroy(s_g_balls_lbl);
  text_layer_destroy(s_g_balls_val);
  text_layer_destroy(s_g_outs_lbl);
  text_layer_destroy(s_g_outs_val);
  text_layer_destroy(s_g_strikes_lbl);
  text_layer_destroy(s_g_strikes_val);
  text_layer_destroy(s_g_hint);
  window_destroy(s_game_window);
  s_game_window = NULL;
}

void show_game_window(void) {
  s_game_window = window_create();
  window_set_background_color(s_game_window, GColorBlack);
  window_set_window_handlers(s_game_window, (WindowHandlers){
    .load   = game_window_load,
    .unload = game_window_unload,
  });
  window_set_click_config_provider(s_game_window, game_click_config);
  window_stack_push(s_game_window, true);
}

void hide_game_window(void) {
  if (s_game_window) window_stack_remove(s_game_window, true);
}

/* ═══════════════════════════════════════════════════════════
   SCREEN 3 – SCORE ENTRY WINDOW
   Two phases per inning: Away half, then Home half.
   UP increments, DOWN decrements, SELECT confirms, BACK skips.
   ═══════════════════════════════════════════════════════════ */

static Window    *s_score_window;
static TextLayer *s_sc_title;    /* "Inn 3 – Away" / "Inn 3 – Home" */
static TextLayer *s_sc_runs_lbl;
static TextLayer *s_sc_runs_val;
static TextLayer *s_sc_total;    /* running totals */
static TextLayer *s_sc_hint;

static void score_refresh(void) {
  /* Title */
  if (!ump_ScoreAwayDone) {
    if (ump_Innings - 1 > ump_MaxInnings) {
      snprintf(ump_T_Innings, sizeof(ump_T_Innings),
               "Extra %d – Away", ump_Innings - 1 - ump_MaxInnings);
    } else {
      snprintf(ump_T_Innings, sizeof(ump_T_Innings),
               "Inn %d – Away", ump_Innings - 1);
    }
  } else {
    if (ump_Innings - 1 > ump_MaxInnings) {
      snprintf(ump_T_Innings, sizeof(ump_T_Innings),
               "Extra %d – Home", ump_Innings - 1 - ump_MaxInnings);
    } else {
      snprintf(ump_T_Innings, sizeof(ump_T_Innings),
               "Inn %d – Home", ump_Innings - 1);
    }
  }
  text_layer_set_text(s_sc_title, ump_T_Innings);

  /* Run value */
  snprintf(ump_T_Balls, sizeof(ump_T_Balls), "%d",
           ump_ScoreAwayDone ? ump_ScoreHome : ump_ScoreAway);
  text_layer_set_text(s_sc_runs_val, ump_T_Balls);

  /* Totals */
  snprintf(ump_T_Score, sizeof(ump_T_Score), "A:%d  H:%d",
           ump_AwayTotal, ump_HomeTotal);
  text_layer_set_text(s_sc_total, ump_T_Score);
}

static void score_up_click(ClickRecognizerRef recognizer, void *context) {
  if (!ump_ScoreAwayDone) ump_ScoreAway++;
  else                     ump_ScoreHome++;
  score_refresh();
}

static void score_down_click(ClickRecognizerRef recognizer, void *context) {
  if (!ump_ScoreAwayDone) { if (ump_ScoreAway > 0) ump_ScoreAway--; }
  else                     { if (ump_ScoreHome > 0) ump_ScoreHome--; }
  score_refresh();
}

/* forward decl */
static void finish_score_entry(bool skipped);

static void score_select_click(ClickRecognizerRef recognizer, void *context) {
  /* Confirm current half */
  finish_score_entry(false);
}

static void score_back_click(ClickRecognizerRef recognizer, void *context) {
  /* Skip current half – record as -1 */
  finish_score_entry(true);
}

static void score_click_config(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP,     score_up_click);
  window_single_click_subscribe(BUTTON_ID_DOWN,   score_down_click);
  window_single_click_subscribe(BUTTON_ID_SELECT, score_select_click);
  window_single_click_subscribe(BUTTON_ID_BACK,   score_back_click);
}

static void score_window_load(Window *window) {
  Layer *root = window_get_root_layer(window);
  GFont font24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  GFont font28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  GFont font18 = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  GFont font14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);

  s_sc_title = text_layer_create(GRect(0, 4, 144, 28));
  text_layer_set_text_alignment(s_sc_title, GTextAlignmentCenter);
  text_layer_set_font(s_sc_title, font24);
  layer_add_child(root, (Layer *)s_sc_title);

  s_sc_runs_lbl = text_layer_create(GRect(0, 40, 144, 20));
  text_layer_set_text(s_sc_runs_lbl, "Runs scored:");
  text_layer_set_text_alignment(s_sc_runs_lbl, GTextAlignmentCenter);
  text_layer_set_font(s_sc_runs_lbl, font14);
  layer_add_child(root, (Layer *)s_sc_runs_lbl);

  s_sc_runs_val = text_layer_create(GRect(0, 58, 144, 40));
  text_layer_set_text_alignment(s_sc_runs_val, GTextAlignmentCenter);
  text_layer_set_font(s_sc_runs_val, font28);
  layer_add_child(root, (Layer *)s_sc_runs_val);

  s_sc_total = text_layer_create(GRect(0, 106, 144, 26));
  text_layer_set_text_alignment(s_sc_total, GTextAlignmentCenter);
  text_layer_set_font(s_sc_total, font18);
  layer_add_child(root, (Layer *)s_sc_total);

  s_sc_hint = text_layer_create(GRect(0, 140, 144, 26));
  text_layer_set_text(s_sc_hint, "UP/DN +/-  SEL=OK  BCK=Skip");
  text_layer_set_font(s_sc_hint, font14);
  text_layer_set_text_alignment(s_sc_hint, GTextAlignmentCenter);
  layer_add_child(root, (Layer *)s_sc_hint);

  score_refresh();
}

static void score_window_unload(Window *window) {
  text_layer_destroy(s_sc_title);
  text_layer_destroy(s_sc_runs_lbl);
  text_layer_destroy(s_sc_runs_val);
  text_layer_destroy(s_sc_total);
  text_layer_destroy(s_sc_hint);
  window_destroy(s_score_window);
  s_score_window = NULL;
}

void show_score_window(void) {
  ump_Screen = SCREEN_SCORE;
  s_score_window = window_create();
  window_set_background_color(s_score_window, GColorBlack);
  window_set_window_handlers(s_score_window, (WindowHandlers){
    .load   = score_window_load,
    .unload = score_window_unload,
  });
  window_set_click_config_provider(s_score_window, score_click_config);
  window_stack_push(s_score_window, true);
}

void hide_score_window(void) {
  if (s_score_window) window_stack_remove(s_score_window, true);
}

/* Called when entering score phase after 3 outs in bottom half */
static void enter_score_phase(void) {
  ump_ScoreAway     = 0;
  ump_ScoreHome     = 0;
  ump_ScoreAwayDone = false;
  show_score_window();
}

/* Process one half of score entry (away first, then home) */
static void finish_score_entry(bool skipped) {
  int inningIdx = ump_Innings - 2; /* innings was already incremented */

  if (!ump_ScoreAwayDone) {
    /* Finished entering away half */
    if (skipped) {
      ump_AwayInning[inningIdx] = -1;
    } else {
      ump_AwayInning[inningIdx] = ump_ScoreAway;
      ump_AwayTotal += ump_ScoreAway;
    }
    ump_ScoreAwayDone = true;
    ump_ScoreHome = 0;
    score_refresh();
  } else {
    /* Finished entering home half – inning complete */
    if (skipped) {
      ump_HomeInning[inningIdx] = -1;
    } else {
      ump_HomeInning[inningIdx] = ump_ScoreHome;
      ump_HomeTotal += ump_ScoreHome;
    }
    ump_InningsRecorded++;
    hide_score_window();

    /* ── Check end-of-game / extra innings ── */
    int completedInning = ump_Innings - 1;
    if (completedInning >= ump_MaxInnings) {
      if (ump_AwayTotal != ump_HomeTotal) {
        /* Game over – one team leads */
        ump_GameEnd = true;
        show_gameover_window();
        return;
      }
      /* Tied after regulation – extra inning continues */
      /* ump_Innings was already incremented; play continues */
    }

    /* Continue to next inning */
    ump_Screen = SCREEN_GAME;
    game_window_update();
  }
}

/* ═══════════════════════════════════════════════════════════
   SCREEN 4 – GAME OVER WINDOW
   ═══════════════════════════════════════════════════════════ */

static Window    *s_over_window;
static TextLayer *s_ov_title;
static TextLayer *s_ov_result;
static TextLayer *s_ov_scores;
static TextLayer *s_ov_hint;

static void over_select_click(ClickRecognizerRef recognizer, void *context) {
  /* Return to setup for a new game */
  hide_gameover_window();
  hide_game_window();
  show_setup_window();
}

static void over_click_config(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, over_select_click);
}

static void over_window_load(Window *window) {
  Layer *root = window_get_root_layer(window);
  GFont font24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  GFont font18 = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  GFont font14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);

  s_ov_title = text_layer_create(GRect(0, 8, 144, 28));
  text_layer_set_text(s_ov_title, "GAME OVER");
  text_layer_set_text_alignment(s_ov_title, GTextAlignmentCenter);
  text_layer_set_font(s_ov_title, font24);
  layer_add_child(root, (Layer *)s_ov_title);

  s_ov_result = text_layer_create(GRect(0, 44, 144, 28));
  text_layer_set_text_alignment(s_ov_result, GTextAlignmentCenter);
  text_layer_set_font(s_ov_result, font18);
  layer_add_child(root, (Layer *)s_ov_result);

  s_ov_scores = text_layer_create(GRect(0, 80, 144, 50));
  text_layer_set_text_alignment(s_ov_scores, GTextAlignmentCenter);
  text_layer_set_font(s_ov_scores, font18);
  layer_add_child(root, (Layer *)s_ov_scores);

  s_ov_hint = text_layer_create(GRect(0, 140, 144, 26));
  text_layer_set_text(s_ov_hint, "SELECT: new game");
  text_layer_set_font(s_ov_hint, font14);
  text_layer_set_text_alignment(s_ov_hint, GTextAlignmentCenter);
  layer_add_child(root, (Layer *)s_ov_hint);

  /* Populate result */
  if (ump_AwayTotal > ump_HomeTotal) {
    text_layer_set_text(s_ov_result, "Away wins!");
  } else if (ump_HomeTotal > ump_AwayTotal) {
    text_layer_set_text(s_ov_result, "Home wins!");
  } else {
    text_layer_set_text(s_ov_result, "It's a tie!");
  }

  snprintf(ump_T_Score, sizeof(ump_T_Score), "Away: %d\nHome: %d",
           ump_AwayTotal, ump_HomeTotal);
  text_layer_set_text(s_ov_scores, ump_T_Score);
}

static void over_window_unload(Window *window) {
  text_layer_destroy(s_ov_title);
  text_layer_destroy(s_ov_result);
  text_layer_destroy(s_ov_scores);
  text_layer_destroy(s_ov_hint);
  window_destroy(s_over_window);
  s_over_window = NULL;
}

void show_gameover_window(void) {
  ump_Screen = SCREEN_GAMEOVER;
  s_over_window = window_create();
  window_set_background_color(s_over_window, GColorBlack);
  window_set_window_handlers(s_over_window, (WindowHandlers){
    .load   = over_window_load,
    .unload = over_window_unload,
  });
  window_set_click_config_provider(s_over_window, over_click_config);
  window_stack_push(s_over_window, true);
}

void hide_gameover_window(void) {
  if (s_over_window) window_stack_remove(s_over_window, true);
}
