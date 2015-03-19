#pragma once
#include <pebble.h>

#define MAX_INNINGS 7 /* 7 innings per game */

static char ump_T_Balls[2] = "0";
static char ump_T_Strikes[2] = "0";
static char ump_T_Outs[2] = "0";
static char ump_T_HomeAway[5] = "Away";
static char ump_T_Innings[2] = "-";

static bool ump_End = false;
static bool ump_Away = true;

static int ump_Outs = 0;
static int ump_Balls = 0;
static int ump_Strikes = 0;
static int ump_Innings = 1;
