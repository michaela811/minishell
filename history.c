#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void execute_command(const char* input)
{
    printf("Output from command: %s\n", input);
    // Suppose the output here includes line breaks or moves the cursor
}

int main()
{
    char* input;

    while ((input = readline("myshell> ")) != NULL) {
        if (*input) {  // Check if the input is not empty
            add_history(input);
            execute_command(input);
        }

        // Inform Readline that the cursor is on a new line
        rl_on_new_line();

        free(input);
    }
    return 0;
}
