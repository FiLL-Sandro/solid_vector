#include "vector_func.h"

int initialized_solid_vector(solid_vector_t *vec,  methods_solid_vector_t *meth_vec,
							 size_t size_instance, void (*clean_instance)(void *instance))
{
	EASY_IF(vec == NULL || meth_vec == NULL,
			LOG_DEBUG("FAIL: vector or vector methods structures is NULL", NULL);
			goto failure;)

	vec->arr = vec->begin = vec->end = NULL;
	vec->size_proc = vec->size_real = 0;
	vec->size_instance = size_instance;
	vec->clean_instance = clean_instance;

	meth_vec->begin = vec_func_begin;
	meth_vec->end = vec_func_end;
	meth_vec->size = vec_func_size;
	meth_vec->resize = vec_func_resize;
	meth_vec->empty = vec_func_empty;
	meth_vec->at = vec_func_at;
	meth_vec->assign = vec_func_assign;
	meth_vec->push_back = vec_func_push_back;
	meth_vec->pop_back = vec_func_pop_back;
	meth_vec->insert = vec_func_insert;
	meth_vec->erase = vec_func_erase;
	meth_vec->swap = vec_func_swap;
	meth_vec->clear = vec_func_clear;

	return 0;
failure:
	return -1;
}
