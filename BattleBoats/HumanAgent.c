// Authors: Ric Rodriguez and Kevin Jung
#include "Agent.h"
#include "Field.h"
#include "Protocol.h"
#include "Uart1.h"
#include "BOARD.h"
#include "FieldOled.h"
#include "Oled.h"
#include "FieldOled.h"
#include <String.h>
#include <stdlib.h>
#include "Buttons.h"

// Agent Data needed for functions

struct AgentData {
    char tempSentence[PROTOCOL_MAX_MESSAGE_LEN];
    size_t sentenceSize;
    AgentState agentState;
    NegotiationData myNegotiationData;
    NegotiationData oppNegotiationData;
    GuessData myGuessData;
    GuessData oppGuessData;
    ProtocolParserStatus protocolStatus;
    TurnOrder turnOrder;
    FieldOledTurn turn;
};

static Field myField;
static Field enemyField;

// static declaration of agentData
static struct AgentData agentData = {
    .turn = FIELD_OLED_TURN_NONE,
    .agentState = AGENT_STATE_GENERATE_NEG_DATA
};

// Need flags for loss so we still send info
static int processingLoss = 0;

struct PlaceBoats {
    BoatType currBoat;
    BoatDirection currDir;
    uint8_t currCol;
    uint8_t currRow;
};

static struct PlaceBoats placeBoats;

