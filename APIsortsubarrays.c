//Choose the API you want to use here using one of the includes:
#include "NestedRepresentationSingleReferenceAPI.c"
//#include "NestedRepresentationNestedReferenceAPI.c"
//#include "FlatRepresentationSingleReferenceAPI.c"

struct RCDS_array* flipTwoPartArray(int referenceC, RCDS_array* array1, size_t index1, size_t index2) {
    RCDS_INC_RC(array1);
    struct RCDS_array* array2 = RCDS_TAKE_SUBARRAY(1, array1, 1, index1);
    struct RCDS_array* array3 = RCDS_TAKE_SUBARRAY(1, array1, 1, index2);
    RCDS_DEC_RC(array1);
    struct RCDS_array* array4 = RCDS_INSERT_SUBARRAY(1, array2, array1, 1, index2);
    RCDS_DEC_RC(array2);
    RCDS_DEC_RC(array1);
    struct RCDS_array* array5 = RCDS_INSERT_SUBARRAY(referenceC, array3, array4, 1, index1);
    RCDS_DEC_RC(array3);
    RCDS_DEC_RC(array4);
    return array5;
}

int compareIndexes(RCDS_array* array, size_t index1, size_t index2) {
    //0 means index1 is smaller or equal, 1 means index1 bigger
    if(RCDS_SELECT_ELEMENT(array, 3, index1, 0, 0) <= RCDS_SELECT_ELEMENT(array, 3, index2, 0, 0)) {
        return 0;
    } else {
        return 1;
    }
}

struct RCDS_array* bubblesortOnce(RCDS_array* array) {
    for(size_t i = 0; i < RCDS_SUBARRAY_LENGTH(array, 0)-1; i++) {
        if (compareIndexes(array, i, i+1) != 0) {
            array = flipTwoPartArray(1, array, i, i+1);
        }
    }
    return array;
}

struct RCDS_array* bubblesort(RCDS_array* array) {
    for(size_t i = 0; i < RCDS_SUBARRAY_LENGTH(array, 0)-1; i++) {
        array = bubblesortOnce(array);
    }
    return array;
}

struct RCDS_array* gen_testarray() {
    struct RCDS_array* testarray = RCDS_GEN_ARRAY(1, nestedArray, 11, 11,
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 2, 2, 3),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 4, 5, 6, 7)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 3, 3,
            RCDS_GEN_ARRAY(1, intArray, 5, 5, 1, 9, 10, 11, 12),
            RCDS_GEN_ARRAY(1, intArray, 2, 2, 13, 14),
            RCDS_GEN_ARRAY(1, intArray, 1, 1, 15)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 7, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 9, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 11, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 8, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 10, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 5, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 4, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 6, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 3, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
        )
    );
    return testarray;
}

int main() {
    struct RCDS_array* testarray = gen_testarray();
    RCDS_PRINT_ARRAY(testarray);
    struct RCDS_array* sortedarray = bubblesort(testarray);
    RCDS_PRINT_ARRAY(sortedarray);
    return 0;
}
