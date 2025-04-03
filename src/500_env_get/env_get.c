#include "../minishell.h"

char    **init_env(char **env)
{
    char    **new_env;
    int     n;

    n = 0;
    while (env[n])
        ++n;
    new_env = ft_calloc((n + 1), sizeof(char *));
    if (!new_env)
        return (exit_error(MALLOC_ERR, errno), NULL);
    n = -1;
    while (env[++n])
    {
        new_env[n] = ft_strdup(env[n]);
        if (!new_env[n])
            return (NULL);
    }
    new_env[n] = NULL;
    return (new_env);
}

char    *extract_var(char *var, char **env)
{
    char    *tmp;
    int     len;

    if ((var == NULL) || (env == NULL))
        return (NULL);
    tmp = ft_strjoin(var, "=");
    len = ft_strlen(tmp);
    while(*env)
    {
        if (ft_strncmp(tmp, *env, len) == 0)
        {
            var = ft_strdup(*env + len);
            break ;
        }
        ++env;
    }
    free(tmp);
    return (var);
}