void AgentInit(void)
{
    FieldInit(&myField, FIELD_POSITION_EMPTY);
    FieldInit(&enemyField, FIELD_POSITION_UNKNOWN);
    uint8_t placement = 1;
    uint8_t buttonStatus;
    Field tmp;
    int changeFlag=0;
    placeBoats.currBoat=FIELD_BOAT_SMALL;
    while (placement) {
        //Sets up for each state
        tmp = myField;
        placeBoats.currRow = 0;
        placeBoats.currCol = 0;
        placeBoats.currDir = FIELD_BOAT_DIRECTION_EAST;
        switch (placeBoats.currBoat) {
        case FIELD_BOAT_SMALL:
            myField=tmp;
            FieldAddBoat(&myField, placeBoats.currRow, placeBoats.currCol, placeBoats.currDir, 
                    placeBoats.currBoat);
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_MINE);
            // Loops till the user puts down the ship
            while(1){
                buttonStatus = ButtonsCheckEvents();
                if (buttonStatus & BUTTON_EVENT_4DOWN) {
                    placeBoats.currDir++;
                    changeFlag=1;
                    placeBoats.currDir %= 4;
                } else if (buttonStatus & BUTTON_EVENT_3DOWN) {
                    placeBoats.currCol++;
                    changeFlag=1;
                    // Bounds checking
                    switch(placeBoats.currDir){
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (placeBoats.currCol+FIELD_BOAT_LIVES_SMALL>FIELD_COLS) {
                            placeBoats.currCol=0;
                        }
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=FIELD_BOAT_LIVES_SMALL-1;
                        }
                        break;
                    default:
                        FATAL_ERROR();
                    }
                } else if (buttonStatus & BUTTON_EVENT_2DOWN) {
                    placeBoats.currRow++;
                    changeFlag=1;
                    // Bounds Checking
                    switch(placeBoats.currDir){
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=FIELD_BOAT_LIVES_SMALL-1;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (placeBoats.currRow+FIELD_BOAT_LIVES_SMALL>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    default:
                        FATAL_ERROR();
                    }
                } else if ((buttonStatus & BUTTON_EVENT_1DOWN) && FieldAddBoat(&tmp, 
                        placeBoats.currRow, placeBoats.currCol, placeBoats.currDir, 
                        placeBoats.currBoat)!=STANDARD_ERROR){
                    FieldOledDrawScreen(&tmp, &enemyField, FIELD_OLED_TURN_MINE);
                    break;
                }
                // Whenever position changes we need to clear and redraw the boat we're adding
                if(changeFlag){ 
                    myField=tmp;
                    FieldAddBoat(&myField, placeBoats.currRow, placeBoats.currCol, 
                            placeBoats.currDir, placeBoats.currBoat);
                    FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_MINE);
                    changeFlag=0;
                }
                
            } 
            myField=tmp;
            placeBoats.currBoat = FIELD_BOAT_MEDIUM;
            break;
        case FIELD_BOAT_MEDIUM:
            myField=tmp;
            FieldAddBoat(&myField, placeBoats.currRow, placeBoats.currCol, placeBoats.currDir,
                    placeBoats.currBoat);
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_MINE);
            while(1){
                buttonStatus = ButtonsCheckEvents();
                if (buttonStatus & BUTTON_EVENT_4DOWN) {
                    placeBoats.currDir++;
                    changeFlag=1;
                    placeBoats.currDir %= 4;
                } else if (buttonStatus & BUTTON_EVENT_3DOWN) {
                    placeBoats.currCol++;
                    changeFlag=1;
                    switch(placeBoats.currDir){
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (placeBoats.currCol+FIELD_BOAT_LIVES_MEDIUM>FIELD_COLS) {
                            placeBoats.currCol=0;
                        }
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=FIELD_BOAT_LIVES_MEDIUM-1;
                        }
                        break;
                    default:
                        FATAL_ERROR();
                    }
                } else if (buttonStatus & BUTTON_EVENT_2DOWN) {
                    placeBoats.currRow++;
                    changeFlag=1;
                    switch(placeBoats.currDir){
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=FIELD_BOAT_LIVES_MEDIUM-1;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (placeBoats.currRow+FIELD_BOAT_LIVES_MEDIUM>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    default:
                        FATAL_ERROR();
                    }
                } else if ((buttonStatus & BUTTON_EVENT_1DOWN) && FieldAddBoat(&tmp, 
                        placeBoats.currRow, placeBoats.currCol, placeBoats.currDir, 
                        placeBoats.currBoat)!=STANDARD_ERROR){
                    FieldOledDrawScreen(&tmp, &enemyField, FIELD_OLED_TURN_MINE);
                    break;
                }
                // Whenever position changes we need to clear and redraw the boat we're adding
                if(changeFlag){ 
                    myField=tmp;
                    FieldAddBoat(&myField, placeBoats.currRow, placeBoats.currCol,
                            placeBoats.currDir, placeBoats.currBoat);
                    FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_MINE);
                    changeFlag=0;
                }
                
            } 
            //myField is now equal to the updated tmp field.
            myField=tmp;
            placeBoats.currBoat = FIELD_BOAT_LARGE;
            break;
        case FIELD_BOAT_LARGE:
            myField=tmp;
            FieldAddBoat(&myField, placeBoats.currRow, placeBoats.currCol, placeBoats.currDir, 
                    placeBoats.currBoat);
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_MINE);
            while(1){
                buttonStatus = ButtonsCheckEvents();
                if (buttonStatus & BUTTON_EVENT_4DOWN) {
                    placeBoats.currDir++;
                    changeFlag=1;
                    placeBoats.currDir %= 4;
                } else if (buttonStatus & BUTTON_EVENT_3DOWN) {
                    placeBoats.currCol++;
                    changeFlag=1;
                    switch(placeBoats.currDir){
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (placeBoats.currCol+FIELD_BOAT_LIVES_LARGE>FIELD_COLS) {
                            placeBoats.currCol=0;
                        }
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=FIELD_BOAT_LIVES_LARGE-1;
                        }
                        break;
                    default:
                        FATAL_ERROR();
                    }
                } else if (buttonStatus & BUTTON_EVENT_2DOWN) {
                    placeBoats.currRow++;
                    changeFlag=1;
                    switch(placeBoats.currDir){
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=FIELD_BOAT_LIVES_LARGE-1;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (placeBoats.currRow+FIELD_BOAT_LIVES_LARGE>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    default:
                        FATAL_ERROR();
                    }
                } else if ((buttonStatus & BUTTON_EVENT_1DOWN) && FieldAddBoat(&tmp, 
                        placeBoats.currRow, placeBoats.currCol, placeBoats.currDir, 
                        placeBoats.currBoat)!=STANDARD_ERROR){
                    FieldOledDrawScreen(&tmp, &enemyField, FIELD_OLED_TURN_MINE);
                    break;
                }
                // Whenever position changes we need to clear and redraw the boat we're adding
                if(changeFlag){ 
                    myField=tmp;
                    FieldAddBoat(&myField, placeBoats.currRow, placeBoats.currCol, 
                            placeBoats.currDir, placeBoats.currBoat);
                    FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_MINE);
                    changeFlag=0;
                }
                
            } 
            //myField is now equal to the updated tmp field.
            myField=tmp;
            placeBoats.currBoat = FIELD_BOAT_HUGE;
            break;
        case FIELD_BOAT_HUGE:
            myField=tmp;
            FieldAddBoat(&myField, placeBoats.currRow, placeBoats.currCol, placeBoats.currDir, 
                    placeBoats.currBoat);
            FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_MINE);
            while(1){
                buttonStatus = ButtonsCheckEvents();
                if (buttonStatus & BUTTON_EVENT_4DOWN) {
                    placeBoats.currDir++;
                    changeFlag=1;
                    placeBoats.currDir %= 4;
                } else if (buttonStatus & BUTTON_EVENT_3DOWN) {
                    placeBoats.currCol++;
                    changeFlag=1;
                    switch(placeBoats.currDir){
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (placeBoats.currCol+FIELD_BOAT_LIVES_HUGE>FIELD_COLS) {
                            placeBoats.currCol=0;
                        }
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (placeBoats.currCol>FIELD_COLS-1) {
                            placeBoats.currCol=FIELD_BOAT_LIVES_HUGE-1;
                        }
                        break;
                    default:
                        FATAL_ERROR();
                    }
                } else if (buttonStatus & BUTTON_EVENT_2DOWN) {
                    placeBoats.currRow++;
                    changeFlag=1;
                    switch(placeBoats.currDir){
                    case FIELD_BOAT_DIRECTION_NORTH:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=FIELD_BOAT_LIVES_HUGE-1;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_EAST:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_SOUTH:
                        if (placeBoats.currRow+FIELD_BOAT_LIVES_HUGE>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    case FIELD_BOAT_DIRECTION_WEST:
                        if (placeBoats.currRow>FIELD_ROWS-1) {
                            placeBoats.currRow=0;
                        }
                        break;
                    default:
                        FATAL_ERROR();
                    }
                } else if ((buttonStatus & BUTTON_EVENT_1DOWN) && FieldAddBoat(&tmp, 
                        placeBoats.currRow, placeBoats.currCol, placeBoats.currDir,
                        placeBoats.currBoat)!=STANDARD_ERROR){
                    FieldOledDrawScreen(&tmp, &enemyField, FIELD_OLED_TURN_MINE);
                    break;
                }
                // Whenever position changes we need to clear and redraw the boat we're adding
                if(changeFlag){ 
                    myField=tmp;
                    FieldAddBoat(&myField, placeBoats.currRow, placeBoats.currCol, 
                            placeBoats.currDir, placeBoats.currBoat);
                    FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_MINE);
                    changeFlag=0;
                }
                
            } 
            //myField is now equal to the updated tmp field.
            myField=tmp;
            placement=0;
            break;
        }
        
    }
}


