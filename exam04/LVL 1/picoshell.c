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

int ft_strcmp(const char* s1, const char *s2)
{
    while(*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

int mypico(char *cmds[])
{
    int status  = 0;
    int res = 0;
    int i = 0;
    int fd[2] = {0, 0};
    pid_t pid;
    int prev_fd = 0;
    while(cmds[i])
    {
        int start = i;
        while (cmds[i] && cmds[i] != "|")
            i++;
        int has_pipe = 0;
        if (cmds[i])
            has_pipe = 1;
        pipe(fd);
        pid = fork();
        if (pid == 0)
        {
            if (prev_fd)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (has_pipe)
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
                close(fd[0]);
            }
            cmds[i] = NULL;
            execvp(cmds[start], &cmds[start]);
            exit(1);
        }
        else
        {
            if(prev_fd)
                close(prev_fd);
            if (has_pipe)
            {
                prev_fd = fd[0];
                close(fd[1]);
            }
            if (cmds[i])
                i++;
        }
    }
    while (wait(&status) > 0)
    {
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            res = 1;
    }
    return 1;
}
int    picoshell(char *cmds[])
{
    int status = 0;
    int res = 0;
    pid_t pid;
    int fd[2];
    int i = 0;
    int prev_fd = 0;
    while(cmds[i])
    {
        int start = i;
        while (cmds[i], strcmp(cmds[i], "|") != 0)
        {
            i++;
        }
        int has_pipe = cmds[i];
        pipe(fd);
        pid = fork();
        if (pid == 0)
        {
            if (prev_fd)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (has_pipe)
            {
                dup2(fd[1],STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
            }
            cmds[i] = NULL;
            execvp(cmds[start], &cmds[start]);
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
            if (cmds[i])
                i++;
        }
    }
    while (wait(&status) > 0)
    {
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            res = 1;
    }
    return res;
}

int main(int ac, char *av[])
{
    //picoshell(&av[1]);
    mypicoshell(&av[1]);
    return 0;
}

int mypicoshell(char *cmds[])
{
    int i = 0;
    int fd[2];
    int status = 0;
    int prev_fd = 0;
    pid_t pid;
    int ret = 0;

    while (cmds[i])
    {
        int start = i;

        while (cmds[i] && ft_strcmp(cmds[i], "|") != 0)
            i++;

        int has_pipe = (cmds[i] != NULL);

        if (has_pipe)
            if (pipe(fd) == -1)
                return 1;

        pid = fork();
        if (pid == -1)
            return 1;

        if (pid == 0)
        {
            if (prev_fd)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (has_pipe)
            {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }

            cmds[i] = NULL;
            execvp(cmds[start], &cmds[start]);
            exit(1);
        }

        if (prev_fd)
            close(prev_fd);

        if (has_pipe)
        {
            close(fd[1]);
            prev_fd = fd[0];
        }

        if (cmds[i])
            i++;
    }

    while (wait(&status) > 0)
    {
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            ret = 1;
    }

    return ret;
}