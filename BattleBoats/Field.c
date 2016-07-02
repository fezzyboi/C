// Authors: Ric Rodriguez and Kevin Jung
#include "Field.h"
#include "Protocol.h"
#include "BOARD.h"
#include <stdint.h>

void FieldInit(Field *f, FieldPosition p)
{
    // Loop through the field and initialize to the given FieldPosition
    int i, j;
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            f->field[i][j] = p;
        }
    }
    f->hugeBoatLives = FIELD_BOAT_LIVES_HUGE;
    f->largeBoatLives = FIELD_BOAT_LIVES_LARGE;
    f->mediumBoatLives = FIELD_BOAT_LIVES_MEDIUM;
    f->smallBoatLives = FIELD_BOAT_LIVES_SMALL;
}

FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col)
{
    // If out of bounds we return a nonsensical value
    if (col > FIELD_COLS - 1 || row > FIELD_ROWS - 1) {
        return FIELD_POSITION_SMALL_BOAT;
    }
    // If not out of bounds return the FieldPosition there
    return f->field[row][col];
}

FieldPosition FieldSetLocation(Field *f, uint8_t row, uint8_t col, FieldPosition p)
{
    // Sets a location to the given FieldPosition and returns the previous value
    FieldPosition tmp = FieldAt(f, row, col);
    f->field[row][col] = p;
    return tmp;
}

uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type)
{
    // Checks if the row and col values are out of bounds
    if (FIELD_COLS - 1 - col < 0 || FIELD_ROWS - 1 - row < 0) {
        return STANDARD_ERROR;
    }

    int boatSpace;
    FieldPosition boat;
    switch (type) {
    case FIELD_BOAT_SMALL:
        boatSpace = FIELD_BOAT_LIVES_SMALL;
        boat = FIELD_POSITION_SMALL_BOAT;
        break;
    case FIELD_BOAT_MEDIUM:
        boatSpace = FIELD_BOAT_LIVES_MEDIUM;
        boat = FIELD_POSITION_MEDIUM_BOAT;
        break;
    case FIELD_BOAT_LARGE:
        boatSpace = FIELD_BOAT_LIVES_LARGE;
        boat = FIELD_POSITION_LARGE_BOAT;
        break;
    case FIELD_BOAT_HUGE:
        boat = FIELD_POSITION_HUGE_BOAT;
        boatSpace = FIELD_BOAT_LIVES_HUGE;
        break;
    }

    // For every directional case, we check to see if the boat can be placed there
    int boatIndex, rowIndex = row, colIndex = col;
    switch (dir) {
    case FIELD_BOAT_DIRECTION_NORTH:
        for (boatIndex = 0; boatIndex < boatSpace; boatIndex++) {
            if (FieldAt(f, rowIndex--, colIndex)) {
                return STANDARD_ERROR;
            }
        }
        rowIndex = row;
        for (boatIndex = 0; boatIndex < boatSpace; boatIndex++) {
            f->field[rowIndex--][colIndex] = boat;
        }
        break;
    case FIELD_BOAT_DIRECTION_SOUTH:
        for (boatIndex = 0; boatIndex < boatSpace; boatIndex++) {
            if (FieldAt(f, rowIndex++, colIndex)) {
                return STANDARD_ERROR;
            }
        }
        rowIndex = row;
        for (boatIndex = 0; boatIndex < boatSpace; boatIndex++) {
            f->field[rowIndex++][colIndex] = boat;
        }
        break;
    case FIELD_BOAT_DIRECTION_EAST:
        for (boatIndex = 0; boatIndex < boatSpace; boatIndex++) {
            if (FieldAt(f, rowIndex, colIndex++)) {
                return STANDARD_ERROR;
            }
        }
        colIndex = col;
        for (boatIndex = 0; boatIndex < boatSpace; boatIndex++) {
            f->field[rowIndex][colIndex++] = boat;
        }
        break;
    case FIELD_BOAT_DIRECTION_WEST:
        for (boatIndex = 0; boatIndex < boatSpace; boatIndex++) {
            if (FieldAt(f, rowIndex, colIndex--)) {
                return STANDARD_ERROR;
            }
        }
        colIndex = col;
        for (boatIndex = 0; boatIndex < boatSpace; boatIndex++) {
            f->field[rowIndex][colIndex--] = boat;
        }
        break;
    }
    return SUCCESS;
}

