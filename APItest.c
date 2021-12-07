
//Choose the API you want to use here using one of the includes:
//#include "NestedRepresentationSingleReferenceAPI.c"
//#include "NestedRepresentationNestedReferenceAPI.c"
#include "FlatRepresentationSingleReferenceAPI.c"

struct RCDS_array* flipTwoPartArray(int referenceC, RCDS_array* array1, size_t index1, size_t index2) {
    struct RCDS_array* array2 = RCDS_TAKE_SUBARRAY(1, array1, 1, index1);
    struct RCDS_array* array3 = RCDS_TAKE_SUBARRAY(1, array1, 1, index2);
    RCDS_DEC_RC(array1);
    struct RCDS_array* array4 = RCDS_GEN_ARRAY(referenceC, nestedArray, 2, 2, array3, array2);
    return array4;
}

int main() {
    printf("hello!\n");
    struct RCDS_array* test1 = RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 1, 2, 3),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 4, 5, 6, 7)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 3, 3,
            RCDS_GEN_ARRAY(1, intArray, 5, 5, 8, 9, 10, 11, 12),
            RCDS_GEN_ARRAY(1, intArray, 2, 2, 13, 14),
            RCDS_GEN_ARRAY(1, intArray, 1, 1, 15)
        )
    );
    RCDS_PRINT_ARRAY(test1);
    struct RCDS_array* test6 = RCDS_SWAP_SUBARRAYS(2, test1, 2, 0, 1);
    //struct RCDS_array* test6 = flipTwoPartArray(2, test1, 0, 1);
    RCDS_PRINT_ARRAY(test6);
    RCDS_DEC_RC(test6);
    printf("\nSelection of [1][0][2] in test6: %d\n", RCDS_SELECT_ELEMENT(test6, 3, 1, 0, 2));
    RCDS_DEC_RC(test6);
    //struct RCDS_array* test2 = RCDS_GEN_ARRAY(1, intArray, 10, 5, 23, -23, -348, 0, 5);
    //struct RCDS_array* test3 = RCDS_GEN_EMPTY_ARRAY(2, intArray, 30);
    //RCDS_PRINT_ARRAY(test2);
    //RCDS_PRINT_ARRAY(test3);
    /*RCDS_DEC_RC(test3);
    RCDS_PRINT_ARRAY(test3);
    RCDS_DEC_RC(test3);
    //Array should be freed at next print since RC is equal to 0
    RCDS_PRINT_ARRAY(test3);*/
    return 0;
}
