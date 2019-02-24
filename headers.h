#ifndef HEADERS_H
#define HEADERS_H

extern "C" {
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}

#include"constant.h"
#include <cstdlib>
#include <ctime> 

void DrawString(SDL_Surface *screen, int x, int y, const char* text, SDL_Surface *charset);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);


void count_characters(int &number_of_lines, int &max_number_of_characters_in_row, FILE *f);
board_t** create_board(board_info_t board_info);
void initial_fulfillment(board_t** board, board_info_t board_info);
void copy_default(board_t** board, FILE *f, board_info_t board_info);

void draw(board_t** board, SDL_Surface *screen, board_info_t board_info, graphic_t graphic, refresh_t refresh, int heroFix= ANIM_FRAME_FIX);
void initialize_board(board_t** board, board_info_t board_info, char* path_to_board);
void count_dimensions(char* path_to_board, int &number_of_lines, int &max_number_of_characters_in_row);
void remove_on_screen(SDL_Surface *screen, refresh_t refresh);
void moving(board_t** board, board_info_t board_info, int move_x, int move_y, SDL_Surface *screen, graphic_t graphic, refresh_t refresh, text_t *text, myTime_t *time, colours_t colours);
int check_if_win(board_t** board, board_info_t board_info);
void draw_win(SDL_Surface *screen, int colour, int colour2, text_t *text, SDL_Surface *charset, myTime_t time);
void draw_anim(board_t** board, board_info_t board_info, SDL_Surface *screen, int move_x, int move_y, graphic_t graphic, int img_position_x, int img_position_y, refresh_t refresh, text_t *text, myTime_t *time, colours_t colours);
int check_if_upload_ok(SDL_Surface* screen, SDL_Surface* img, refresh_t refresh, SDL_Window *window);
void remove_board(board_t** board, board_info_t board_info);
void draw_timer(SDL_Surface *screen, text_t *text, graphic_t graphic, myTime_t *time, colours_t colours);
void count_time(myTime_t *time);
void zero_time(myTime_t *time);
menu_buttons_t* create_tab(int number);
void draw_selected(SDL_Surface *screen, int colour_selected, graphic_t graphic, menu_buttons_t selected);
menu_buttons_t select_in_menu(menu_buttons_t* tab, int size, int direction, int &do_user_back);
menu_buttons_t** create_tab_menu_buttons(int number);
int count_how_many_chars_in_tab(char* buffer);
void copy_chars_tab(char* buffer, char* destination);
void get_char_sdl(SDL_Event event, int &option, char &character, int &type, int &click);
char** create_double_char(int x, int y);
/*void int_to_char(int number, char *tab);
void save_backwords(char* tab);*/

#endif