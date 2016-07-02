// Authors: Ric Rodriguez and Kevin Jung
#include "Protocol.h"
#include "BOARD.h"
#include "Oled.h"
#include "Leds.h"
#include "Uart1.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// Converts an ascii hex char to uint8_t format
// If input wasn't a valid hex, it'll return 16
static uint8_t CharHexToInt(char input);

// Calculates a checksum based on the input string
static uint8_t CalcChecksum(char* input);

static uint8_t CharHexToInt(char input)
{
    uint8_t output = 16;
    //check for 0-9
    if ((input >= '0')&&(input <= '9')) {
        output = input - '0';
    }
    //check for a-f
    if ((input >= 'a')&&(input <= 'f')) {
        output = input - 'a' + 10;
    }
    return output;
}

static uint8_t CalcChecksum(char* input)
{
    int index;
    uint8_t output;
    // Generates the checksum by looping through and XOR'ing in 8 bit segments 
    output = input[0];
    for (index = 1; (input[index] != '\0')&&(index < PROTOCOL_MAX_PAYLOAD_LEN); index++) {
        output = output^input[index];
    }
    return output;
}

int ProtocolEncodeCooMessage(char *message, const GuessData *data)
{
    // Clear the message
    int index = 0;
    for (index = 0; index < PROTOCOL_MAX_MESSAGE_LEN; index++) {
        message[index] = '\0';
    }
    // Encodes the string and returns the length
    sprintf(message, PAYLOAD_TEMPLATE_COO, data->row, data->col);
    char tempStr[PROTOCOL_MAX_MESSAGE_LEN];
    sprintf(tempStr, MESSAGE_TEMPLATE, message, CalcChecksum(message));
    strcpy(message, tempStr);
    return strlen(message);
}

int ProtocolEncodeHitMessage(char *message, const GuessData *data)
{
    // Clear the message
    int index = 0;
    for (index = 0; index < PROTOCOL_MAX_MESSAGE_LEN; index++) {
        message[index] = '\0';
    }
    // Encodes the string and returns the length
    sprintf(message, PAYLOAD_TEMPLATE_HIT, data->row, data->col, data->hit);
    char tempStr[PROTOCOL_MAX_MESSAGE_LEN];
    sprintf(tempStr, MESSAGE_TEMPLATE, message, CalcChecksum(message));
    strcpy(message, tempStr);
    return strlen(message);
}

int ProtocolEncodeChaMessage(char *message, const NegotiationData *data)
{
    // Clear the message
    int index = 0;
    for (index = 0; index < PROTOCOL_MAX_MESSAGE_LEN; index++) {
        message[index] = '\0';
    }
    // Encodes the string and returns the length
    sprintf(message, PAYLOAD_TEMPLATE_CHA, data->encryptedGuess, data->hash);
    char tempStr[PROTOCOL_MAX_MESSAGE_LEN];
    sprintf(tempStr, MESSAGE_TEMPLATE, message, CalcChecksum(message));
    strcpy(message, tempStr);
    return strlen(message);
}

int ProtocolEncodeDetMessage(char *message, const NegotiationData *data)
{
    // Clear the message
    int index = 0;
    for (index = 0; index < PROTOCOL_MAX_MESSAGE_LEN; index++) {
        message[index] = '\0';
    }
    // Encodes the string and returns the length
    sprintf(message, PAYLOAD_TEMPLATE_DET, data->guess, data->encryptionKey);
    char tempStr[PROTOCOL_MAX_MESSAGE_LEN];
    sprintf(tempStr, MESSAGE_TEMPLATE, message, CalcChecksum(message));
    strcpy(message, tempStr);
    return strlen(message);
}

// Keeps track of protocol state machine states

typedef enum {
    WAITING,
    RECORDING,
    FIRST_CHECKSUM_HALF,
    SECOND_CHECKSUM_HALF,
    NEWLINE
} ProtocolStates;

