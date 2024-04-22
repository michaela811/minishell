#include "lexer.h"

t_env *create_env_var(const char *name, const char *value)
{
    t_env *var = malloc(sizeof(t_env));
    if (var == NULL)
		return NULL;
    var->name = strdup(name);
	if (var->name == NULL)
		return(free(var), NULL);
    var->value = strdup(value);
	if (var->value == NULL)
		return(free(var->name), free(var), NULL);
    var->next = NULL;
    return var;
}


t_env *init_environment(char **envp)
{
    t_env *head = NULL;
    char **env = envp;
    while (*env)
	{
        char *entry = *env;
        char *separator = strchr(entry, '=');
        if (separator)
		{
            *separator = '\0';  // Temporarily terminate the string to isolate the name
            t_env *var = create_env_var(entry, separator + 1);
			if (var == NULL)
				return (free_env(head), NULL);
            *separator = '=';  // Restore the environment string
            var->next = head;
            head = var;
        }
        env++;
    }
    return head;
}

char *get_env_var(t_env *head, const char *name)
{
    while (head != NULL) {
        if (strcmp(head->name, name) == 0) {
            return head->value;  // Return the value of the variable if found
        }
        head = head->next;  // Move to the next node in the list
    }
    return NULL;  // Return NULL if the variable is not found
}

t_env *find_env_var(t_env *head, const char *name)
{
    while (head)
	{
        if (strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

int update_add_env_var(t_env **head, const char *name, const char *value)
{
    t_env *var;
    t_env *new_var;

    var = find_env_var(*head, name);
    if (var)
	{
        free(var->value);
        var->value = strdup(value);
        if (var->value == NULL)
            return (perror("Memory allocation error"), 1);
    }
	else//Probably we don't need this as we don't implement
	{
        new_var = create_env_var(name, value);
        if (new_var == NULL)
            return (perror("Memory allocation error"), 1);
        new_var->next = *head;
        *head = new_var;
    }
    return (0);
}

void free_env(t_env *head)
{
    t_env *current = head;
    while (current)
	{
        t_env *next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }
}

int count_env_list(t_env *head)
{
    int count = 0;
    t_env *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}

char *create_env_str(t_env *current)
{
    char *env_str = malloc(strlen(current->name) + strlen(current->value) + 2);
    if (env_str == NULL)
        return NULL;
    strcpy(env_str, current->name);
    strcat(env_str, "=");
    strcat(env_str, current->value);
    return env_str;
}

void free_env_array(char **env_array, int count)
{
    for (int i = 0; i < count; i++)
        free(env_array[i]);
    free(env_array);
}

char **env_list_to_array(t_env *head)
{
    int count;
    char **env_array;

    count = count_env_list(head);
    **env_array = malloc((count + 1) * sizeof(char *));
    if (env_array == NULL)
        return (NULL);
    t_env *current = head;
    for (int i = 0; i < count; i++)
    {
        env_array[i] = create_env_str(current);
        if (env_array[i] == NULL)
        {
            free_env_array(env_array, i);
            return (NULL);
        }
        current = current->next;
    }
    env_array[count] = NULL;
    return (env_array);
}
/*
char **env_list_to_array(t_env *head)
{
    int count = 0;
	int i = 0;
	int j = 0;
    t_env *current;
    char **env_char;

    current = head;
    while (current != NULL)
	{
        count++;
        current = current->next;
    }
    **env_char = malloc((count + 1) * sizeof(char *));
    if (env_char == NULL)
        return NULL;
    current = head;
	while (i < count)
	{
        env_char[i] = malloc(strlen(current->name) + strlen(current->value) + 2);
        if (env_char[i] == NULL)
		{
			while (j < i)
				free(env_char[j++]);
            free(env_char);
            return NULL;
        }
        strcpy(env_char[i], current->name);
        strcat(env_char[i], "=");
        strcat(env_char[i], current->value);
        current = current->next;
		i++;
    }
    env_char[count] = NULL;
    return env_char;
}*/


