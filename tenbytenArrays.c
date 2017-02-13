#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

// Prototypes

void setRoomsArray();
void openDirectories();

void startGame();
void continueGame();
void getPlayerLocation();
void endGame();

// hard coded string arrays
// 10 rooms + 1 is 11,  10 rooms + 1 blank + 1 column for roomType = 12
int rooms[11][12];

// hard coded room Names :( remove ideally)
char *roomNames[11] = {
  "blankroom","ALPHA", "BETA", "CHI", "FOUR",
  "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN"
};

// rooms but be 4  types, since C array default value is 0
char *roomTypes[4] = {"blank","START_ROOM", "MID_ROOM", "END_ROOM"};

// current player position is a int variable which holds which room
// player is currently at.  If value is 6, then player is in roomNames[6]
int currentPlayerPosition = 0;

// variable to count steps
int stepCounter = 0;
// array to hold roomlocations traveled through
int travelHistory[100];
int travelHistoryPointer = 0;

int main(){


  setRoomsArray();

  openDirectories();

  startGame();
  continueGame();
  endGame();


  return 0;
}


void openDirectories(){
  // pointers
  DIR *dir;
  struct dirent *sd;

  // use this string to hold the directory we'll be using. eg kwongb.rooms.12345
  char finalDirectory[40];
  strcat(finalDirectory,"./"); // file will need this
  printf("RIght now the finalDirectory is %s\n", finalDirectory);
  long int timeMS = 0; // variable to hold result of mtime
  char tmpDirHolder[35]; // a string variable to hold value of newest directory
  //   FOR GETTING CORRECT RECENT Kwongb.rooms.
  //  iterate over all the directories. Check that the directory / file name
  // has kwongb and numbers eg 1234 that represents the PID
  DIR  *d;
  struct dirent *dirr;
  d = opendir(".");
  if (d)
  {

    while ((dirr = readdir(d)) != NULL)
    {
      //printf("%s is a directory name!!!!!!!!!!\n", dirr->d_name);
      char directoryName[100];
      strcpy(directoryName,dirr->d_name);
      char first12Chars[12];
      strlcpy(first12Chars, directoryName, 12); // copy 6 values kwongb start at pos 0

      int isKwongb = strcmp("kwongb.room", first12Chars);
      if(isKwongb ==0){
        printf("the substring is %s and the file path is %s\n", first12Chars, directoryName);

        //  now that we have only directories that start with kwongb.rooms.nnnnn
        // find the newest directory

        struct stat buf;
        stat(directoryName, &buf);

        printf("st_mode for this file is = %ld \n", buf.st_mtime);

        if(buf.st_mtime > timeMS){
          timeMS  = buf.st_mtime;  // set timeMS to newer file
          strcpy(tmpDirHolder,directoryName);
        }
      }
    } // end of the while loop

    closedir(d);

    //now that all directories have been looped through
    strcat(finalDirectory, tmpDirHolder); // add file name of newest file
    strcat(finalDirectory, "/"); // add the final backslash
    printf("FINALLY, the newest kwongb dir is %s\n", finalDirectory);
  }

  //  END of GETTING CORRECT RECENT



  // open the correct directory ---FIX -------
  dir = opendir(finalDirectory);       // HERE !!!!!!!!!!!!! HERE!!!!!!!!

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
      //printf("this is a . file\n");  // delete this later
    } else {
      // then file isn't . or .., so process this file

      // use a variable fileName to store the file name
      char fileName[10];
      strcpy(fileName, sd->d_name);
      //printf("The file name is %s\n", fileName);


      // concatentate the file name with the directory for the complete file path
      //char filePath[30] = "./kwongb.rooms.43075/";    // HERE !!!!!!!!!!!!! HERE!!!!!!!!
      char filePath[40];
      strcpy(filePath, finalDirectory);
      printf("4  the filepath using the final directory is %s\n", filePath);
      strcat(filePath, fileName);
      //printf("1 This is the file path%s\n", filePath);

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
      int currentRow = 0;  // DELETE this, no longer necessary for 11x12

      // this actually loops over each line of the file    LINE LINE  LINE LINE  LINE  LINE  LINE
      while(fgets(line, sizeof(line), roomFile) != NULL){


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
          //strlcpy(newStr, &line[11], lenToCopy+1);   //***********************
          memcpy(newStr, line +11, lenToCopy+1 );
          newStr[lenToCopy] = '\0'; // force null terminator at end of string
          //printf("THe room NAME is:   %s\n", newStr);


          // this for loop iterates and finds which corresponding
          // room NAME value it is, either 0..9 or start, mid, end
          int mpmm;  // crazy iterator names because C is so buggy
          for(mpmm= 0; mpmm < 11; mpmm++){
            if(strcmp(newStr, roomNames[mpmm]) == 0){
              // we've found the room. Set the currentRow
              currentRow = mpmm;
              //printf("confirmed, the ROOM NAME is %s\n", roomNames[i]);
            }
          }

        }



         // TYPE TYPE TYPE TYPE TYPE TYPE TYPE TYPE TYPE TYPE TYPE TYPE

        //  Scan the each line of the file to find the line that specifies ROOM TYPE
        //  match the room type with Room Type values. Remember that
        //  rooms Start, Mid, and End,  correspond to 1,2,3 respectively
        //  Put the value, 1,2 or 3 value into rooms[][] column [][11] or the 12th column
        //strlcpy(substringForRoomType, &line[0], 10); // onl
        memcpy(substringForRoomType, line, 10);
        substringForRoomType[9] ='\0';// force null terminator
        //printf("bird is %s\n", substringForRoomType);

        // if the line starts with ROOM NAME,
        int cmpWithRoomType;
        cmpWithRoomType = strcmp("ROOM TYPE", substringForRoomType);
        if(cmpWithRoomType == 0){

          lenToCopy = strlen(line) - 12;  // "ROOM TYPE: " is 11, then + 1
          char typeStr[lenToCopy +1];
          // 11 since where start, starts at 0
          //strlcpy(typeStr, &line[11], lenToCopy+2);
          memcpy(typeStr, line+11 , lenToCopy+2 );
          typeStr[lenToCopy+1] = '\0'; // force null terminator at end of string
          printf("THe room TYPE is:   %s\n", typeStr);

          // this for loop iterates and finds which corresponding
          // room type value it is, either  1, 2 ,3 or start, mid, end
          // start i at 1 because room type names are 1,2, or 3
          int nope;
          for(nope = 1; nope < 4; nope++){
            if(strcmp(typeStr, roomTypes[nope]) == 0){
              // set rooms array with  the correct roomType
              rooms[currentRow][11] = nope;
              //printf("confirmed, the type is %s\n", roomTypes[rooms[currentRow][11]]);
            }
          }

        } // end cmpWithRoomType == 0








        // copy the line into substringForConnect CONECT CONNECT
        strncpy(substringForConnect, line, 7); //7 for connect
        //printf("The substring is %s\n", substringForConnect);

        //  if the line starts with CONNECT, we add the room connection
        int cmpWithConnect; // to hold result of string comparison
        cmpWithConnect = strcmp("CONNECT", substringForConnect);

        if(cmpWithConnect == 0){
          //printf("Thisi is the connect lien\n");

          lenToCopy = strlen(line) - 15;  // "CONNECTION 1: " is 14, then + 1
          char newStr[lenToCopy +1];
          //strlcpy(newStr, &line[14], lenToCopy+1);  // removing this strlcpy line
          memcpy(newStr, line+14,lenToCopy);
          newStr[lenToCopy] = '\0';

          printf("I am connected to room:%s\n", newStr);


          // this for loop iterates and finds which corresponding
          // room NAME value it is, either 0..9 or start, mid, end
          // then correctly set the
          // start at 1 because valid roomNames start at element 1 fo roomNames array
          int posCol;
          for(posCol = 1; posCol < 11; posCol++){
            if(strcmp(newStr, roomNames[posCol]) == 0){
              //  is hard coded because roomName will be at index[fileCounter][0]
              // We have a connection! connectionPositioner starts at 2
              rooms[currentRow][posCol] = 55; // using 55 as a number signifying that rooms are connected
              rooms[posCol][currentRow] = 55;  // if room a goes to b, then b goes to a
              //printf("therefore %s is connected to %s\n", roomNames[currentRow], roomNames[i]);
            }
          }

        }




      } // end of while that fgets each line




      fclose(roomFile);
      //printf("Just closed room file \n\n");
      fileCounter = fileCounter + 1; // increment the file counter


    } // end of else (it isn't a . or .. file)

  }  // end of while readdir

  // close directory
  closedir(dir);
}