// Keep track of all data for ProtocolDecode()
struct ProtocolData {
    char sentence[PROTOCOL_MAX_MESSAGE_LEN];
    int index;
    ProtocolStates protocolStates;
    uint8_t sum;
};
// static declaration of a ProtocolData struct
static struct ProtocolData protocolData = {
    .sentence = "",
    .protocolStates = WAITING
};

ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData)
{
    if (in == '\0') {
        return PROTOCOL_PARSING_GOOD;
    }
    uint8_t firstHalf;
    uint8_t secondHalf;
    switch (protocolData.protocolStates) {
    case WAITING:
        // Make sure the input is $
        if (in == '$') {
            int i;
            for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
                protocolData.sentence[i] = '\0';
            }
            protocolData.index = 0;
            protocolData.protocolStates = RECORDING;
            return PROTOCOL_PARSING_GOOD;
        }
        return PROTOCOL_WAITING;
    case RECORDING:
        // Stores data until an asterisk
        if (in == '*') {
            protocolData.protocolStates = FIRST_CHECKSUM_HALF;
        } else {
            protocolData.sentence[protocolData.index] = in;
            protocolData.index++;
        }
        return PROTOCOL_PARSING_GOOD;
    case FIRST_CHECKSUM_HALF:
        // Verifies that the first half of the check sum is a valid hex char
        firstHalf = CharHexToInt(in);
        if ((firstHalf >= 0) && (firstHalf <= 0xF)) {
            protocolData.sum = firstHalf << 4;
            protocolData.protocolStates = SECOND_CHECKSUM_HALF;
            return PROTOCOL_PARSING_GOOD;
        } else {
            protocolData.protocolStates = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
    case SECOND_CHECKSUM_HALF:
        // Verifies the second half of the check sum is a valid hex char and that the sum matches
        secondHalf = CharHexToInt(in);
        protocolData.sum = protocolData.sum | secondHalf;
        if (((secondHalf >= 0) && (secondHalf <= 0xF)) && ((protocolData.sum) ==
                CalcChecksum(protocolData.sentence))) {
            protocolData.sentence[protocolData.index] = '\0';
            protocolData.protocolStates = NEWLINE;
            return PROTOCOL_PARSING_GOOD;
        } else {
            protocolData.protocolStates = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
    case NEWLINE:
        // Checks for a newline or carriage return
        if (in == '\n') {
            char* testMessage;
            testMessage = strtok(protocolData.sentence, ",");
            protocolData.protocolStates = WAITING;
            // Parses appropriately based on message
            if (strcmp(testMessage, "COO") == 0) {
                gData->row = atoi(strtok(NULL, ","));
                gData->col = atoi(strtok(NULL, ","));
                return PROTOCOL_PARSED_COO_MESSAGE;
            } else if (strcmp(testMessage, "HIT") == 0) {
                gData->row = atoi(strtok(NULL, ","));
                gData->col = atoi(strtok(NULL, ","));
                gData->hit = atoi(strtok(NULL, ","));
                return PROTOCOL_PARSED_HIT_MESSAGE;
            } else if (strcmp(testMessage, "CHA") == 0) {
                nData->encryptedGuess = atoi(strtok(NULL, ","));
                nData->hash = atoi(strtok(NULL, ","));
                return PROTOCOL_PARSED_CHA_MESSAGE;
            } else if (strcmp(testMessage, "DET") == 0) {
                nData->guess = atoi(strtok(NULL, ","));
                nData->encryptionKey = atoi(strtok(NULL, ","));
                return PROTOCOL_PARSED_DET_MESSAGE;
            } else {
                return PROTOCOL_PARSING_FAILURE;
            }
        } else {
            return PROTOCOL_PARSING_FAILURE;
        }
    default:
        FATAL_ERROR();
    }
}

void ProtocolGenerateNegotiationData(NegotiationData *data)
{
    // guess, and encryption key are generated randomly and masked to 16 bits
    data->guess = rand() & 0xFFFF;
    data->encryptionKey = rand() & 0xFFFF;
    // encryptedGuess is 16 bit XOR of guess and encryptionKey
    data->encryptedGuess = data->guess^data->encryptionKey;
    // 8 bit hash generated by breaking the guess and key up into chunks
    data->hash = ((data->guess & 0xFF00) >> 8)^(data->guess & 0x00FF)^
            ((data->encryptionKey & 0xFF00) >> 8)^(data->encryptionKey & 0x00FF);
}

uint8_t ProtocolValidateNegotiationData(const NegotiationData *data)
{
    // Check checksum is equal to what we'd expect and check if encryptedGuess is equal 
    if ((data->hash == (((data->guess & 0xFF00) >> 8)^(data->guess & 0x00FF)^
            ((data->encryptionKey & 0xFF00) >> 8)^(data->encryptionKey & 0x00FF))) &&
            (data->encryptedGuess == (data->encryptionKey^data->guess))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData)
{
    // Determine if we're playing high or low roll
    int large = 0x0001 & (myData->encryptionKey^oppData->encryptionKey);
    int myDataLarger;

    // Determine who's data is larger
    if (myData->guess > oppData->guess) {
        myDataLarger = 1;
    } else if (myData->guess < oppData->guess) {
        myDataLarger = 0;
    } else {
        return TURN_ORDER_TIE;
    }

    // Returns the appropriate order depending on previous determinations
    if (((large) && (myDataLarger)) || ((!large)&&(!myDataLarger))) {
        return TURN_ORDER_START;
    } else {
        return TURN_ORDER_DEFER;
    }

}

