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

int picoshell(char *av[])
{
    int fd[2];
    int i = 0;
    int prev_fd = 0;
    int status = 0;
    pid_t pid;
    while (av[i])
    {
        int offset = i;
        while (av[i] && ft_strcmp(av[i], "|") != 0)
            i++;
        if (av[i])
        {
            if (pipe(fd) == -1)
                return 1;
        }
        pid = fork();
        if (pid == -1)
            return 1;
        if (pid == 0)
        {
            if (prev_fd != 0)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                    exit(1);
                close(prev_fd);
            }
            if (av[i])
            {
                close(fd[0]);
                if (dup2(fd[1], STDOUT_FILENO) == -1)
                    exit(1);
                close(fd[1]);
            }
        }
        if (prev_fd)
            close(prev_fd);
        if (av[i])
        {
            prev_fd = fd[0];
            close(fd[1]);
            i++;
        }
    }

    //여기 조금 더 공부할것!
    int res = 0;
    while (wait(&status) > 0)
    {
        if ((WEXITSTATUS(status) != 0) && WIFEXITED(status))
            res = 1;
        if (!WIFEXITED(status))
            res = 1;
    }
    return res;
}
int mypicoshell(char *cmds[])
{
    int i = 0;
    int fd[2];
    int status = 0;
    int prev_fd = 0;
    int ret = 0;
    pid_t pid = 0;
    while (cmds[i])
    {
        int offset = i;
        while (cmds[i] && ft_strcmp(cmds[i], "|") != 0)
            i++;
        if (cmds[i])
        {
            if (pipe(fd) == -1)
                return 1;
        }
        pid = fork();
        if (pid == -1)
            return 1;
        if (pid == 0)
        {
            if (prev_fd != 0)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                    exit(1);
                close(prev_fd);
            }

            if (cmds[i] != NULL)
            {
                close(fd[0]);
                if (dup2(fd[1], STDOUT_FILENO) == -1)
                    exit(1);
                close(fd[1]);
            }
            cmds[i] = NULL;
            execvp(cmds[offset], &cmds[offset]);
            exit(1);
        }
        else
        {
            if (prev_fd != 0)
                close(prev_fd);
            if (cmds[i] != NULL)
            {
                close(fd[1]);
                i++;
                prev_fd = fd[0];
            }
        }
    }
    while (wait(&status) > 0)
    {
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) //정상종료했는데 exit 코드가 0이 아닌가? 그럼 에러!
            ret = 1;
        else if (!WIFEXITED(status)) //정상종료가아님.
            ret = 1;
    }
    return ret;
}


// int picoshell(char *cmds[])
// {
//     int i = 0;
//     int fd[2];
//     int status = 0;
//     int in_fd = 0;
//     int ret = 0;
//     pid_t pid = 0;

//     while (cmds[i])
//     {
//         int start = i;
//         while (cmds[i] && ft_strcmp(cmds[i], "|") != 0)
//             i++;
//         if (cmds[i])
//         {
//             if (pipe(fd) == -1)
//                 return PIPE_ERROR;
//         }
//         pid = fork();
//         if ( pid== -1)
//             return 1;
//         if (pid == 0)
//         {
//             if (in_fd != 0)
//             {
//                 if (dup2(in_fd, STDIN_FILENO) == -1)
//                     exit(1);
//                 close(in_fd);
//              }
//             if (cmds[i] != NULL)
//             {
//                 close(fd[0]);
//                 if (dup2(fd[1], STDOUT_FILENO) == -1)
//                     exit(1);
//                 close(fd[1]);
//             }
//             cmds[i] = NULL;
//             execvp(cmds[start], &cmds[start]);
//             exit(1);
//         }
//         else
//         {
//             if (in_fd != 0)
//                 close(in_fd);
//             if (cmds[i] != NULL)
//             {
//                 close(fd[1]);
//                 in_fd = fd[0];
//                 i++;
//             }
//         }
//     }

//     while (wait(&status) > 0)
//     {
//         if (WIFEXITED(status) && WEXITSTATUS(status) != 0) //정상종료 && exit 코드가 0이 아닌가? 그럼 에러!
//             ret = 1;
//         else if (!WIFEXITED(status)) //정상종료가아님.
//             ret = 1;
//     }
//     return ret;
// }

int main(int ac, char *av[])
{
    //picoshell(&av[1]);
    mypicoshell(&av[1]);
    return 0;
}