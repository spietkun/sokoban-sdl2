#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"headers.h"
#include <cstdlib>
#include <ctime>


// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};

// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};


void count_characters(int &number_of_lines, int &max_number_of_characters_in_row, FILE *f)
{
	int character = 0;
	int number_of_characters = 0;
	number_of_lines = 0;
	max_number_of_characters_in_row = 0;
	while (1)
	{
		character = fgetc(f);
		if (character == EOF)
		{
			if (number_of_lines != 0 || (number_of_characters!=0 && number_of_lines == 0)) // if there is sth in the file, add one more to number of lines, that will not be added in if (character == '\n')
			{
				number_of_lines++;
				if (max_number_of_characters_in_row < number_of_characters)
				{
					max_number_of_characters_in_row = number_of_characters;
				}
			}
			break;
		}
		number_of_characters++;

		if (character == '\n')
		{
			number_of_lines++;
			if (max_number_of_characters_in_row < number_of_characters)
			{
				max_number_of_characters_in_row = number_of_characters;
			}
			number_of_characters = 0;
		}
	}
}

board_t** create_board(board_info_t board_info)
{
	board_t **board = NULL;
	board = (board_t**)malloc(board_info.number_of_lines * sizeof(board_t*));
	if (board == NULL)
	{
		printf("Error: no memmory for creating a board");
	}
	for (int i = 0; i < board_info.number_of_lines; i++)
	{
		board[i] = NULL;
		board[i] = (board_t*)malloc(board_info.max_number_of_characters_in_row * sizeof(board_t));

		if (board[i] == NULL)
		{
			printf("Error: no memmory for creating a board");
		}
	}
	return board;
}

void initial_fulfillment(board_t** board, board_info_t board_info)
{
	for (int i = 0; i < board_info.number_of_lines; i++)
	{
		for (int j = 0; j < board_info.max_number_of_characters_in_row; j++)
		{
			board[i][j].value = OUTSIDE; // - means, there is nothing to show
		}
	}
}

void copy_default(board_t** board, FILE *f, board_info_t board_info)
{
	for (int i = 0; i < board_info.number_of_lines; i++)
	{
		for (int j = 0; j < board_info.max_number_of_characters_in_row; j++)
		{
			int character = fgetc(f);
			board[i][j].value = character;
			if (character == '\n' || character == EOF)
			{
				break;
			}
		}
	}
}

void draw(board_t** board, SDL_Surface *screen, board_info_t board_info, graphic_t graphic, refresh_t refresh, int heroFix)
{
	int img_length = BOARD_IMG_LENGTH_X;
	int img_height = BOARD_IMG_HEIGHT_Y;
	int img_position_x = BOARD_START_X;
	int img_position_y = BOARD_START_Y;

	for (int i = 0; i < board_info.number_of_lines; i++)
	{
		for (int j = 0; j < board_info.max_number_of_characters_in_row; j++)
		{
			switch (board[i][j].value)
			{
			case HERO:
				if(heroFix== ANIM_FRAME_FIX)
					DrawSurface(screen, graphic.hero, img_position_x, img_position_y);
				else
					DrawSurface(screen, graphic.floor, img_position_x, img_position_y);
				break;
			case HERO_ON_OBJECTIVE:
				if (heroFix == ANIM_FRAME_FIX)
					DrawSurface(screen, graphic.hero, img_position_x, img_position_y);
				else
					DrawSurface(screen, graphic.objective, img_position_x, img_position_y);
				break;
			case CHEST:
				DrawSurface(screen, graphic.chest, img_position_x, img_position_y);
				break;
			case CHEST_ON_OBJECTIVE:
				DrawSurface(screen, graphic.chest, img_position_x, img_position_y);
				break;
			case OBJECTIVE:
				DrawSurface(screen, graphic.objective, img_position_x, img_position_y);
				break;
			case FLOOR:
				DrawSurface(screen, graphic.floor, img_position_x, img_position_y);
				break;
			case WALL:
				DrawSurface(screen, graphic.wall, img_position_x, img_position_y);
				break;	
			}
			img_position_x += img_length;
		}
		img_position_x = BOARD_START_X;
		img_position_y += img_height;
	}
	
}

