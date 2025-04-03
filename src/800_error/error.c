#include "../minishell.h"

int exit_error(char *msg, int status)
{
    ft_putstr_fd(msg, STDERR_FILENO);
    g_exit = status;
    return (FAILURE);
}