#pragma once

void ps1_cardman_init(void);
int ps1_cardman_write_sector(int sector, void *buf512);
void ps1_cardman_flush(void);
void ps1_cardman_open(void);
void ps1_cardman_close(void);
int ps1_cardman_get_idx(void);
int ps1_cardman_get_channel(void);

void ps1_cardman_next_channel(void);
void ps1_cardman_prev_channel(void);
void ps1_cardman_next_idx(void);
void ps1_cardman_prev_idx(void);

void ps1_cardman_set_gameid(const char* game_id);