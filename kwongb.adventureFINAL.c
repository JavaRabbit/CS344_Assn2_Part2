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
void endGame();
void* printTime();
void* readTimeFromFile();

// hard coded string arrays
// 10 rooms + 1 is 11,  10 rooms + 1 blank + 1 column for roomType = 12
int rooms[11][12];

char *roomNames[11] = {
  "placeHolderRoom","APPLE", "BEARCAVES", "CHICKENFARM", "DUCKVILLAGE",
  "ELEPHANT", "FROGKINGDOM", "GIRRAFFEZOO", "HIPPOTOWN", "ITCHYFLIES", "RABBITS"
};


// create thread and mutex
pthread_mutex_t mutexLock, mutexLock2;
pthread_t thr1, thr2;

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
  char finalDirectory[40]={}; // initialize to all zeros

  strcat(finalDirectory,"./"); // file will need this
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
      char directoryName[100];
      strcpy(directoryName,dirr->d_name);
      char first12Chars[12];
      //strlcpy(first12Chars, directoryName, 12); // copy 6 values kwongb start at pos 0
      memcpy(first12Chars, directoryName, 12);
      first12Chars[11]='\0'; // for the last to be a null terminator

      int isKwongb = strcmp("kwongb.room", first12Chars);
      if(isKwongb ==0){
        //  now that we have only directories that start with kwongb.rooms.nnnnn
        // find the newest directory

        struct stat buf;
        stat(directoryName, &buf);

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
  }

  //  END of GETTING CORRECT RECENT

  // open the correct directory
  dir = opendir(finalDirectory);

  if(dir == NULL){
    printf("no such folder\n");
    exit(1);
  }

  // this counts which file I'm interating over.
  int fileCounter = 0;

  // use a while loop to loop over all files in dir EACH FILE
  while ( (sd = readdir(dir)) != NULL){

    // use strcmp to skip over . and .. files
    int result;
    int result2; // 0 means equal strings
    result = strcmp(".", sd->d_name);
    result2 = strcmp("..", sd->d_name);
    if(result ==0 || result2 ==0 ){
      // do nothing. it's a . or .. file
    } else {
      // then file isn't . or .., so process this file

      // use a variable fileName to store the file name
      char fileName[10];
      strcpy(fileName, sd->d_name);

      // concatentate the file name with the directory for the complete file path
      char filePath[40];
      strcpy(filePath, finalDirectory);
      strcat(filePath, fileName);

      // set the file pointer to the correct name
      FILE *roomFile = fopen(filePath, "r");

      // optional, but check if file pointer is null
      if(roomFile == NULL){
        printf("roomFile is  null");
      }

      // now that we have a file, let's read the first line and get the room name
      char line[50]; // this holds the actual full line read from FILE
      // declare a variable to hold a substring
      char substringForConnect[8]={};  // connect has 7 chars, then +1
      char substringForRoomName[10]={}; // room name has 9 chars, then + 1
      char substringForRoomType[10]={}; // room type has 9 char

      int lenToCopy = 0; // to hold the substring to copy, eg. room name, connection

      // instantiate a variable to hold connections.
      // iterate over connection with roomNames 1-10 (not 0 because 0 is blankroom)
      // the first connection goes into rooms[fileCounter][2].
      // the last connection goes into rooms[fileCounter][7]
      int currentRow = 0;  // This hold which row we're working on. It depends on the file. We check the ro
      // room name and if the room name matches, we set current row.

      // this actually loops over each line of the file    LINE LINE  LINE LINE  LINE  LINE  LINE
      while(fgets(line, sizeof(line), roomFile) != NULL){


        // ROOM NAME ROOM NAME,  this portion of the method gets the room name
        strncpy(substringForRoomName, line, 9); //9 char since 9  for "room Name"

        // if the line starts with ROOM NAME,
        int cmpWithRoomName=0;
        cmpWithRoomName = strcmp("ROOM NAME", substringForRoomName);

        if(cmpWithRoomName == 0){

          // Now lets get the room name, because we detected that this is a room name file
          lenToCopy = strlen(line) - 12;  // "ROOM NAME: " is 11, then + 1

          // initialize a string of 1 byte larger than whats needed
          char newStr[lenToCopy +1];
          // 11 since where start, starts at 0
          memcpy(newStr, line +11, lenToCopy+1 );
          newStr[lenToCopy] = '\0'; // force null terminator at end of string

          // this for loop iterates and finds which corresponding
          // room NAME value it is, either 0..9 or start, mid, end
          int mpmm=0;  // crazy iterator names because C is so buggy
          for(mpmm= 0; mpmm < 11; mpmm++){
            if(strcmp(newStr, roomNames[mpmm]) == 0){
              // we've found the room. Set the currentRow
              currentRow = mpmm;
            }
          }

        }

        //  Scan the each line of the file to find the line that specifies ROOM TYPE
        //  match the room type with Room Type values. Remember that
        //  rooms Start, Mid, and End,  correspond to 1,2,3 respectively
        //  Put the value, 1,2 or 3 value into rooms[][] column [][11] or the 12th column
        memcpy(substringForRoomType, line, 10);
        substringForRoomType[9] ='\0'; // force null terminator to end string

        // if the line starts with ROOM NAME,
        int cmpWithRoomType;
        cmpWithRoomType = strcmp("ROOM TYPE", substringForRoomType);
        if(cmpWithRoomType == 0){
          lenToCopy = strlen(line) - 12;  // "ROOM TYPE: " is 11, then + 1
          char typeStr[lenToCopy +1];
          // 11 since where start, starts at 0
          memcpy(typeStr, line+11 , lenToCopy+2 );
          typeStr[lenToCopy+1] = '\0'; // force null terminator at end of string

          // this for loop iterates and finds which corresponding
          // room type value it is, either  1, 2 ,3 or start, mid, end
          // start i at 1 because room type names are 1,2, or 3
          int nope=0;
          for(nope = 1; nope < 4; nope++){
            if(strcmp(typeStr, roomTypes[nope]) == 0){
              // set rooms array with  the correct roomType
              rooms[currentRow][11] = nope;
            }
          }

        } // end cmpWithRoomType == 0

        // copy the line into substringForConnect CONECT CONNECT
        strncpy(substringForConnect, line, 7); //7 for connect

        //  if the line starts with CONNECT, we add the room connection
        int cmpWithConnect; // to hold result of string comparison
        cmpWithConnect = strcmp("CONNECT", substringForConnect);

        if(cmpWithConnect == 0){

          lenToCopy = strlen(line) - 15;  // "CONNECTION 1: " is 14, then + 1
          char newStr[lenToCopy +1];
          memcpy(newStr, line+14,lenToCopy);
          newStr[lenToCopy] = '\0';

          // this for loop iterates and finds which corresponding
          // room NAME value it is, either 0..9 or start, mid, end
          // then correctly set the
          // start at 1 because valid roomNames start at element 1 fo roomNames array
          int posCol=0;
          for(posCol = 1; posCol < 11; posCol++){
            if(strcmp(newStr, roomNames[posCol]) == 0){
              // We have a connection! connectionPositioner starts at 2
              rooms[currentRow][posCol] = 55; // using 55 as a number signifying that rooms are connected
              rooms[posCol][currentRow] = 55;  // if room a goes to b, then b goes to a
            }
          }

        }
      } // end of while that fgets each line


      fclose(roomFile);
      fileCounter = fileCounter + 1; // increment the file counter

    } // end of else (it isn't a . or .. file)

  }  // end of while readdir

  // close directory
  closedir(dir);
}


