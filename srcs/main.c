/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfrants <vfrants@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 14:28:52 by vfrants           #+#    #+#             */
/*   Updated: 2024/05/12 20:19:28 by vfrants          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wong_kar_wai.h"

bool is_invalid_size(const int size, WINDOW *main_window)
{
	int width = 0, height = 0;

	getmaxyx(main_window, height, width);
	if ((size == 4 && (width < GRID_FOUR_MIN_WIDTH || height < GRID_FOUR_MIN_HEIGHT))
		|| (size == 3 && (width < GRID_THREE_MIN_WIDTH || height < GRID_THREE_MIN_HEIGHT))
		|| (size == 5 && (width < GRID_FIVE_MIN_WIDTH || height < GRID_FIVE_MIN_HEIGHT)))
	{
		endwin();
		ft_putendl_fd("Please resize the window\n", 2);
		return true;
	}
	return false;
}

WINDOW	*init_game_window(void)
{
	int width = 0, height = 0;

	WINDOW *main_window = initscr();	// initialize ncurses and create a window
	if(has_colors() == FALSE)
	{
		endwin();
		ft_putendl_fd("Your terminal does not support color\n", 2);
	}
	signal(SIGWINCH, resize_handler);	// handle window resize
	noecho(); 							// don't echo any keypresses
	keypad(stdscr, TRUE); 				// enable special keys like arrow keys
	curs_set(0); 						// hide the cursor
	nodelay(main_window, TRUE);			// make getch non-blocking
	start_color();						// enable colors

	for (int i = 1; i < 7; i++)
		init_pair(i, i, COLOR_BLACK);

	getmaxyx(main_window, height, width);
	return (main_window);
}

int put_won_window(WINDOW *window)
{
	int width, height;

	getmaxyx(window, width, height);
	WINDOW *won_window = newwin(5, 36, width / 2 - 2, height / 2 - 18);
	box(won_window, 0, 0);
	wattron(won_window, COLOR_PAIR(2));
	mvwprintw(won_window, 2, 2, "You won! Press Enter to continue");
	wattroff(won_window, COLOR_PAIR(2));
	wrefresh(won_window);

	while (1)
	{
		int ch = getch();
		if (ch == 10)
		{
			delwin(won_window);
			endwin();
			return GAME_CONTINUE;
		}
		else if (ch != -1)
		{
			delwin(won_window);
			endwin();
			return GAME_WON;
		}
	}
}

t_score game_loop(WINDOW *main_window,
	int size, int board[size][size], int nbr_len)
{
	t_score score = 0;
	bool moved = false;

	print_board(main_window, size, board, nbr_len, score);
	while (1)
	{
		timeout(1000); // wait for 1 second for user input (non-blocking)
		int userInput = getch();

		if (received_signal == SIGWINCH)
		{
			endwin();
			main_window = init_game_window();
			if (is_invalid_size(size, main_window))
				break ;
			received_signal = 0;
		}
		if (userInput == KEY_ESC)
			break ;
		moved = move_and_merge(userInput, size, board, &score);
		if (game_over(size, board) == GAME_WON)
			if (put_won_window(main_window) == GAME_CONTINUE)
				continue ;
			else
				break ;
		else if (game_over(size, board) == GAME_LOST)
			break ;
		else if (moved)
			generate_number(size, board);
		print_board(main_window, size, board, nbr_len, score);
	}
	return score;
}

void put_final_window(t_score score)
{
	int width = 0, height = 0;

	getmaxyx(stdscr, height, width);
	WINDOW *final_window = newwin(5, 30, height / 2 - 2, width / 2 - 15);
	box(final_window, 0, 0);
	mvwprintw(final_window, 2, 2, "Your score: %llu", score);
	wrefresh(final_window);
	for (int i = 0; i < 2000000000; i++);
	delwin(final_window);
}

int main()
{
	int 	size = 4;
	const int nbr_len = size == 3 ? SIZE_THREE : size == 4 ? SIZE_FOUR : SIZE_FIVE;
	int board[size][size];
	srand(time(NULL));
	for (int i = 0; i < size; i++)
		ft_bzero(board[i], size * sizeof(int));
	generate_number(size, board);
	generate_number(size, board);
	WINDOW *win = init_game_window();
	if (!is_invalid_size(size, win))
	{
		t_score score = game_loop(win, size, board, nbr_len);
		put_final_window(score);
		endwin();
	}
	return 0;
}
