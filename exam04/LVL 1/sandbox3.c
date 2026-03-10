#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

static pid_t g_child_pid = 0;
static bool g_is_timeout = false;

void handle_timeout(int sig)
{
    (void)sig;
    if (g_child_pid > 0)
    {
        kill (g_child_pid, SIGKILL);
        g_is_timeout = true;
    }
}

static void verbose_message(int status, unsigned int timeout)
{
    if (g_is_timeout)
    {
        printf("Bad function timeout %u\n", timeout);
    }
    else if (WIFEXITED(status))
    {
        int res = WEXITSTATUS(status);
        if (res == 0)
            printf("Nice function!\n");
        else
            printf("Bad function \n %d\n", res);
    }
    else if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        printf("bad function with %s\n", strsignal(sig));
    }
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    struct sigaction old_sa;
    struct sigaction sa;
    int status = 0;
    g_is_timeout = false;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handle_timeout;

    if (sigaction(SIGALRM, &sa, &old_sa) == -1) // restore previous signal handler in the old_sa it is just effected by SIGALRM
        return (-1);

    g_child_pid = fork();
    if (g_child_pid == -1)
    {
        sigaction(SIGALRM, &old_sa, NULL);
        return (-1);
    }
    if (g_child_pid == 0)
    {
        f();
        exit(0);
    }
    alarm(timeout);
    while (waitpid(g_child_pid, &status, 0) == -1)
    {
        if (errno != EINTR)
            break;
    }
    //restore previous signal handler and g_child_pid 
    alarm(0);
    sigaction(SIGALRM, &old_sa, NULL);
    g_child_pid = 0;
    
    if (verbose)
        verbose_message(status, timeout);
    
    if (g_is_timeout)
        g_is_timeout = 0;

    if (WIFEXITED(status))
    {
        if (WEXITSTATUS(status) == 0)
            return (1);
        else
            return (0);
    }
    if (WIFSIGNALED(status))
        return (0);

    return (0); 
}