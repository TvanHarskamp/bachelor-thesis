#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct RCDS_array {
    int referenceCount;
    char kind;
    size_t length;
    union {
        int* intArray;
        char* charArray;
        float* floatArray;
        double* doubleArray;
        struct RCDS_array* nestedArray;
    };
};
#define f 'f'
#define fArray floatArray



typedef int i;
typedef char c;
typedef float f;
typedef double d;
typedef struct RCDS_array r;

#define RCDS_GEN_ARRAY(RCDS_referenceC,RCDS_kind,RCDS_data...) ({ \
    size_t size = sizeof((RCDS_kind[]){RCDS_data}); \
    RCDS_kind* valuesArray = malloc(RCDS_size); \
    memcpy(valuesArray, (RCDS_kind[]){data}, size); \
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array)); \
    *RC_array = (RCDS_array){.referenceCount = RC_referenceC, .kind = 'RC_kind', .length = size/sizeof(RCDS_kind), .data = valuesArray}; \
    RC_array; \
})

#define RCDS_GEN_EMPTY_ARRAY(RCDS_referenceC,RCDS_kind,RCDS_length) ({ \
    RCDS_kind* valuesArray = calloc((size_t)RCDS_length, sizeof(RCDS_kind)); \
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array)); \
    *RC_array = (const struct RCDS_array){.referenceCount = (int)RCDS_referenceC, .kind = (char)'RCDS_kind', .length = (size_t)RCDS_length, .floatArray = valuesArray}; \
    RC_array; \
})


int main() {
    printf("hello!\n");
    struct RCDS_array* test1 = RCDS_GEN_EMPTY_ARRAY(2,f,43);
    return 0;
}
