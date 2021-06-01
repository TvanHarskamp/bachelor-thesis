#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct intArray {
    int* array;
    size_t size;
};

struct nestedIntArray {
    struct intArray* arrayPointers;
    int nestedSize;
    int RC;
};

#define RCDS_GEN_ARRAY(referenceC,arr...) ({ \
    size_t size = sizeof((int []){arr}); \
    int* finalArray = malloc(size); \
    memcpy(finalArray, (int []){arr}, size); \
    struct intArray* intArrayPointer = malloc(sizeof(struct intArray)); \
    struct intArray intArray = { .array = finalArray, .size = size/sizeof(int)}; \
    intArrayPointer[0] = intArray; \
    struct nestedIntArray nestedArray = {.arrayPointers = intArrayPointer, .nestedSize = 1, .RC = referenceC}; \
    nestedArray; \
})

#define RCDS_INCR_RC(RCDS) RCDS.RC++
#define RCDS_DECR_RC(RCDS) if(--RCDS.RC <= 0) \
    for (size_t i = 0; i < RCDS.nestedSize; i++) \
        free(RCDS.arrayPointers[i].array); \
    free(RCDS.arrayPointers);




//#define RCDS_GEN_NESTARRAY(intArray1,intArray2,referenceC) ({
//})




int main() {
    printf("hello!\n");
    struct nestedIntArray test1 = RCDS_GEN_ARRAY(1, 1000, 2000, 3000, 4000, 5000);

    for(unsigned int i = 0; i < test1.arrayPointers[0].size; i++) {
        printf("Array element %u: %d\n", i, test1.arrayPointers[0].array[i]);
    }
    RCDS_INCR_RC(test1);
    //printf("Value first elt: %d\nRC: %d\n", test1.arrayPointers[0].array[0], test1.RC);
    RCDS_DECR_RC(test1);
    //printf("Value first elt: %d\nRC: %d\n", test1.arrayPointers[0].array[0], test1.RC);
    RCDS_DECR_RC(test1);
    printf("Value first elt: %d    Size: %Iu    RC: %d\n",
        test1.arrayPointers[0].array[0], test1.arrayPointers[0].size, test1.RC);

    return 0;
}