void initialize_board(board_t ** board, board_info_t board_info, char* path_to_board)
{
	FILE *f = fopen(path_to_board, "r");
	if (f == NULL)
	{
		perror("Nie udalo sie otworzyc pliku z planszπ");
		return;
	}
		initial_fulfillment(board, board_info);
		copy_default(board, f, board_info);
		fclose(f);
}

void count_dimensions(char* path_to_board, int &number_of_lines, int &max_number_of_characters_in_row)
{
	FILE *f = fopen(path_to_board, "r");
	if (f == NULL)
	{
		perror("Nie udalo sie otworzyc pliku z planszπ");
	}
	else
	{
		count_characters(number_of_lines, max_number_of_characters_in_row, f);
	}
	fclose(f);
}

void remove_on_screen(SDL_Surface *screen, refresh_t refresh)
{
	SDL_UpdateTexture(refresh.scrtex, NULL, screen->pixels, screen->pitch);
		//SDL_RenderClear(refresh.renderer);
	SDL_RenderCopy(refresh.renderer, refresh.scrtex, NULL, NULL);
	SDL_RenderPresent(refresh.renderer);
}

void moving(board_t** board, board_info_t board_info, int move_x, int move_y, SDL_Surface *screen, graphic_t graphic, refresh_t refresh, text_t *text, myTime_t *time, colours_t colours)
{
	int img_length = BOARD_IMG_LENGTH_X;
	int img_height = BOARD_IMG_HEIGHT_Y;
	int img_position_x = BOARD_START_X;
	int img_position_y = BOARD_START_Y;

	for (int i = 0; i < board_info.number_of_lines; i++)
	{
		for (int j = 0; j < board_info.max_number_of_characters_in_row; j++) // divide to 2 cases: when hero goes alone or pushes the chest.
		{ // check if user wants to move the hero, if there is a board in the direction he is heading to, if there is a chest in front of him
			if ((board[i][j].value == HERO || board[i][j].value == HERO_ON_OBJECTIVE) && ((i + move_y) != (board_info.number_of_lines) && (j + move_x) != (board_info.max_number_of_characters_in_row)) && ((i + 2*move_y) != (board_info.number_of_lines) && (j + 2*move_x) != (board_info.max_number_of_characters_in_row)) && (board[i + move_y][j + move_x].value == CHEST || board[i + move_y][j + move_x].value == CHEST_ON_OBJECTIVE) && (board[i + 2 * move_y][j + 2 * move_x].value == FLOOR || board[i + 2 * move_y][j + 2 * move_x].value == OBJECTIVE))
			{
				if (board[i][j].value == HERO_ON_OBJECTIVE)
				{
					board[i][j].value = OBJECTIVE;
				}
				else
				{
					board[i][j].value = FLOOR;
				}
				if (board[i + move_y][j + move_x].value == CHEST_ON_OBJECTIVE)
				{
					board[i + move_y][j + move_x].value = HERO_ON_OBJECTIVE;
				}
				else
				{
					board[i + move_y][j + move_x].value = HERO;
				}
			
				if (board[i + 2 * move_y][j + 2 * move_x].value == OBJECTIVE)
				{
					board[i + 2 * move_y][j + 2 * move_x].value = CHEST_ON_OBJECTIVE;
				}
				else
				{
					board[i + 2 * move_y][j + 2 * move_x].value = CHEST;
				}
				draw_anim(board, board_info, screen, move_x, move_y, graphic, img_position_x, img_position_y, refresh, text, time, colours);
				time->number_of_steps++;
				return;
			}
			
			else if ((board[i][j].value == HERO || board[i][j].value == HERO_ON_OBJECTIVE) && ((i + move_y) != (board_info.number_of_lines) && (j + move_x) != (board_info.max_number_of_characters_in_row)) && (board[i + move_y][j + move_x].value == FLOOR || board[i + move_y][j + move_x].value == OBJECTIVE))
			{
				if (board[i][j].value == HERO_ON_OBJECTIVE)
				{
					board[i][j].value = OBJECTIVE;
				}
				else
				{
					board[i][j].value = FLOOR;
				}
				if (board[i + move_y][j + move_x].value == OBJECTIVE)
				{
					board[i + move_y][j + move_x].value = HERO_ON_OBJECTIVE;
				}
				else 
				{
					board[i + move_y][j + move_x].value = HERO;
				}
			
				draw_anim(board, board_info, screen, move_x, move_y, graphic, img_position_x, img_position_y, refresh, text, time, colours);
				time->number_of_steps++;
				return;
			}

			img_position_x += img_length;
		}
		img_position_x = BOARD_START_X;
		img_position_y += img_height;
	}

}



