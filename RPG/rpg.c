// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "UnixBOARD.h"



// User libraries
#include "Game.h"

// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****
static char title[GAME_MAX_ROOM_TITLE_LENGTH+1];
static char desc[GAME_MAX_ROOM_DESC_LENGTH+1];
// **** Declare any function prototypes here ****
void Clear(char *buf)
{
	int i=0;
	for(;i<strlen(buf);i++){
		buf[i]='\0';
	}
}

int main()
{
	if(GameInit()==STANDARD_ERROR){
		exit(1);
	}
	for(;;){
		Clear(title);
		GameGetCurrentRoomTitle(title);
		Clear(desc);
		GameGetCurrentRoomDescription(desc);
        printf("FUCK\n");
        printf("\x1b[2J\x1b[H\x1b[30;46m%s\x1b[37;40m\n%s\n", title, desc);
        if(GameGetCurrentRoomExits()!=0){
        	printf("\x1b?\x1b[250B\x1b[3A\x1b[4C%sNorth\n%sWest\x1b[5C%sEast\n\x1b[4C%sSouth\x1b[37;40m\n",
        		(GameGetCurrentRoomExits()&GAME_ROOM_EXIT_NORTH_EXISTS)==0?"\x1b[31m":"\x1b[32m",
        		(GameGetCurrentRoomExits()&GAME_ROOM_EXIT_WEST_EXISTS)==0?"\x1b[31m":"\x1b[32m",
        		(GameGetCurrentRoomExits()&GAME_ROOM_EXIT_EAST_EXISTS)==0?"\x1b[31m":"\x1b[32m",
        		(GameGetCurrentRoomExits()&GAME_ROOM_EXIT_SOUTH_EXISTS)==0?"\x1b[31m":"\x1b[32m");
        
        printf("Please enter a direction in which to travel(n,s,e,w) or press q to quit: \n");
        char tmp;
        scanf("%c",&tmp);
        if(tmp=='n'){
        	GameGoNorth();
        }else if(tmp=='s'){
        	GameGoSouth();
        }else if(tmp=='e'){
        	GameGoEast();
        }else if(tmp=='w'){
        	GameGoWest();
        }else if(tmp=='q'){
        	exit(0);
        }
	}else{
		    printf("\x1b?\x1b[250B\x1b[2A\x1b[41;30mYou lose!\x1b[37;40m");
            exit(0);
	}
}
    /******************************** Your custom code goes below here ********************************/



    /**************************************************************************************************/
}

