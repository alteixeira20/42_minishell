#include "../minishell.h"

char	**env_add_var(char **env, char *new_var)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env && env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env && env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i++] = ft_strdup(new_var);
	new_env[i] = NULL;
	return (new_env);
}

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

static void	ft_swapenvs(char **a, char **b)
{
	char	*tmp;

	if (!a || !b)
		return ;
	tmp = *a;
	*a = *b;
	*b = tmp;
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
        ft_swapenvs(&(*env)[i], &new);
        free(new);
    }
    return (SUCCESS);
}
