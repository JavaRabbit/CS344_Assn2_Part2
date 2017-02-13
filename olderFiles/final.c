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


struct Room rooms7[8];

// Prototypes
void openDirCreateStructs();
void fiz();

int main(){
  printf("testing\n");
  openDirCreateStructs();
  //fiz();
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
  int lenToCopy = 0;

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
      printf("1 This is the file path%s\n", filePath);

      // set the file pointer to the correct name
      FILE *roomFile = fopen(filePath, "r");

      // optional, but check if file pointer is null
      if(roomFile == NULL){
        printf("roomFile is  null");
      }

      // now that we have a file, let's read the first line and get the room name
      char line[50];

      // create a struct with the name of room name. Actual struct name could be anything
      //struct Room baz;
      rooms7[structCounter].roomName = fileName; // because name of file is the name of the room

      // now that struct is complete, put it into struct array
      //rooms7[structCounter] = baz;
      printf("Location is at %d\n", structCounter);
      printf("Just added %s\n", rooms7[structCounter].roomName);
      printf("Location is at %d\n", structCounter);
      structCounter = structCounter + 1;


      // this might change and maybe I'll use while loop to scan for room name


      // declare a variable to hold a substring
      char substring[8];


      // iterate over each line of the array
      while(fgets(line, sizeof(line), roomFile) != NULL){

         // while loop is a place to check for connecting rooms
         strncpy(substring, line, 7); //7 for connect
         //printf("the substring is: %s\n", substring);


         //  if the line starts with CONNECT, we add the room connection
         int fifi; // to hold result of string comparison
         fifi = strcmp("CONNECT", substring);

         if(fifi ==0){
           printf("this is a connection\n");

           printf("the length of line is %lu\n", strlen(line));


           //int lenToCopy=0; // always initialize to 0, otherwise issues!!! of too long copy
           lenToCopy = strlen(line) - 15;  // 14 + 1 = 15
           printf("%d is how much to copy\n", lenToCopy);
           printf("%s", line); // line already has new line at end

           //char newStr[lenToCopy ]; // initialize a string of 1 byte larger than whats needed
           //char *newStr=(char*)malloc(sizeof(char)*lenToCopy);
           char newStr[lenToCopy +1];
           //strncpy(newStr, &line[14], lenToCopy);   // 14 since where start, starts at 0
           strlcpy(newStr, &line[14], lenToCopy+1);
           printf("%s\n", newStr);


           // clear the string array
           //free(newStr);

         }

      }

      // check for room type

      // close roomFile pointer
      fclose(roomFile);
      printf("%s is the name of array 0\n", rooms7[0].roomName);
      printf("FILE NOW CLOSED **********\n");
      printf("\n");


    }

  }



  // close directory
  closedir(dir);
  printf("\n FIZZZ ------\n");
  printf("Why is this    %s\n", rooms7[0].roomName);

}

void fiz(){
  printf("\n FIZZZ ------\n");
  printf("%s\n", rooms7[1].roomName);
}
