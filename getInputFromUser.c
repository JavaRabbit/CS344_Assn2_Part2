#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(){
  char destination[15];
  printf("Where do you want to go?\n");
  fgets(destination, 15, stdin);
  printf("So you want to go to %s\n", destination);

  // trim destination of the newline
  int i  = strlen(destination - 1);



  cmp = strcmp(aName, destination);
  if(cmp == 0){
    printf("the strings match\n");
  } else {
    printf("nope, string does not match china\n");
  }
  return 0;
}
