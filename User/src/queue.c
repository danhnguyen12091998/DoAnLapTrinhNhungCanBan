/*
 * Date: 25/4/2016
 * Writter: ESS
 * Description: This file contains function implementations for queue data structure
 */

#include "queue.h"
#include <string.h>

void queue_init(queue_t* queue)
{
    queue->capacity = 0;
    memset(queue->data, 0, sizeof(queue->data) / sizeof(char));
}

void queue_push(queue_t* queue, const char data)
{
    if (!queue_is_full(queue))
    {
        queue->data[queue->capacity] = data;
        queue->capacity = queue->capacity + 1;
    }
}

char queue_pop(queue_t* queue)
{
    // Preparations
    char result = 0;
    uint16_t index_data = 0;
    
    if (!queue_is_empty(queue))
    {
        result = queue->data[0];
        queue->capacity = queue->capacity - 1;
        
        for (index_data = 0; index_data < queue->capacity; index_data++)
        {
            queue->data[index_data] = queue->data[index_data + 1];
        }
        queue->data[queue->capacity] = 0;
    }
    
    return result;
}

char queue_peek(const queue_t* queue)
{
    // Preparations
    char result = 0;
    
    if (!queue_is_empty(queue))
    {
        result = queue->data[0];
    }
   
   return result;
}
char queue_is_empty(const queue_t* queue)
{
    // Preparations
    char result = 0;
    
    if (queue->capacity == 0)
    {
        result = 1;
    }
    
    return result;
}

char queue_is_full(const queue_t* queue)
{
    // Preparations
    char result = 0;
    
    if (queue->capacity == (sizeof(queue->data) / sizeof(char)))
    {
        result = 1;
    }
    
    return result;
}

void queue_push_string(queue_t* queue, const char* str, uint16_t len)
{
    // Preparations
    uint16_t index = 0;
    
    while ((index < len) && !queue_is_full(queue))
    {
        queue_push(queue, str[index]);
        index = index + 1;
    }
}

/* End of file */
