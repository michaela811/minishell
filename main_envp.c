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

void update_add_env_var(t_env **head, const char *name, const char *value)
{
    t_env *var = find_env_var(*head, name);
    if (var)
	{
        free(var->value);
        var->value = strdup(value);
    }
	else//Probably we don't need this as we don't implement
	{
        t_env *new_var = create_env_var(name, value);
        new_var->next = *head;
        *head = new_var;
    }
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

char **env_list_to_array(t_env *head)
{
    // First, count the number of elements in the list
    int count = 0;
	int i = 0;
	int j = 0;

    t_env *current = head;
    while (current != NULL)
	{
        count++;
        current = current->next;
    }
    // Allocate an array of the right size
    // We add 1 to the count for the NULL terminator
    char **env_char = malloc((count + 1) * sizeof(char *));
    if (env_char == NULL)
        return NULL;  // Return NULL on allocation failure
    // Now fill in the array
    current = head;
	while (i < count)
	{
        // Allocate a string for the environment variable
        // The +2 is for the '=' and the null terminator
        env_char[i] = malloc(strlen(current->name) + strlen(current->value) + 2);
        if (env_char[i] == NULL)
		{
			while (j < i)
			{
				free(env_char[j]);
				j++;
			}
            free(env_char);
            return NULL;
        }
        // Copy the key, the '=', and the value into the string
        strcpy(env_char[i], current->name);
        strcat(env_char[i], "=");
        strcat(env_char[i], current->value);
        current = current->next;
		i++;
    }
    // The last element of the array is a NULL pointer
    env_char[count] = NULL;
    return env_char;
}


