#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "GeneralAPI.h"

struct RCDS_array {
    int kind;
    size_t length;
    union {
        int* intArray;
        RCDS_array** nestedArray;
    };
    int referenceCount;
};

size_t RCDS_TOTAL_LENGTH(RCDS_array* RC_array) {
    if(RC_array->kind == nestedArray) {
        size_t totalLength = 0;
        for(size_t i = 0; i < RC_array->length; i++) {
            totalLength += RCDS_TOTAL_LENGTH(RC_array->nestedArray[i]);
        }
        return totalLength;
    }
    else {
        return RC_array->length;
    }
}

size_t RCDS_SUBARRAY_LENGTH(RCDS_array* RC_array, size_t valistLength, ...) {
    va_list vl;
    va_start(vl, valistLength);
    struct RCDS_array* subarrayPointer = RC_array;
    for(size_t i = 0; i < valistLength; i++) {
        subarrayPointer = subarrayPointer->nestedArray[va_arg(vl, size_t)];
    }
    va_end(vl);
    return subarrayPointer->length;
}

struct RCDS_array* recursivelyCopyArray(RCDS_array* copied_array) {
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    if(copied_array->kind == nestedArray) {
        RC_array->referenceCount = 1;
        RC_array->kind = copied_array->kind;
        RC_array->length = copied_array->length;
        RC_array->nestedArray = malloc(sizeof(RCDS_array*)*RC_array->length);
        for(size_t i = 0; i < RC_array->length; i++) {
            RC_array->nestedArray[i] = recursivelyCopyArray(copied_array->nestedArray[i]);
        }
        return RC_array;
    }
    else {
        RC_array->referenceCount = 1;
        RC_array->kind = copied_array->kind;
        RC_array->length = copied_array->length;
        RC_array->intArray = malloc(sizeof(int)*RC_array->length);
        for(size_t i = 0; i < RC_array->length; i++) {
            RC_array->intArray[i] = copied_array->intArray[i];
        }
        return RC_array;
    }
}

void RCDS_DELETE_ARRAY(RCDS_array* deleted_array) {
    if(deleted_array->kind == nestedArray) {
        free(deleted_array->nestedArray);
    } else {
        free(deleted_array->intArray);
    }
    free(deleted_array);
    deleted_array = NULL;
}

void recursivelyIncreaseRC(RCDS_array* RC_array, int increase) {
    RC_array->referenceCount += increase;
    if(RC_array->kind == nestedArray) {
        for(size_t i = 0; i < RC_array->length; i++) {
            recursivelyIncreaseRC(RC_array->nestedArray[i], increase);
        }
    }
}

struct RCDS_array* RCDS_GEN_ARRAY(int referenceC, int kind, size_t arrayLength, size_t valistLength, ...) {
    if(referenceC <= 0) {
        return NULL;
    }
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    if(kind == nestedArray) {
        RCDS_array** valuesArray = malloc(sizeof(RCDS_array*)*arrayLength);
        va_list vl;
        va_start(vl, valistLength);
        for(int i = 0; i < valistLength && i < arrayLength; i++) {
            valuesArray[i] = va_arg(vl, RCDS_array*);
            recursivelyIncreaseRC(valuesArray[i], referenceC-1);
        }
        va_end(vl);
        *RC_array = (const struct RCDS_array){.kind = kind, .length = arrayLength, .nestedArray = valuesArray, .referenceCount = referenceC};
    }
    else {
        int* valuesArray = malloc(sizeof(int)*arrayLength);
        va_list vl;
        va_start(vl, valistLength);
        for(int i = 0; i < valistLength && i < arrayLength; i++) {
            valuesArray[i] = va_arg(vl, int);
        }
        va_end(vl);
        *RC_array = (const struct RCDS_array){.kind = kind, .length = arrayLength, .intArray = valuesArray, .referenceCount = referenceC};
    }
    return RC_array;
}

struct RCDS_array* RCDS_GEN_EMPTY_ARRAY(int referenceC, int kind, size_t arrayLength) {
    if(referenceC <= 0) {
        return NULL;
    }
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    int* valuesArray = calloc(arrayLength, sizeof(int));
    *RC_array = (const struct RCDS_array){.referenceCount = referenceC, .kind = kind, .length = arrayLength, .intArray = valuesArray};
    return RC_array;
}

void RCDS_INC_RC(RCDS_array* RC_array) {
    RC_array->referenceCount++;
    if(RC_array->kind == nestedArray) {
        for(size_t i = 0; i < RC_array->length; i++) {
            RCDS_INC_RC(RC_array->nestedArray[i]);
        }
    }
}

void RCDS_DEC_RC(RCDS_array* RC_array) {
    if(RC_array->kind == nestedArray) {
        for(size_t i = 0; i < RC_array->length; i++) {
            RCDS_DEC_RC(RC_array->nestedArray[i]);
        }
    }
    RC_array->referenceCount--;
    if(RC_array->referenceCount == 0) {
        RCDS_DELETE_ARRAY(RC_array);
    }
}

