#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Prototypes
void fiz();
void openDirectories();

char *rooms[7][8];


int main(){

  printf("starting usingArrays.c\n");
  rooms[0][0] = "room at 0 0 ";

  fiz();
  openDirectories();


  return 0;
}

void fiz(){
  printf("room at 0 0 is: %s\n", rooms[0][0]);
}

void openDirectories(){
  // pointers
  DIR *dir;
  struct dirent *sd;

  // open the correct directory ---FIX -------
  dir = opendir("./kwongb.rooms");

  if(dir == NULL){
    printf("no such folder\n");
    exit(1);
  }

  // this counts which file I'm interating over.
  int fileCounter = 0;

  // use a while loop to loop over all files in dir EACH FILE EACH FILE EACH FILE EACH FILE EACH FILE
  while ( (sd = readdir(dir)) != NULL){
    printf("File is %s\n", sd->d_name);

    // use strcmp to skip over . and .. files
    int result;
    int result2; // 0 means equal strings
    result = strcmp(".", sd->d_name);
    result2 = strcmp("..", sd->d_name);
    if(result ==0 || result2 ==0 ){
      printf("this is a . file\n");
    } else {
      // then file isn't . or ..

      // use a varialbe fileName to store the file name
      char fileName[10];
      strcpy(fileName, sd->d_name);
      //printf("The file name is %s\n", fileName);


      // concatentate the file name with the directory for the complete file path
      char filePath[30] = "./kwongb.rooms/";

      strcat(filePath, fileName);
      printf("1 This is the file path%s\n", filePath);

      // set the file pointer to the correct name
      FILE *roomFile = fopen(filePath, "r");

      // optional, but check if file pointer is null
      if(roomFile == NULL){
        printf("roomFile is  null");
      }


      // now that we have a file, let's read the first line and get the room name
      char line[50]; // this holds the actual full line read from FILE
      // declare a variable to hold a substring
      char substringForConnect[8];  // connect has 7 chars, then +1
      char substringForRoomName[10]; // room name has 9 chars, then + 1
      char substringForRoomType[10]; // room type has 9 char

      int lenToCopy = 0; // to hold the substring to copy, eg. room name, connection

      // this actually loops over each line of the file    LINE LINE  LINE LINE  LINE  LINE  LINE
      while(fgets(line, sizeof(line), roomFile) != NULL){
        //printf("%s\n", line);

        // copy the line into substringForConnect
        strncpy(substringForConnect, line, 7); //7 for connect
        //printf("The substring is %s\n", substringForConnect);

        //  if the line starts with CONNECT, we add the room connection
        int cmpWithConnect; // to hold result of string comparison
        cmpWithConnect = strcmp("CONNECT", substringForConnect);

        if(cmpWithConnect == 0){
          printf("Thisi is the connect lien\n");

          lenToCopy = strlen(line) - 15;  // "CONNECTION 1: " is 14, then + 1
          char newStr[lenToCopy +1];
          strlcpy(newStr, &line[14], lenToCopy+1);
          printf("I am connected to room:   %s\n", newStr);
        }






        // copy the line into
        strncpy(substringForRoomName, line, 9); //9  for "room Name"

        // if the line starts with ROOM NAME,
        int cmpWithRoomName;
        cmpWithRoomName = strcmp("ROOM NAME", substringForRoomName);

        if(cmpWithRoomName == 0){
          //printf("THIS is the room name line\n");

          // Now lets get the room name!
          //printf("the length of line is %lu\n", strlen(line));
          lenToCopy = strlen(line) - 12;  // "ROOM NAME: " is 11, then + 1
          //printf("%d is how much to copy\n", lenToCopy);

          //char newStr[lenToCopy ]; // initialize a string of 1 byte larger than whats needed
          //char *newStr=(char*)malloc(sizeof(char)*lenToCopy);
          char newStr[lenToCopy +1];
          // 11 since where start, starts at 0
          strlcpy(newStr, &line[11], lenToCopy+1);
          printf("THe room NAME is:   %s\n", newStr);
        }




      } // end of while that fgets each line

      fclose(roomFile);
      printf("Just closed room file \n\n");
    } // end of else (it isn't a . or .. file)

  }  // end of while readdir

  // close directory
  closedir(dir);
}