int check_if_win(board_t** board, board_info_t board_info)
{
	int objective_is = 0;
	for (int i = 0; i < board_info.number_of_lines; i++)
	{
		for (int j = 0; j < board_info.max_number_of_characters_in_row; j++)
		{
			if (board[i][j].value == OBJECTIVE || board[i][j].value == HERO_ON_OBJECTIVE)
			{
				objective_is = 1;
				return objective_is;
			}
		}
	}
	return objective_is;
}

void draw_win(SDL_Surface *screen, int colour, int colour2, text_t* text, SDL_Surface *charset, myTime_t time)
{
	DrawRectangle(screen, WIN_BUTTON_X, WIN_BUTTON_Y, WIN_BUTTON_WIDTH, WIN_BUTTON_HEIGHT, colour, colour2);
	sprintf((*text).info, "nice, VICTORY!");

	int string_Y = WIN_STRING_Y;
	int string_X = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen((*text).info) * 8 / 2;
	DrawString(screen, string_X, string_Y, (*text).info, charset);
	
	sprintf((*text).info, "Time: %0.1lf s", time.final_user_time);
	string_X = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen((*text).info) * 8 / 2;
	string_Y += WIN_Y_SPACE;
	DrawString(screen, string_X, string_Y, (*text).info, charset);

	sprintf((*text).info, "Number of steps: %d", time.final_user_steps);
	string_X = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen((*text).info) * 8 / 2;
	string_Y += WIN_Y_SPACE;
	DrawString(screen, string_X, string_Y, (*text).info, charset);

	sprintf((*text).info, "Do you want to save your score in the leaderboard?");
	string_X = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen((*text).info) * 8 / 2;
	string_Y += WIN_Y_SPACE;
	DrawString(screen, string_X, string_Y, (*text).info, charset);

	sprintf((*text).info, "Y/N");
	string_X = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen((*text).info) * 8 / 2;
	string_Y += WIN_Y_SPACE;
	DrawString(screen, string_X, string_Y, (*text).info, charset);
}

void draw_anim(board_t** board, board_info_t board_info, SDL_Surface *screen, int move_x, int move_y, graphic_t graphic, int img_position_x, int img_position_y, refresh_t refresh, text_t* text, myTime_t *time, colours_t colours)
{
	int buffer_x = 0;
	int buffer_y = 0;
	if (move_y == UP)
	{
		buffer_x = 0;
		buffer_y = -1;
	}
	else if (move_y == DOWN)
	{
		buffer_x = 0;
		buffer_y = 1;
	}
	else if (move_x == LEFT)
	{
		buffer_x = -1;
		buffer_y = 0;
	}
	else if (move_x == RIGHT)
	{
		buffer_x = 1;
		buffer_y = 0;
	}
	
		for (int i = 0; i < ANIM_FRAME; i++)
		{
			count_time(time);
			if (i <= ANIM_FRAME/3) DrawSurface(screen, graphic.hero_anim1, img_position_x + buffer_x*MULTIPLIER*i, img_position_y + buffer_y*MULTIPLIER*i);
			else if (i <= ANIM_FRAME/3*2) DrawSurface(screen, graphic.hero_anim, img_position_x + buffer_x*MULTIPLIER*i, img_position_y + buffer_y*MULTIPLIER*i);
			else if (i <= ANIM_FRAME) DrawSurface(screen, graphic.hero_anim2, img_position_x + buffer_x*MULTIPLIER*i, img_position_y + buffer_y*MULTIPLIER*i);
			remove_on_screen(screen, refresh);
			draw(board, screen, board_info, graphic, refresh, i);
			draw_timer(screen, text, graphic, time, colours);
			time->frames++;
		}
		
	remove_on_screen(screen, refresh);
	return;
}
int check_if_upload_ok(SDL_Surface* screen, SDL_Surface* img, refresh_t refresh, SDL_Window *window)
{
	int if_uploaded = 0;
	if (img == NULL) {
		printf("error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(refresh.scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(refresh.renderer);
		SDL_Quit();
		if_uploaded =1; //  1 if not uploaded
	};
	return if_uploaded;
}

void remove_board(board_t** board, board_info_t board_info)
{
	if (board) {
		for (int i = 0; i < board_info.number_of_lines; i++)
		{
			free((board)[i]);
		}
		free(board);
	}
}

void draw_timer(SDL_Surface *screen, text_t* text, graphic_t graphic, myTime_t *time, colours_t colours)
{
	// tekst informacyjny / info text
	DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, colours.pink, colours.pink);
	//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
	sprintf(text->fps_timer, "Sokoban, time = %.1lf s  %.0lf frames / s", time->worldTime, time->fps);
	DrawString(screen, screen->w / 2 - strlen(text->fps_timer) * 8 / 2, 10, text->fps_timer, graphic.charset);
	//	      "Esc - exit, \030 - faster, \031 - slower"
	sprintf(text->info, "Number of steps = %d", time->number_of_steps);
	DrawString(screen, screen->w / 2 - strlen(text->info) * 8 / 2, 26, text->info, graphic.charset);
}

