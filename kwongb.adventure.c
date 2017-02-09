#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>


// Prototypes
void findDir();

int main(){
  printf("thello world\n");
  printf("My process id is %d\n", getpid());
  findDir();

  // open the right directory using stat and st_mtime
  return 0;
}

void findDir(){
  DIR *dir;
  struct dirent *sd;
  FILE *entry_file;

  dir = opendir("./kwongb.rooms");
  if(dir == NULL){
    printf("no such folder\n");
    exit(1);
  }

  int result;
  while ( (sd = readdir(dir)) != NULL){
    printf("Files are %s\n", sd->d_name);
    result = strcmp("ALPHA", sd->d_name);
    if(result == 0){
      printf("this is the alpha file\n");
    }

  }

  closedir(dir);
}
