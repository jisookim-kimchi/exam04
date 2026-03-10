# 42-Exam-rank-4
Answer for exam rank 4 (42 School)


If anyone have better options / optimisations or even if you had passed the exam and have the corrected exercices, let me know !

DISCORD : Lastdady



## 자식 프로세스 디버깅! 하는법!

(gdb) set detach-on-fork off  <-- "둘 다 꽉 잡아!"
(gdb) set follow-fork-mode child <-- "일단 애기 따라갈게"
(gdb) r
... (자식 프로세스 디버깅 중) ...
(gdb) info inferiors
  Num  Description       ...
* 2    process 12345     (현재 내가 있는 자식)
  1    process 12344     (멈춰있는 부모)
(gdb) inferior 1          <-- "부모 몸으로 빙의!"
... (이제 부모 프로세스 디버깅 중) ...

ft_popen.c 에서 자식프로세스에서 file 실행전에 continue & 하면 백그라운드도 실행됨 그리고 부모 프로세스로 넘어가자!



# sandbox.c

문제의도 : 어떤 함수 (f)를 안전하게 실행해보고, 착한 앤지 나쁜 앤지 감시하는 프로그램.

nice function : exit code 0 

bad function : 시간초과 정해진 시간(timeout)안에 안 끝남 (무한루프)
비정상 종료 : 0이 아닌 값으로 exit.
폭사 : 시그널(세그폴트 등) 맞고 죽음.

## alarm() : 나한테 N초 뒤에 SIGALRM 이라는 시그널을 보내줘
alarm(5) -> 5초 후에 SIGALRM 이라는 시그널을 보냄.
기본 행동: 만약 우리가 아무런 조치를 취하지 않은 상태에서 이 알람(시그널)을 받으면, 프로그램은 **"종료"**됩니다.

## sigaction 구조체.
어떻게 행동할지 를 정의하는 설명서.
struct sigaction sa = {0};
sa.sa_handler = do_nothing;





# LEVEL2
vbc.c

트리 계산기 만들기다..
우선순위가먼지 보자.
일단 () 무조건 괄호가 높음.



## GDB

### set args [인자값] 해서 실행 하면 



## 메모리 누수 체크
### valgrind --leak-check=full ./a.out [인자값]



1. ft_popen.c
type == 'r' (Read 모드)
목적: 자식의 실행 결과(출력)를 부모가 읽어오기

text
[ 자식 프로세스 ]                      [ 부모 프로세스 ]
      (Child)                            (Parent)
         |                                  ^
  [표준 출력(stdout)]                       |
   (원래는 모니터)                          |
         |                                  |
  dup2(fd[1], 1)                     ft_popen() 이
         |                           리턴해준 fd[0]
         v                                  ^
   +-----------+                      +-----------+
   |   fd[1]   | -------------------> |   fd[0]   |
   +-----------+      [ PIPE ]        +-----------+
    (쓰기 전용)                        (읽기 전용)
   자식이 쏜다! (Write)             부모가 받는다! (Read)


type == 'w' (Write 모드)
목적: 부모가 데이터를 주면 자식이 그걸 받아서 처리 (예: grep)

text
[ 부모 프로세스 ]                      [ 자식 프로세스 ]
      (Parent)                            (Child)
         |                                  |
   ft_popen() 이                            |
   리턴해준 fd[1]                           |
         |                           [표준 입력(stdin)]
         |                            (원래는 키보드)
         v                                  ^
   +-----------+                      +-----------+
   |   fd[1]   | -------------------> |   fd[0]   |
   +-----------+      [ PIPE ]        +-----------+
    (쓰기 전용)                        (읽기 전용)
                                     dup2(fd[0], 0)
                                            |
   부모가 쏜다! (Write)             자식이 받는다! (Read)


   # picoshell
   sed s/a/b/g:
   s (substitute): 교체하라는 명령입니다.
   /a/b/: 'a'를 'b'로 바꾸라는 뜻입니다.
   g (global): 줄에 있는 모든 'a'를 다 바꾸라는 뜻입니다. 만약 g가 없으면 첫 번째 'a'만 바뀝니다.

   