void count_time(myTime_t *time)
{
	time->t2 = SDL_GetTicks();
	time->delta = (time->t2 - time->t1) * 0.001;
	time->t1 = time->t2;
	time->worldTime += time->delta;


	time->fpsTimer += time->delta;
	if (time->fpsTimer > 0.5) {
		time->fps = time->frames * 2;
		time->frames = 0;
		time->fpsTimer -= 0.5;
	};
}

void zero_time(myTime_t *time)
{
	time->t1 = SDL_GetTicks();
	time->frames = 0;
	time->fpsTimer = 0;
	time->fps = 0;
	time->worldTime = 0;
	time->number_of_steps = 0;
}
void draw_menu_button(SDL_Surface *screen, text_t* text, graphic_t graphic, myTime_t *time, colours_t colours)
{
	// tekst informacyjny / info text
	DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, colours.pink, colours.pink);
	//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
	sprintf(text->fps_timer, "Sokoban, time = %.1lf s  %.0lf frames / s", time->worldTime, time->fps);
	DrawString(screen, screen->w / 2 - strlen(text->fps_timer) * 8 / 2, 10, text->fps_timer, graphic.charset);
	//	      "Esc - exit, \030 - faster, \031 - slower"
	sprintf(text->info, "Number of steps = %d", time->number_of_steps);
	DrawString(screen, screen->w / 2 - strlen(text->info) * 8 / 2, 26, text->info, graphic.charset);
}

menu_buttons_t* create_tab(int number)
{
	menu_buttons_t* tab = NULL;
	tab = (menu_buttons_t*)malloc(number * sizeof(menu_buttons_t));
	if (tab == NULL)
	{
		printf("Error: nno memory for creating a board");
	}
	return tab;
}

menu_buttons_t** create_tab_menu_buttons(int number)
{
	menu_buttons_t** tab = NULL;
	tab = (menu_buttons_t**)malloc(number * sizeof(menu_buttons_t*));
	if (tab == NULL)
	{
		printf("Error: no memory for creating a board");
	}
	return tab;
}

void draw_selected(SDL_Surface *screen, int colour_selected, graphic_t graphic, menu_buttons_t selected)
{
	if (selected.if_selected == SELECTED)
	{
		DrawRectangle(screen, MENU_BUTTON_X, selected.menu_button_y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, colour_selected, colour_selected);
		DrawString(screen, selected.menu_string_x, selected.menu_string_y, selected.text, graphic.charset);
	}

}
menu_buttons_t select_in_menu(menu_buttons_t* tab, int size, int direction, int &do_user_back)
{
	int num = 0;
	int nothing_selected = TRUTH;
	for (int i = 0; i < size; i++)
	{
		if (tab[i].if_selected == SELECTED)
		{
			if (do_user_back == FALSITY)
			{
				tab[i].if_selected = UNSELECTED;
				num = i + direction;
				if (num >= 0 && num < size)
				{
					tab[num].if_selected = SELECTED;
					nothing_selected = FALSITY;
					return tab[num];
				}
			}
			else 
			{
			do_user_back = FALSITY;
			tab[i].if_selected = UNSELECTED;
			}
		}

	}
	if (nothing_selected == TRUTH)
	{
		tab[0].if_selected = SELECTED;
		return tab[0];
	}
	tab[0].if_selected = SELECTED;
	return tab[0];
}

