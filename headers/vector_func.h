#ifndef VECTOR_FUNC_H
#define VECTOR_FUNC_H

#include "solid_vector.h"
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
extern bool vec_func_empty(solid_vector_t *vec);
extern void* vec_func_at(solid_vector_t *vec, size_t index);
extern void vec_func_assign(solid_vector_t *vec, void *begin, void *end, size_t repeater);
extern void vec_func_push_back(solid_vector_t *vec, void *value);
extern void vec_func_pop_back(solid_vector_t *vec);
extern void vec_func_insert(solid_vector_t *vec, void *begin, void *end, size_t index);
extern void vec_func_erase(solid_vector_t *vec, void *begin, void *end);
extern void vec_func_swap(solid_vector_t *vec_l, solid_vector_t *vec_r);
extern void vec_func_clear(solid_vector_t *vec);

#endif
