
#include "../includes/wong_kar_wai.h"

static const char **ft_number(int n)
{
	static const char *one[5] = {" # ", "## ", " # ", " # ", "###"};
	static const char *two[5] = {"###", "  #", "###", "#  ", "###"};
	static const char *three[5] = {"###", "  #", "###", "  #", "###"};
	static const char *four[5] = {"# #", "# #", "###", "  #", "  #"};
	static const char *five[5] = {"###", "#  ", "###", "  #", "###"};
	static const char *six[5] = {"###", "#  ", "###", "# #", "###"};
	static const char *seven[5] = {"###", "  #", "  #", "  #", "  #"};
	static const char *eight[5] = {"###", "# #", "###", "# #", "###"};
	static const char *nine[5] = {"###", "# #", "###", "  #", "###"};
	static const char *zero[5] = {"###", "# #", "# #", "# #", "###"};
	static const char **numbers[10] = {zero, one, two, three, four, five, six, seven, eight, nine};

	return numbers[n % 10];
}

static void print_grid(const int size, const int nbr_len)
{
	const int limit_row = size * nbr_len * 4;
	const int limit_col = size * 6;

	for (int col = 0; col <= size; col++)
		for (int row = 0; row < limit_col; row++)
			mvprintw(row + 1, (nbr_len * 4) * col + 1, "|");

	for (int row = 0; row <= size; row++)
		for (int col = 0; col < limit_row; col++)
			mvprintw(6 * row + 1, col + 1, "-");

	for (int x = 0; x <= size; x++)
		for (int y = 0; y <= size; y++)
			mvprintw(6 * y + 1, nbr_len * 4 * x + 1, "+");
}

static inline void getxy(int col, int row, const int nbr_len, int *x, int *y)
{
	*x = nbr_len * 4 * col + 2;
	*y = 6 * row + 2;
}


static int nbrlen(int nbr)
{
	int len = 0;

	while (nbr)
	{
		nbr /= 10;
		len++;
	}
	return len;
}

static void put_digit_w(int x, int y, int nbr)
{
	const char **number = ft_number(nbr);

	for (int i = 0; i < 5; i++)
		mvprintw(y + i, x, "%s", number[i]);
}

static void put_nbr_w(int x, int y, int nbr)
{
	int len = nbrlen(nbr) - 1;

	while (nbr != 0)
	{
		put_digit_w(x + 4 * len, y, nbr % 10);
		nbr /= 10;
		len--;
	}
}

static void print_score(const int size, t_score score)
{
	int x = 0;

	if (size == 3)
		x = GRID_THREE_MIN_WIDTH / 2;
	else if (size == 4)
		x = GRID_FOUR_MIN_WIDTH / 2;
	else if (size == 5)
		x = GRID_FIVE_MIN_WIDTH / 2;

	mvprintw(1, x - 6, "Score: %llu", score);
}

int power_of_two(int nbr)
{
	int counter = 0;

	while (nbr >> counter > 0)
		counter++;
	return counter;
}

void print_board(WINDOW *win, const int size, int board[size][size], const int nbr_len, t_score score)
{
	static int counter = 0;
	int x, y;

	clear();
	wattron(win, COLOR_PAIR(counter % 5 + 1));
	box(win, 0, 0);
	print_grid(size, nbr_len);
	print_score(size, score);
	wattroff(win, COLOR_PAIR(counter % 5 + 1));

	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++)
		{
			if (board[row][col] == 0)
				continue;
			int color = power_of_two(board[row][col]) % 5 + 1;
			wattron(win, COLOR_PAIR(color));
			getxy(col, row, nbr_len, &x, &y);
			put_nbr_w(x, y, board[row][col]);
			wattroff(win, COLOR_PAIR(color));
		}

	wrefresh(win);
	counter++;
}
