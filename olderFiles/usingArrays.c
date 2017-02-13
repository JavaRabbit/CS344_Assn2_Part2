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
void setRoomsArray();
void openDirectories();
void testPrint();
void startGame();
void continueGame();
void getPlayerLocation();

// hard coded string arrays

int rooms[7][8];

// hard coded room Names
char *roomNames[11] = {
  "blankroom","ALPHA", "BETA", "CHI", "FOUR",
  "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN"
};


char *roomTypes[3] = {"START_ROOM", "MID_ROOM", "END_ROOM"};

//  values possible are 0-6, but actual room name of where player is at
// is  roomName[roooms[currentPlayerPosition][0]]
int currentPlayerPosition = 0;

int main(){


  setRoomsArray();
  //fiz();
  openDirectories();
  //testPrint();
  startGame();
  continueGame();
  //getPlayerLocation();

  return 0;
}

void fiz(){
  rooms[0][1] = 44;
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

      // instantiate a variable to hold connections.
      // iterate over connection with roomNames 1-10 (not 0 because 0 is blankroom)
      // the first connection goes into rooms[fileCounter][2].
      // the last connection goes into rooms[fileCounter][7]
      int connectionPositioner = 2;

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

          // this for loop iterates and finds which corresponding
          // room NAME value it is, either 0..9 or start, mid, end
          for(int i = 0; i < 11; i++){
            if(strcmp(newStr, roomNames[i]) == 0){
              //  is hard coded because roomName will be at index[fileCounter][0]
              // We have a connection! connectionPositioner starts at 2
              rooms[fileCounter][connectionPositioner] = i;
              connectionPositioner = connectionPositioner + 1;
              printf("therefore %s is connected to %s\n", roomNames[rooms[fileCounter][0]], roomNames[rooms[fileCounter][connectionPositioner - 1]]);
            }
          }

        }



        // ROOM NAME ROOM NAME  ROOM NAME  ROOM NAME  ROOM NAME
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
          char newStr[lenToCopy +1];
          // 11 since where start, starts at 0
          strlcpy(newStr, &line[11], lenToCopy+1);
          printf("THe room NAME is:   %s\n", newStr);


          // this for loop iterates and finds which corresponding
          // room NAME value it is, either 0..9 or start, mid, end
          for(int i = 0; i < 11; i++){
            if(strcmp(newStr, roomNames[i]) == 0){
              //  is hard coded because roomName will be at index[fileCounter][0]
              rooms[fileCounter][0] = i;
              printf("confirmed, the ROOM NAME is %s\n", roomNames[rooms[fileCounter][0]]);
            }
          }

        }

         // TYPE TYPE TYPE TYPE TYPE TYPE TYPE TYPE TYPE TYPE TYPE TYPE

        // copy into substringForRoomType, starting at 0 inclusive, and put in
        // 9 chars "Room TYPE" is 9, then add 1
        strlcpy(substringForRoomType, &line[0], 10); // onl
        //printf("bird is %s\n", substringForRoomType);

        // if the line starts with ROOM NAME,
        int cmpWithRoomType;
        cmpWithRoomType = strcmp("ROOM TYPE", substringForRoomType);
        if(cmpWithRoomType == 0){

          lenToCopy = strlen(line) - 12;  // "ROOM TYPE: " is 11, then + 1
          char typeStr[lenToCopy +1];
          // 11 since where start, starts at 0
          strlcpy(typeStr, &line[11], lenToCopy+2);
          printf("THe room TYPE is:   %s\n", typeStr);

          // this for loop iterates and finds which corresponding
          // room type value it is, either 0, 1, 2 or start, mid, end
          for(int i = 0; i < 3; i++){
            if(strcmp(typeStr, roomTypes[i]) == 0){
              // 1 is hard coded because type is at column 1
              rooms[fileCounter][1] = i;
              printf("confirmed, the type is %d\n", rooms[fileCounter][1]);
            }
          }

        } // end cmpWithRoomType == 0



      } // end of while that fgets each line




      fclose(roomFile);
      printf("Just closed room file \n\n");
      fileCounter = fileCounter + 1; // increment the file counter


    } // end of else (it isn't a . or .. file)

  }  // end of while readdir

  // close directory
  closedir(dir);
}


void testPrint(){

  printf("The value of %d\n", rooms[6][7]);

}

// this method is going to set all value of the room array to -1
void setRoomsArray(){
  for(int i = 0; i < 7; i++){
    for(int j = 0; i < 8; i++){
      rooms[i][j] = -55;   // set all values to -1
    }
  }
}

void startGame(){
  // first we need to find the room which is the start room
  // iterate over each room 0-6 inclusive, and see which one has 0
  for(int roomPos = 0; roomPos < 7; roomPos++){
    // remember that room type lives in column [1]
    if(rooms[roomPos][1] == 0){
      printf("the start room is found, the room pos is %d\n", roomPos);
      // now that the room 0-6 is found, aka row of array 7x8, set value
      currentPlayerPosition = roomPos;
    }
  }

}


void continueGame(){
  // check if at end room
  // if not at end room, display the current location to the player
  printf("CURRENT LOCATION: %s\n", roomNames[currentPlayerPosition+1]);
  printf("POSSIBLE CONNECTIONS: ");
  // loop to show all location. Might want to store into array
  // start colIterator at 2 since its where the first match is.
  for(int colIterator = 2; colIterator < 9; colIterator++){
    if(rooms[currentPlayerPosition][colIterator] != 0){
      int connectingRoom = rooms[currentPlayerPosition][colIterator];
      printf("the connecting room value is %d\n", connectingRoom);
      printf("%s ", roomNames[connectingRoom]);
    }
  }
  //printf("SOme Locatoin");
  printf("\nWHERE TO? >");
}

void getPlayerLocation(){
  printf("the player is at %d\n", currentPlayerPosition);
}
