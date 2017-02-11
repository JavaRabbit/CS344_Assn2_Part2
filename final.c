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
      // here is looping over files

      // use a varialbe fileName to store the file name
      char fileName[10];
      strcpy(fileName, sd->d_name);
      //printf("The file name is %s\n", fileName);


      // concatentate the file name with the directory for the complete file path
      char filePath[30] = "./kwongb.rooms/";

      strcat(filePath, fileName);
      printf("%s\n", filePath);

      // set the file pointer to the correct name
      FILE *roomFile = fopen(filePath, "r");

      // optional, but check if file pointer is null
      if(roomFile == NULL){
        printf("roomFile is  null");
      }

      // now that we have a file, let's read the first line and get the room name
      char line[50];

      // create a struct with the name of room name. Actual struct name could be anything
      struct Room baz;
      baz.roomName = fileName;

      while(fgets(line, sizeof(line), roomFile) != NULL){
         printf("Created a struct with room name: %s\n", baz.roomName);

      }

      // now that struct is complete, put it into struct array
      rooms7[structCounter] = baz;
      structCounter++;

      // close roomFile pointer
      fclose(roomFile);


    }

  }

  printf("the last struct is %s\n", rooms7[3].roomName);

  // close directory
  closedir(dir);

}
