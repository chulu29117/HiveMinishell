/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierce <mpierce@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:02:07 by clu               #+#    #+#             */
/*   Updated: 2025/05/22 14:53:13 by mpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _XOPEN_SOURCE 700

# include "libft.h"
# include <stdbool.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>

// Define error messages
# define SHLV "minishell: warning: shell level (%d) too high, resetting to 1\n"
# define SYNTAX "Syntax error: near unexpected token "
# define AMBIGUOUS "error: ambiguous redirect\n"
# define HEREDOC_LIM "minishell: maximum here-document count exceeded\n"
# define PROMPT_START "\001\033[1;36m\002minishell:"
# define PROMPT_END "$ \001\033[0m\002"

// Define quoted states
# define UNQUOTED	 0
# define SINGLE_QUOTED 1
# define DOUBLE_QUOTED 2

extern volatile sig_atomic_t	g_signal;

// Define token types
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ERROR
}	t_token_type;

// Segment structure
typedef struct s_segment
{
	char				*seg;
	int					quoted;
	struct s_segment	*next;
}	t_segment;

// Token structure
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				quoted;
	t_segment		*segments;
	struct s_token	*next;
	struct s_token	*prev;
	bool			expanded;
}	t_token;

// Word structure
typedef struct s_word
{
	int			has_quote;
	int			quoted_state;
	t_segment	*segments;
}	t_word;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_APPEND,
	NODE_HEREDOC,
	NODE_MISSCMD
}	t_node_type;

typedef struct s_infile
{
	char	*name;
	int		heredoc;
}	t_infile;

typedef struct s_cmd
{
	char		**full_cmd;
	t_infile	**infile;
	char		**outfile;
	int			*append;
	bool		from_expand;
}	t_cmd;

typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	t_cmd			*cmd;
}	t_ast;

// ms structs
typedef struct s_ms
{
	char	**envp;
	int		elements;
	int		exit_status;
	t_token	*token;
	t_word	*word;
	t_ast	*ast;
	t_cmd	*cmd;
	int		fd_in;
	int		fd_out;
	int		ms_fd[2];
	int		prev_fd;
	pid_t	*pids;
	int		pid_index;
	int		cmd_no;
	int		cmd_index;
	char	*cmd_path;
	int		std_copy[2];
	bool	child;
	char	*heredoc_name;
	char	*prompt;
	int		heredoc_no;
	int		reset[2];
	int		infile_index;
	bool	syntax_error;
	bool	sigfirst;
	bool	heredoc_stop;
}	t_ms;

////////////////////////////////// FUNCTIONS //////////////////////////////////

////////////////////////////////// Parsing ////////////////////////////////////
void		process_input(t_ms *ms);
bool		process_tokens(t_ms *ms, char *input);

////////////////////////////////// Lexer //////////////////////////////////////
t_token		*lexer(t_ms *ms, const char *input);
t_segment	*read_word_seg(t_ms *ms, const char *input, int *i,
				int *quoted_state);
void		handle_s_quote(t_ms *ms, const char *input, int *i, t_word *word);
void		handle_d_quote(t_ms *ms, const char *input, int *i, t_word *word);
void		handle_var_seg(t_ms *ms, const char *input, int *i, t_word *word);
void		handle_lit_seg(t_ms *ms, const char *input, int *i, t_word *word);
t_token		*handle_pipe(int *i);
t_token		*handle_redirection(const char *input, int *i);
bool		is_operator(char c);
t_token		*allocate_token(void);
t_token		*create_token(t_token_type type, char *value, int quoted);
void		append_token(t_token **tokens, t_token *new_token);
void		free_token(t_token *token);
void		free_tokens(t_token *tokens);

/////////////////////////////////// Expander //////////////////////////////////
void		update_token(t_ms *ms);
char		*assemble_token(t_ms *ms, t_token *token, bool check_heredoc,
				bool *exp);
void		expand_token_value(t_ms *ms, t_token *token, bool check_heredoc);
char		*expand_input(t_ms *ms, char *content, bool *was_expanded);
char		*expand_var_token(t_ms *ms, char *content, int *pos);
char		*expand_literal_token(t_ms *ms, char *content, int *pos);
void		retokenize(t_ms *ms);
bool		check_ambiguous_redirect(t_ms *ms);

////////////////////////////////// Parser /////////////////////////////////////
t_ast		*parse_input(t_token **tokens, t_ms *ms);
t_ast		*parse_cmd(t_token **tokens, t_ms *ms);
bool		lead_redirect(t_token **current, t_cmd *cmd, t_ms *ms);
bool		process_cmd_name(t_token **current, t_cmd *cmd, int *arg,
				t_ms *ms);
bool		process_cmd_args(t_token **current, t_cmd *cmd, int *arg,
				t_ms *ms);
t_ast		*finalize_cmd_node(t_token **tokens, t_token *current,
				t_cmd *cmd, t_ms *ms);
t_ast		*create_missing_cmd_node(t_token **tokens, t_token *current,
				t_cmd *cmd, t_ms *ms);
bool		validate_input(t_ms *ms, t_token *tokens);
void		clean_all_heredocs(t_ms *ms);
bool		parse_redirect(t_ms *ms, t_token **tokens, t_cmd *cmd);
t_ast		*new_ast_node(t_node_type type, t_ast *left, t_ast *right,
				t_cmd *cmd);
t_cmd		*new_cmd(void);
void		handle_out_redirect(t_token *token, t_cmd *cmd);
t_infile	*new_infile(char *name, int is_heredoc);
bool		ft_add_to_infile(t_infile ***array, t_infile *infile);
bool		handle_in_redirect(t_ms *ms, t_token *token, t_cmd *cmd);
bool		setup_heredoc(t_ms *ms, t_token *token, t_infile **infile);
bool		check_heredoc_limit(t_ms *ms);

////////////////////////////////// Builtins ///////////////////////////////////
int			print_error(t_ms *ms, char *name, char *msg);
char		*find_var(t_ms *ms, char **envp, char *name);
bool		export_var_to_envp(t_ms *ms, char *var);
int			find_in_envp(t_ms *ms, char **envp, char *var);
void		bi_exit(t_ms *ms, int ex_code, int error);
int			parse_cd(t_ms *ms, char **dir);
int			write_pwd(t_ms *ms);
void		write_env(t_ms *ms);
void		unset_envp(t_ms *ms, char *name);
int			echo(t_ms *ms, char **lines, bool nl, int flags);
void		dup_envp(t_ms *ms, char **temp, int i, int j);
void		arr_dup_fail(t_ms *ms, char **arr, int j);
void		export_print_env(t_ms *ms);
bool		check_valid_identifier(t_ms *ms, char *name);
char		*append_var(t_ms *ms, char *var, char *name, int j);
void		getcwd_error(t_ms *ms, char *s1, char *s2);
void		run_unset(t_ms *ms, t_cmd *cmd);
void		oldpwd(t_ms *ms);
void		run_export(t_ms *ms, char *var, char *name, bool append);
void		exit_args(t_ms *ms, char **full_cmd, int i);
void		run_export_args(t_ms *ms, char **full_cmd);
void		invalid_export(t_ms *ms, char *name);
void		check_valid_arg_exit(t_ms *ms, char *code);

////////////////////////////////// Utils //////////////////////////////////////
void		*x_malloc(t_ms *ms, size_t size);
char		*x_strdup(t_ms *ms, const char *s);
char		*x_substr(t_ms *ms, const char *s, unsigned int start, size_t len);
char		*x_strjoin_free(t_ms *ms, char *s1, char *s2);
void		free_envp(t_ms *ms);
void		free_ms(t_ms *ms);
void		free_segments(t_segment *seg);
void		free_word(t_word *word);
void		free_cmd(t_cmd *cmd);
void		free_ast(t_ast *ast);
void		free_structs(t_ms *ms);
void		sys_error(t_ms *ms, char *error_msg);
void		cmd_error(t_cmd *cmd, char *error_msg);
void		error(t_ms *ms, char *error_msg);
void		builtin_err(t_ms *ms, char *error_msg);
void		get_prompt(t_ms *ms);
void		shlvl(t_ms *ms);
void		update_env(t_ms *ms);
void		error_join(t_ms *ms, char *name, char *error);

////////////////////////////////// Executor ///////////////////////////////////
void		ms_error(t_ms *ms, char *msg, int ex_code, int free_msg);
void		validation(t_ms *ms, t_cmd *cmd);
void		close_fd(t_ms *ms);
void		run_execve(t_ms *ms, t_cmd *cmd);
void		full_error_msg(t_ms *ms, char *s1, char *s2);
void		parse_cmds(t_ms *ms, t_ast *ast);
void		run_pipes(t_ms *ms, t_ast *ast);
void		run_cmd(t_ms *ms, t_cmd *cmd);
int			handle_infiles(t_ms *ms, t_infile **infile);
int			handle_outfiles(t_ms *ms, char **outfile, int *append);
int			handle_heredoc(t_ms *ms, const char *heredoc, char *name, int quo);
int			start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo);
void		free_heredoc(t_ms *ms);
void		ft_free_infile(t_infile **array);
void		run_execve(t_ms *ms, t_cmd *cmd);
void		run_executor(t_ms *ms, int i);
void		close_pipes(t_ms *ms);
int			handle_files(t_ms *ms, t_cmd *cmd);
char		*heredoc_name(t_ms *ms, int i);
int			redirect_out_helper(t_ms *ms, char *file, int dir);
void		run_no_cmd(t_ms *ms, t_ast *ast);
void		reset_std(t_ms *ms);
void		close_heredoc_sigint(t_ms *ms, char *name);
void		heredoc_help(t_ms *ms, char *line, int fd, int quo);
void		find_exe(t_ms *ms, t_cmd *cmd);
char		*get_path(t_ms *ms, t_cmd *cmd, char **envp, int i);
void		next_pipe(t_ms *ms, int last);
int			readline_break(char *line, int fd);
char		*get_line(char *temp, char *line);
void		wait_help(t_ms *ms);

////////////////////////////////// Signals ////////////////////////////////////
void		do_sigint(int a, siginfo_t *b, void *c);
void		handle_signal(int sig);
int			heredoc_sigint(void);
t_ms		*get_minishell(t_ms *ms);
void		set_signals(void);
void		reset_signals(void);
void		hold_signals(void);

#endif