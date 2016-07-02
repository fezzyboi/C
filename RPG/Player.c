#include "Player.h"
static uint8_t inventory[INVENTORY_SIZE], index=0;
int AddToInventory(uint8_t item)
{
    int i;
    for (i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == 0) {
            inventory[i] = item;
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;
}

int FindInInventory(uint8_t item)
{
	int i;
	for(i=0;i<INVENTORY_SIZE;i++){
		if(inventory[i]==item){
			return SUCCESS;
		}
	}
	return STANDARD_ERROR;
}
