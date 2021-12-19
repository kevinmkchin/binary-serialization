#ifndef _INCLUDE_BYTE_BUFFER_H_
#define _INCLUDE_BYTE_BUFFER_H_

#include <stdlib.h>
#include <string.h>

struct ByteBuffer
{
    uint8_t* data;
    uint32_t position;  // basically the cursor
    uint32_t size;      // currently populated size
    uint32_t capacity;  // currently allocated size
};

#define BYTE_BUFFER_DEFAULT_CAPACITY 1024

void ByteBufferInit(ByteBuffer* buffer)
{
    // TODO(Kevin): If using custom memory alloc, replace the following line
    buffer->data = (uint8_t*) malloc(BYTE_BUFFER_DEFAULT_CAPACITY);
    memset(buffer->data, 0, BYTE_BUFFER_DEFAULT_CAPACITY);
    buffer->size = 0;
    buffer->position = 0;
}

ByteBuffer ByteBufferNew()
{
    ByteBuffer buffer = {0};
    ByteBufferInit(&buffer);
    return buffer;
}

void ByteBufferFree(ByteBuffer* buffer)
{
    if(buffer && buffer->data) 
    {
        free(buffer->data);
    }
}

void ByteBufferClear(ByteBuffer* buffer)
{
    buffer->size = 0;
    buffer->position = 0;   
}

void ByteBufferResize(ByteBuffer* buffer, size_t sz)
{
    uint8_t* data = (uint8_t*)realloc(buffer->data, sz);
    if(data == NULL)
    {
        return;
    }
    buffer->data = data;
    buffer->capacity = (uint32_t)sz;
}

void ByteBufferSeekToStart(ByteBuffer* buffer)
{
    buffer->position = 0;
}

void ByteBufferSeekToEnd(ByteBuffer* buffer)
{
    buffer->position = buffer->size;
}

void ByteBufferAdvancePosition(ByteBuffer* buffer, size_t sz)
{
    buffer->position += (uint32_t)sz; 
}

void __byteBufferWriteImpl(ByteBuffer* buffer, void* data, size_t sz)
{
    size_t totalWriteSize = buffer->position + sz;
    if(totalWriteSize >= buffer->capacity)
    {
        size_t capacity = buffer->capacity ? buffer->capacity * 2 : BYTE_BUFFER_DEFAULT_CAPACITY;
        while(capacity < totalWriteSize)
        {
            capacity *= 2;
        }
        ByteBufferResize(buffer, capacity);
    }
    memcpy(buffer->data + buffer->position, data, sz);
    buffer->position += sz;
    buffer->size += sz;
}

// Generic write function
#define ByteBufferWrite(_buffer, T, _val)\
do {\
    ByteBuffer* _bb = (_buffer);\
    size_t _sz = sizeof(T);\
    T _v = _val;\
    __byteBufferWriteImpl(_bb, (void*)&(_v), _sz);\
} while(0)

// Generic read function
#define ByteBufferRead(_buffer, T, _val_p)\
do {\
    T* _v = (T*)(_val_p);\
    ByteBuffer* _bb = (_buffer);\
    *(_v) = *(T*)(_bb->data + _bb->position);\
    _bb->position += sizeof(T);\
} while(0)

#define ByteBufferWriteBulk(_buffer, _data, _size)\
do {\
    __byteBufferWriteImpl(_buffer, _data, _size);\
} while(0)

#define ByteBufferReadBulk(_buffer, _dest_p, _size)\
do {\
    ByteBuffer* _bb = (_buffer);\
    memcpy((_dest_p), _bb->data + _bb->position, (_size));\
    _bb->position += (uint32_t)(_size);\
} while(0)


#endif // _INCLUDE_BYTE_BUFFER_H_
