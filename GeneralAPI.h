#ifndef GENERAL_API_H
# define GENERAL_API_H

struct RCDS_array;
typedef struct RCDS_array RCDS_array;
enum RCDS_DATA_TYPES {intArray, charArray, floatArray, doubleArray, nestedArray};
#define IS_NESTED -1000

void RCDS_DELETE_ARRAY(RCDS_array*);
struct RCDS_array* RCDS_GEN_ARRAY(int, int, size_t, size_t, ...);
struct RCDS_array* RCDS_GEN_EMPTY_ARRAY(int, int, size_t);
void RCDS_INC_RC(RCDS_array*);
void RCDS_DEC_RC(RCDS_array*);
int RCDS_SELECT_ELEMENT(RCDS_array*, size_t, ...);
void RCDS_MOD_ELEMENT(int, RCDS_array*, size_t, ...);
struct RCDS_array* RCDS_SELECT_SUBARRAY(int, RCDS_array*, size_t, ...);
void RCDS_PRINT_ARRAY(RCDS_array*);


#endif
