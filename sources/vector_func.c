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
static void free_vector(struct solid_vector *vec);
static void cleaning_items(vector_settings_t *v_s, void *begin, void *end);
static void update_vector_settings(vector_settings_t *v_s);
static bool update_sizes_for_realloc(vector_settings_t* v_s, vec_oper_t oper, va_list val);
static void vector_realloc(solid_vector_t *vec, vec_oper_t oper, ...);
static size_t size_range(struct solid_vector *vec, void *begin, void *end);

void* vec_func_begin(solid_vector_t *vec)
{
	assert(check_vector_validity(vec) == true);

	return vec->v_s.begin;
}

void* vec_func_end(solid_vector_t *vec)
{
	assert(check_vector_validity(vec) == true);

	return vec->v_s.end;
}

size_t vec_func_size(solid_vector_t *vec)
{
	assert(check_vector_validity(vec) == true);

	return vec->v_s.size_proc;
}

void vec_func_resize(solid_vector_t *vec, size_t new_size_proc)
{
	vector_settings_t *v_s = NULL;
	void *ptr_1 = NULL;

	assert(check_vector_validity(vec) == true);

	v_s = &(vec->v_s);
	EASY_IF(new_size_proc == v_s->size_proc, goto success;)

	if (new_size_proc < v_s->size_proc)
	{
		if (v_s->clean_instance != NULL)
		{
			VOID_OFFSET(ptr_1, new_size_proc * v_s->size_instance);
			cleaning_items(v_s, ptr_1, v_s->end);
		}
	}
	else if (new_size_proc > v_s->size_real)
	{
		vector_realloc(vec, OPER_RESIZE, new_size_proc);
	}

	v_s->size_proc = new_size_proc;
	update_vector_settings(v_s);

	assert(check_vector_validity(vec) == true);

success:
	return;
}

bool vec_func_empty(struct solid_vector *vec)
{
	assert(check_vector_validity(vec) == true);

	return vec->v_s.size_proc == 0 ? true : false;
}

void* vec_func_at(struct solid_vector *vec, size_t index)
{
	void *ptr = NULL;

	assert(check_vector_validity(vec) == true);

	ptr = vec->v_s.begin;
	EASY_IFEL(index < vec->v_s.size_proc,
			  VOID_OFFSET(ptr, index * vec->v_s.size_instance);,

			  ptr = NULL;)

	return ptr;
}

void vec_func_assign(struct solid_vector *vec, void *begin, void *end, size_t repeater)
{
	void *ptr = NULL;

	assert(check_vector_validity(vec) == true);

	free_vector(vec);

	EASY_IF(end != NULL,
			repeater = size_range(vec, begin, end);)

	vector_realloc(vec, OPER_ASSIGN, repeater);
	vec->v_s.size_proc = repeater;
	update_vector_settings(&(vec->v_s));

	EASY_IFEL(end != NULL,
			  memcpy(vec->v_s.arr, begin, repeater * vec->v_s.size_instance);,

			  ptr = vec->v_s.begin;
			  while(repeater-- != 0)
				ptr = mempcpy(ptr, begin, vec->v_s.size_instance);
			)

	assert(check_vector_validity(vec) == true);
}

void vec_func_push_back(struct solid_vector *vec, void *value)
{
	vector_settings_t *v_s = NULL;

	assert(check_vector_validity(vec) == true);

	v_s = &(vec->v_s);
	++(v_s->size_proc);
	vector_realloc(vec, OPER_PUSH_BACK);
	update_vector_settings(v_s);

	VOID_OFFSET(v_s->end, -((long)v_s->size_instance));
	assert(v_s->end != NULL);
	v_s->end = mempcpy(v_s->end, value, v_s->size_instance);
}

void vec_func_pop_back(struct solid_vector *vec)
{
	vector_settings_t *v_s = NULL;
	void *ptr = NULL;

	assert(check_vector_validity(vec) == true);

	VOID_OFFSET(v_s->end, -v_s->size_instance);
	EASY_IF(v_s->clean_instance != NULL,
			v_s->clean_instance(ptr);)

	--(v_s->size_proc);
	vector_realloc(vec, OPER_POP_BACK);
	update_vector_settings(v_s);
}

void vec_func_insert(struct solid_vector *vec, size_t index, void *begin, void *end)
{
	void *ptr =NULL, *pos = NULL;
	size_t size_ins = 0, size_after_pos = 0;

	assert(check_vector_validity(vec) == true);

	EASY_IFEL(end != NULL,
			  size_ins = size_range(vec, begin, end);,

			  size_ins = 1;)

	vec->v_s.size_proc += size_ins;
	vector_realloc(vec, OPER_INSERT);
	update_vector_settings(&(vec->v_s));

	pos = vec->v_s.begin, VOID_OFFSET(pos, index * vec->v_s.size_instance);
	ptr = pos, VOID_OFFSET(ptr, size_ins * vec->v_s.size_instance);
	size_after_pos = vec->v_s.size_proc - (index + 1);
	memmove(ptr, pos, size_after_pos * vec->v_s.size_instance);

	memcpy(pos, begin, size_ins * vec->v_s.size_instance);

	assert(check_vector_validity(vec) == true);
}