// this method is going to set all value of the room array to -1
// rooms array is 11 x 12
void setRoomsArray(){
  int row=0, col=0;
  for( row = 0; row < 11; row++){
    for(col = 0; col < 12; col++){
      rooms[row][col] = 0;   // set all values to -1
    }
  }
}

void startGame(){
  // first we need to find the room which is the start room
  // iterate over each room 1-10 inclusive, and see which one has 0

  int roomPos=0;
  for(roomPos = 1; roomPos < 11; roomPos++){
    // remember that room type lives in column [11], and startRoom is value 1
      if(rooms[roomPos][11] == 1){
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

    int wantedTime = 0;  // to be used as a boolean to indicateif user wanted time
    char destination[15];
    fgets(destination, 15, stdin);

    // trim off new line character
    if(strlen(destination) > 0){
      int newLineLocation = strlen(destination) - 1;
      destination[newLineLocation] = '\0';
    }
     
     // if user entered 'time', use
     int timeCompare;
     timeCompare = strcmp(destination, "time");
     if(timeCompare == 0){

	wantedTime = 5;   // set to a number if user wanted Time
       // initiate mutex lock
       pthread_mutex_init(&mutexLock, NULL);  // 1
       pthread_mutex_lock(&mutexLock); // 2
        pthread_mutex_init(&mutexLock2, NULL);

        // use pthread_create to create a thread that shows time
	// arguments are address of the thread, NULL for attributes

       pthread_create(&thr1, NULL, printTime, NULL); // step 3
       pthread_create(&thr2, NULL, readTimeFromFile, NULL); // step 3
        pthread_mutex_lock(&mutexLock2);
    
	// join threads after completion
	pthread_join(thr1, NULL);
        pthread_join(thr2, NULL);        

        // destroy the mutex after completion
	pthread_mutex_destroy(&mutexLock);
        pthread_mutex_destroy(&mutexLock2); 
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
      if(wantedTime ==5){
         continue; // don't print error message
	}
        printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN\n");
      
      continue;
    }


  }


} // end of continueGame

void endGame(){
  printf("Congrats the game is finally over\n");
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", stepCounter);
  int counter;
  for(counter= 0; counter < travelHistoryPointer; counter++){
    printf("%s\n", roomNames[travelHistory[counter]]);
  }
}

// this method prints time to a file
// this method must complete BEFORE readTimeForFile can run 
void* printTime(void* arg){
 //printf("in printTime\n");
 // write a file currentTime.txt
 FILE *fp = NULL;
 char fileName[] = "currentTime.txt";
 fp = fopen(fileName,"w");

// get time and print to file
 time_t rawtime;
 struct tm * timeinfo;
 time (&rawtime);
 timeinfo = localtime (&rawtime);
 fprintf(fp,"%s\n", asctime(timeinfo));

 fclose(fp);
 // unlock the mutex after this thread completes, 
 pthread_mutex_unlock(&mutexLock);
}

// this  method displays the time. 
// it has to run AFTER print time is completed!
void *readTimeFromFile(void * arg){
 // lock this thread until the print completes
 pthread_mutex_lock(&mutexLock);	

char timeFile[] = "currentTime.txt";
FILE *file = fopen("currentTime.txt", "r");
if(file == NULL){
 printf("Can't read currentTime.txt\n");
}
char line[50];
while(fgets(line,sizeof(line), file) != NULL){
 printf("\n%s\n",line);
}
// close the file after reading
fclose(file);
 pthread_mutex_unlock(&mutexLock2); 
}