int AgentRun(char in, char *outBuffer)
{
    int i;
    uint32_t prevRow, prevCol;
    Field tempField;
    switch (agentData.agentState) {
    case AGENT_STATE_GENERATE_NEG_DATA:
        // Generates negotiation data and Challenge message
        ProtocolGenerateNegotiationData(&agentData.myNegotiationData);
        agentData.sentenceSize = ProtocolEncodeChaMessage(outBuffer, &agentData.myNegotiationData);
        // changes the state and returns message size
        agentData.agentState = AGENT_STATE_SEND_CHALLENGE_DATA;
        if (in == '$') {
            agentData.protocolStatus = ProtocolDecode(in, &agentData.oppNegotiationData,
                    &agentData.oppGuessData);
        }
        return agentData.sentenceSize;
    case AGENT_STATE_SEND_CHALLENGE_DATA:
        agentData.protocolStatus = ProtocolDecode(in, &agentData.oppNegotiationData,
                &agentData.oppGuessData);
        if (agentData.protocolStatus == PROTOCOL_PARSING_GOOD) {
            // If not done parsing the string, return 0 to indicate we haven't received full string
            return 0;
        } else if (agentData.protocolStatus == PROTOCOL_WAITING) {
            // If we're still waiting for data, we just don't do anything
            return 0;
        } else if (agentData.protocolStatus == PROTOCOL_PARSED_CHA_MESSAGE) {
            // If we're done parsing, ProtocolDecode will have recorded the opponent's data
            // Sends the determine data
            agentData.sentenceSize = ProtocolEncodeDetMessage(outBuffer,
                    &agentData.myNegotiationData);
            // Advance the state and return message size 
            agentData.agentState = AGENT_STATE_DETERMINE_TURN_ORDER;
            return agentData.sentenceSize;
        } else {
            // We've received an unexpected message or a failed message
            // Clear the oled and set oled to ERROR_STRING_PARSING
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(AGENT_ERROR_STRING_PARSING);
            agentData.agentState = AGENT_STATE_INVALID;
            OledUpdate();
            return 0;
        }
    case AGENT_STATE_DETERMINE_TURN_ORDER:
        agentData.protocolStatus = ProtocolDecode(in, &agentData.oppNegotiationData,
                &agentData.oppGuessData);
        if (agentData.protocolStatus == PROTOCOL_PARSING_GOOD) {
            // If not done parsing the string, return 0 to indicate we haven't received full string
            return 0;
        } else if (agentData.protocolStatus == PROTOCOL_WAITING) {
            // If we're still waiting for data, we just don't do anything
            return 0;
        } else if (agentData.protocolStatus == PROTOCOL_PARSED_DET_MESSAGE) {
            // If we're done parsing, ProtocolDecode will have recorded the opponent's data
            if (ProtocolValidateNegotiationData(&agentData.oppNegotiationData) == FALSE) {
                // The sent us messed up data
                // Clear the oled and set oled to ERROR_STRING_NEG_DATA
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(AGENT_ERROR_STRING_NEG_DATA);
                agentData.agentState = AGENT_STATE_INVALID;
                OledUpdate();
                return 0;
            } else {
                //Determine the turn order
                agentData.turnOrder = ProtocolGetTurnOrder(&agentData.myNegotiationData,
                        &agentData.oppNegotiationData);
                if (agentData.turnOrder == TURN_ORDER_START) {
                    // Set turn to MINE
                    agentData.turn = FIELD_OLED_TURN_MINE;
                    // Update Screen
                    FieldOledDrawScreen(&myField, &enemyField, agentData.turn);
                    agentData.agentState = AGENT_STATE_SEND_GUESS;
                    // Intentional fall through since we know we need to send something now
                } else if (agentData.turnOrder == TURN_ORDER_DEFER) {
                    // Set turn to THEIRS
                    agentData.turn = FIELD_OLED_TURN_THEIRS;
                    // Update Screen
                    FieldOledDrawScreen(&myField, &enemyField, agentData.turn);

                    // Change state and return 0 to indicate we're waiting and not sending
                    agentData.agentState = AGENT_STATE_WAIT_FOR_GUESS;
                    return 0;
                } else {
                    // They sent us an identical guess
                    // Clear the oled and set oled to ERROR_STRING_ORDERING
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(AGENT_ERROR_STRING_ORDERING);
                    agentData.agentState = AGENT_STATE_INVALID;
                    OledUpdate();
                    return 0;
                }
            }
        } else {
            // We've received an unexpected message or a failed message. 
            // Clear the oled and set oled to ERROR_STRING_PARSING
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(AGENT_ERROR_STRING_PARSING);
            agentData.agentState = AGENT_STATE_INVALID;
            OledUpdate();
            return 0;
        }
    case AGENT_STATE_SEND_GUESS:
        //Stores the current state of the enemy's field
        tempField = enemyField;
        agentData.myGuessData.row = 0;
        agentData.myGuessData.col = 0;
        FieldSetLocation(&enemyField, 0, 0, FIELD_POSITION_CURSOR);
        
        prevRow = 0, prevCol = 0;
        FieldOledDrawScreen(&myField, &enemyField, agentData.turn);
        while (1) {
            uint8_t buttonStatus = ButtonsCheckEvents();
            //Check for a press
            if ((buttonStatus & BUTTON_EVENT_1DOWN)&&(FIELD_POSITION_UNKNOWN ==
                    tempField.field[agentData.myGuessData.row][agentData.myGuessData.col])) {
                break;
            } else if (buttonStatus & BUTTON_EVENT_3DOWN) {
                agentData.myGuessData.row++;
                // Reset to the left or the top
                if (agentData.myGuessData.row >= FIELD_ROWS) {
                    agentData.myGuessData.row = 0;
                }
            } else if (buttonStatus & BUTTON_EVENT_4DOWN) {
                agentData.myGuessData.col++;
                if (agentData.myGuessData.col >= FIELD_COLS) {
                    agentData.myGuessData.col = 0;
                }
            }
            if ((prevRow != agentData.myGuessData.row) || (prevCol != agentData.myGuessData.col)) {
                FieldOledDrawScreen(&myField, &tempField, agentData.turn);
                // Clear the enemyField
                enemyField = tempField;
                // Set the new cursor location on the enemyField
                FieldSetLocation(&enemyField, agentData.myGuessData.row, agentData.myGuessData.col, 
                        FIELD_POSITION_CURSOR);
                FieldOledDrawScreen(&myField, &enemyField, agentData.turn);
                prevRow = agentData.myGuessData.row;
                prevCol = agentData.myGuessData.col;
            }
        }
        for (i = 0; i < BOARD_GetPBClock() / 8; i++);
        // Encode COO message 
        agentData.sentenceSize = ProtocolEncodeCooMessage(outBuffer, &agentData.myGuessData);
        // Advance the state and return message size
        agentData.agentState = AGENT_STATE_WAIT_FOR_HIT;
        return agentData.sentenceSize;
    case AGENT_STATE_WAIT_FOR_HIT:
        agentData.protocolStatus = ProtocolDecode(in, &agentData.oppNegotiationData,
                &agentData.myGuessData);
        if (agentData.protocolStatus == PROTOCOL_PARSING_GOOD) {
            // If not done parsing the string, return 0 to indicate we haven't received full string
            return 0;
        } else if (agentData.protocolStatus == PROTOCOL_WAITING) {
            // If we're still waiting for data, we just don't do anything
            return 0;
        } else if (agentData.protocolStatus == PROTOCOL_PARSED_HIT_MESSAGE) {
            // If we're done parsing, ProtocolDecode will have recorded the opponent's hit data
            // Update knowledge of enemy fields
            FieldUpdateKnowledge(&enemyField, &agentData.myGuessData);
            // If hit is final blow:
            if (AgentGetEnemyStatus() == 0) {
                // Set turn to none
                agentData.turn = FIELD_OLED_TURN_NONE;
                agentData.agentState = AGENT_STATE_WON;
            } else {
                // If enemy boats alive: 
                // Set turn to theirs
                agentData.turn = FIELD_OLED_TURN_THEIRS;
                // Advance the state and return 0 to indicate we're waiting
                agentData.agentState = AGENT_STATE_WAIT_FOR_GUESS;
            }
            // Update screen
            FieldOledDrawScreen(&myField, &enemyField, agentData.turn);
            return 0;
        } else {
            // We've received an unexpected message or a failed message. 
            // At this point it's clear the game is invalid
            // Clear the oled and set oled to ERROR_STRING_PARSING
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(AGENT_ERROR_STRING_PARSING);
            agentData.agentState = AGENT_STATE_INVALID;
            OledUpdate();
            return 0;
        }
    case AGENT_STATE_WAIT_FOR_GUESS:
        agentData.protocolStatus = ProtocolDecode(in, &agentData.oppNegotiationData,
                &agentData.oppGuessData);
        if (agentData.protocolStatus == PROTOCOL_PARSING_GOOD) {
            // If not done parsing the string, return 0 to indicate we haven't received full string
            return 0;
        } else if (agentData.protocolStatus == PROTOCOL_WAITING) {
            // If we're still waiting for data, we just don't do anything
            return 0;
        } else if (agentData.protocolStatus == PROTOCOL_PARSED_COO_MESSAGE) {
            // If we're done parsing, ProtocolDecode will have recorded the opponent's coo data
            // Register hit
            FieldRegisterEnemyAttack(&myField, &agentData.oppGuessData);
            // Prepare hit message
            agentData.sentenceSize = ProtocolEncodeHitMessage(outBuffer,
                    &agentData.oppGuessData);
            // If the hit is a final blow we set a flag
            if (AgentGetStatus() == 0) {
                // Set Flag up
                processingLoss = 1;
                // Set turn to none
                agentData.turn = FIELD_OLED_TURN_NONE;
                // Change state
                agentData.agentState = AGENT_STATE_LOST;
            } else {
                // Set turn to mine
                agentData.turn = FIELD_OLED_TURN_MINE;
                agentData.agentState = AGENT_STATE_SEND_GUESS;
            }
            // Update screen
            FieldOledDrawScreen(&myField, &enemyField, agentData.turn);
            return agentData.sentenceSize;


        } else {
            // We've received an unexpected message or a failed message. 
            // Clear the oled and set oled to ERROR_STRING_PARSING
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(AGENT_ERROR_STRING_PARSING);
            agentData.agentState = AGENT_STATE_INVALID;
            OledUpdate();
            return 0;
        }
    case AGENT_STATE_INVALID:
        // Never leave this state
        return 0;
    case AGENT_STATE_LOST:
        processingLoss = 0;
        // Never leave this state
        return 0;
    case AGENT_STATE_WON:
        // Never leave this state
        return 0;
    default:
        // You broke it :(
        FATAL_ERROR();

    }
}

uint8_t AgentGetStatus(void)
{
    if (processingLoss == 1) {
        return 1;
    }
    return FieldGetBoatStates(&myField);
}

uint8_t AgentGetEnemyStatus(void)
{
    return FieldGetBoatStates(&enemyField);
}
