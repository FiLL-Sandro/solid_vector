#ifndef SOLID_VECTOR_H
#define SOLID_VECTOR_H

#include "common_data.h"
#include "vector_func.h"
#include "definitions.h"

extern int initialized_solid_vector(struct solid_vector *vec,
									size_t size_instance,
									void (*clean_instance)(void *instance));

#endif
