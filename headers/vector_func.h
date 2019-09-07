#ifndef VECTOR_FUNC_H
#define VECTOR_FUNC_H

#include "common_data.h"
#include "mathematic.h"
#include "definitions.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

extern void* vec_func_begin(solid_vector_t *vec);
extern void* vec_func_end(solid_vector_t *vec);
extern size_t vec_func_size(solid_vector_t *vec);
extern void vec_func_resize(solid_vector_t *vec, size_t new_size_proc);
extern bool vec_func_empty(struct solid_vector *vec);
extern void* vec_func_at(struct solid_vector *vec, size_t index);
extern void vec_func_assign(struct solid_vector *vec, void *begin, void *end, size_t repeater);
extern void vec_func_push_back(struct solid_vector *vec, void *value);
extern void vec_func_pop_back(struct solid_vector *vec);
extern void vec_func_insert(struct solid_vector *vec, size_t index, void *begin, void *end);
extern void vec_func_erase(struct solid_vector *vec, void *begin, void *end);
extern void vec_func_swap(struct solid_vector *vec_1, struct solid_vector *vec_2);
extern void vec_func_clear(struct solid_vector *vec);

#endif