FieldPosition FieldRegisterEnemyAttack(Field *f, GuessData *gData)
{
    // Makes sure we don't accidentally overwrite Hit or Miss data
    FieldPosition prev = FieldAt(f, gData->row, gData->col);
    if (prev == FIELD_POSITION_HIT || prev == FIELD_POSITION_MISS) {
        return FIELD_POSITION_EMPTY;
    }
    
    // Checks the FieldPosition and acts appropriately 
    if (prev == FIELD_POSITION_HUGE_BOAT || prev == FIELD_POSITION_LARGE_BOAT ||
            prev == FIELD_POSITION_MEDIUM_BOAT || prev == FIELD_POSITION_SMALL_BOAT) { 
        switch (prev) {
        case FIELD_POSITION_SMALL_BOAT:
            f->smallBoatLives--;
            if (f->smallBoatLives == 0) {
                gData->hit = HIT_SUNK_SMALL_BOAT;
            } else {
                gData->hit = HIT_HIT;
            }
            break;
        case FIELD_POSITION_MEDIUM_BOAT:
            f->mediumBoatLives--;
            if (f->mediumBoatLives == 0) {
                gData->hit = HIT_SUNK_MEDIUM_BOAT;
            } else {
                gData->hit = HIT_HIT;
            }
            break;
        case FIELD_POSITION_LARGE_BOAT:
            f->largeBoatLives--;
            if (f->largeBoatLives == 0) {
                gData->hit = HIT_SUNK_LARGE_BOAT;
            } else {
                gData->hit = HIT_HIT;
            }
            break;
        case FIELD_POSITION_HUGE_BOAT:
            f->hugeBoatLives--;
            if (f->hugeBoatLives == 0) {
                gData->hit = HIT_SUNK_HUGE_BOAT;

            } else {
                gData->hit = HIT_HIT;
            }
            break;
        case FIELD_POSITION_EMPTY:case FIELD_POSITION_MISS:case FIELD_POSITION_UNKNOWN: 
        case FIELD_POSITION_HIT:case FIELD_POSITION_CURSOR:
            break;
        }
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
    } else {
        gData->hit = HIT_MISS;
        f->field[gData->row][gData->col] = FIELD_POSITION_MISS;
    }
    return prev;
}

FieldPosition FieldUpdateKnowledge(Field *f, const GuessData *gData)
{
    // Changes the hit status based on the data we receive
    HitStatus hitStatus = gData->hit;
    FieldPosition prev = FieldAt(f, gData->row, gData->col);
    switch (hitStatus) {
    case HIT_MISS:
        f->field[gData->row][gData->col] = FIELD_POSITION_EMPTY;
        break;
    case HIT_HIT:
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        break;
    case HIT_SUNK_SMALL_BOAT:
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->smallBoatLives = 0;
        break;
    case HIT_SUNK_MEDIUM_BOAT:
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->mediumBoatLives = 0;
        break;
    case HIT_SUNK_LARGE_BOAT:
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->largeBoatLives = 0;
    case HIT_SUNK_HUGE_BOAT:
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->hugeBoatLives = 0;
        break;
    }
    return prev;
}

uint8_t FieldGetBoatStates(const Field *f)
{
    // Returns boat states based on how many are unsunk
    uint8_t states = 0;
    if (f->hugeBoatLives > 0) {
        states |= FIELD_BOAT_STATUS_HUGE;
    }
    if (f->largeBoatLives > 0) {
        states |= FIELD_BOAT_STATUS_LARGE;
    }
    if (f->mediumBoatLives > 0) {
        states |= FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->smallBoatLives > 0) {
        states |= FIELD_BOAT_STATUS_SMALL;
    }
    return states;
}
