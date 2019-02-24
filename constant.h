#ifndef STALE_H
#define STALE_H

//SCREEN DIMENSIONS
#define SCREEN_WIDTH	1000
#define SCREEN_HEIGHT	700

//BACKGROUND COORDINATES
#define MENU_X_CORD (SCREEN_WIDTH/2)
#define MENU_Y_CORD (SCREEN_HEIGHT/2)

//MENU BUTTON VALUES
#define CHARSET_HIGHT 1

#define MENU_BUTTON_WIDTH (SCREEN_WIDTH/3)
#define MENU_BUTTON_HEIGHT (30)
#define MENU_BUTTON_X (SCREEN_WIDTH - MENU_X_CORD - MENU_BUTTON_WIDTH/2)
#define MENU_BUTTON_Y (SCREEN_HEIGHT - MENU_Y_CORD - MENU_BUTTON_HEIGHT/2)
#define MENU_STRING_Y (MENU_BUTTON_Y + MENU_BUTTON_HEIGHT/2 - 4*CHARSET_HIGHT) // substruct 4 because of charset height
#define MENU_Y_SPACE (MENU_BUTTON_HEIGHT*1.3)

#define NUMBER_OF_STRINGS 5
#define WIN_BUTTON_HEIGHT (MENU_BUTTON_HEIGHT*6)
#define WIN_BUTTON_WIDTH (MENU_BUTTON_WIDTH*1.5)
#define WIN_BUTTON_X (SCREEN_WIDTH - MENU_X_CORD - WIN_BUTTON_WIDTH/2)
#define WIN_BUTTON_Y (SCREEN_HEIGHT - MENU_Y_CORD - WIN_BUTTON_HEIGHT/2)
#define WIN_STRING_Y (WIN_BUTTON_Y + WIN_BUTTON_HEIGHT/NUMBER_OF_STRINGS/2 - CHARSET_HIGHT*NUMBER_OF_STRINGS) // substruct 4 because of charset height
#define WIN_Y_SPACE (WIN_BUTTON_HEIGHT/NUMBER_OF_STRINGS)
//BOARD INFO
#define BOARD_START_X	64
#define BOARD_START_Y	100

//ANIM_FRAMES
#define ANIM_FRAME 32
#define ANIM_FRAME_FIX (ANIM_FRAME-1)

//BOARD_FIELD INFO
#define BOARD_IMG_LENGTH_X	64
#define BOARD_IMG_HEIGHT_Y	64
#define IMG_SIZE 64
#define MULTIPLIER (IMG_SIZE/ ANIM_FRAME)

// DIFFER MOVEMENTS
#define LEFT -1
#define RIGHT 1
#define UP -1
#define DOWN 1

//ASCII CODES FOR VALUES IN THE BOARD
#define HERO 56 // 8
#define FLOOR 46 // .
#define CHEST 99 // c
#define OBJECTIVE 42 // *
#define CHEST_ON_OBJECTIVE 35 // #
#define HERO_ON_OBJECTIVE 36 // $
#define WALL 111 // o
#define OUTSIDE 32 // space

//colours, rgb values
#define PINK 0xE5, 0x24, 0x58 
#define BLACK 0x00, 0x00, 0x00
#define GREEN 0x00, 0xFF, 0x00
#define RED 0xFF, 0x00, 0x00
#define BLUE 0x11, 0x11, 0xCC
#define YELLOW 0xF2, 0xDF, 0x10
#define ORANGE 255, 187, 40

//path to file
#define PATH_SIZE 100

//MENU_VALUES
#define SELECTED 1
#define UNSELECTED 0

//MENU_OPTIONS
#define OPTIONS -1
#define SELECT_LEVEL 0
#define HIGHSCORES 1
#define CUSTOM_BOARD 2
#define EXIT 3
#define NEW_GAME 4
#define GAME_FINISHED 5
#define LEADERBOARD 6

//
#define TRUTH 1
#define FALSITY 0

//MOVING IN MENU
#define UP_MENU -1
#define DOWN_MENU 1

//PATH_FIX
#define PATH_PREFIX "./boards/"
#define PATH_SUFFIX ".txt"

//PATH_FIX
#define PATH_HIGHSCORE_PREFIX "./highscores/"
#define PATH_HIGHSCORE_SUFFIX "_leaderboard.txt"

//ASCII
#define SPACE 32
#define BACKSPACE 8
#define ENTER 13

//OPTIONS IN EDIT
#define TYPYING_NAME 2
#define SAVE_BOARD_NAME 3
#define EDIT_BOARD 4
#define DECISION 5
#define CONFIRMATION 6
//BOARD TYPE

//USER_DATA
#define USER_NAME_SIZE 21
#define MAX_STEPS 10

//MENU SIZE
#define MENU_SIZE 4

//
typedef struct 
{
	int value;
}board_t;

//GRAPHIC TYPE
typedef struct 
{
	SDL_Surface *hero, *hero_anim, *hero_anim1, *hero_anim2, *chest, *objective, *floor, *wall, *charset, *menu1;
}graphic_t;

//BOARD_INFO TYPE
typedef struct 
{
	int number_of_lines;
	int max_number_of_characters_in_row;
}board_info_t;

//REFRESH_SCREEN TYPE
typedef struct 
{
	SDL_Texture *scrtex;
	SDL_Renderer *renderer;
}refresh_t;

//COLOURS TYPE
typedef struct 
{
	int czarny;
	int zielony;
	int czerwony;
	int niebieski;
	int pink;
	int yellow;
	int white;
	int orange;
}colours_t;

typedef struct
{
	char fps_timer[128];
	char info[128];
	char name[128];
}text_t;

typedef struct
{	
	double worldTime;
	double fpsTimer;
	double fps;
	double delta;
	int t1;
	int t2;
	int frames;
	int number_of_steps;
	int final_user_steps;
	double final_user_time;

}myTime_t;

typedef struct
{
	int menu_button_y;
	int menu_string_x;
	int menu_string_y;
	char text[200];
	char path[100];
	int value;
	int if_selected;
}menu_buttons_t;

#endif
