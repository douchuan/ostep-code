#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    
    int mypipe[2];
    if (pipe(mypipe)) {
      fprintf(stderr, "Pipe failed.\n");
      exit(1);
    }

    int rc0 = fork();
    if (rc0 < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc0 == 0) {
        //reader
        close(mypipe[1]);
        printf("reader: hello, I am child (pid:%d)\n", (int) getpid());
        char buf[128];
        read(mypipe[0], buf, 128);
        printf("reader: buf = %s\n", buf);
        close(mypipe[0]);
	    exit(0);
    }

    int rc1 = fork();
    if (rc1 < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc1 == 0) {
        //reader
        close(mypipe[0]);
        printf("writer: hello, I am child (pid:%d)\n", (int) getpid());
        char *msg = "message";
        write(mypipe[1], msg, strlen(msg) + 1);
        close(mypipe[1]);
        exit(0);
    }


    int wc;
    while ((wc = wait(NULL)) != -1) {
        printf("hello, I am parent of (wc:%d) (pid:%d)\n",
            wc, (int) getpid());
    }

    return 0;
}
