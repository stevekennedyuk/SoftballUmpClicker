#pragma once
#include <pebble.h>

/* ── Game configuration ─────────────────────────────────── */
#define MAX_INNINGS_SOFTBALL  7
#define MAX_INNINGS_BASEBALL  9
#define MAX_EXTRA_INNINGS     9   /* safety ceiling for extras */

/* ── Screen / UI state machine ──────────────────────────── */
typedef enum {
  SCREEN_SETUP = 0,   /* game-type selection                  */
  SCREEN_GAME,        /* live pitch-count display             */
  SCREEN_SCORE,       /* score entry after each full inning   */
  SCREEN_GAMEOVER     /* final result                         */
} AppScreen;

/* ── Live game state ─────────────────────────────────────── */
static AppScreen ump_Screen = SCREEN_SETUP;

static bool ump_Softball    = true;   /* true = softball (7), false = baseball (9) */
static int  ump_MaxInnings  = MAX_INNINGS_SOFTBALL;

static int  ump_Innings     = 1;
static bool ump_Away        = true;   /* true = away batting (top), false = home (bottom) */
static int  ump_Balls       = 0;
static int  ump_Strikes     = 0;
static int  ump_Outs        = 0;
static bool ump_GameEnd     = false;

/* cumulative totals */
static int  ump_AwayTotal   = 0;
static int  ump_HomeTotal   = 0;

/* per-inning scores (index 0 = inning 1) – -1 means skipped */
static int  ump_AwayInning[MAX_INNINGS_BASEBALL + MAX_EXTRA_INNINGS];
static int  ump_HomeInning[MAX_INNINGS_BASEBALL + MAX_EXTRA_INNINGS];
static int  ump_InningsRecorded = 0;

/* score-entry scratch values */
static int  ump_ScoreAway   = 0;
static int  ump_ScoreHome   = 0;
static bool ump_ScoreAwayDone = false;  /* true once away half entered */

/* ── Undo snapshot ───────────────────────────────────────── */
typedef struct {
  int  innings;
  bool away;
  int  balls, strikes, outs;
  bool gameEnd;
  int  awayTotal, homeTotal;
  int  awayInning[MAX_INNINGS_BASEBALL + MAX_EXTRA_INNINGS];
  int  homeInning[MAX_INNINGS_BASEBALL + MAX_EXTRA_INNINGS];
  int  inningsRecorded;
  AppScreen screen;
} UmpState;

static UmpState ump_LastState;
static bool     ump_HasLastState = false;

/* ── Temp string buffers ─────────────────────────────────── */
static char ump_T_Balls[4];
static char ump_T_Strikes[4];
static char ump_T_Outs[4];
static char ump_T_Innings[8];
static char ump_T_Away[8];
static char ump_T_Home[8];
static char ump_T_Score[32];