void vec_func_erase(struct solid_vector *vec, void *begin, void *end)
{
	size_t size_of_range = 0, size_after_end = 0;

	assert(check_vector_validity(vec) == true);

	EASY_IFEL(end != NULL,
			  size_of_range = size_range(vec, begin, end);,

			  size_of_range = 1;)

	size_after_end = vec->v_s.size_proc -
					 size_range(vec, vec->v_s.begin, end);

	EASY_IF(vec->v_s.clean_instance != NULL,
			cleaning_items(&(vec->v_s), begin, end);)

	memmove(begin, end, size_after_end * vec->v_s.size_instance);
	vec->v_s.size_proc -= size_of_range;
	update_vector_settings(&(vec->v_s));

	assert(check_vector_validity(vec) == true);
}

void vec_func_swap(struct solid_vector *vec_1, struct solid_vector *vec_2)
{
	vector_settings_t v_s;

	assert(check_vector_validity(vec_1) == true);
	assert(check_vector_validity(vec_2) == true);

	v_s = vec_1->v_s, vec_1->v_s = vec_2->v_s, vec_2->v_s = v_s;

	assert(check_vector_validity(vec_1) == true);
	assert(check_vector_validity(vec_2) == true);
}

void vec_func_clear(struct solid_vector *vec)
{
	assert(check_vector_validity(vec) == true);

	free_vector(vec);
}

//static_functions___________________________________________________

//check_args_________________________________________________________
bool check_vector_validity(solid_vector_t *vec)
{
	vector_settings_t *v_s = NULL;

	EASY_IF(vec == NULL,
			LOG_DEBUG("FAIL: solid_vector is NULL", NULL);
			goto failure;)

	v_s = &(vec->v_s);
	EASY_IF((v_s->size_proc > v_s->size_real) || v_s->size_instance == 0,
			LOG_DEBUG("FAIL: size_proc more then size_real or size_instance == 0\n"
					  "(size_proc: %lu, size_real: %lu, size_instance: %lu)",
					  v_s->size_proc, v_s->size_real, v_s->size_instance);
			goto failure;)

	EASY_IF(v_s->size_proc != 0 && (v_s->begin == NULL ||
									v_s->end == NULL ||
									v_s->arr == NULL),
			LOG_DEBUG("FAIL: begin, end or arr equal NULL but size_proc no\n"
					  "(begin: %p, end: %p, arr: %p)",
					  (void*)v_s->begin, (void*)v_s->end, (void*)v_s->arr);
			goto failure;)

	EASY_IF(v_s->begin != NULL && v_s->begin == v_s->end,
			LOG_DEBUG("FAIL: begin == end", NULL);
			goto failure;)

	return true;
failure:
	return false;
}

//___________________________________________________________________

size_t size_range(struct solid_vector *vec, void *begin, void *end)
{
	size_t result = 0;

	for (result = 0; begin != end;
		 VOID_OFFSET(begin, vec->v_s.size_instance), ++result);

	return result;
}

void free_vector(struct solid_vector *vec)
{
	EASY_IF(vec->v_s.clean_instance != NULL && vec->v_s.size_proc != 0,
			cleaning_items(&(vec->v_s), vec->v_s.begin, vec->v_s.end);)

	free(vec->v_s.arr), vec->v_s.size_real = 0;
	update_vector_settings(&(vec->v_s));
}

void cleaning_items(vector_settings_t *v_s, void *begin, void *end)
{
	for (; begin != end; VOID_OFFSET(begin, v_s->size_instance))
	{
		v_s->clean_instance(begin);
	}
}

void update_vector_settings(vector_settings_t *v_s)
{
	EASY_IF(v_s->size_real == 0,
			v_s->size_proc = 0; v_s->arr = NULL;)
	EASY_IF(v_s->size_proc == 0, v_s->begin = v_s->end = NULL;)

	EASY_IF(v_s->arr != NULL,
			v_s->begin = v_s->arr;
			VOID_OFFSET(v_s->end, v_s->size_proc * v_s->size_instance);)
}

bool update_sizes_for_realloc(vector_settings_t* v_s, vec_oper_t oper, va_list val)
{
	switch (oper)
	{
		case OPER_RESIZE: case OPER_ASSIGN:
			v_s->size_real = va_arg(val, size_t);
			break;
		case OPER_PUSH_BACK:
			EASY_IFELIF(v_s->arr == NULL,
						v_s->size_real = REAL_SIZE_INITIAL;
							goto success;,

						v_s->size_proc >= v_s->size_real,
						v_s->size_real = multipl_ulong_double(v_s->size_real, SIZE_FACTOR);
							goto success;)
			goto failure;
		case OPER_POP_BACK:
			EASY_IFEL(div_ulong_ulong(v_s->size_real, v_s->size_proc) >= SIZE_RATIO,
					v_s->size_real = v_s->size_proc;,

					goto failure;)
			break;
		case OPER_INSERT:
			EASY_IF(v_s->size_proc >= v_s->size_real,
					v_s->size_real = v_s->size_proc;
					goto success;)
			goto failure;
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
	flag_realloc = update_sizes_for_realloc(&(vec->v_s), oper, val);
	va_end(val);

	if (flag_realloc == true)
	{
		EASY_REALLOC(vec->v_s.arr, vec->v_s.arr, vec->v_s.size_real)
	}
}

//___________________________________________________________________
