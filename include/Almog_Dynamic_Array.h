#ifndef ALMOG_DYNAMIC_ARRAY_H_
#define ALMOG_DYNAMIC_ARRAY_H_

#include <stdlib.h>
#include <assert.h>

#define INIT_CAPACITY 10

#ifndef ADA_MALLOC
#define ADA_MALLOC malloc
#endif /*ADA_MALLOC*/

#ifndef ADA_REALLOC
#define ADA_REALLOC realloc
#endif /*ADA_REALLOC*/

#ifndef ADA_ASSERT
#define ADA_ASSERT assert
#endif /*ADA_ASSERT*/

/* typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array; */

#define ada_init_array(type, header) do {                                       \
        header.capacity = INIT_CAPACITY;                                        \
        header.length = 0;                                                      \
        header.elements = (type *)ADA_MALLOC(sizeof(type) * header.capacity);   \
        ADA_ASSERT(header.elements != NULL);                                    \
    } while (0)                                                                 \

#define ada_resize(type, header, new_capacity) do {                                                         \
        type *ada_temp_pointer = (type *)ADA_REALLOC((void *)(header.elements), new_capacity*sizeof(type)); \
        if (ada_temp_pointer == NULL) {                                                                     \
            exit(1);                                                                                        \
        }                                                                                                   \
        header.elements = ada_temp_pointer;                                                                 \
        ADA_ASSERT(header.elements != NULL);                                                                \
        header.capacity = new_capacity;                                                                     \
    } while (0)                                                                                             \

#define ada_appand(type, header, value) do {                        \
        if (header.length >= header.capacity) {                     \
            ada_resize(type, header, (int)(header.capacity*1.5));   \
        }                                                           \
        header.elements[header.length] = value;                     \
        header.length++;                                            \
    } while (0)                                                     \

#define ada_insert(type, header, value, index) do {                                                         \
    ADA_ASSERT(index >= 0);                                                                                 \
    ADA_ASSERT(index - (int)index == 0);                                                                    \
    ada_appand(type, header, header.elements[header.length-1]);                                             \
    for (size_t ada_for_loop_index = header.length-2; ada_for_loop_index > index; ada_for_loop_index--) {   \
        header.elements[ada_for_loop_index] = header.elements [ada_for_loop_index-1];                       \
    }                                                                                                       \
    header.elements[index] = value;                                                                         \
} while (0)                                                                                                 \

#endif /*ALMOG_DYNAMIC_ARRAY_H_*/
