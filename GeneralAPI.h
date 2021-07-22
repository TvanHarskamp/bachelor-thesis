#ifndef GENERAL_API_H
# define GENERAL_API_H

struct RCDS_array;
void RCDS_DELETE_ARRAY(struct RCDS_array*);
struct RCDS_array* RCDS_GEN_ARRAY(int, int, size_t, size_t, ...);
struct RCDS_array* RCDS_GEN_EMPTY_ARRAY(int, int, size_t);
int* RCDS_SELECT_ELEMENT(struct RCDS_array*, size_t, ...);
void RCDS_PRINT_ARRAY(struct RCDS_array*);


#endif
