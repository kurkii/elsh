#include <stdbool.h>
#define BUFFER_SIZE 0x1000
#define ELSH_PROMPT_DELIMITER "@"
#define ELSH_PROMPT_LIMIT_LEFT "["
#define ELSH_PROMPT_LIMIT_RIGHT "]"
char **parse_args(char *buffer);
bool execute_builtin_command(char *command, char **argv);

/* used for builtin commands - identifies them with a function */
typedef struct{
    char *command;
    void *function;
} builtin_command_t;

void init_builtin_commands();
void builtin_exit();