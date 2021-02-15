#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        char *tag = argv[1];
        if (strcmp(tag, "execl") == 0) {
            execl("/bin/ls", "ls", "-l", (char *) NULL);
        } else if (strcmp(tag, "execle") == 0) {
            char* env_list[] = { 
                "SOURCE=MYDATA",
                "TARGET=OUTPUT",
                "lines=65",
                NULL,
            };
            execle("/bin/ls", "ls", "-l", (char *) NULL, env_list);
        } else if (strcmp(tag, "execlp") == 0) {
            execlp("/bin/ls", "ls", "-l", (char *) NULL);
        } else if (strcmp(tag, "execv") == 0) {
            char* myargs[] = {
                "ls",
                "-l",
                NULL,
            };
            execv("/bin/ls", myargs);
        } else if (strcmp(tag, "execvp") == 0) {
            char* myargs[] = {
                "ls",
                "-l",
                NULL,
            };
            execvp(myargs[0], myargs);  // runs word count
        }
    } else {
        int wc = wait(NULL);
        printf("hello, I am parent of %d (pid:%d), wc = %d\n",
	       rc, (int) getpid(), wc);
    }
    return 0;
}
