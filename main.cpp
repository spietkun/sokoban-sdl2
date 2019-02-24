#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <cstdlib>
#include <ctime>

extern "C" {
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}

#include <stdio.h>
#include <iostream>
#include"headers.h"


#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv)
{
	int option = OPTIONS;

	char* path;
	char path_custom[PATH_SIZE];
	
	int number_of_boards;
	int number_of_lines;
	int max_number_of_characters_in_row;
	char prefix[] = PATH_PREFIX;
	char suffix[] = PATH_SUFFIX;
	char highscores_prefix[] = PATH_HIGHSCORE_PREFIX;
	char highscores_suffix[] = PATH_HIGHSCORE_SUFFIX;
	int prefix_size = count_how_many_chars_in_tab(prefix);
	int suffix_size = count_how_many_chars_in_tab(suffix);
	int highscores_prefix_size = count_how_many_chars_in_tab(highscores_prefix);
	int highscores_suffix_size = count_how_many_chars_in_tab(highscores_suffix);

	int do_user_back = FALSITY;
	int selected_text_size; //used in FINISH_GAME
	int duration; //used in CREATE_CUSTOM
	
	char character;
	int click = FALSITY;
	int type = TYPYING_NAME; // creating a board consists of 3 types: TYPYING NAME, SAVING BOARD NAME AND EDITING BOARD
	int first_click = TRUTH; // check if it's first user's click
	char user_name[USER_NAME_SIZE] = {};
	int i = 0; // iterator
	char steps_in_char[MAX_STEPS];
	char time_in_char[MAX_STEPS];
	int steps_in_char_size;
	int time_in_char_size;
	int buf;
	char** leader_board;
	char buffer_edit_board[] = "Write the name of your board";
	char buffer_edit_usernm[] = "Write your nick";
	int buffer_size;
	int deletor;

	int rc;
	int menu = 1;
	int quit = 0;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *hero, *chest, *objective, *floor, *wall, *hero_anim1, *hero_anim2;
	SDL_Window *window;
	FILE *f;
	board_info_t files;
	menu_buttons_t* buttons;
	
	board_info_t board_info;
	board_t **board;

	refresh_t refresh;
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, // fullscreen mode
		&window, &refresh.renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(refresh.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(refresh.renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Sokoban");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	refresh.scrtex = SDL_CreateTexture(refresh.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	graphic_t graphic;
	graphic.hero = SDL_LoadBMP("./graphic/hero64.bmp");
	graphic.chest = SDL_LoadBMP("./graphic/chest64.bmp");
	graphic.objective = SDL_LoadBMP("./graphic/objective64.bmp");
	graphic.floor = SDL_LoadBMP("./graphic/floor64.bmp");
	graphic.wall = SDL_LoadBMP("./graphic/wall64.bmp");
	graphic.hero_anim1 = SDL_LoadBMP("./graphic/hero64anim1b.bmp");
	graphic.hero_anim2 = SDL_LoadBMP("./graphic/hero64anim2b.bmp");
	graphic.hero_anim = SDL_LoadBMP("./graphic/hero64animb.bmp");
	graphic.charset = SDL_LoadBMP("./cs8x8.bmp");
	graphic.menu1 = SDL_LoadBMP("./graphic/menu1.bmp");

	if (check_if_upload_ok(screen, graphic.hero, refresh, window)) return 1;
	if (check_if_upload_ok(screen, graphic.chest, refresh, window)) return 1;
	if (check_if_upload_ok(screen, graphic.objective, refresh, window)) return 1;
	if (check_if_upload_ok(screen, graphic.floor, refresh, window)) return 1;
	if (check_if_upload_ok(screen, graphic.wall, refresh, window)) return 1;
	if (check_if_upload_ok(screen, graphic.hero_anim1, refresh, window)) return 1;
	if (check_if_upload_ok(screen, graphic.hero_anim2, refresh, window)) return 1;
	if (check_if_upload_ok(screen, graphic.charset, refresh, window)) return 1;
	if (check_if_upload_ok(screen, graphic.menu1, refresh, window)) return 1;

	SDL_SetColorKey(graphic.charset, true, 0x000000);
	
	
	text_t text;
	

	colours_t colours;
	colours.czarny = SDL_MapRGB(screen->format, BLACK);
	colours.zielony = SDL_MapRGB(screen->format, GREEN);
	colours.czerwony = SDL_MapRGB(screen->format, RED);
	colours.niebieski = SDL_MapRGB(screen->format, BLUE);
	colours.pink = SDL_MapRGB(screen->format, PINK);
	colours.yellow = SDL_MapRGB(screen->format, YELLOW);
	colours.orange = SDL_MapRGB(screen->format, ORANGE);


	int number_of_menu_buttons = 4;
	int colour_selected = colours.orange;
	int colour_unselected = colours.pink;

	menu_buttons_t* menu_tab = create_tab(number_of_menu_buttons);
	

	sprintf(menu_tab[0].text, "NEW GAME");
	sprintf(menu_tab[1].text, "HIGHSCORES");
	sprintf(menu_tab[2].text, "CREATE YOUR OWN BOARD");
	sprintf(menu_tab[3].text, "EXIT");
	menu_tab[0].value = SELECT_LEVEL;
	menu_tab[1].value = HIGHSCORES;
	menu_tab[2].value = CUSTOM_BOARD;
	menu_tab[3].value = EXIT;
	
	menu_buttons_t selected;
	for (int i = 0; i < MENU_SIZE; i++)
	{
		if (i == 0)
		{
			menu_tab[i].menu_button_y = MENU_BUTTON_Y;
			menu_tab[i].menu_string_y = MENU_STRING_Y;
		}
		else
		{
			menu_tab[i].menu_button_y = menu_tab[i - 1].menu_button_y + MENU_Y_SPACE;
			menu_tab[i].menu_string_y = menu_tab[i - 1].menu_string_y + MENU_Y_SPACE;
		}
		menu_tab[i].menu_string_x = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen(menu_tab[i].text) * 8 / 2;
	}

	////////////////////////////////////////////////////////////////////////////////////////

	

	/////////////////////////////////////////////////////
	while (option != EXIT)
	{
		switch (option)
		{
		case OPTIONS:

			while (option == OPTIONS) {

				DrawSurface(screen, graphic.menu1, MENU_X_CORD, MENU_Y_CORD);
				for (int i = 0; i < MENU_SIZE; i++)
				{

					DrawRectangle(screen, MENU_BUTTON_X, menu_tab[i].menu_button_y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, colour_unselected, colour_unselected);
					DrawString(screen, menu_tab[i].menu_string_x, menu_tab[i].menu_string_y, menu_tab[i].text, graphic.charset);
				}
				draw_selected(screen, colour_selected, graphic, selected);
				remove_on_screen(screen, refresh);


				while (SDL_PollEvent(&event)) {
					switch (event.type)
					{
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) option = EXIT;
						else if (event.key.keysym.sym == SDLK_UP)
						{
							int direction = UP_MENU;
							selected = select_in_menu(menu_tab, number_of_menu_buttons, direction, do_user_back);
						}

						else if (event.key.keysym.sym == SDLK_DOWN)
						{
							int direction = DOWN_MENU;
							selected = select_in_menu(menu_tab, number_of_menu_buttons, direction, do_user_back);
						}

						else if (event.key.keysym.sym == SDLK_RETURN)
						{
							if (selected.if_selected != UNSELECTED) 
							{
								option = selected.value;
								selected.if_selected = UNSELECTED;
							}
						}
						break;
					case SDL_KEYUP:
						break;
					case SDL_QUIT:
						option = EXIT;
						break;
					};
				};
			};
			break;

		case SELECT_LEVEL:
			path = "./boards/boards_list.txt";
			count_dimensions(path, number_of_lines, max_number_of_characters_in_row);

			files.max_number_of_characters_in_row = max_number_of_characters_in_row + 1 + prefix_size + suffix_size; // "1" for 0 at the end of the string
			files.number_of_lines = number_of_lines;
			buttons = create_tab(files.number_of_lines);

			f = fopen(path, "r");
			if (f == NULL)
			{
				perror("Nie udalo sie otworzyc pliku z plansz¹");
				return 1;
			}

			for (int i = 0; i < files.number_of_lines; i++)
			{
				char character;
				buttons[i].value = NEW_GAME;
				for (int j = 0; j < files.max_number_of_characters_in_row; j++)
				{
					if (j >= prefix_size)
					{
						character = fgetc(f);
						if (character != '\n' && character != EOF)
						{
							buttons[i].path[j] = character;
							buttons[i].text[j - prefix_size] = character; //need to " - prefix_size because of iteration starting later"
						}
						else
						{
							buttons[i].text[j - prefix_size] = 0; //end of the name
							for (int k = 0; k < suffix_size; k++)
							{
								buttons[i].path[j + k] = suffix[k];
							}
							buttons[i].path[j + suffix_size] = 0; //end of the path 
							break;
						}
					}
					else
					{
						buttons[i].path[j] = prefix[j];
					}

				}


				if (i == 0)
				{
					buttons[i].menu_button_y = MENU_BUTTON_Y;
					buttons[i].menu_string_y = MENU_STRING_Y;
				}
				else
				{
					buttons[i].menu_button_y = buttons[i - 1].menu_button_y + MENU_Y_SPACE;
					buttons[i].menu_string_y = buttons[i - 1].menu_string_y + MENU_Y_SPACE;
				}
				buttons[i].menu_string_x = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen(buttons[i].text) * 8 / 2;
			}
			fclose(f);

			while (option == SELECT_LEVEL) {
				
				DrawSurface(screen, graphic.menu1, MENU_X_CORD, MENU_Y_CORD);
				for (int i = 0; i < files.number_of_lines; i++)
				{
					DrawRectangle(screen, MENU_BUTTON_X, buttons[i].menu_button_y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, colour_unselected, colour_unselected);
					DrawString(screen, buttons[i].menu_string_x, buttons[i].menu_string_y, buttons[i].text, graphic.charset);
				}
				draw_selected(screen, colour_selected, graphic, selected);
				remove_on_screen(screen, refresh);


				while (SDL_PollEvent(&event)) {
					switch (event.type)
					{
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) option = EXIT;
						else if (event.key.keysym.sym == SDLK_UP)
						{
							int direction = UP_MENU;
							selected = select_in_menu(buttons, files.number_of_lines, direction, do_user_back);
						}

						else if (event.key.keysym.sym == SDLK_DOWN)
						{
							int direction = DOWN_MENU;
							selected = select_in_menu(buttons, files.number_of_lines, direction, do_user_back);
							printf("%s", selected.text);
							printf("%d", selected.if_selected);
						}

						else if (event.key.keysym.sym == SDLK_RETURN)
						{
							if (selected.if_selected != UNSELECTED)
							{
								option = selected.value;
								selected.if_selected = UNSELECTED;
							}
						}
						else if (event.key.keysym.sym == SDLK_BACKSPACE)
						{
							option = OPTIONS;
							selected.if_selected = UNSELECTED;
							do_user_back = TRUTH;
						}
						break;
					case SDL_KEYUP:
						break;
					case SDL_QUIT:
						option = EXIT;
						break;
					};
				};

				
			}
			free(buttons);
			break;
		

		case HIGHSCORES:

			path = "./boards/boards_list.txt";
			count_dimensions(path, number_of_lines, max_number_of_characters_in_row);

			files.max_number_of_characters_in_row = max_number_of_characters_in_row + 1 + highscores_prefix_size + highscores_suffix_size; // "1" for 0 at the end of the string
			files.number_of_lines = number_of_lines;
			buttons = create_tab(files.number_of_lines);

			f = fopen(path, "r");
			if (f == NULL)
			{
				perror("Nie udalo sie otworzyc pliku z plansz¹");
				return 1;
			}

			for (int i = 0; i < files.number_of_lines; i++)
			{
				char character;
				buttons[i].value = LEADERBOARD;
				for (int j = 0; j < files.max_number_of_characters_in_row; j++)
				{
					if (j >= highscores_prefix_size)
					{
						character = fgetc(f);
						if (character != '\n' && character != EOF)
						{
							buttons[i].path[j] = character;
							buttons[i].text[j - highscores_prefix_size] = character; //need to " - highscores_prefix_size because of iteration starting later"
						}
						else
						{
							buttons[i].text[j - highscores_prefix_size] = 0; //end of the name
							for (int k = 0; k < highscores_suffix_size; k++)
							{
								buttons[i].path[j + k] = highscores_suffix[k];
							}
							buttons[i].path[j + highscores_suffix_size] = 0; //end of the path 
							break;
						}
					}
					else
					{
						buttons[i].path[j] = highscores_prefix[j];
					}

				}


				if (i == 0)
				{
					buttons[i].menu_button_y = MENU_BUTTON_Y;
					buttons[i].menu_string_y = MENU_STRING_Y;
				}
				else
				{
					buttons[i].menu_button_y = buttons[i - 1].menu_button_y + MENU_Y_SPACE;
					buttons[i].menu_string_y = buttons[i - 1].menu_string_y + MENU_Y_SPACE;
				}
				buttons[i].menu_string_x = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen(buttons[i].text) * 8 / 2;
			}
			fclose(f);
			printf("%s", buttons[0].path);
			while (option == HIGHSCORES) {

				DrawSurface(screen, graphic.menu1, MENU_X_CORD, MENU_Y_CORD);
				for (int i = 0; i < files.number_of_lines; i++)
				{
					DrawRectangle(screen, MENU_BUTTON_X, buttons[i].menu_button_y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, colour_unselected, colour_unselected);
					DrawString(screen, buttons[i].menu_string_x, buttons[i].menu_string_y, buttons[i].text, graphic.charset);
				}
				draw_selected(screen, colour_selected, graphic, selected);
				remove_on_screen(screen, refresh);


				while (SDL_PollEvent(&event)) {
					switch (event.type)
					{
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) option = EXIT;
						else if (event.key.keysym.sym == SDLK_UP)
						{
							int direction = UP_MENU;
							selected = select_in_menu(buttons, files.number_of_lines, direction, do_user_back);
						}

						else if (event.key.keysym.sym == SDLK_DOWN)
						{
							int direction = DOWN_MENU;
							selected = select_in_menu(buttons, files.number_of_lines, direction, do_user_back);
							printf("%s", selected.text);
							printf("%d", selected.if_selected);
						}

						else if (event.key.keysym.sym == SDLK_RETURN)
						{
							if (selected.if_selected != UNSELECTED)
							{
								option = selected.value;
								selected.if_selected = UNSELECTED;
							}
						}
						else if (event.key.keysym.sym == SDLK_BACKSPACE)
						{
							option = OPTIONS;
							selected.if_selected = UNSELECTED;
							do_user_back = TRUTH;
						}
						break;
					case SDL_KEYUP:
						break;
					case SDL_QUIT:
						option = EXIT;
						break;
					};
				};


			}
			free(buttons);

			
			break;


		case LEADERBOARD:

			count_dimensions(selected.path, number_of_lines, max_number_of_characters_in_row);
			files.max_number_of_characters_in_row = max_number_of_characters_in_row + 1 + prefix_size + suffix_size; // "1" for 0 at the end of the string
			files.number_of_lines = number_of_lines - 1; // SUBSTRUCT ONE BECAUSE THERE IS ALWAYS '\n' AT THE END OF THE LEADERBOARDS_LISTS
			buttons = create_tab(files.number_of_lines);
			f = fopen(selected.path, "r");
			for (int i = 0; i < files.number_of_lines; i++)
			{
				for (int j = 0; j < max_number_of_characters_in_row; j++)
				{
					character = fgetc(f);
					if (character != '\n' && character != 0 && character != EOF)
					{
						buttons[i].text[j] = character;
					}
					else
					{
						buttons[i].text[j] = 0;
						break;
					}
				}

				if (i == 0)
				{
					buttons[i].menu_button_y = MENU_BUTTON_Y;
					buttons[i].menu_string_y = MENU_STRING_Y;
				}
				else
				{
					buttons[i].menu_button_y = buttons[i - 1].menu_button_y + MENU_Y_SPACE;
					buttons[i].menu_string_y = buttons[i - 1].menu_string_y + MENU_Y_SPACE;
				}
				buttons[i].menu_string_x = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen(buttons[i].text) * 8 / 2;
			}
			while (option == LEADERBOARD)
			{
				DrawSurface(screen, graphic.menu1, MENU_X_CORD, MENU_Y_CORD);
				for (int i = 0; i < files.number_of_lines; i++)
				{
					DrawRectangle(screen, MENU_BUTTON_X, buttons[i].menu_button_y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, colour_unselected, colour_unselected);
					DrawString(screen, buttons[i].menu_string_x, buttons[i].menu_string_y, buttons[i].text, graphic.charset);
				}
				remove_on_screen(screen, refresh);
				
				while (SDL_PollEvent(&event)) {
					switch (event.type)
					{
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) option = EXIT;
					
						else if (event.key.keysym.sym == SDLK_BACKSPACE)
						{
							option = HIGHSCORES;
							selected.if_selected = UNSELECTED;
							do_user_back = TRUTH;
						}
						break;
					case SDL_KEYUP:
						break;
					case SDL_QUIT:
						option = EXIT;
						break;
					};
				};
			}
			free(buttons);
			break;

		case NEW_GAME:

			myTime_t time;
			time.t1 = SDL_GetTicks();
			time.frames = 0;
			time.fpsTimer = 0;
			time.fps = 0;
			time.worldTime = 0;
			time.number_of_steps = 0;

			path = selected.path;
			count_dimensions(path, number_of_lines, max_number_of_characters_in_row);
			
			board_info.max_number_of_characters_in_row = max_number_of_characters_in_row;
			board_info.number_of_lines = number_of_lines;
			
			board = create_board(board_info);
			initialize_board(board, board_info, path);


			while (option == NEW_GAME) {

				DrawSurface(screen, graphic.menu1, MENU_X_CORD, MENU_Y_CORD);
				count_time(&time);

				draw_timer(screen, &text, graphic, &time, colours);

				draw(board, screen, board_info, graphic, refresh);
				int objective_is = check_if_win(board, board_info); // if objective_is == 0, the game ended
				remove_on_screen(screen, refresh);

				if (objective_is == 0)
				{ 
					time.final_user_time = time.worldTime;
					time.final_user_steps = time.number_of_steps;
					option = GAME_FINISHED;
				}

				// obs³uga zdarzeñ (o ile jakieœ zasz³y) / handling of events (if there were any)

				while (SDL_PollEvent(&event)) {
					switch (event.type)
					{
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) option = EXIT;
						else if (event.key.keysym.sym == SDLK_n)
						{
							remove_board(board, board_info);
							board = create_board(board_info);
							initialize_board(board, board_info, path);
							zero_time(&time);

						}
						else if (event.key.keysym.sym == SDLK_UP)
						{
							int move_x = 0;
							int move_y = UP;
							moving(board, board_info, move_x, move_y, screen, graphic, refresh, &text, &time, colours);
						}

						else if (event.key.keysym.sym == SDLK_DOWN)
						{
							int move_x = 0;
							int move_y = DOWN;
							moving(board, board_info, move_x, move_y, screen, graphic, refresh, &text, &time, colours);
						}

						else if (event.key.keysym.sym == SDLK_LEFT)
						{
							int move_x = LEFT;
							int move_y = 0;
							moving(board, board_info, move_x, move_y, screen, graphic, refresh, &text, &time, colours);
						}

						else if (event.key.keysym.sym == SDLK_RIGHT)
						{
							int move_x = RIGHT;
							int move_y = 0;
							moving(board, board_info, move_x, move_y, screen, graphic, refresh, &text, &time, colours);
						}
						else if (event.key.keysym.sym == SDLK_BACKSPACE)
						{
							option = SELECT_LEVEL;
							selected.if_selected = UNSELECTED;
							do_user_back = TRUTH;
						}
						break;
					case SDL_KEYUP:
						break;
					case SDL_QUIT:
						option = EXIT;
						break;

					};
					time.frames++;

				};
			}
			remove_board(board, board_info);
			break;

		case GAME_FINISHED:

			draw_win(screen, colours.pink, colours.pink, &text, graphic.charset, time);
			remove_on_screen(screen, refresh);
			selected_text_size = count_how_many_chars_in_tab(selected.text);

			duration = highscores_prefix_size + selected_text_size + highscores_suffix_size;
			for (int i = 0; i < highscores_prefix_size; i++) // COPY PREFIX TO PATH
			{
				path_custom[i] = highscores_prefix[i];
			}
			for (int i = 0; i < selected_text_size; i++) // COPY NAME OF THE FILE TO PATH
			{
				path_custom[highscores_prefix_size + i] = selected.text[i];
			}
			for (int i = 0; i < highscores_suffix_size; i++) // COPY SUFFIX TO PATH
			{
				path_custom[highscores_prefix_size + selected_text_size + i] = highscores_suffix[i];
			}
			
			path_custom[duration] = NULL; // COPY 0 TO THE END OF PATH

			printf("%s", path_custom); // path to leaderboard;
			character = 0; // make sure it won't have y or n inside
			first_click = TRUTH;
			click = FALSITY;
			type = DECISION;
			i = 0;
			buffer_size = count_how_many_chars_in_tab(buffer_edit_usernm);
			copy_chars_tab(buffer_edit_usernm, user_name);
			buffer_size = count_how_many_chars_in_tab(user_name);
			deletor = 1;

			while (option == GAME_FINISHED)
			{
				if (type == TYPYING_NAME)
				{
					int string_X = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen(user_name) * 8 / 2;
					DrawSurface(screen, graphic.menu1, MENU_X_CORD, MENU_Y_CORD);

					if (type == TYPYING_NAME)
					{
						DrawRectangle(screen, MENU_BUTTON_X, MENU_BUTTON_Y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, colour_unselected, colour_unselected);
						DrawString(screen, string_X, MENU_STRING_Y, user_name, graphic.charset);
					}
					remove_on_screen(screen, refresh);
				}
				
				if (character == 'n' && click == TRUTH && first_click == TRUTH) 
				{
					option = OPTIONS;
					click = FALSITY;
					first_click = FALSITY;
				}
				else if (character == 'y' && click == TRUTH && first_click == TRUTH)
				{
					type = TYPYING_NAME;
					first_click = FALSITY;
					click = FALSITY;
				}

				if (type == TYPYING_NAME && click == TRUTH) ///
				{
					if (deletor == 1)
					{
						for (int i = 0; i < buffer_size; i++)
						{
							user_name[i] = 0;
						}
						deletor = 0;
					}
					if (i < USER_NAME_SIZE && i >= 0)
					{
						if (character != BACKSPACE && character != ENTER && (i < USER_NAME_SIZE - 1 && i >= 0))
						{
							user_name[i] = character;
							i++;
						}
						else if (character == BACKSPACE && (i < USER_NAME_SIZE && i > 0))
						{
							user_name[i-1] = 0;
							i--;
						}
						else if (character == ENTER)
						{
							user_name[i] = 0;
							type = CONFIRMATION;
						}
					}

					click = FALSITY;
				}

				if (type == CONFIRMATION) // put time, number of steps and name of the player to the board's highscores file.
				{
					fopen(path_custom, "a");
					sprintf(steps_in_char, "%d", time.final_user_steps);
					sprintf(time_in_char, "%0.1lf", time.final_user_time);
					
					printf("%s", time_in_char);
					
					steps_in_char_size = count_how_many_chars_in_tab(steps_in_char);
					time_in_char_size = count_how_many_chars_in_tab(time_in_char);

					for (int i = 0; i < steps_in_char_size; i++)
					{
						character = steps_in_char[i];
						fputc(character, f);
					}
					character = ' ';   // SPACE BETWEEN
					fputc(character, f);
					for (int i = 0; i < time_in_char_size; i++)
					{
						character = time_in_char[i];
						fputc(character, f);
					}
					character = ' '; // SPACE BETWEEN
					fputc(character, f);

					i = 0;
		
					while (user_name[i] != 0)
					{
						fputc(user_name[i], f);
						i++;
					}
					character = '\n'; // '\n' at the end of the user
					fputc(character, f);
					fclose(f);
					option = OPTIONS;
				}

				while (SDL_PollEvent(&event)) {
					switch (event.type)
					{
					case SDL_KEYDOWN:

						get_char_sdl(event, option, character, type, click);

						if (event.key.keysym.sym == SDLK_y && type == DECISION) { character = 'y'; click = TRUTH; }
						else if (event.key.keysym.sym == SDLK_n && type == DECISION) { character = 'n'; click = TRUTH; }

						break;
					case SDL_KEYUP:
						break;
					case SDL_QUIT:
						option = EXIT;
						break;
					};
				};
			}

			break;

		case CUSTOM_BOARD:
			
			f = fopen("./boards/boards_list.txt", "a");
			if (f == NULL)
			{
				perror("Nie udalo sie otworzyc pliku ");
				return 1;
			}
			char character;
			click = FALSITY;
			type = TYPYING_NAME; // creating a board consists of 3 types: TYPYING NAME, SAVING BOARD NAME AND EDITING BOARD
			first_click = TRUTH; // check if it's first user's click
			int board_created = FALSITY; // check if mallock worked
			int counter = 0;
			buffer_size = count_how_many_chars_in_tab(buffer_edit_board);
			copy_chars_tab(buffer_edit_board, text.name);

			while (option == CUSTOM_BOARD) {
				
				int string_X = MENU_BUTTON_X + (MENU_BUTTON_WIDTH / 2) - strlen(text.name) * 8 / 2;
				DrawSurface(screen, graphic.menu1, MENU_X_CORD, MENU_Y_CORD);
				if (type == TYPYING_NAME)
				{
					DrawRectangle(screen, MENU_BUTTON_X, MENU_BUTTON_Y, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, colour_unselected, colour_unselected);
					DrawString(screen, string_X, MENU_STRING_Y, text.name, graphic.charset);
				}

				
				
				
				if (click == TRUTH && type == TYPYING_NAME)
				{
					if (first_click == TRUTH)
					{
						fputc('\n', f); // add '\n' before addind name to the board
						first_click = FALSITY;
					}
					if (character != BACKSPACE)
					{
						fputc(character, f);

						text.name[counter] = character;
						text.name[counter + 1] = 0;
						counter++;

						click = FALSITY;
					}

				}

				else if (type == SAVE_BOARD_NAME)
				{
					duration = highscores_prefix_size + counter + highscores_suffix_size;
					for (int i = 0; i < highscores_prefix_size; i++) // COPY PREFIX TO PATH
					{
						path_custom[i] = highscores_prefix[i];
					}
					for (int i = 0; i < counter; i++) // COPY NAME OF THE FILE TO PATH
					{
						path_custom[highscores_prefix_size + i] = text.name[i];
					}
					for (int i = 0; i < highscores_suffix_size; i++) // COPY SUFFIX TO PATH
					{
						path_custom[highscores_prefix_size + counter + i] = highscores_suffix[i];
					}
					path_custom[duration] = NULL; // COPY 0 TO THE END OF PATH

					f = fopen(path_custom, "a"); // create leaderboard file
					fclose(f);

					duration = prefix_size + counter + suffix_size;
					for (int i = 0; i < prefix_size; i++) // COPY PREFIX TO PATH
					{
						path_custom[i] = prefix[i];
					}
					for (int i = 0; i < counter; i++) // COPY NAME OF THE FILE TO PATH
					{
						path_custom[prefix_size + i] = text.name[i];
					}
					for (int i = 0; i < suffix_size; i++) // COPY SUFFIX TO PATH
					{
						path_custom[prefix_size + counter + i] = suffix[i];
					}
					path_custom[duration] = NULL; // COPY 0 TO THE END OF PATH

					f = fopen(path_custom, "w+");
					fclose(f);
					type = EDIT_BOARD;
				}

				else if (click == TRUTH && type == EDIT_BOARD)
				{
					f = fopen(path_custom, "a");
					fputc(character, f);
					fclose(f);
				}

				if (type == EDIT_BOARD && click == TRUTH)
				{
					count_dimensions(path_custom, number_of_lines, max_number_of_characters_in_row);
					board_info.max_number_of_characters_in_row = max_number_of_characters_in_row;
					board_info.number_of_lines = number_of_lines;
					board = create_board(board_info);
					initialize_board(board, board_info, path_custom);
					printf("%d", number_of_lines);
					printf("  %d", max_number_of_characters_in_row);
					board_created = TRUTH;
					click = FALSITY;
				}
				if (type == EDIT_BOARD)
				{
					if (board_created == TRUTH)
					{
						draw(board, screen, board_info, graphic, refresh);
					}
				}
				remove_on_screen(screen, refresh);

				while (SDL_PollEvent(&event)) {
					switch (event.type)
					{
					case SDL_KEYDOWN:
						
						get_char_sdl(event, option, character, type, click);

						if (event.key.keysym.sym == SDLK_RETURN && type == TYPYING_NAME) { type = SAVE_BOARD_NAME; fclose(f); click = FALSITY; }

						else if (event.key.keysym.sym == SDLK_o && type == EDIT_BOARD) { character = 'o'; click = TRUTH; if (board_created == TRUTH) { remove_board(board, board_info); } }
						else if (event.key.keysym.sym == SDLK_8 && type == EDIT_BOARD) { character = '8'; click = TRUTH; if (board_created == TRUTH) { remove_board(board, board_info); } }
						else if (event.key.keysym.sym == SDLK_c && type == EDIT_BOARD) { character = 'c'; click = TRUTH; if (board_created == TRUTH) { remove_board(board, board_info); } }
						else if (event.key.keysym.sym == SDLK_o && type == EDIT_BOARD) { character = 'o'; click = TRUTH; if (board_created == TRUTH) { remove_board(board, board_info); } }
						else if (event.key.keysym.sym == SDLK_o && type == EDIT_BOARD) { character = 'o'; click = TRUTH; if (board_created == TRUTH) { remove_board(board, board_info); } }
						else if (event.key.keysym.sym == SDLK_d && type == EDIT_BOARD) { character = '*'; click = TRUTH; if (board_created == TRUTH) { remove_board(board, board_info); } }
						else if (event.key.keysym.sym == SDLK_DOWN && type == EDIT_BOARD) { character = '\n'; click = TRUTH; if (board_created ==TRUTH) { remove_board(board, board_info); } }
						else if (event.key.keysym.sym == SDLK_e && type == EDIT_BOARD) { option = OPTIONS; fclose(f); if (board_created == TRUTH) { remove_board(board, board_info); } }
						else if (event.key.keysym.sym == SDLK_f && type == EDIT_BOARD) { character = '.'; click = TRUTH; if (board_created == TRUTH) { remove_board(board, board_info); } }

						else if (event.key.keysym.sym == SDLK_DOWN)
						{
							
						}

						
						
						break;
					case SDL_KEYUP:
						break;
					case SDL_QUIT:
						option = EXIT;
						break;
					};
				};

			}

			break;


		}
	}
	
		// zwolnienie powierzchni / freeing all surfaces
		SDL_FreeSurface(graphic.charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(refresh.scrtex);
		SDL_DestroyRenderer(refresh.renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();

	
    return 0;
}

