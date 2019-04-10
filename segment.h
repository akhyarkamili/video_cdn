#include <stdlib.h>

typedef struct segment_list { } segment_list_t;
typedef struct segment { } segment_t;

/* Add more functions as necessary */
segment_list_t *new_segment_list();
segment_t *find_bitrates_by_name(segment_list_t *sl, char *seg_name);
void insert_segment(segment_list_t *sl, segment_t *seg);