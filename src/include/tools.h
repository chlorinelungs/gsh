#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int gsh_cd(int argc, char *argv[]);
int gsh_ls(int argc, char *argv[]);
int gsh_help(int argc, char *argv[]);
int gsh_exit(int argc, char *argv[]);

#define RED "\x1b[31m"
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

char *builtin_str[] = {
    "cd",
    "ls",
    "help",
    "exit",
};

char *builtin_help[] = {
    "[directory]",
    "(lists directories)",
    "(shows this menu)",
    "(exits gsh)",
};

int (*builtin_func[])(int argc, char *argv[]) ={
    &gsh_cd,
    &gsh_ls,
    &gsh_help,
    &gsh_exit
};

int gsh_num_builtins(){
    return sizeof(builtin_str) / sizeof(char *);
}

int gsh_cd(int argc, char *argv[]){
    if(argv[1] == NULL){
	fprintf(stderr,"gsh: expected argument to \"cd\"\n");
    }else{
	if(chdir(argv[1]) != 0){
	    perror("gsh");
	}
    }
    return 1;
}

int showAll = 0;

int gsh_ls(int argc, char *argv[]){
    int opt;
    while ((opt = getopt(argc, argv, "a")) != -1){
	switch (opt){
	    case 'a':
		showAll = 1;
		break;
	    default:
		fprintf(stderr, "useage: %s [-a] [path]\n", argv[0]);
		return 1;
	}
    }
    const char *path = (optind < argc) ? argv[optind] : ".";
    DIR *dir = opendir(path);

    if(!dir){
	perror("gsh");
	return 1;
    }
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
	if (!showAll && entry->d_name[0] == '.') continue;
	struct stat st;
	stat(entry->d_name, &st);
	
	if(S_ISDIR(st.st_mode)){
	    printf(BLUE "%s " RESET, entry->d_name);
	}else if(S_ISREG(st.st_mode)){
	    printf("%s " RESET, entry->d_name);
	}else{
	    printf(GREEN "%s " RESET, entry->d_name);
	}
    }

    closedir(dir);
    printf("\n");
    return 1;
}

int gsh_help(int argc, char *argv[]){
    int i;
    printf("'Generic' Shell\n");
    printf("build in programs:\n");

    for(i = 0; i < gsh_num_builtins(); i++) {
	printf("%s %s\n", builtin_str[i], builtin_help[i]);
    }

    return 1;
}

int gsh_exit(int argc, char *argv[]){
    return 0;
}
