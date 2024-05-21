
#ifndef WONG_KAR_WAI_H
# define WONG_KAR_WAI_H

# include "../libft/libft.h"

# include <ncurses.h>
# include <stdbool.h>
# include <signal.h>
# include <time.h>

# define GRID_THREE_MIN_WIDTH 39
# define GRID_THREE_MIN_HEIGHT 19
# define GRID_FOUR_MIN_WIDTH 83
# define GRID_FOUR_MIN_HEIGHT 27
# define GRID_FIVE_MIN_WIDTH 163
# define GRID_FIVE_MIN_HEIGHT 33

enum e_const
{
    WIN_VALUE = 2048
};

# define ERROR "An error occurred\n"
# define KEY_ESC 27
# define LEADERBOARD_FILE "leaderboard.txt"
# define MAX_NICKNAME_LEN 10
# define MAX_RECORD_LEN 30

typedef unsigned long long   t_score;

typedef struct  s_record
{
    char    *name;
    t_score score;
}   t_record;

typedef enum e_number_len
{
    SIZE_THREE = 3,
    SIZE_FOUR = 5,
    SIZE_FIVE = 8,
}  t_number_len;

typedef enum e_status
{
    GAME_WON,
    GAME_LOST,
    GAME_CONTINUE,
}	t_status;

typedef enum e_selection
{
    SELECTED,
    NOT_SELECTED,
}	t_selection;

extern int  received_signal;

void    print_board(WINDOW *win, const int size,
            int board[size][size], const int nbr_len, t_score score);
bool    move_and_merge(int direction, int size,
            int board[size][size], t_score *score);
void	add_record(t_list **leaderboard, t_score score, char *name);
void    rotate_board_clockwise(int size, int board[size][size]);
int     select_menu(char **username, t_list *leaderboard);
void    generate_number(int size, int board[size][size]);
int     game_over(int size, int board[size][size]);
int     save_leaderboard(t_list *leaderboard);
void	free_record(void *record);
t_list	*read_leaderboard(bool *error);
void    resize_handler(int sig);
t_list  *ft_lstsort(t_list *lst);
WINDOW  *init(void);

#endif // WONG_KAR_WAI_H
