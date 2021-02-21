#include "execute.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int dupPipe(int pip[2], int end, int destfd){

  if(dup2(pip[end], destfd) == -1){
    fprintf(stderr, "Error dup pipe\n");
    return -1;
  }

  return destfd;
}

int redirect(char *filename, int flags, int destfd){

  int filefd;
  fflush(stdout);

  //read
  if(flags == 0){
    filefd = open(filename, flags);
  }
  //write
  else if(flags == 1){
    //file does not exitst, create it.
    if(access(filename, F_OK) == -1){
      filefd = open(filename, flags | O_CREAT);
    }
    else{
      errno = 17;
      fprintf(stderr, "%s\n", strerror(errno));
      return -1;
    }
  }

  if(filefd < 0){
    fprintf(stderr, "%s\n", strerror(errno));
    return -1;
  }

  dup2(filefd, destfd);
  close(filefd);
  return destfd;
}
