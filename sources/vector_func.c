#include "vector_func.h"

#define REAL_SIZE_INITIAL 5
#define SIZE_FACTOR 1.5l
#define SIZE_RATIO 3.l

typedef enum vec_oper_e
{
	OPER_RESIZE = 1,
	OPER_ASSIGN,
	OPER_PUSH_BACK,
	OPER_POP_BACK,
	OPER_INSERT
} vec_oper_t;

static bool check_vector_validity(solid_vector_t *vec);
static bool is_inside_range(solid_vector_t *vec, void *ptr);
static void free_vector(solid_vector_t *vec);
static void cleaning_items(solid_vector_t *vec, void *begin, void *end);
static void update_vector_settings(solid_vector_t *vec);
static bool update_size_real(solid_vector_t* v_s, vec_oper_t oper, va_list val);
static void vector_realloc(solid_vector_t *vec, vec_oper_t oper, ...);
static size_t size_range(solid_vector_t *vec, void *begin, void *end);

void* vec_func_begin(solid_vector_t *vec)
{
	assert(check_vector_validity(vec) == true);

	return vec->begin;
}

void* vec_func_end(solid_vector_t *vec)
{
	assert(check_vector_validity(vec) == true);

	return vec->end;
}

size_t vec_func_size(solid_vector_t *vec)
{
	assert(check_vector_validity(vec) == true);

	return vec->size_proc;
}

void vec_func_resize(solid_vector_t *vec, size_t new_size_proc)
{
	void *ptr_1 = NULL;

	assert(check_vector_validity(vec) == true);

	EASY_IF(new_size_proc == vec->size_proc, goto success;)

	if (new_size_proc < vec->size_proc)
	{
		if (vec->clean_instance != NULL)
		{
			ptr_1 = vec->begin;
			VOID_OFFSET(ptr_1, new_size_proc * vec->size_instance);
			cleaning_items(vec, ptr_1, vec->end);
		}
	}
	else if (new_size_proc > vec->size_real)
	{
		vector_realloc(vec, OPER_RESIZE, new_size_proc);
	}

	vec->size_proc = new_size_proc;
	update_vector_settings(vec);

	assert(check_vector_validity(vec) == true);

success:
	return;
}

bool vec_func_empty(solid_vector_t *vec)
{
	assert(check_vector_validity(vec) == true);

	return vec->size_proc == 0 ? true : false;
}

void* vec_func_at(solid_vector_t *vec, size_t index)
{
	void *ptr = NULL;

	assert(check_vector_validity(vec) == true);
	assert(index < vec->size_proc);

	ptr = vec->begin;
	VOID_OFFSET(ptr, index * vec->size_instance);

	return ptr;
}

void vec_func_assign(solid_vector_t *vec, void *begin, void *end, size_t repeater)
{
	void *ptr = NULL;

	assert(check_vector_validity(vec) == true);
	assert(begin < end);

	free_vector(vec);

	EASY_IF(end != NULL,
			repeater = size_range(vec, begin, end);)

	vector_realloc(vec, OPER_ASSIGN, repeater);
	vec->size_proc = repeater;
	update_vector_settings(vec);

	EASY_IFEL(end != NULL,
			  memcpy(vec->arr, begin, repeater * vec->size_instance);,

			  ptr = vec->begin;
			  while(repeater-- != 0)
				ptr = mempcpy(ptr, begin, vec->size_instance);
			)

	assert(check_vector_validity(vec) == true);
}

void vec_func_push_back(solid_vector_t *vec, void *value)
{
	assert(check_vector_validity(vec) == true);

	++(vec->size_proc);
	vector_realloc(vec, OPER_PUSH_BACK);
	update_vector_settings(vec);

	VOID_OFFSET(vec->end, -((long)vec->size_instance));
	//assert(vec->end != NULL);
	vec->end = mempcpy(vec->end, value, vec->size_instance);
}

void vec_func_pop_back(solid_vector_t *vec)
{
	assert(check_vector_validity(vec) == true);

	VOID_OFFSET(vec->end, -vec->size_instance);
	EASY_IF(vec->clean_instance != NULL,
			vec->clean_instance(vec->end);)

	--(vec->size_proc);
	vector_realloc(vec, OPER_POP_BACK);
	update_vector_settings(vec);
}

void vec_func_insert(solid_vector_t *vec, void *begin, void *end, size_t index)
{
	void *ptr_end =NULL, *ptr_begin = NULL;
	size_t size_ins = 0, size_remain = 0;

	assert(check_vector_validity(vec) == true);
	assert(index < vec->size_proc);
	assert(begin < end);

	size_remain = vec->size_proc - (index + 1);
	EASY_IFEL(end != NULL,
			  size_ins = size_range(vec, begin, end);,

			  size_ins = 1;)

	vec->size_proc += size_ins;
	vector_realloc(vec, OPER_INSERT);
	update_vector_settings(vec);

	ptr_begin = vec->begin, VOID_OFFSET(ptr_begin, index * vec->size_instance);
	ptr_end = ptr_begin, VOID_OFFSET(ptr_end, size_ins * vec->size_instance);

	memmove(ptr_end, ptr_begin, size_remain * vec->size_instance);
	memcpy(ptr_begin, begin, size_ins * vec->size_instance);

	assert(check_vector_validity(vec) == true);
}

