#include "Game.h"
#include "Player.h"
#include <stdio.h>
#include <stdlib.h>	
#define PATH "RoomFiles/room"

typedef struct room_struct{
	int roomNumber;
	uint8_t exits;
	char titleBuffer[GAME_MAX_ROOM_TITLE_LENGTH+1];
	char itemList[INVENTORY_SIZE];
	char descBuffer[GAME_MAX_ROOM_DESC_LENGTH+1];
	uint8_t north;
	uint8_t east;
	uint8_t south;
	uint8_t west;
}Room;

static Room room;

int Load(int num)
{
	room.roomNumber=num;
	char roomBuffer[32];
	sprintf(roomBuffer, "%s%d.txt", PATH, room.roomNumber);
	FILE *file=fopen(roomBuffer, "rb");
	if(file==NULL){
		exit(1);
	}
	int i=0;
	for(;i<GAME_MAX_ROOM_TITLE_LENGTH;i++){
		room.titleBuffer[i]='\0';
	}
	for(i=0;i<GAME_MAX_ROOM_DESC_LENGTH;i++){
		room.descBuffer[i]='\0';
	}

	 i=0;
	int tmp;
	int descLen=0;
	int titleLen=0;
	int reqs=FALSE;
	titleLen=fgetc(file);
	titleLen^=(DECRYPTION_BASE_KEY+room.roomNumber);
	while(i<titleLen){
		tmp=fgetc(file);
		tmp^=(DECRYPTION_BASE_KEY+room.roomNumber);
		room.titleBuffer[i++]=tmp;		
	}
	int items;
	while((items=(fgetc(file)^(DECRYPTION_BASE_KEY+room.roomNumber)))!=EOF && !reqs){
		printf("%d\n", items);
		reqs=TRUE;
		while(items>0){
			uint8_t itemReq=(fgetc(file)^(DECRYPTION_BASE_KEY+room.roomNumber));
			if(!FindInInventory(itemReq)){
				reqs=FALSE;
			}
			items--;
		}
		descLen=(fgetc(file)^(DECRYPTION_BASE_KEY+room.roomNumber));
		i=0;
		if(reqs){
			while(i<descLen){
				room.descBuffer[i++]=(fgetc(file)^(DECRYPTION_BASE_KEY+room.roomNumber));
			}
		}else{
			while(i<descLen){
				fgetc(file);
				i++;
			}
		}
		int stash=(fgetc(file)^(DECRYPTION_BASE_KEY+room.roomNumber));
		while(stash>0){
			int loot=(fgetc(file)^(DECRYPTION_BASE_KEY+room.roomNumber));
			AddToInventory(loot);
			stash--;
		}
		int nextRoom=(fgetc(file)^(DECRYPTION_BASE_KEY+room.roomNumber));
		if(reqs){
			room.north=nextRoom;
		}
		nextRoom=(fgetc(file)^(DECRYPTION_BASE_KEY+room.roomNumber));
		if(reqs){
			room.east=nextRoom;
		}
		nextRoom=(fgetc(file)^(DECRYPTION_BASE_KEY+room.roomNumber));
		if(reqs){
			room.south=nextRoom;
		}
		nextRoom=(fgetc(file)^(DECRYPTION_BASE_KEY+room.roomNumber));

		if(reqs){
			room.west=nextRoom;
		}
	}
	fclose(file);
	if(reqs==FALSE){
		return STANDARD_ERROR;
	}
	room.roomNumber=num;
	return SUCCESS;


}
int GameGoNorth(void){
	if((GameGetCurrentRoomExits()&GAME_ROOM_EXIT_NORTH_EXISTS)!=0){
		if(Load(room.north)==SUCCESS){
			return SUCCESS;
		}
	}
	return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void){
	if((GameGetCurrentRoomExits()&GAME_ROOM_EXIT_NORTH_EXISTS)!=0){
		if(Load(room.east)==SUCCESS){
			return SUCCESS;
		}
	}
	return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoSouth(void){
	if((GameGetCurrentRoomExits()&GAME_ROOM_EXIT_SOUTH_EXISTS)!=0){
		if(Load(room.south)==SUCCESS){
			return SUCCESS;
		}
	}
	return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoWest(void){
	if((GameGetCurrentRoomExits()&GAME_ROOM_EXIT_WEST_EXISTS)!=0){
		if(Load(room.west)==SUCCESS){
			return SUCCESS;
		}
	}
	return STANDARD_ERROR;
}


int GameInit()
{
	if(!Load(STARTING_ROOM)){
		FATAL_ERROR();
	}
	return SUCCESS;
}
int GameGetCurrentRoomTitle(char *title){
	int i=0;
	for(;i<GAME_MAX_ROOM_TITLE_LENGTH;i++){
		title[i]=room.titleBuffer[i];
	}
	return i;
}

int GameGetCurrentRoomDescription(char *desc){
	int i=0;
	for(;i<GAME_MAX_ROOM_DESC_LENGTH;i++){
		desc[i]=room.descBuffer[i];
	}
	return i;
}

uint8_t GameGetCurrentRoomExits(void)
{
	int value=0;
	if(room.north){
		value |= GAME_ROOM_EXIT_NORTH_EXISTS;
	}
	if(room.south){
		value |= GAME_ROOM_EXIT_SOUTH_EXISTS;
	}
	if(room.west){
		value |= GAME_ROOM_EXIT_WEST_EXISTS;
	}
	if(room.east){
		value |= GAME_ROOM_EXIT_EAST_EXISTS;
	}
	return value;
}





