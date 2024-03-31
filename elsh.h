#include <stdbool.h>
#include <sys/types.h>
#define ELSH_PROMPT_DELIMITER "@"
#define ELSH_PROMPT_LIMIT_LEFT "{"
#define ELSH_PROMPT_LIMIT_RIGHT "}"
#define ELSH_PROMPT_SYMBOL "$ "

#define BUFFER_SIZE 0x1000

#define ELSH_VERSION_MAJOR 1
#define ELSH_VERSION_MINOR 0
char **parse_args(char *buffer);
bool execute_builtin_command(char *command, char **argv);

/* used for builtin commands - identifies them with a function */
typedef struct{
    char *command;
    void *function;
} builtin_command_t;

void init_builtin_commands();
void builtin_exit();
int builtin_cd();
void builtin_help();
char *cwd2rel(char *cwd);
int pipe_two_processes(char **argv_a, char **argv_b);