int count_how_many_chars_in_tab(char* buffer)
{
	int i = 0;
	int count = 0;
	while (buffer[i] != 0)
	{
		count++;
		i++;
	}
	return count;
}

void copy_chars_tab(char* buffer, char* destination)
{
	int i = 0;
	int count = 0;
	while (buffer[i] != 0)
	{
		destination[i] = buffer[i];
		i++;
	}
	destination[i] = 0;
}

void get_char_sdl(SDL_Event event, int &option, char &character, int &type, int &click)
{
	if (event.key.keysym.sym == SDLK_ESCAPE) option = EXIT;
	else if (event.key.keysym.sym == SDLK_0 && type == TYPYING_NAME) { character = '0'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_1 && type == TYPYING_NAME) { character = '1'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_2 && type == TYPYING_NAME) { character = '2'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_3 && type == TYPYING_NAME) { character = '3'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_4 && type == TYPYING_NAME) { character = '4'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_5 && type == TYPYING_NAME) { character = '5'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_6 && type == TYPYING_NAME) { character = '6'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_7 && type == TYPYING_NAME) { character = '7'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_8 && type == TYPYING_NAME) { character = '8'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_8 && type == TYPYING_NAME) { character = '9'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_8 && type == TYPYING_NAME) { character = '8'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_9 && type == TYPYING_NAME) { character = '9'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_a && type == TYPYING_NAME) { character = 'a'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_b && type == TYPYING_NAME) { character = 'b'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_c && type == TYPYING_NAME) { character = 'c'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_d && type == TYPYING_NAME) { character = 'd'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_e && type == TYPYING_NAME) { character = 'e'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_f && type == TYPYING_NAME) { character = 'f'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_g && type == TYPYING_NAME) { character = 'g'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_h && type == TYPYING_NAME) { character = 'h'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_i && type == TYPYING_NAME) { character = 'i'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_j && type == TYPYING_NAME) { character = 'j'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_k && type == TYPYING_NAME) { character = 'k'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_l && type == TYPYING_NAME) { character = 'l'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_m && type == TYPYING_NAME) { character = 'm'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_n && type == TYPYING_NAME) { character = 'n'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_o && type == TYPYING_NAME) { character = 'o'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_p && type == TYPYING_NAME) { character = 'p'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_r && type == TYPYING_NAME) { character = 'r'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_s && type == TYPYING_NAME) { character = 's'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_t && type == TYPYING_NAME) { character = 't'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_u && type == TYPYING_NAME) { character = 'u'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_w && type == TYPYING_NAME) { character = 'w'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_x && type == TYPYING_NAME) { character = 'x'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_y && type == TYPYING_NAME) { character = 'y'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_z && type == TYPYING_NAME) { character = 'z'; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_SPACE && type == TYPYING_NAME) { character = SPACE; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_BACKSPACE && type == TYPYING_NAME) { character = BACKSPACE; click = TRUTH; }
	else if (event.key.keysym.sym == SDLK_RETURN && type == TYPYING_NAME) { character = ENTER; click = TRUTH; }
}

/*void int_to_char(int number, char *tab)
{
	int i = 0;
	int buffer = 10;
	int buffer1 = 1;
	int some;
	while (i<MAX_STEPS)
	{
		some = number%buffer;
		some = some / buffer1;
		tab[i] = some;

		i++;
		buffer = buffer * 10;
		buffer1 = buffer1 * 10;
	}
}
void save_backwords (char* tab)
{
	int buffer = 0;
	int counter = 0;
	int size = count_how_many_chars_in_tab(tab);
	for (int i = 0; i < size; i++)
	{
		if (tab[size - i] != 0 || buffer == 1)
		{
			tab[counter] = tab[size - i];
			buffer = 1;
			counter++;
		}
	}
}*/
char** create_double_char(int x, int y)
{
	char **board = NULL;
	board = (char**)malloc(y * sizeof(char*));
	if (board == NULL)
	{
		printf("Error: no memmory for creating a board");
	}
	for (int i = 0; i < x; i++)
	{
		board[i] = NULL;
		board[i] = (char*)malloc(x * sizeof(char));

		if (board[i] == NULL)
		{
			printf("Error: no memmory for creating a board");
		}
	}
	return board;
}

