/*
*   
* A general-purpose circular buffer library with features optimized for embedded applications.
* 
*/
#include "circularBuffer.h"


/* ------------------------------------------------------------------------------------------------------
 * Name		:	cBuffer_Init
 * Description	:	Initializes the circular buffer by resetting its data and indices.
 * Parameter 1	:	Pointer to the circular buffer structure.
 * Return Type	:	none (void)
 * Note		:       - Resets all elements of the circular buffer to 0.
 *                      - Sets both front and rear pointers to 0, indicating an empty buffer.  
 * ----------------------------------------------------------------------------------------------------- -*/
void cBuffer_Init(cBuffer_t *cBuffer)
{
    /* Reset buffer data to 0 */
    memset(cBuffer->data, 0, C_BUFFER_SIZE);
    /* Set buffer to empty state (front == rear) */
    cBuffer->front = 0;
    cBuffer->rear  = 0;
    
}


/* ------------------------------------------------------------------------------------------------------
 * Name		:	cBuffer_Clean
 * Description	:	Cleans or resets the circular buffer to its initial state.
 * Parameter 1	:	Pointer to the circular buffer structure.
 * Return Type	:	none (void)
 * Note		:       - Resets the buffer data and pointers by calling the initialization function.
 * ----------------------------------------------------------------------------------------------------- -*/
void cBuffer_Clean(cBuffer_t *cBuffer)
{
    /* Reinitialize the buffer to its default state */
    cBuffer_Init(cBuffer);
}


/* ------------------------------------------------------------------------------------------------------
 * Name		:	cBuffer_add_char
 * Description	:	Adds a character to the circular buffer if space is available.
 * Parameter 1	:	Pointer to the circular buffer structure.
 * Parameter 2	:	Character (data) to be added.
 * Return Type	:	Status of the operation (cBufferStatus_t).
 * Note		:       - If the buffer is full, the function returns CBUFFER_FULL, and no data is added. 
 *                      - Buffer full condition is ((rear + 1) % C_BUFFER_SIZE == front).
 * ----------------------------------------------------------------------------------------------------- -*/
cBufferStatus_t cBuffer_add_char(cBuffer_t *cBuffer, uint8_t data)
{
    /* add if not full */
    if ( !( ((cBuffer->rear + 1) % C_BUFFER_SIZE) == (cBuffer->front) ) )
    {
        /* Increment rear */
        cBuffer->rear = ((cBuffer->rear + 1) % C_BUFFER_SIZE);
        /* Add data */
        cBuffer->data[cBuffer->rear] = data;
        
        return CBUFFER_SUCCESS;
    }
    else
    {
        return CBUFFER_FULL;
    }
}


/* ------------------------------------------------------------------------------------------------------
 * Name		:	cBuffer_read_char
 * Description	:	Reads a character from the circular buffer if data is available.
 * Parameter 1	:	Pointer to the circular buffer structure.
 * Parameter 2	:	Pointer to the variable where the read character will be stored.
 * Return Type	:	Status of the operation (cBufferStatus_t).
 * Note		:       - Reading buffer removes the data.
 *                      - Returns CBUFFER_EMPTY if the buffer is empty.  
 *                      - Buffer empty condition is (rear == front).
 * ----------------------------------------------------------------------------------------------------- -*/
cBufferStatus_t cBuffer_read_char(cBuffer_t *cBuffer, uint8_t *readChar)
{
    /* read if not empty */
    if ( !((cBuffer->front) == (cBuffer->rear)) )
    {
        /* Increment Front */
        cBuffer->front = ((cBuffer->front + 1) % C_BUFFER_SIZE);
        /* Read */
        *readChar = (cBuffer->data[cBuffer->front]);
        
        return CBUFFER_SUCCESS;
    }
    else
    {
        return CBUFFER_EMPTY;
    }
}


/* ------------------------------------------------------------------------------------------------------
 * Name		:	cBuffer_add_string
 * Description	:	Adds a string to the circular buffer if sufficient space is available and ensures it is null-terminated.
 * Parameter 1	:	Pointer to the circular buffer structure.
 * Parameter 2	:	String to be added.
 * Return Type	:	Status of the operation (cBufferStatus_t).
 * Note		:       - The function ensures strings added to the buffer are null-terminated, preserving the validity of the string.
 *                      - Exit status CBUFFER_OVERFLOW indicates insufficient space to add the string. No partial data is added.
 *                      - The required buffer space is calculated as strlen(string) + 1 (for the null terminator). 
 * ----------------------------------------------------------------------------------------------------- -*/
cBufferStatus_t cBuffer_add_string(cBuffer_t *cBuffer, const char *stringData)
{
    uint32_t str_length = strlen(stringData);

    /* get availabe space */
    uint32_t buffer_spaceAvailable = cBuffer_get_availableSpace(cBuffer);

    if (buffer_spaceAvailable < (str_length + 1)) /* +1 for the null terminator */ 
    {
        /* cBuffer will overflow */
        return CBUFFER_OVERFLOW;
    }
    
    /* Add string to the buffer  */
    for (uint32_t i = 0; i < str_length; i++)
    {
        cBuffer_add_char(cBuffer, stringData[i]);
    }
    /* Add NULL, string terminator */
    cBuffer_add_char(cBuffer, '\0');

    return CBUFFER_SUCCESS;

}


