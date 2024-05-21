
#include "../includes/wong_kar_wai.h"

static inline int is_full(int size, int board[size][size])
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (board[i][j] == 0)
				return 0;
	return 1;
}

void generate_number(int size, int board[size][size])
{
	if (!is_full(size, board))
	{
		int i, j;
		do
		{
			i = rand() % size;
			j = rand() % size;
		} while (board[i][j] != 0);
		int value = (rand() % 10 == 0) ? 4 : 2;
		board[i][j] = value;
	}
}

static bool move_left(int size, int board[size][size])
{
	bool moved = true;
	bool res = false;
	while (moved)
	{
		moved = false;
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size - 1; j++)
				if (board[i][j] == 0 && board[i][j + 1] != 0)
				{
					board[i][j] = board[i][j + 1];
					board[i][j + 1] = 0;
					moved = true;
					res = true;
				}
	}
	return res;
}

static bool merge_left(int size, int board[size][size], t_score *score)
{
	bool res = false;

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size - 1; j++)
			if (board[i][j] == board[i][j + 1] && board[i][j] != 0)
			{
				*score += board[i][j] * 2;
				board[i][j] *= 2;
				board[i][j + 1] = 0;
				res = true;
			}
	return res;
}

static bool shift_left(int size, int board[size][size], t_score *score)
{
	bool first, second, third;
	first = move_left(size, board);
	second = merge_left(size, board, score);
	third = move_left(size, board);
	return first || second || third;
}

void rotate_board_clockwise(int size, int board[size][size])
{
	int i, j, temp;
	for (i = 0; i < size / 2; i++)
		for (j = i; j < size - i - 1; j++)
		{
			temp = board[i][j];
			board[i][j] = board[size - 1 - j][i];
			board[size - 1 - j][i] = board[size - 1 - i][size - 1 - j];
			board[size - 1 - i][size - 1 - j] = board[j][size - 1 - i];
			board[j][size - 1 - i] = temp;
		}
}

bool move_and_merge(int direction, int size, int board[size][size], t_score *score)
{
	if (direction == -1)
		return false;

	bool res = false;
	switch (direction)
	{
		case KEY_DOWN:
			rotate_board_clockwise(size, board);
			res = shift_left(size, board, score);
			rotate_board_clockwise(size, board);
			rotate_board_clockwise(size, board);
			rotate_board_clockwise(size, board);
			break;
		case KEY_LEFT:
			res = shift_left(size, board, score);
			break;
		case KEY_RIGHT:
			rotate_board_clockwise(size, board);
			rotate_board_clockwise(size, board);
			res = shift_left(size, board, score);
			rotate_board_clockwise(size, board);
			rotate_board_clockwise(size, board);
			break;
		case KEY_UP:
			rotate_board_clockwise(size, board);
			rotate_board_clockwise(size, board);
			rotate_board_clockwise(size, board);
			res = shift_left(size, board, score);
			rotate_board_clockwise(size, board);
			break;
	}
	clear();
	refresh();
	return res;
}

bool is_won(int size, int board[size][size])
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (board[i][j] == WIN_VALUE)
				return true;
	return false;
}

int game_over(int size, int board[size][size])
{
	static bool score_run = false;

	if (!score_run && is_won(size, board))
	{
		score_run = true;
		return GAME_WON;
	}
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (board[i][j] == 0)
				return GAME_CONTINUE;
			else if ((i < size - 1 && board[i][j] == board[i + 1][j])
				|| (j < size - 1 && board[i][j] == board[i][j + 1]))
				return GAME_CONTINUE;
	return GAME_LOST;
}
