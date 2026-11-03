int gsh_cd(char **args);
int gsh_help(char **args);
int gsh_exit(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
};

char *builtin_args[] = {
    "[directory]",
    "(shows this menu)",
    "(exits gsh)",
};

int (*builtin_func[])(char **) ={
    &gsh_cd,
    &gsh_help,
    &gsh_exit
};

int gsh_num_builtins(){
    return sizeof(builtin_str) / sizeof(char *);
}

int gsh_cd(char **args){
    if(args[1] == NULL){
	fprintf(stderr,"gsh: expected argument to \"cd\"\n");
    }else{
	if(chdir(args[1]) != 0){
	    perror("gsh");
	}
    }
    return 1;
}

int gsh_help(char **args){
    int i;
    printf("'Generic' Shell\n");
    printf("build in programs:\n");

    for(i = 0; i < gsh_num_builtins(); i++) {
	printf("%s %s\n", builtin_str[i], builtin_args[i]);
    }

    return 1;
}

int gsh_exit(char **args){
    return 0;
}