/* ------------------------------------------------------------------------------------------------------
 * Name		:	cBuffer_peek
 * Description	:	Retrieves a character at a specific index in the circular buffer without removing it.
 * Parameter 1	:	Pointer to the circular buffer structure.
 * Parameter 2	:	Index to peek.
 * Parameter 3  :       Pointer to a variable where the peeked data will be stored.
 * Return Type	:	Status of the operation (cBufferStatus_t).
 * Note		:       - Peeking does not remove the data from the buffer.
 *                      - Returns CBUFFER_FAIL if the index is invalid.
 * ----------------------------------------------------------------------------------------------------- -*/
cBufferStatus_t cBuffer_peek(cBuffer_t const * const cBuffer, uint32_t peekIndex, uint8_t *peekResult)
{
    /* Valid data bytes in buffer */
    uint32_t data_inSourceBuffer = cBuffer_get_usedSpace(cBuffer);

    if (peekIndex >= data_inSourceBuffer)
    {
        /* Invalid Index */
        return CBUFFER_FAIL;
    }
    /* Actual index within the valid data range (+ 1 to make it 0 indexed) */
    uint32_t actualIndex = (cBuffer->front + peekIndex + 1) % C_BUFFER_SIZE;

    /* Read data at actual Index */
    *peekResult = cBuffer->data[actualIndex];

    return CBUFFER_SUCCESS;

}


/* ------------------------------------------------------------------------------------------------------
 * Name		:	cBuffer_read_string
 * Description	:	Reads a string of up to str_length characters from the circular buffer into the destination buffer.
 * Parameter 1	:	Pointer to the circular buffer structure.
 * Parameter 2	:	Number of characters to read (string length).
 * Parameter 3  :       Pointer to the destination buffer where the string will be stored.
 * Return Type	:	Status of the operation (cBufferStatus_t).
 * Note		:       - The destination buffer (moved string) is null-terminated by the function to ensure string validity.
 *                      - Returns CBUFFER_FAIL if the circular buffer contains fewer than str_length bytes.
 *                      - The function reads and removes the characters from the source circular buffer.
 * ----------------------------------------------------------------------------------------------------- -*/
cBufferStatus_t cBuffer_read_string(cBuffer_t *cBuffer, uint32_t str_length, uint8_t *DestinationBuffer)
{
    /* Check if source buffer has requested number of bytes */
    uint32_t data_inSourceBuffer = cBuffer_get_usedSpace(cBuffer);

    if (data_inSourceBuffer < str_length)
    {
        /* Not enough data in the source buffer */
        return CBUFFER_FAIL;
    }

    /* Copy data from source to destination buffer */
    for (uint32_t i = 0; i < str_length; i++)
    {
        cBuffer_read_char(cBuffer, &DestinationBuffer[i]);
    }

    /* Add string terminator to keep destination string valid */
    DestinationBuffer[str_length] = '\0';
    
    return CBUFFER_SUCCESS;

}


/* ------------------------------------------------------------------------------------------------------
 * Name		:	cBuffer_get_availableSpace
 * Description	:	Calculates and returns the available space in the circular buffer in bytes.
 * Parameter 1	:	Pointer to the circular buffer structure.
 * Return Type	:	Space available in the buffer in bytes (uint32_t).
 * Note		:       - The function ensures proper wrap-around behavior when indices reach the array's end.
 *                      - Available space is always calculated with one reserved slot to differentiate between full and empty buffer conditions.
 * ----------------------------------------------------------------------------------------------------- -*/
uint32_t cBuffer_get_availableSpace(cBuffer_t const * const cBuffer)
{
    /*
    * Check: (cBuffer->front > cBuffer->rear)
    *        - Buffer has wrapped around (rear wrapped back to start).
    *        - Available space is the gap between front and rear, minus one reserved slot.
    *
    * Check: (cBuffer->front <= cBuffer->rear)
    *        - Buffer has not wrapped around (rear is ahead of or equal to front).
    *        - Available space is the remaining slots after the occupied region.
    */
    return ( (cBuffer->front > cBuffer->rear) ? 
             (cBuffer->front - cBuffer->rear - 1) :
             (C_BUFFER_SIZE - (cBuffer->rear - cBuffer->front + 1)) );
}


/* ------------------------------------------------------------------------------------------------------
 * Name		:	cBuffer_get_usedSpace
 * Description	:	Calculates and returns the used space in the circular buffer in bytes.
 * Parameter 1	:	Pointer to the circular buffer structure.
 * Return Type	:	Space used in the buffer in bytes (uint32_t).
 * Note		:       - The function ensures proper wrap-around behavior when indices reach the array's end.
 *                      - Used space is the total number of bytes currently stored in the buffer.
 * ----------------------------------------------------------------------------------------------------- -*/
uint32_t cBuffer_get_usedSpace(cBuffer_t const * const cBuffer)
{
    /*
    * Check: (cBuffer->rear >= cBuffer->front)
    *        - Buffer has not wrapped around.
    *        - Used space is calculated as (rear - front).
    *
    * Check: (cBuffer->rear < cBuffer->front)
    *        - Buffer has wrapped around (rear looped back to start).
    *        - Used space is calculated as:
    *          (end of buffer - front) + (rear - start of buffer).
    */
    return ( (cBuffer->rear >= cBuffer->front) ?
           (cBuffer->rear - cBuffer->front) :
           (C_BUFFER_SIZE - (cBuffer->front - cBuffer->rear)) );

}
