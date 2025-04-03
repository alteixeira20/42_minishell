#include "../minishell.h"

int var_from_env(char *var, char **env)
{
    int i;
    int len;

    if (!*env)
        return(NO_ENV);
    i = -1;
    len = ft_strlen(var);
    while (env[++i])
    {
        if ((ft_strncmp(var, env[i], len) == SUCCESS) \
                && ((env[i][len] == '=') || (env[i][len == '\0'])))
                return (i);
    }
    return (NO_VAR);
}

int set_var(char *var, char *val, char ***env)
{
    char    *new;
    int     i;

    if (!*env)
        return (NO_ENV);
    if (val)
        new = ft_strjoin(var, "="), ft_strdup(val);
    else
        new = ft_strdup(var);
    i = var_from_env(var, *env);
    if (i == NO_VAR)
        (*env) = env_add_var(*env, new);
    else
    {
        ft_swapstrs(&(*env)[i], &new);
        free(new);
    }
    return (SUCCESS);
}