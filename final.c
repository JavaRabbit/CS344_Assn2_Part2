#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct Room{
  char *roomName;
  char *roomType;
  struct Room *connectingRooms[6]; // 6 max connections
};

struct Room rooms7[7];

// Prototypes
void openDirCreateStructs();

int main(){
  printf("testing\n");
  openDirCreateStructs();
  return 0;
}

void openDirCreateStructs(){
  // pointers
  DIR *dir;
  struct dirent *sd;

  // open the correct directory ---FIX -------
  dir = opendir("./kwongb.rooms");


  // a variable for the struct array counter
  int structCounter = 0;

  while ( (sd = readdir(dir)) != NULL){

    // if the file is a . or .. skip
    int result;
    int result2;
    result = strcmp(".", sd->d_name);
    result2 = strcmp("..", sd->d_name);
    if(result ==0 || result2 ==0 ){
      // if the file is  . or .. file, break out of while loop
      printf("this is a . file\n"); // actually should be break here
    } else {
      printf("Files are %s\n", sd->d_name);

      // now that we have a file, let's read the first line and get the room name
      


    }

  }

  // close directory
  closedir(dir);

}
