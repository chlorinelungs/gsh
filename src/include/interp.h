#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/stat.h>

#include <tools.h>

#define GSH_RL_BUFSIZE 1024
#define GSH_TOK_BUFSIZE 64
#define GSH_TOK_DELIM " \t\r\n\a"


char *gsh_read(void){
    char *line = NULL;
    ssize_t bufsize = 0;

    if(getline(&line, &bufsize, stdin) == -1){
	if(feof(stdin)){
	    exit(EXIT_SUCCESS);
	}else{
	    perror("readline");
	    exit(EXIT_FAILURE);
	}
    }
    return line;
}

int gsh_launch(int argc, char **args){
    pid_t pid, wpid;
    pid = fork();
    int status;

    if(pid == 0){
	if(execvp(args[0], args) == -1){
	    perror("gsh");
	}
	exit(EXIT_FAILURE);
    }else if(pid < 0){
	perror("gsh");
    }else{
	do{
	    wpid = waitpid(pid, &status, WUNTRACED);
	}while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}

char **gsh_split(char *line)
{
  int bufsize = GSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, RED "gsh: allocation error\n" RESET);
    exit(EXIT_FAILURE);
  }

  token = strtok(line, GSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += GSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, RED "gsh: allocation error\n" RESET);
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, GSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}


int gsh_exec(int argc,char **args){
    int i;
    if(args[0] == NULL){
	return 1;
    }

    for(i = 0; i < gsh_num_builtins(); i++){
	if(strcmp(args[0], builtin_str[i]) == 0){
	    return (*builtin_func[i])(argc,args);
	}
    }
    return gsh_launch(argc,args);
}

void gsh_loop(int argc, char *argv[]){
    char *line;
    char **args;
    int status;
    char cwd[PATH_MAX];

    do{
	printf(GREEN ":3c > " BLUE "%s $: " RESET,getcwd(cwd, sizeof(cwd)));
	line = gsh_read();
	args = gsh_split(line);
	status = gsh_exec(argc,args);
	free(line);
	free(args);
    }while(status);
}
