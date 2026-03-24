/* Assignment name:    picoshell
Expected files:        picoshell.c
Allowed functions:    close, fork, wait, exit, execvp, dup2, pipe
___

Write the following function:

int    picoshell(char *cmds[]);

The goal of this function is to execute a pipeline. It must execute each
commands [sic] of cmds and connect the output of one to the input of the
next command (just like a shell).

Cmds contains a null-terminated list of valid commands. Each rows [sic]
of cmds are an argv array directly usable for a call to execvp. The first
arguments [sic] of each command is the command name or path and can be passed
directly as the first argument of execvp.

If any error occur [sic], The function must return 1 (you must of course
close all the open fds before). otherwise the function must wait all child
processes and return 0. You will find in this directory a file main.c which
contain [sic] something to help you test your function.


Examples: 
./picoshell /bin/ls "|" /usr/bin/grep picoshell
picoshell
./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
squblblb/
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


int picoshell(char ***cmds)
{
    int status = 0, res = 0;
    pid_t pid;
    int fd[2];
    int prev_fd = 0;
    int i = 0;
    int has_pipe = 0;
    printf("%s\n" ,*cmds[i]);
    while (cmds[i])
    {
        has_pipe = 0;
        int start = i;
        // while (cmds[i] && strcmp(*cmds[i], "|") != 0)
        //     i++;
        if (cmds[i + 1]) //뒤에도 더블어레이가 있다면? 그럼 | 겠거니 하는거지
             has_pipe = 1;

        if (has_pipe)
        {
            if (pipe(fd) < 0)
            {
                return 1;
            }
        }
        pid = fork();
        if (pid < 0)
        {
            return 1;
        }

        if (pid == 0)
        {
            // Child
            if (prev_fd)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (has_pipe)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
            }
            execvp(cmds[start][0], cmds[start]);
            perror("execvp");
            exit(1);
        }
        else
        {
            if (prev_fd)
                close(prev_fd);
            if (has_pipe)
            {
                prev_fd = fd[0];
                close(fd[1]);
            }
        }
        i++;
    }

    while (wait(&status) > 0) {
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            res = 1;
    }
    return res;
}

int count_cmds(int argc, char *argv[])
{
    int count = 1;  // at least one command
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "|") == 0)
            count++;
    }
    return count;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return  1;

    int cmd_count = count_cmds(argc, argv);
    char ***cmds = calloc(cmd_count + 1, sizeof(char **));
    if (!cmds)
        return (perror("calloc"), 1);

    // Parsear argumentos y construir array de comandos
    int i = 1, j = 0;
    while (i < argc)
    {
        int len = 0;
        while (i + len < argc && strcmp(argv[i + len], "|") != 0)
            len++;
        
        cmds[j] = calloc(len + 1, sizeof(char *));
        if (!cmds[j])
            return (perror("calloc"), 1);
        
        for (int k = 0; k < len; k++)
            cmds[j][k] = argv[i + k];
        cmds[j][len] = NULL;
        
        i += len + 1;  // Saltar el "|"
        j++;
    }
    cmds[cmd_count] = NULL;

    int ret = picoshell(cmds);

    // Limpiar memoria
    for (int i = 0; cmds[i]; i++)
        free(cmds[i]);
    free(cmds);

    return ret;
}