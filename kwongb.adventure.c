#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>


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
  dir = opendir("./kwongb.rooms");
  if(dir == NULL){
    printf("no such folder\n");
    exit(1);
  }
  closedir(dir);
}
