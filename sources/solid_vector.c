#include "solid_vector.h"

int initialized_solid_vector(struct solid_vector *vec, size_t size_instance,
							  void (*clean_instance)(void *instance))
{
	vector_settings_t *v_s = NULL;

	EASY_IF(vec == NULL,
			LOG_DEBUG("FAIL: solid_vector is NULL", NULL);
			goto failure;)

	v_s = &(vec->v_s);

	v_s->arr = v_s->begin = v_s->end = NULL;
	v_s->size_proc = v_s->size_real = 0;
	v_s->size_instance = size_instance;
	v_s->clean_instance = clean_instance;

	vec->begin = vec_func_begin;
	vec->end = vec_func_end;
	vec->size = vec_func_size;
	vec->resize = vec_func_resize;
	vec->empty = vec_func_empty;
	vec->at = vec_func_at;
	vec->assign = vec_func_assign;
	vec->push_back = vec_func_push_back;
	vec->pop_back = vec_func_pop_back;
	vec->insert = vec_func_insert;
	vec->erase = vec_func_erase;
	vec->swap = vec_func_swap;
	vec->clear = vec_func_clear;

	return 0;
failure:
	return -1;
}
