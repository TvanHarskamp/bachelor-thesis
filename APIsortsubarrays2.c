//Choose the API you want to use here using one of the includes:
//#include "NestedRepresentationSingleReferenceAPI.c"
#include "NestedRepresentationNestedReferenceAPI.c"
//#include "FlatRepresentationSingleReferenceAPI.c"
#include <time.h>
#include <unistd.h>

int compareIndexes(RCDS_array* array, size_t index1, size_t index2) {
    //0 means index1 is smaller or equal, 1 means index1 bigger
    if(RCDS_SELECT_ELEMENT(array, 6, (size_t)index1/40%2, (size_t)index1/20%2, (size_t)index1/10%2, (size_t)index1%10, (size_t)0, (size_t)0)
    <= RCDS_SELECT_ELEMENT(array, 6, (size_t)index2/40%2, (size_t)index2/20%2, (size_t)index2/10%2, (size_t)index2%10, (size_t)0, (size_t)0)) {
        return 0;
    } else {
        return 1;
    }
}

struct RCDS_array* bubblesortOnce(RCDS_array* array, size_t length) {
    struct RCDS_array* helparray;
    for(size_t i = 0; i < length-1; i++) {
        if (compareIndexes(array, i, i+1) != 0) {
            helparray = RCDS_SWAP_SUBARRAYS(1, array, 8, (size_t)i/40%2, (size_t)i/20%2, (size_t)i/10%2, (size_t)i%10,
                (size_t)(i+1)/40%2, (size_t)(i+1)/20%2, (size_t)(i+1)/10%2, (size_t)(i+1)%10);
            RCDS_DEC_RC(array);
            array = helparray;
        }
    }
    return array;
}

struct RCDS_array* bubblesort(RCDS_array* array, size_t length) {
    for(size_t i = 0; i < length-1; i++) {
        array = bubblesortOnce(array, length);
    }
    return array;
}

struct RCDS_array* gen_testarray1(size_t length, int referenceC) {
    struct RCDS_array* testarray = RCDS_GEN_ARRAY(referenceC, nestedArray, 10, 10,
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 2, 2, 3),
            RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 3, 3,
            RCDS_GEN_ARRAY(1, intArray, 5, 5, 1, 9, 10, 11, 12),
            RCDS_GEN_ARRAY(1, intArray, 2, 2, 13, 14),
            RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 7, 0, 0),
            RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 9, 0, 0),
            RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 8, 0, 0),
            RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 10, 0, 0),
            RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 5, 0, 0),
            RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 4, 0, 0),
            RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 6, 0, 0),
            RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 3, 0, 0),
            RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
        )
    );
    return testarray;
}

struct RCDS_array* gen_testarray2(RCDS_array* RC_array, size_t depth) {
    if(depth > 0) {
        return gen_testarray2(RCDS_GEN_ARRAY(2, nestedArray, 2, 2, RC_array, RC_array), depth-1);
    }
    else {
        return RC_array;
    }
}

int main() {
    size_t depth = 3;
    struct RCDS_array* testarray = gen_testarray2(gen_testarray1(10, (size_t)pow(2, depth)), depth);
    RCDS_PRINT_ARRAY(testarray);
    double time_spent = 0.0;
    clock_t begin = clock();
    struct RCDS_array* sortedarray = bubblesort(testarray, (size_t)pow(2, depth)*10);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The execution time is %f seconds.\n", time_spent);
    RCDS_PRINT_ARRAY(sortedarray);
    return 0;
}
