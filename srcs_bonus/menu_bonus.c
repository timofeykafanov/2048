
#include "../includes/wong_kar_wai.h"
#include <ncurses.h>

void show_leaderboard(t_list *leaderboard)
{
	if (!leaderboard)
		return;

	int height = 0, width = 0;
	getmaxyx(stdscr, height, width);

	mvprintw(3, width / 2 - 8, "Leaderboard:");
	int iter = 0;
	for (t_list *elem = leaderboard; elem && iter < 5; elem = elem->next)
	{
		t_record *record = elem->content;
		mvprintw(4 + iter, width / 2 - 8, "%s: %llu", record->name, record->score);
		iter++;
	}
}

WINDOW	*init(void)
{
	int width = 0, height = 0;

	WINDOW *main_window = initscr();		// initialize ncurses and create a window
	signal(SIGWINCH, resize_handler);		// handle window resize
	noecho(); 								// don't echo any keypresses
	keypad(stdscr, TRUE); 					// enable special keys like arrow keys
	curs_set(0); 							// hide the cursor
	nodelay(main_window, TRUE);				// make getch non-blocking
	start_color();							// enable colors
	init_pair(1, COLOR_WHITE, COLOR_BLACK);

	getmaxyx(main_window, height, width);
	if (height < GRID_THREE_MIN_HEIGHT  + 10 || width < GRID_THREE_MIN_WIDTH)
	{
		endwin();
		return NULL;
	}

	return (main_window);
}

static void display_options(WINDOW *main_window, int option,
	t_list *leaderboard)
{
	int width = 0, height = 0;

	clear();
	show_leaderboard(leaderboard);
	box(main_window, 0, 0);
	getmaxyx(main_window, height, width);
	mvprintw(height / 2 - 1, width / 2 - 8, "Select game mode:");

	if (option == 3)
		attron(A_REVERSE);
	mvprintw(height / 2, width / 2 - 2, "3x3");
	attroff(A_REVERSE);

	if (option == 4)
		attron(A_REVERSE);
	mvprintw(height / 2 + 1, width / 2 - 2, "4x4");
	attroff(A_REVERSE);

	if (option == 5)
		attron(A_REVERSE);
	mvprintw(height / 2 + 2, width / 2 - 2, "5x5");
	attroff(A_REVERSE);

	refresh();
}

int input_listener(int *selected)
{
	timeout(1000);
	int ch = getch();

	if (ch == 10)
		return (SELECTED);
	else if (ch == KEY_UP)
		*selected = (*selected - 1) > 2 ? *selected - 1 : 5;
	else if (ch == KEY_DOWN)
		*selected = (*selected + 1) < 6 ? *selected + 1 : 3;
	else if (ch == KEY_ESC)
		return (*selected = 0, SELECTED);
	return (NOT_SELECTED);
}

static int select_dificulty(t_list *leaderboard)
{
	int selected = 4;
	WINDOW *main_window = init();
	endwin();
	main_window = init();
	if (main_window == NULL)
	{
		endwin();
		return 0;
	}
	display_options(main_window, selected, leaderboard);

	while (1)
	{
		if (received_signal == SIGWINCH)
		{
			endwin();
			main_window = init();
			if (main_window == NULL)
				return (0);
			received_signal = 0;
		}
		if (input_listener(&selected) == SELECTED)
			break;
		else
			display_options(main_window, selected, leaderboard);
	}
	endwin();
	return (selected);
}

static void display_input_field(WINDOW *main_window,
	char *selected, t_list *leaderboard)
{
	int width = 0, height = 0;

	clear();
	show_leaderboard(leaderboard);
	box(main_window, 0, 0);
	getmaxyx(main_window, height, width);
	mvprintw(height / 2 - 1, width / 2 - 8, "Enter your nickname:");
	mvprintw(height / 2, width / 2 - 8, "Name: %s", selected == NULL ? "" : selected);
	refresh();
}

static char	*ft_str_cat_char(char *selected, char c)
{
	char *new = ft_calloc(ft_strlen(selected) + 2, 1);

	if (!new)
		return (NULL);
	ft_strlcpy(new, selected, ft_strlen(selected) + 1);
	new[ft_strlen(selected)] = c;
	free(selected);
	return (new);
}

static char	*get_username(t_list *leaderboard)
{
	char *username = NULL;
	WINDOW *main_window = init();
	endwin();
	main_window = init();
	if (main_window == NULL)
	{
		endwin();
		return 0;
	}
	while (1)
	{
		timeout(1000);
		display_input_field(main_window, username, leaderboard);
		int ch = getch();

		if (received_signal == SIGWINCH)
		{
			clear();
			endwin();
			main_window = init();
			if (main_window == NULL)
				return 0;
			received_signal = 0;
		}
		if (ch == 10)
			break;
		else if (ch == KEY_ESC)
		{
			clear();
			endwin();
			return (NULL);
		}
		else if (ch == KEY_BACKSPACE && username && ft_strlen(username) > 0)
			username[ft_strlen(username) - 1] = '\0';
		else if (ft_isalnum(ch))
			username = ft_str_cat_char(username, ch);
		if (ft_strlen(username) >= MAX_NICKNAME_LEN)
			break;
	}
	clear();
	endwin();
	return (username);
}

int select_menu(char **username, t_list *leaderboard)
{
	*username = get_username(leaderboard);
	if (!*username || !**username)
		return (0);
	int status = select_dificulty(leaderboard);
	return (status);
}
