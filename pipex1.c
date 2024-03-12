#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
 pid_t   pid;
 int pfd[2];

 pipe(pfd);
 pid = fork();
 if (pid < 0)
 {
  fprintf(stderr, "Fork2 failed.\n");
  return 1;
 }
 else if (pid == 0)
 {
  close(pfd[0]); //close unused end (the reading end) of the pipe
  dup2(pfd[1], 1);
  close(pfd[1]); //close it immediately as it will no longer be used
  write(1, "this is a new message", 22);
  exit(EXIT_SUCCESS);
 }
 else
 {
  close(pfd[1]); //close unused end (the writing end) of the pipe
  dup2(pfd[0], 0);
  close(pfd[0]); //close it immediately as it will no longer be used
  char  str[22];
  read(0, str, 22);
  printf("Parent received:%s\n", str);
  wait(NULL);
 }
 return EXIT_SUCCESS;
}