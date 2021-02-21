#include "mish.h"

/*Program to represent a shell, for more info visit the header file*/

int main(int argc, char *argv[]){

  active_pid = 0;

  command commands[MAXCOMMANDS];

  signal(SIGINT, signal_handler);

  do{

    char input[SIZE];
    print_dir();

    char c;
    int i = 0;
    while((c = fgetc(stdin)) != '\n'){
      if(c == EOF){
         exit(0);
      }
      input[i++] = c;
    }

    input[i] = '\0';

    if(strlen(input) > 1 && strlen(input) <= SIZE){

      int comc = parse(input, commands);

      //no pipes
      if(comc == 1){
        if(execOwn(commands[0]) < 0){
          execNoPipe(commands[0]);
        }
      }
      else if(comc > 1){
        execWPipe(commands, comc);
      }
      else{
        fprintf(stderr, "Wrong input\n");
      }
    }

    active_pid = 0;
  }while(1);

}

void print_dir(){

  if (isatty(STDOUT_FILENO) && isatty(STDIN_FILENO)) {
    fprintf(stdout, "mish%% ");
    fflush(stdout);
  }
}

int execNoPipe(command commands){

  pid_t pid = fork();
  if(pid < 0){
    perror("Error forking child");
    return 1;
  }
  //child
  else if(pid == 0){

    //check for redirections
    if(commands.infile != NULL){
      if(redirect(commands.infile, 0, STDIN_FILENO) < 0)
        exit(EXIT_FAILURE);
    }

    if(commands.outfile != NULL){
      if(redirect(commands.outfile, 1, STDOUT_FILENO) < 0)
          exit(EXIT_FAILURE);
    }


    if(execvp(commands.argv[0], commands.argv) < 0){
      fprintf(stderr, "Could not execute command '%s'\n", commands.argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  //waiting for child to finish
  else{
    pid_list[active_pid++] = pid;
    wait(NULL);
  }
  return 0;
}

int execWPipe(command commands[], int pipes){

  int pip[2];
  pid_t pid;
  int tempfd = 0; //stdin

  for(int i = 0; i < pipes; i++){

    pipe(pip);
    if(pipe < 0){
      fprintf(stderr, "Could not create pipe.\n");
      exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid < 0){
      exit(EXIT_FAILURE);
    }
    else if(pid == 0){

      if(commands[i].infile != NULL){
        if(redirect(commands[i].infile, 0, STDIN_FILENO) < 0)
          exit(EXIT_FAILURE);
      }

      if(commands[i].outfile != NULL){
        if(redirect(commands[i].outfile, 1, STDOUT_FILENO) < 0)
            exit(EXIT_FAILURE);
      }

      if(dup2(tempfd, 0) == -1){
        printf("Could not dup file descriptor.\n");
        exit(EXIT_FAILURE);
      }

      if(i < (pipes-1)){
        if(dupPipe(pip, 1, 1) < 0)
          exit(EXIT_FAILURE);
      }
      close(pip[0]);

      if(execvp(commands[i].argv[0], commands[i].argv) < 0){
        fprintf(stderr, "Invalid command '%s'\n", commands[i].argv[0]);
        exit(EXIT_FAILURE);
      }
    }
    else{
      pid_list[active_pid++] = pid;
      close(pip[1]);
      tempfd = pip[0];
    }

  }
  for(int i = 0; i < pipes; i++){
    wait(NULL);
  }

  return 0;
}

int execOwn(command commands){

  if(strcmp(commands.argv[0], "cd") == 0){
    if(commands.argc > 1){
      if(chdir(commands.argv[1]) < 0){
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
      }
    }
    else{
      if(chdir(getenv("HOME")) < 0){
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
      }
    }
  }

  else if(strcmp(commands.argv[0], "echo") == 0){

    echocmd(commands);

  }

  else if(strcmp(commands.argv[0], "exit") == 0){
    exit(0);
  }

  else{
    return -1;
  }
  return 0;
}

void print_command(command com) {
    int i = 0;

    printf("{\n");
    printf("  Argv: [");

    while (*(com.argv + i) != NULL) {
        /* Print each argument. */
        printf("%s", (*(com.argv + i)));
        if (i < com.argc - 1) printf(", ");
        i++;
    }
    printf("]\n");

    printf("  Argc: %d\n", com.argc);
    printf("  Infile: %s\n", com.infile);
    printf("  Outfile: %s\n", com.outfile);
    printf("}\n");
}

int echocmd(command commands){

  //temp filefd
  int tempin, tempout;

  if(commands.infile != NULL){
    tempin = dup(0);
    if((redirect(commands.infile, 0, STDIN_FILENO)) < 0){
      return -1;
    }
  }

  if(commands.outfile != NULL){
    tempout = dup(1);
    if((redirect(commands.outfile, 1, STDOUT_FILENO)) < 0){
      return -1;
    }
  }

  for(int i = 1; i < commands.argc; i++){
    printf("%s", commands.argv[i]);
    if(i < (commands.argc - 1)){
      fprintf(stdout, " ");
    }
  }
  fprintf(stdout, "\n");
  fflush(stdout);

  //reset stdio
  if(commands.infile != NULL)
    dup2(tempin, 0);

  if(commands.outfile != NULL)
    dup2(tempout, 1);

  return 0;
}