void vec_func_erase(solid_vector_t *vec, void *begin, void *end)
{
	size_t size_of_range = 0, size_remain = 0;

	assert(check_vector_validity(vec) == true);
	assert(begin < end);
	assert(is_inside_range(vec, begin) == true);
	assert(is_inside_range(vec, end) == true);

	EASY_IFEL(end != NULL,
			  size_of_range = size_range(vec, begin, end);,

			  size_of_range = 1;)

	size_remain = vec->size_proc -
					 size_range(vec, vec->begin, end);

	EASY_IF(vec->clean_instance != NULL,
			cleaning_items(vec, begin, end);)

	memmove(begin, end, size_remain * vec->size_instance);
	vec->size_proc -= size_of_range;
	update_vector_settings(vec);

	assert(check_vector_validity(vec) == true);
}

void vec_func_swap(solid_vector_t *vec_l, solid_vector_t *vec_r)
{
	solid_vector_t vec_tmp;

	assert(check_vector_validity(vec_l) == true);
	assert(check_vector_validity(vec_r) == true);

	vec_tmp = *vec_l, *vec_l = *vec_r, *vec_r = vec_tmp;

	assert(check_vector_validity(vec_l) == true);
	assert(check_vector_validity(vec_r) == true);
}

void vec_func_clear(solid_vector_t *vec)
{
	assert(check_vector_validity(vec) == true);

	free_vector(vec);
}

//static_functions___________________________________________________

//check_args_________________________________________________________
bool check_vector_validity(solid_vector_t *vec)
{
	EASY_IF(vec == NULL,
			LOG_DEBUG("FAIL: solid_vector is NULL", NULL);
			goto failure;)

	EASY_IF((vec->size_proc > vec->size_real) || vec->size_instance == 0,
			LOG_DEBUG("FAIL: size_proc more then size_real or size_instance == 0\n"
					  "(size_proc: %lu, size_real: %lu, size_instance: %lu)",
					  vec->size_proc, vec->size_real, vec->size_instance);
			goto failure;)

	EASY_IF(vec->size_proc != 0 && (vec->begin == NULL ||
									vec->end == NULL ||
									vec->arr == NULL),
			LOG_DEBUG("FAIL: begin, end or arr equal NULL but size_proc no\n"
					  "(begin: %p, end: %p, arr: %p)",
					  (void*)vec->begin, (void*)vec->end, (void*)vec->arr);
			goto failure;)

	EASY_IF(vec->begin != NULL && vec->begin >= vec->end,
			LOG_DEBUG("FAIL: begin >= end", NULL);
			goto failure;)

	return true;
failure:
	return false;
}

//___________________________________________________________________

bool is_inside_range(solid_vector_t *vec, void *ptr)
{
	EASY_IFEL(ptr >= vec->begin && ptr < vec->end,
			  return true;,

			  return false;)
}

size_t size_range(solid_vector_t *vec, void *begin, void *end)
{
	return (u_long)((u_int8_t*)end - (u_int8_t*)begin) / vec->size_instance;
}

void free_vector(solid_vector_t *vec)
{
	EASY_IF(vec->clean_instance != NULL && vec->size_proc != 0,
			cleaning_items(vec, vec->begin, vec->end);)

	free(vec->arr), vec->size_real = 0;
	update_vector_settings(vec);
}

void cleaning_items(solid_vector_t *vec, void *begin, void *end)
{
	for (; begin != end; VOID_OFFSET(begin, vec->size_instance))
	{
		vec->clean_instance(begin);
	}
}

void update_vector_settings(solid_vector_t *vec)
{
	EASY_IF(vec->size_real == 0,
			vec->size_proc = 0; vec->arr = NULL;)
	EASY_IF(vec->size_proc == 0, vec->begin = vec->end = NULL;)

	EASY_IF(vec->arr != NULL && vec->size_proc != 0,
			vec->begin = vec->end = vec->arr;
			VOID_OFFSET(vec->end, vec->size_proc * vec->size_instance);)
}

bool update_size_real(solid_vector_t* vec, vec_oper_t oper, va_list val)
{
	switch (oper)
	{
		case OPER_RESIZE: case OPER_ASSIGN:
			vec->size_real = va_arg(val, size_t);
			break;
		case OPER_PUSH_BACK:
			EASY_IFELIF(vec->arr == NULL,
						vec->size_real = REAL_SIZE_INITIAL;
							goto success;,

						vec->size_proc >= vec->size_real,
						vec->size_real = multipl_ulong_double(vec->size_real, SIZE_FACTOR);
							goto success;)
			goto failure;
		case OPER_POP_BACK:
			EASY_IFEL(div_ulong_ulong(vec->size_real, vec->size_proc) >= SIZE_RATIO,
					vec->size_real = vec->size_proc;,

					goto failure;)
			break;
		case OPER_INSERT:
			EASY_IFEL(vec->size_proc >= vec->size_real,
					vec->size_real = vec->size_proc;
					goto success;,

					goto failure;)
		default:
			LOG_DEBUG("FAIL: unknown operation;(oper: %u)", oper);
			goto failure;
	}

success:
	va_end(val);
	return true;
failure:
	va_end(val);
	return false;
}

void vector_realloc(solid_vector_t* vec, vec_oper_t oper, ...)
{
	bool flag_realloc = false;
	va_list val;

	va_start(val, oper);
	flag_realloc = update_size_real(vec, oper, val);
	va_end(val);

	if (flag_realloc == true)
	{
		EASY_REALLOC(vec->arr, vec->arr, vec->size_real * vec->size_instance)
	}
}

//___________________________________________________________________
