/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonroy- <mmonroy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 09:09:03 by aleon-ca          #+#    #+#             */
/*   Updated: 2020/10/25 10:34:01 by aleon-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <stdlib.h>
# include "../libft/inc/libft.h"
# include "../libft/inc/ft_printf.h"

# define EPARSE "\U0001F633 minishell: parse error.\n"
# define ESUBEOF "\U0001F633 minishell: EOF while looking for matching quote\n"

/*
** The t_command_table struct contains the information of a set of
** input commands, i.e, some input line of the form:
**
** command_name arguments | command_name2 ... | ... > output > input >> append;
**
** which we will use to stablish the correct pipe and redirection relations
** between commands.
** The simple_commands contain the name and arguments of
** an input command, while input_file its input redirections and so on:
**
** simple_commands[i][] = {name, argument #1, ... argument #n, NULL}
** input_files[i][] = {input #1, ... input #n, NULL}
*/

typedef struct	s_command_table
{
	char		***simple_commands;
	int			simple_commands_num;
	char		***input_files;
	char		***output_files;
	char		***append_files;
	char		***dummy_files;
}				t_command_table;

/*
** General Env variables
*/
char			**g_env;
/*
** main.c
*/
void			show_prompt(void);
/*
** parser_error.c
*/
void			free_cmd_table(t_command_table *table, int num);
int				free_errpars(t_command_table *t, int n, char **lines);
int				is_parser_error(t_command_table *tan, int i);
/*
** parser_redirections_utils.c
*/
void			redir_files_updt(t_command_table *ta, int *i, char *str, int t);
void			count_redirections(char **arr, int *count);
void			init_redirection_arr(t_command_table *tab, int *i, int *count);
char			*dup_till_symbol(char *str);
/*
** parser_redirections.c
*/
int				set_redirection_arr(t_command_table *tab, int *i);
/*
** parser_utils5.c
*/
char			*ft_strchr__quots(char **quotpos, char *str, char c);
char			*ft_str2chr__quots(char **quotpos, char *str, char c);
char			*ft_str1chr__quots(char **quotpos, char *str, char c);
char			**set_quotpos_arr(char *str);
int				count_not_quoted_char(char **quotpos, char *str, char c);
/*
** parser_utils4.c
*/
char			*smallest_non_zero(char *s1, char *s2);
void			ft_add_str(char ***arrdir, char *str);
void			remove_quots(char **strdir);
void			remove_dummy_quots(char **table);
/*
** parser_utils3.c
*/
void			remove_backslash(char **strdir, char *slashpos);
void			replace_var_in_str(char **strdir, char *ptr);
void			replace_env_var(t_command_table *table);
char			**remove_empty_str(char **arr);
/*
** parser_utils2.c
*/
int				is_inside_jth_quote_pair(char **quotpos, char *pos);
char			**ft_split__quots(char *str, char c);
/*
** parser_utils.c
*/
char			*ft_add_char(char *str, char c);
void			read_input(char **input);
void			resize_arr_skip_pos(char ***arr, int pos);
/*
** parser.c
*/
int				tokenize(char **a, t_command_table *t, int n);
/*
** executor_utils.c
*/
int				is_built_in(char **arr);
int				is_start_exec_path(char **strdir);
void			choose__exec(char **arr, t_command_table *tab);
int				is_cmd_cd(char **arr, int num);
void			launch_exec(char **arr);
/*
** executor.c
*/
void			executor(t_command_table *table, int table_num);
/*
** env_handler.c
*/
void			save_env(int argc, char **argv, char **envp);
void			exit_minishell(void);
char			*env_selector(char *env);
int				envp_len(char **envp);
/*
** error_handler.c
*/
void			fork_error(void);
void			cmd_not_found(char *str);
/*
** cmd_cd.c
*/
int				cmd_cd(char **arg);
/*
** cmd_env.c
*/
void			cmd_env(char **arg);
/*
** cmd_echo.c
*/
void			cmd_echo(char **argv);
/*
** cmd_pwd.c
*/
void			cmd_pwd(char **arg);
/*
** cmd_export.c
*/
void			cmd_export(char **arg);
/*
** cmd_unset.c
*/
int				ft_find_char(char *str, char c);
void			free_double(char **str);
void			cmd_unset(char **arg);
/*
** cmd_exit.c
*/
void			cmd_exit(void);
/*
** cmd_utils.c
*/
int				read_input_subshell(char **input, char c);
int				quotes_handler(char **arg, int i);
void			export_no_arg(char **temp);
/*
** absolute_path.c
*/
int				is_absolute_path(char **strdir);
/*
** signal_handler.c
*/
void			signal_handler(int sig);
void			child_signal_handler(int sig);
#endif
