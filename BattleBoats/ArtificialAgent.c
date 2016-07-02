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

void AgentInit(void)
{
    uint8_t row = rand() % FIELD_ROWS;
    uint8_t col = rand() % FIELD_COLS;
    uint8_t dir = rand() & 3;
    FieldInit(&myField, FIELD_POSITION_EMPTY);
    while (FieldAddBoat(&myField, row, col, dir, FIELD_BOAT_SMALL) == STANDARD_ERROR) {
        row = rand() % FIELD_ROWS;
        col = rand() % FIELD_COLS;
        dir = rand() & 3;
    }
    while (FieldAddBoat(&myField, row, col, dir, FIELD_BOAT_MEDIUM) == STANDARD_ERROR) {
        row = rand() % FIELD_ROWS;
        col = rand() % FIELD_COLS;
        dir = rand() & 3;
    }
    while (FieldAddBoat(&myField, row, col, dir, FIELD_BOAT_HUGE) == STANDARD_ERROR) {
        row = rand() % FIELD_ROWS;
        col = rand() % FIELD_COLS;
        dir = rand() & 3;
    }
    while (FieldAddBoat(&myField, row, col, dir, FIELD_BOAT_LARGE) == STANDARD_ERROR) {
        row = rand() % FIELD_ROWS;
        col = rand() % FIELD_COLS;
        dir = rand() & 3;
    }
    FieldInit(&enemyField, FIELD_POSITION_UNKNOWN);
}

int AgentRun(char in, char *outBuffer)
{
    int i;
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
        // Generate valid coordinates using rand()
        do {
            agentData.myGuessData.col = rand() % FIELD_COLS;
            agentData.myGuessData.row = rand() % FIELD_ROWS;
        } while (FIELD_POSITION_UNKNOWN !=
                enemyField.field[agentData.myGuessData.row][agentData.myGuessData.col]);
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
