#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    
    int fn = open("p6_out.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    int loops = atoi(argv[1]);

    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
  
        char *s = "Child: messages messages messages messages1 messages messages messages messages messages\n";
        int i = 0;
        while (i < loops) {
            write(fn, s, strlen(s));
            i ++;
        }
	    sleep(1);
        close(fn);
    } else {
        // parent goes down this path (original process)
        char *s = "Parent: messages messages messages messages2 messages messages messages messages messages\n";
        int i = 0;
        while (i < loops) {
            write(fn, s, strlen(s));
            i ++;
        }
        close(fn);
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
	       rc, wc, (int) getpid());
   
    }
    return 0;
}
