/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:40 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 16:28:58 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

enum e_token_type
{
	PIPE = 1,
	WORD,
	RED_TO,
	RED_FROM,
	APPEND,
	HERE_DOC,
};

/*typedef struct MemoryBlock {
	void *address;
	size_t size;
	const char *file;
	int line;
	struct MemoryBlock *next;
} MemoryBlock;*/

typedef struct s_token
{
	char				*lexeme;
	enum e_token_type	type;
}	t_token;

typedef struct s_token_list
{
	struct s_token		*token;
	struct s_token_list	*next;
}	t_token_list;

typedef struct s_parse_tree
{
	struct s_token		*data;
	struct s_parse_tree	*child;
	struct s_parse_tree	*sibling;
}	t_p_tree;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_exec_vars
{
	int		fd_in;
	int		fd_out;
	int		capacity;
	char	**args;
	int		i;
	int		error;
	int		inside_single_quotes;
	int		inside_double_quotes;
	int		end;
	int		open_fd_in;
}	t_exec_vars;

// Building struct to free when exiting
typedef struct s_free_data
{
	t_p_tree		*tree;
	t_p_tree		*tree_start;
	t_env			*env;
	t_token_list	*token_list;
	t_token_list	*token_list_start;
	char			**environment;
}	t_free_data;

typedef struct s_handle_vars
{
	char	buffer[1024];
	char	*token;
	char	*delimiters;
	char	**current;
	char	**result;
	char	*current_start;
}	t_handle_vars;

typedef struct	s_here_doc_data
{
	int						fd;
}	t_hd_data;
