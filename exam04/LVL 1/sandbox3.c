
/*
Assignment name		:	sandbox
Expected files		:	sandbox.c
Allowed functions	:	fork, waitpid, exit, alarm, sigaction, kill,
                                                printf, strsignal, errno
===============================================================================

Write the following function:

#include <stdbool.h>
int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)

This function must test if the function f is a nice function or a bad function,
you will return 1 if f is nice , 0 if f is bad or -1 in case of an error in
your function.

A function is considered bad if it is terminated or stopped by a signal
(segfault, abort...), if it exit with any other exit code than 0 or if it
times out.

If verbose is true, you must write the appropriate message among the following:

"Nice function!\n"
"Bad function: exited with code <exit_code>\n"
"Bad function: <signal description>\n"
"Bad function: timed out after <timeout> seconds\n"

You must not leak processes (even in zombie state, this will be checked using
wait).

We will test your code with very bad functions.
*/

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
  
}
/*
    child process : check just f is bad or nice or stopped by signal?
    parent process : check if its timeout?

*/
int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    struct sigaction old_sa = {0}; //it is default signal.
    struct sigaction sa = {0};
    int status = 0;
    g_is_timeout = false;
    sa.sa_handler = &handle_timeout;

    // restore previous signal handler in the old_sa it is just effected by SIGALRM
    //old_sa : before calling it saving previos signal setting for SIGALRM i dont know if is there but good practice! if there's no any setting for SIGALRM then SIGDEFAULT.
    if (sigaction(SIGALRM, &sa, &old_sa) == -1)
        return (-1);

    g_child_pid = fork();
    if (g_child_pid == -1)
    {
        sigaction(SIGALRM, &old_sa, NULL);  //if failed then we have to reset to oldversion. 
        return (-1);
    }
    if (g_child_pid == 0)
    {
        f();
        exit(0);
    }
    alarm(timeout);
    while (waitpid(g_child_pid, &status, 0) == -1) //failed for waiting child process cause by any reasons...
    {
        if (errno != EINTR) //that reason if not signal intterupt. then break?
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

int main()
{
    return 0;
}