#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "GeneralAPI.h"

typedef struct RCDS_segmentPair RCDS_segmentPair;

struct RCDS_segmentPair {
    size_t segment;
    size_t offset;
};

struct RCDS_array {
    int referenceCount;
    int kind;
    size_t length;
    size_t depth;
    size_t* segmentShape;
    RCDS_segmentPair** segments;
    int* intArray;
};

void RCDS_DELETE_ARRAY(RCDS_array* deleted_array) {
    free(deleted_array->intArray);
    for(int i = 0; i < deleted_array->depth; i++) {
        free(deleted_array->segments[i]);
    }
    free(deleted_array->segmentShape);
    free(deleted_array);
}

struct RCDS_array* RCDS_GEN_ARRAY(int referenceC, int kind, size_t arrayLength, size_t valistLength, ...) {
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    if(kind == nestedArray) {
        //Check for input errors
        if(arrayLength != valistLength) {
            printf("Error: number of arrays must be the same as the number of arrays in the va_list.\n");
            exit(EXIT_FAILURE);
        }
        if(valistLength <= 0) {
            printf("Error: number of arrays must be >0.\n");
            exit(EXIT_FAILURE);
        }

        //Read all the nested arrays from the variable arglist to a temporary list, and compute the depth, length and segmentshape
        va_list vl;
        va_start(vl, valistLength);
        RCDS_array** tempList = malloc(sizeof(RCDS_array*)*valistLength);
        tempList[0] = va_arg(vl, RCDS_array*);
        kind = tempList[0]->kind;
        size_t depth = tempList[0]->depth + 1;
        size_t length = tempList[0]->length;
        size_t* segmentShape = malloc(sizeof(size_t)*depth);
        segmentShape[0] = 1;
        for(size_t i = 1; i < depth; i++) {
            segmentShape[i] = tempList[0]->segmentShape[i-1];
        }
        for(size_t j = 1; j < valistLength; j++) {
            tempList[j] = va_arg(vl, RCDS_array*);
            length += tempList[j]->length;
            for(size_t i = 1; i < depth; i++) {
                segmentShape[i] += tempList[j]->segmentShape[i-1];
            }
        }
        va_end(vl);

        //Allocate the memory for the segments using the segmentShape, and fill the segments
        RCDS_segmentPair** segments = malloc(sizeof(RCDS_segmentPair*)*depth);
        segments[0] = malloc(sizeof(RCDS_segmentPair));
        segments[0][0].segment = valistLength;
        segments[0][0].offset = 0;
        for(size_t i = 1; i < depth; i++) {
            size_t index = 0;
            size_t offset = 0;
            segments[i] = malloc(sizeof(RCDS_segmentPair)*segmentShape[i]);
            for(size_t j = 0; j < valistLength; j++) {
                for(size_t k = 0; k < tempList[j]->segmentShape[i-1]; k++, index++) {
                    segments[i][index].segment = tempList[j]->segments[i-1][k].segment;
                    segments[i][index].offset = offset;
                    offset += segments[i][index].segment;
                }
            }
        }

        //Allocate the memory for the valuesArray using the length, and fill the valuesArray
        int* valuesArray = malloc(sizeof(int)*length);
        size_t index = 0;
        for(size_t j = 0; j < valistLength; j++) {
            for(size_t i = 0; i < tempList[j]->length; i++, index++) {
                valuesArray[index] = tempList[j]->intArray[i];
            }
        }

        //Free the memory of the lower level arrays
        for(size_t i = 0; i < valistLength; i++) {
            if(tempList[i]->referenceCount == 0) {
                RCDS_DELETE_ARRAY(tempList[i]);
            }
        }
        free(tempList);

        *RC_array = (const struct RCDS_array) {.referenceCount = referenceC, .kind = kind, .length = length, .depth = depth, .segmentShape = segmentShape, .segments = segments, .intArray = valuesArray};
    }
    else {
        //Create basic array with depth 1 of the values given
        size_t* segmentShape = malloc(sizeof(size_t));
        segmentShape[0] = 1;
        RCDS_segmentPair** segments = malloc(sizeof(RCDS_segmentPair*));
        segments[0] = malloc(sizeof(RCDS_segmentPair));
        segments[0][0].segment = arrayLength;
        segments[0][0].offset = 0;
        int* valuesArray = malloc(sizeof(int)*arrayLength);
        va_list vl;
        va_start(vl, valistLength);
        for(size_t i = 0; i < valistLength && i < arrayLength; i++) {
            valuesArray[i] = va_arg(vl, int);
        }
        va_end(vl);
        *RC_array = (const struct RCDS_array) {.referenceCount = referenceC, .kind = kind, .length = arrayLength, .depth = 1, .segmentShape = segmentShape, .segments = segments, .intArray = valuesArray};
    }
    return RC_array;
}

