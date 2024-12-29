#ifndef INC_CIRCULARBUFFER_H_
#define INC_CIRCULARBUFFER_H_

/*
*   
* A general-purpose circular buffer library with features optimized for embedded applications.
* 
*/

#include <stdint.h>
#include <string.h>


/*- Circular Buffer Size  -*/
#define C_BUFFER_SIZE   (50)

/*- Circular Buffer Structure  -*/
typedef struct
{
    uint8_t     data[C_BUFFER_SIZE];    /* Buffer Array to store data */
    uint32_t    front;                  /* Front index of the buffer */
    uint32_t    rear;                   /* Rear index of the buffer */

} cBuffer_t;

/* Status Codes for Circular Buffer Operations */
typedef enum 
{
    CBUFFER_SUCCESS         = 0,        /* Operation successfull */
    CBUFFER_EMPTY           = -1,       /* Buffer is empty */
    CBUFFER_FULL            = -2,       /* Buffer is full */
    CBUFFER_OVERFLOW        = -3,       /* Data will overflow the buffer */
    CBUFFER_FAIL            = -4,       /* Generic operation failure */
    CBUFFER_INVALID_STRING  = -5        /* Operation successfull */

} cBufferStatus_t;

/*- Initializes the circular buffer -*/
void cBuffer_Init(cBuffer_t *cBuffer);
/*- Cleans up the circular buffer -*/
void cBuffer_Clean(cBuffer_t *cBuffer);
/*- Adds a character to the circular buffer -*/
cBufferStatus_t cBuffer_add_char(cBuffer_t *cBuffer, uint8_t data);
/*- Reads a character from the circular buffer -*/
cBufferStatus_t cBuffer_read_char(cBuffer_t *cBuffer, uint8_t *readChar);
/*- Adds a string to the circular buffer -*/
cBufferStatus_t cBuffer_add_string(cBuffer_t *cBuffer, const char *stringData);
/*- Reads a string from the circular buffer -*/
cBufferStatus_t cBuffer_read_string(cBuffer_t *cBuffer, uint32_t str_length, uint8_t *DestinationBuffer);
/*- Peeks at an index in the circular buffer -*/
cBufferStatus_t cBuffer_peek(cBuffer_t const * const cBuffer, uint32_t peekIndex, uint8_t *peekResult);
/*- Gets the available space in the circular buffer -*/
uint32_t cBuffer_get_availableSpace(cBuffer_t const * const cBuffer);
/*- Gets the used space in the circular buffer -*/
uint32_t cBuffer_get_usedSpace(cBuffer_t const * const cBuffer);
/*-  -*/

#endif /* INC_CIRCULARBUFFER_H_ */