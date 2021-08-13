
//Choose the API you want to use here using one of the includes:
//#include "NestedMemoryNestedDescSingleReferenceAPI.c"
#include "NestedMemoryNestedDescNestedReferenceAPI.c"
//#include "FlatMemorySegmentedDescSingleReferenceAPI.c"

struct RCDS_array* flipFirstAndSecondIndex(int referenceC, RCDS_array* array1) {
    if(array1->kind == nestedArray) {
        RCDS_INC_RC(array1);
        struct RCDS_array* array2 = RCDS_SELECT_SUBARRAY(1, array1, 1, 0);
        RCDS_DEC_RC(array1);
        struct RCDS_array* array3 = RCDS_SELECT_SUBARRAY(1, array1, 1, 1);
        RCDS_DEC_RC(array1);
        struct RCDS_array* array4 = RCDS_GEN_ARRAY(2, nestedArray, 2, 2, array3, array2);
        RCDS_DEC_RC(array2);
        RCDS_DEC_RC(array3);
        return array4;
    } else {
        RCDS_INC_RC(array1);
        int buffer1 = RCDS_SELECT_ELEMENT(array1, 1, 0);
        int buffer2 = RCDS_SELECT_ELEMENT(array1, 1, 1);
        RCDS_MOD_ELEMENT(buffer2, array1, 1, 0);
        RCDS_MOD_ELEMENT(buffer1, array1, 1, 1);
        return array1;
    }
}

int main() {
    printf("hello!\n");
    struct RCDS_array* test1 = RCDS_GEN_ARRAY(2, nestedArray, 2, 2,
        RCDS_GEN_ARRAY(0, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(0, intArray, 3, 3, 1, 2, 3),
            RCDS_GEN_ARRAY(0, intArray, 4, 4, 4, 5, 6, 7)
        ),
        RCDS_GEN_ARRAY(0, nestedArray, 3, 3,
            RCDS_GEN_ARRAY(0, intArray, 5, 5, 8, 9, 10, 11, 12),
            RCDS_GEN_ARRAY(0, intArray, 2, 2, 13, 14),
            RCDS_GEN_ARRAY(0, intArray, 1, 1, 15)
        )
    );
    struct RCDS_array* test2 = RCDS_GEN_ARRAY(3, intArray, 10, 5, 23, -23, -348, 0, 5);
    //struct RCDS_array* test3 = RCDS_GEN_EMPTY_ARRAY(2, intArray, 30);
    RCDS_PRINT_ARRAY(test1);
    //RCDS_PRINT_ARRAY(test2);
    //RCDS_PRINT_ARRAY(test3);
    struct RCDS_array* test6 = flipFirstAndSecondIndex(1, test1);
    RCDS_PRINT_ARRAY(test6);
    RCDS_DEC_RC(test6);
    RCDS_PRINT_ARRAY(test1);
    /*RCDS_DEC_RC(test3);
    RCDS_PRINT_ARRAY(test3);
    RCDS_DEC_RC(test3);
    //Array should be freed at next print since RC is equal to 0
    RCDS_PRINT_ARRAY(test3);*/
    printf("\nSelection of [1][0][3] in test1: %d\n", RCDS_SELECT_ELEMENT(test1, 3, 1, 0, 3));
    printf("\nSelection of [4] in test2: %d\n", RCDS_SELECT_ELEMENT(test2, 1, 4));
    return 0;
}
