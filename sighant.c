#include "sighant.h"

/**
 * implementation of a singalhandler, for more information
 * visit the header file.
 */

void signal_handler(int sig){

  for(int i = 0; i < active_pid; i++){

    kill(pid_list[i], sig);

  }

  fprintf(stdout, "\n");
  if(active_pid == 0){
    print_dir();
  }
  active_pid = 0;
  return;

}
