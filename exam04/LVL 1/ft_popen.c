#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int ft_popen(const char *file, char *const av[], int type)
{
    if (!file || !av || (type != 'r' && type != 'w'))
        return (-1);

    int fd[2];

    if (pipe(fd) == -1)
        return -1;
    if (type == 'r')
    {
        if (fork() == 0)
        {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execvp(file, av);
            exit(-1);
        }
        close(fd[1]);
        return (fd[0]);
    }
    if (type == 'w')
    {
        if (fork() == 0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            execvp(file, av);
            exit(-1);
        }
        close (fd[0]);
        return (fd[1]); //부모가 fd[1]을 return 받아야만 파이프 write end를 통해 데이터를 보낼 수 있음
    }
    return (-1);
}


int main(void)
{
    // char *const avr[] = {"cat", "ft_popen.c", NULL}; 
    // int fd = ft_popen("cat", avr, 'r');
    // if (fd == -1)
    //     return (1);
    // char buf[1024];
    // int n;
    // // 자식(cat)이 파일 내용을 파이프에 쏘면, 부모가 여기서 읽음
    // while ((n = read(fd, buf, 1023)) > 0)
    // {
    //     write(STDOUT_FILENO, buf, n); // 읽은 내용을 화면에 출력
    // }
    // close(fd);
    
    
    char *const avw[] = {"grep", "hello", NULL};
    int fd2 = ft_popen("grep", avw, 'w'); 

    if (fd2 == -1)
    {
        write(2, "Error\n", 6);
        return (-1);
    }

    // 부모가 "hello world"를 파이프(fd)에 씁니다.
    write(fd2, "hello world\n", 12); 
    write(fd2, "bye bye\n", 8);      
    write(fd2, "hello again\n", 12);

    close(fd2); 
    
    sleep(1);
    return (0);
}