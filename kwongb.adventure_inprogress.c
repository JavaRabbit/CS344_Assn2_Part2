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
void findDir();
void createStructs();
void readStruct();
void createRealStructs();

int main(){
  printf("My process id is %d\n", getpid());
  findDir();
  //createStructs();
  readStruct();
  createRealStructs();


  // open the right directory using stat and st_mtime
  return 0;
}

void findDir(){
  DIR *dir;
  struct dirent *sd;
  //FILE *filepointer; // a pointer to a file

  dir = opendir("./kwongb.rooms");
  if(dir == NULL){
    printf("no such folder\n");
    exit(1);
  }

  int result; // this is for string comparison
  while ( (sd = readdir(dir)) != NULL){
    printf("Files are %s\n", sd->d_name);
    result = strcmp("ALPHA", sd->d_name);
    if(result == 0){
      printf("this is the alpha file\n");
      //filepointer = fopen(sd->d_name, "r");

      FILE *roomFile = fopen("./kwongb.rooms/ALPHA", "r");
      if(roomFile == NULL){
        printf("roomFile is  null");
      }

      char line[50];
      char substring[10];
      while(fgets(line, sizeof(line), roomFile) != NULL){
         printf("%s\n", line );
         strncpy(substring, line, 8);
         printf("%s\n", substring);
      }

      fclose(roomFile);
    }

  }
  closedir(dir);
}

void createStructs(){
  struct Room baz;
  baz.roomName = "foo bar room";
  printf("We live in %s\n", baz.roomName);
  rooms7[0] = baz;

  struct Room baz2;
  baz2.roomName = "foo bar room";
  rooms7[1] = baz2;
  printf("We live in %s\n", baz2.roomName);
  baz.connectingRooms[0] = &baz;
  printf("baz is connected to %s\n", baz.connectingRooms[0]->roomName);
}

void readStruct(){
  printf("Global baz is connected to %s\n", rooms7[1].roomName);
}

void createRealStructs(){
  DIR *dir;
  struct dirent *sd;
  //FILE *filepointer; // a pointer to a file

  dir = opendir("./kwongb.rooms");
  if(dir == NULL){
    printf("no such folder\n");
    exit(1);
  }
  int i = 0;
  while ( (sd = readdir(dir)) != NULL){
    printf("Files are %s\n", sd->d_name);
    int result;
    int result2; // 0 means equal strings
    result = strcmp(".", sd->d_name);
    result2 = strcmp("..", sd->d_name);
    if(result ==0 || result2 ==0 ){
      printf("this is a . file\n");
    } else {
      // create a struct
      // open the file itself, and get the name of the room
      //char cat[10] = sd->d_name;
      //strcpy(cat, sd->d_name);
      struct Room cat;
    }

  }
}
