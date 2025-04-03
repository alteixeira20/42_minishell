#include "minishell.h"

char    *extract_var(char *var, char **env)
{
    char    *tmp;
    char    *get;
    int     len;

    if ((var == NULL) || (env == NULL))
        return (NULL);
    tmp = ft_strjoin(var, "=");
    len = ft_strlen(tmp);
    get = NULL;
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