struct RCDS_array* RCDS_GEN_EMPTY_ARRAY(int referenceC, int kind, size_t arrayLength) {
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    size_t* segmentShape = malloc(sizeof(size_t));
    segmentShape[0] = 1;
    RCDS_segmentPair** segments = malloc(sizeof(RCDS_segmentPair*));
    segments[0] = malloc(sizeof(RCDS_segmentPair));
    segments[0][0].segment = arrayLength;
    segments[0][0].offset = 0;
    int* valuesArray = calloc(arrayLength, sizeof(int));
    *RC_array = (const struct RCDS_array) {.referenceCount = referenceC, .kind = kind, .length = arrayLength, .depth = 1, .segmentShape = segmentShape, .segments = segments, .intArray = valuesArray};
    return RC_array;
}

void RCDS_INC_RC(RCDS_array* RC_array) {
    RC_array->referenceCount++;
}

void RCDS_DEC_RC(RCDS_array* RC_array) {
    RC_array->referenceCount--;
    if(RC_array->referenceCount == 0) {
        RCDS_DELETE_ARRAY(RC_array);
    }
}

int RCDS_SELECT_ELEMENT(RCDS_array* RC_array, size_t valistLength, ...) {
    va_list vl;
    va_start(vl, valistLength);
    size_t offset = 0;
    for(size_t i = 0; i < valistLength; i++) {
        offset = RC_array->segments[i][offset].offset + va_arg(vl, int);
    }
    va_end(vl);
    return RC_array->intArray[offset];
}

void RCDS_MOD_ELEMENT(int value, RCDS_array* RC_array, size_t valistLength, ...) {
    va_list vl;
    va_start(vl, valistLength);
    size_t offset = 0;
    for(size_t i = 0; i < valistLength; i++) {
        offset = RC_array->segments[i][offset].offset + va_arg(vl, int);
    }
    va_end(vl);
    RC_array->intArray[offset] = value;
}

struct RCDS_array* RCDS_TAKE_SUBARRAY(int referenceC, RCDS_array* RC_array, size_t valistLength, ...) {
    int depth = RC_array->depth - valistLength;
    size_t* segmentShape = malloc(sizeof(size_t)*depth);
    RCDS_segmentPair** segments = malloc(sizeof(RCDS_segmentPair*)*depth);
    va_list vl;
    va_start(vl, valistLength);
    size_t offset = 0;
    for(size_t i = 0; i < valistLength; i++) {
        offset = RC_array->segments[i][offset].offset + va_arg(vl, int);
    }
    va_end(vl);
    printf("offset:%Iu\n", offset);
    size_t segmentsPerDepth = 1;
    for(size_t i = 0; i < depth; i++) {
        segmentShape[i] = segmentsPerDepth;
        segments[i] = malloc(sizeof(RCDS_segmentPair)*segmentsPerDepth);
        segmentsPerDepth = 0;
        for(size_t j = 0; j < segmentShape[i]; j++) {
            segments[i][j].segment = RC_array->segments[valistLength+i][offset+j].segment;
            segments[i][j].offset = segmentsPerDepth;
            segmentsPerDepth += segments[i][j].segment;
        }
        offset = RC_array->segments[valistLength+i][offset].offset;
    }
    int* valuesArray = malloc(sizeof(int)*segmentsPerDepth);
    for(size_t i = 0; i < segmentsPerDepth; i++) {
        valuesArray[i] = RC_array->intArray[i+offset];
    }
    struct RCDS_array* RC_subarray = malloc(sizeof(struct RCDS_array));
    *RC_subarray = (const struct RCDS_array) {.referenceCount = referenceC, .kind = intArray, .length = segmentsPerDepth, .depth = depth, .segmentShape = segmentShape, .segments = segments, .intArray = valuesArray};
    return RC_subarray;
}

void RCDS_PRINT_ARRAY(RCDS_array* printed_array) {
    printf("Array:\nreferenceCount:%d, kind:%d, depth:%Iu\n", printed_array->referenceCount, printed_array->kind, printed_array->depth);
    printf("segmentShape:\n[");
    for(int i = 0; i < printed_array->depth-1; i++) {
        printf("%Iu, ", printed_array->segmentShape[i]);
    }
    printf("%Iu", printed_array->segmentShape[printed_array->depth-1]);
    printf("]\nsegments:\n[\n");
    for(int i = 0; i < printed_array->depth; i++) {
        printf("[");
        for(int j = 0; j < printed_array->segmentShape[i]-1; j++) {
            printf("(%Iu,%Iu), ", printed_array->segments[i][j].segment, printed_array->segments[i][j].offset);
        }
        if(printed_array->segmentShape[i]-1 >= 0) {
            printf("(%Iu,%Iu)", printed_array->segments[i][printed_array->segmentShape[i]-1].segment,
                printed_array->segments[i][printed_array->segmentShape[i]-1].offset);
        }
        printf("]\n");
    }
    printf("]\nvaluesArray:\n[");
    for(int i = 0; i < printed_array->length-1; i++) {
        printf("%d, ", printed_array->intArray[i]);
    }
    if(printed_array->length-1 >= 0) {
        printf("%d", printed_array->intArray[printed_array->length-1]);
    }
    printf("]\n\n");
}
