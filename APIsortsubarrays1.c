//Choose the API you want to use here using one of the includes:
//#include "NestedRepresentationSingleReferenceAPI.c"
//#include "NestedRepresentationNestedReferenceAPI.c"
#include "FlatRepresentationSingleReferenceAPI.c"
#include <time.h>
#include <unistd.h>
#include <math.h>

int compareIndexes(RCDS_array* array, size_t index1, size_t index2) {
    //0 means index1 is smaller or equal, 1 means index1 bigger
    if(RCDS_SELECT_ELEMENT(array, 6, (size_t)index1/8%2, (size_t)index1/4%2, (size_t)index1/2%2, (size_t)index1%2, (size_t)0, (size_t)0)
    <= RCDS_SELECT_ELEMENT(array, 6, (size_t)index2/8%2, (size_t)index2/4%2, (size_t)index2/2%2, (size_t)index2%2, (size_t)0, (size_t)0)) {
        return 0;
    } else {
        return 1;
    }
}

struct RCDS_array* bubblesortOnce(RCDS_array* array, size_t length) {
    struct RCDS_array* helparray;
    for(size_t i = 0; i < length-1; i++) {
        if (compareIndexes(array, i, i+1) != 0) {
            helparray = RCDS_SWAP_SUBARRAYS(1, array, 8, (size_t)i/8%2, (size_t)i/4%2, (size_t)i/2%2, (size_t)i%2,
                (size_t)(i+1)/8%2, (size_t)(i+1)/4%2, (size_t)(i+1)/2%2, (size_t)(i+1)%2);
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

struct RCDS_array* gen_testarray1(int random_int) {
    return RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
        RCDS_GEN_ARRAY(1, intArray, 3, 3, random_int, 0, 0),
        RCDS_GEN_EMPTY_ARRAY(1, intArray, 10)
    );
}

struct RCDS_array* gen_testarray2(size_t depth) {
    if(depth > 0) {
        return RCDS_GEN_ARRAY(1, nestedArray, 2, 2, gen_testarray2(depth-1), gen_testarray2(depth-1));
    }
    else {
        return gen_testarray1(rand());
    }
}

int main() {
    srand(37);
    size_t depth = 4;
    struct RCDS_array* testarray = gen_testarray2(depth);
    RCDS_PRINT_ARRAY(testarray);
    double time_spent = 0.0;
    clock_t begin = clock();
    struct RCDS_array* sortedarray = bubblesort(testarray, (size_t)pow(2, depth));
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The execution time is %f seconds.\n", time_spent);
    RCDS_PRINT_ARRAY(sortedarray);
    return 0;
}
