
#include "../includes/wong_kar_wai.h"

static t_score	stoscore(const char *str)
{
	t_score	score = 0, prev = 0;
	while (*str && *str != '\n')
	{
		prev = score;
		if (*str < '0' || *str > '9')
			return (-1);
		score = score * 10 + *str - '0';
		if (score < prev)
			return (-1);
		str++;
	}
	return (score);
}

static inline bool contains_valid_name(const char *name)
{
	while (*name && *name != ' ')
	{
		if (!ft_isalnum(*name))
			return (false);
		name++;
	}
	return (true);
}

static bool	is_line_valid(const char *line)
{
	const char	*space = ft_strchr(line, ' ');

	if (!space)
		return (false);
	if (ft_strrchr(line, ' ') != space)
		return (false);
	if (ft_strlen(line) > MAX_RECORD_LEN - 2)
		return (false);
	if (space - line > MAX_NICKNAME_LEN)
		return (false);
	if (!contains_valid_name(line))
		return (false);
	if (stoscore(ft_strrchr(line, ' ') + 1) == -1ull)
		return (false);
	return (true);
}

static char	*extract_name(const char *line)
{
	static char name[MAX_NICKNAME_LEN + 1];

	ft_bzero(name, sizeof(name));
	ft_strlcpy(name, line, ft_strchr(line, ' ') - line + 1);
	return (name);
}

static t_record	*create_record(const char *line)
{
	t_record	*record = ft_calloc(sizeof(t_record), 1);

	if (!record)
		return (NULL);
	record->score = stoscore(ft_strrchr(line, ' ') + 1);
	record->name = ft_strdup(extract_name(line));
	if (!record->name)
	{
		free(record);
		return (NULL);
	}
	return (record);
}

void	free_record(void *record)
{
	t_record	*rec = record;

	free(rec->name);
	free(rec);
}

t_list *ft_lstsort(t_list *lst)
{
	t_list	*sorted = NULL;

	while (lst)
	{
		t_list	*next = lst->next;
		t_list	**node = &sorted;
		while (*node && ((t_record *)(*node)->content)->score > ((t_record *)lst->content)->score)
			node = &(*node)->next;
		lst->next = *node;
		*node = lst;
		lst = next;
	}
	return (sorted);
}

t_list	*read_leaderboard(bool *error)
{
	if (access(LEADERBOARD_FILE, F_OK) == -1)
	{
		FILE	*file = fopen(LEADERBOARD_FILE, "w");
		fclose(file);
	}
	FILE		*file = fopen(LEADERBOARD_FILE, "r");
	t_list		*leaderboard = NULL;
	char 		line[MAX_RECORD_LEN];

	if (!file)
	{
		return (*error = true, NULL);
	}
	while (fgets(line, MAX_RECORD_LEN, file))
	{
		if (!is_line_valid(line))
		{
			ft_lstclear(&leaderboard, free_record);
			fclose(file);
			return (*error = true, NULL);
		}
		t_record	*record = create_record(line);
		if (!record)
		{
			ft_lstclear(&leaderboard, free_record);
			fclose(file);
			return (*error = true, NULL);
		}
		t_list	*new_node = ft_lstnew(record);
		if (!new_node)
		{
			free_record(record);
			ft_lstclear(&leaderboard, free_record);
			fclose(file);
			return (*error = true, NULL);
		}
		ft_lstadd_back(&leaderboard, new_node);
	}
	fclose(file);

	leaderboard = ft_lstsort(leaderboard);
	return (*error = false, leaderboard);
}

void	add_record(t_list **leaderboard, t_score score, char *name)
{
	t_record	*record = ft_calloc(sizeof(t_record), 1);

	if (!record)
		return ;
	record->score = score;
	record->name = name;
	t_list	*new_node = ft_lstnew(record);
	if (!new_node)
	{
		free_record(record);
		return ;
	}
	ft_lstadd_back(leaderboard, new_node);
}

int save_leaderboard(t_list *leaderboard)
{
	FILE		*file = fopen(LEADERBOARD_FILE, "w");

	if (!file)
		return (1);
	for (t_list *node = leaderboard; node; node = node->next)
	{
		t_record	*record = node->content;
		fprintf(file, "%s %llu\n", record->name, record->score);
	}
	fclose(file);
	return (0);
}