int RCDS_SELECT_ELEMENT(RCDS_array* RC_array, size_t valistLength, ...) {
    va_list vl;
    va_start(vl, valistLength);
    struct RCDS_array* subarrayPointer = RC_array;
    for(;valistLength > 1; valistLength--) {
        subarrayPointer = subarrayPointer->nestedArray[va_arg(vl, size_t)];
    }
    int element = subarrayPointer->intArray[va_arg(vl, size_t)];
    va_end(vl);
    return element;
}

struct RCDS_array* RCDS_MOD_ELEMENT(int referenceC, int value, RCDS_array* RC_array, size_t valistLength, ...) {
    if(referenceC <= 0) {
        return NULL;
    }
    //Make a new array if the reference count of the top array is >1,
    //else just use the old array and add referenceC to the reference count.
    struct RCDS_array* new_RC_array;
    if(RC_array->referenceCount == 1) {
        recursivelyIncreaseRC(RC_array, referenceC);
        new_RC_array = RC_array;
    }
    else {
        new_RC_array = recursivelyCopyArray(RC_array);
        recursivelyIncreaseRC(new_RC_array, referenceC-1);
    }
    //Go to the element and modify it.
    struct RCDS_array* subarrayPointer = new_RC_array;
    va_list vl;
    va_start(vl, valistLength);
    for(;valistLength > 1; valistLength--) {
        subarrayPointer = subarrayPointer->nestedArray[va_arg(vl, size_t)];
    }
    subarrayPointer->intArray[va_arg(vl, size_t)] = value;
    va_end(vl);
    return new_RC_array;
}

struct RCDS_array* RCDS_TAKE_SUBARRAY(int referenceC, RCDS_array* RC_array, size_t valistLength, ...) {
    if(referenceC <= 0) {
        return NULL;
    }
    struct RCDS_array* subarrayPointer = RC_array;
    va_list vl;
    va_start(vl, valistLength);
    for(;valistLength > 0; valistLength--) {
        subarrayPointer = subarrayPointer->nestedArray[va_arg(vl, size_t)];
    }
    va_end(vl);
    recursivelyIncreaseRC(subarrayPointer, referenceC);
    return subarrayPointer;
}

struct RCDS_array* RCDS_SWAP_SUBARRAYS(int referenceC, RCDS_array* RC_array, size_t valistLength, ...) {
    if(referenceC <= 0 || valistLength%2 != 0) {
        return NULL;
    }
    //Make a new array if the reference count of the top array is >1,
    //else just use the old array and add referenceC to the reference count.
    struct RCDS_array* new_RC_array;
    if(RC_array->referenceCount == 1) {
        recursivelyIncreaseRC(RC_array, referenceC);
        new_RC_array = RC_array;
    }
    else {
        new_RC_array = recursivelyCopyArray(RC_array);
        recursivelyIncreaseRC(new_RC_array, referenceC-1);
    }
    //Go to the to be swapped subarrays.
    struct RCDS_array* subarrayPointer1 = new_RC_array;
    struct RCDS_array* subarrayPointer2 = new_RC_array;
    va_list vl;
    va_start(vl, valistLength);
    for(size_t i = 0; i < valistLength/2-1; i++) {
        subarrayPointer1 = subarrayPointer1->nestedArray[va_arg(vl, size_t)];
    }
    size_t index1 = va_arg(vl, size_t);
    for(size_t i = 0; i < valistLength/2-1; i++) {
        subarrayPointer2 = subarrayPointer2->nestedArray[va_arg(vl, size_t)];
    }
    size_t index2 = va_arg(vl, size_t);
    va_end(vl);
    //Swap the two subarrays.
    struct RCDS_array* buffer = subarrayPointer1->nestedArray[index1];
    subarrayPointer1->nestedArray[index1] = subarrayPointer2->nestedArray[index2];
    subarrayPointer2->nestedArray[index2] = buffer;
    return new_RC_array;
}

size_t RCDS_COUNT_ELEMENTS(RCDS_array* RC_array, int element) {
    size_t count = 0;
    if(RC_array->kind == nestedArray) {
        for(size_t i = 0; i < RC_array->length; i++) {
            count += RCDS_COUNT_ELEMENTS(RC_array->nestedArray[i], element);
        }
    }
    else {
        for(size_t i = 0; i < RC_array->length; i++) {
            if(RC_array->intArray[i] == element) {
                count++;
            }
        }
    }
    return count;
}

void RCDS_PRINT_ARRAY(RCDS_array* printed_array) {
    if(printed_array->kind == nestedArray) {
        printf("Reference count: %d, array contains:\n", printed_array->referenceCount);
        for(size_t i = 0; i < printed_array->length; i++) {
            printf("\t");
            RCDS_PRINT_ARRAY(printed_array->nestedArray[i]);
        }
        printf("\n");
    } else {
        printf("\tReference count: %d, valuesArray: [", printed_array->referenceCount);
        for(size_t i = 0; i < printed_array->length-1; i++) {
            printf("%d, ", printed_array->intArray[i]);
        }
        if(printed_array->length-1 >= 0) {
            printf("%d]\n", printed_array->intArray[printed_array->length-1]);
        } else {
            printf("]\n");
        }
    }
}
