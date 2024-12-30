# circularBuffer
 A general-purpose circular buffer library with features optimized for embedded applications.

## Files
**`Inc/circularBuffer.h`**
<p>Header file containing function declarations for buffer initialization and operations. 

**`Src/circularBuffer.c`**
<p> Implementation of the supported functions.

## Buffer Configuration
- The buffer has a fixed size, defined by `C_BUFFER_SIZE` in `circularBuffer.h`.
- Default Size: 50 bytes.
- Update `C_BUFFER_SIZE` to fit your application needs.

## How to Use
**Include the header file in your source file and Initialize the circular buffer**
```c
/* circularBuffer header file */
#inlcude "circularBuffer.h"

/* Initialization */
cBuffer_t myBuffer;
cBuffer_Init(&myBuffer);

/* Use the supported functions for buffer operations */
```
## Supported Functions and Using the Circular Buffer
`void cBuffer_Init(cBuffer_t *cBuffer);`

**Initializes the circular buffer by resetting its data and indices.**

- Resets all elements of the circular buffer to 0.
- Sets both front and rear pointers to 0, indicating an empty buffer. 


```c
    cBuffer_Init(&myBuffer);
```
##

`void cBuffer_Clean(cBuffer_t *cBuffer);`

**Cleans or resets the circular buffer to its initial state.**

- Resets the buffer data and pointers by calling the initialization function.

```c
    cBuffer_Clean(&mybuffer);
```
##
`cBufferStatus_t cBuffer_add_char(cBuffer_t *cBuffer, uint8_t data);`

**Adds a character to the circular buffer if space is available.**

- If the buffer is full, the function returns CBUFFER_FULL, and no data is added. 

```c
    if ( cBuffer_add_char(&mybuffer, 'A') == CBUFFER_SUCCESS )
    {
        /* Character is added successfully. */
    }
```
##
`cBufferStatus_t cBuffer_read_char(cBuffer_t *cBuffer, uint8_t *readChar);`

**Reads a character from the circular buffer if data is available.**

- Reading buffer removes the data.
- Returns CBUFFER_EMPTY if the buffer is empty.  

```c
    uint8_t readData;
    if ( cBuffer_read_char(&mybuffer, &readData) == CBUFFER_SUCCESS )
    {
        /* Character is read successfully. */
        printf("%c", readData);
    }
```
##
`cBufferStatus_t cBuffer_add_string(cBuffer_t *cBuffer, const char *stringData);`

**Adds a string to the circular buffer if sufficient space is available and ensures it is null-terminated.**

- The function ensures strings added to the buffer are null-terminated, preserving the validity of the string.
- Exit status CBUFFER_OVERFLOW indicates insufficient space to add the string. No partial data is added.

```c
    uint8_t stringData[] = "www.LalitK.space";
    if ( cBuffer_add_string(&mybuffer, stringData) == CBUFFER_SUCCESS )
    {
        /* string is added successfully. */
    }
```
##
`cBufferStatus_t cBuffer_read_string(cBuffer_t *cBuffer, uint32_t str_length, uint8_t *DestinationBuffer, uint32_t destBufferSize);
`

**Reads a string of up to str_length characters from the circular buffer into the destination buffer.**

- The destination buffer (moved string) is null-terminated by the function to ensure string validity.
- Returns CBUFFER_FAIL if the circular buffer contains fewer than str_length bytes.

```c
    #define RX_BUFFER_SIZE   (50)
    
    uint8_t readString[RX_BUFFER_SIZE];
    if ( cBuffer_read_string(&mybuffer, strlen(stringData), readString, RX_BUFFER_SIZE) == CBUFFER_SUCCESS )
    {
        /* string is read successfully. */
        printf("%s", readString);
    }
```
##

`cBufferStatus_t cBuffer_peek(cBuffer_t const * const cBuffer, uint32_t peekIndex, uint8_t *peekResult)`

**Retrieves a character at a specific index in the circular buffer without removing it.**

- Peeking does not remove the data from the buffer.
- Returns CBUFFER_FAIL if the index is invalid.

```c
    uint8_t peekedChar;
    uint32_t peekIndex = 15;
    if ( cBuffer_peek(&mybuffer, peekIndex, &peekedChar) == CBUFFER_SUCCESS )
    {
        /* Data at index 'peekIndex' is successfully retrieved.*/
        printf("%c", peekedChar);
    }
```
##

`uint32_t cBuffer_get_availableSpace(cBuffer_t const * const cBuffer)`

**Calculates and returns the available space in the circular buffer in bytes.**

- The function ensures proper wrap-around behavior when indices reach the array's end.
- Available space is always calculated with one reserved slot to differentiate between full and empty buffer conditions.

```c
uint32_t availableSpace = cBuffer_get_availableSpace(&mybuffer);
```
##

`uint32_t cBuffer_get_usedSpace(cBuffer_t const * const cBuffer);`

**Calculates and returns the used space in the circular buffer in bytes.**


- The function ensures proper wrap-around behavior when indices reach the array's end.
- Used space is the total number of bytes currently stored in the buffer.

```c
uint32_t usedSpace = cBuffer_get_usedSpace(&mybuffer);
```
##


