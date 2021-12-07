//Choose the API you want to use here using one of the includes:
//#include "NestedRepresentationSingleReferenceAPI.c"
//#include "NestedRepresentationNestedReferenceAPI.c"
#include "FlatRepresentationSingleReferenceAPI.c"
#include <time.h>
#include <unistd.h>

struct RCDS_array* gen_testarray1() {
    struct RCDS_array* testarray = RCDS_GEN_ARRAY(1, nestedArray, 11, 11,
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 2, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, intArray, 3, 3, 1, 0, 0),
            RCDS_GEN_ARRAY(1, intArray, 4, 4, 0, 0, 0, 0)
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

struct RCDS_array* gen_8_large_empty_arrays(size_t length) {
    struct RCDS_array* testarray = RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
                RCDS_GEN_EMPTY_ARRAY(1, intArray, length),
                RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
            ),
            RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
                RCDS_GEN_EMPTY_ARRAY(1, intArray, length),
                RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
            )
        ),
        RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
                RCDS_GEN_EMPTY_ARRAY(1, intArray, length),
                RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
            ),
            RCDS_GEN_ARRAY(1, nestedArray, 2, 2,
                RCDS_GEN_EMPTY_ARRAY(1, intArray, length),
                RCDS_GEN_EMPTY_ARRAY(1, intArray, length)
            )
        )
    );
    return testarray;
}

int main() {
    struct RCDS_array* testarray = gen_8_large_empty_arrays(100000000);
    double time_spent = 0.0;
    clock_t begin = clock();
    size_t elementCount = RCDS_COUNT_ELEMENTS(testarray, 0);
    printf("Number of zeros: %Iu\n", elementCount);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The execution time is %f seconds.\n", time_spent);
    return 0;
}
