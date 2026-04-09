#pragma once

/* ── Setup screen (game-type selection) ─── */
void show_setup_window(void);
void hide_setup_window(void);

/* ── Main game (pitch count) screen ──────── */
void show_game_window(void);
void hide_game_window(void);
void game_window_update(void);   /* refresh labels without hiding/showing */

/* ── Score entry screen ──────────────────── */
void show_score_window(void);
void hide_score_window(void);

/* ── Game-over screen ────────────────────── */
void show_gameover_window(void);
void hide_gameover_window(void);