// this method is going to set all value of the room array to -1
// rooms array is 11 x 12
void setRoomsArray(){
  int row, col;
  for( row = 0; row < 11; row++){
    for(col = 0; col < 12; col++){
      rooms[row][col] = 0;   // set all values to -1
    }
  }
}

void startGame(){
  // first we need to find the room which is the start room
  // iterate over each room 1-10 inclusive, and see which one has 0
  int roomPos;
  for(roomPos = 1; roomPos < 11; roomPos++){
    // remember that room type lives in column [11], and startRoom is value 1
    if(rooms[roomPos][11] == 1){
      //printf("the start room is found, the room name is %s\n", roomNames[roomPos]);
      // now that the room 1-10 inclusive is found, set value of currentPlayerPosition
      currentPlayerPosition = roomPos;

      // add current room to travel history
      // travelHistory and increment travelHistoryPointer
      travelHistory[travelHistoryPointer] = roomPos;
      travelHistoryPointer++;
    }
  }

}


void continueGame(){



  //  Now that the destination string is trimmed, check to see if it is a valid room
  int mybool = 1;
  while(mybool == 1){

    // check if at end room, remember that room type lives at rooms[currentPlayerPosition][11]
    // end room type is at value 3
    if(rooms[currentPlayerPosition][11] == 3){
      // Player is at end room and is done.  Display path to user.
      // return to end by returning to main method
      return;
    }
    // if not at end room, display the current location to the player
    printf("\nCURRENT LOCATION: %s\n", roomNames[currentPlayerPosition]);
    printf("POSSIBLE CONNECTIONS:");

    // loop to show all room connections. posConnections are from 1-10 inclusive
    // 55 is the magic number that indicates a connection to a room
    int posConnection;
    for(posConnection = 1; posConnection < 11; posConnection++){
      if(rooms[currentPlayerPosition][posConnection] == 55){
        printf(" %s", roomNames[posConnection]);
      }
    }
    printf(".\nWHERE TO? >");

    char destination[15];
    fgets(destination, 15, stdin);


    // trim off new line character
    if(strlen(destination) > 0){
      int newLineLocation = strlen(destination) - 1;
      destination[newLineLocation] = '\0';
    }

    // check destination is valid room and valid connection
    int destinationNum = 0;
    int boolForValidRoom= 0;
    int looper;
    for(looper =1; looper < 11; looper++){
      if(strcmp(destination, roomNames[looper]) == 0){
        //printf("It's a real room %s\n", roomNames[i]);
        destinationNum = looper; // set the room destination number
        //opps set currentPlayerPosition
        boolForValidRoom = 1; // set this value to something arbitrary

      }
    }
    // now check if it is a valid connection
    if((rooms[currentPlayerPosition][destinationNum] == 55) && (boolForValidRoom == 1)){
      //printf("Yes, it is a valid connection\n");
      // this is what actually moves the player
      currentPlayerPosition = destinationNum;  // set the player position to desired destinationNum
      stepCounter++;  // add 1 to the number of moves the player makes
      travelHistory[travelHistoryPointer] = currentPlayerPosition;
      travelHistoryPointer++;
      continue;  // get out of this iteration of the while loop
    } else {
      // if the code gets here means player did not enter a valid room
      printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN\n");
      continue;
    }


  }


} // end of continueGame



void getPlayerLocation(){
  printf("the player is at %d\n", currentPlayerPosition);
}

void endGame(){
  printf("Congrats the game is finally over\n");
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", stepCounter);
  int counter;
  for(counter= 0; counter < travelHistoryPointer; counter++){
    printf("%s\n", roomNames[travelHistory[counter]]);
  }
}
