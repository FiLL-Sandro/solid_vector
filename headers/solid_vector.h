#ifndef SOLID_VECTOR_H
#define SOLID_VECTOR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct solid_vector
{
	size_t size_proc, size_real, size_instance;
	void *begin, *end, *arr;

	/*
	 * pointer to instance cleaner(may be NULL)
	 */
	void (*clean_instance)(void *instance);
} solid_vector_t;

typedef struct methods_solid_vector
{
	/*
	 * return value of @begin field into vector_settings_t;
	 */
	void* (*begin)(solid_vector_t *vec);

	/*
	 * return value of @end field into vector_settings_t
	 */
	void* (*end)(solid_vector_t *vec);

	/*
	 * return value of @size_proc field into vector_settings_t
	 */
	size_t (*size)(solid_vector_t *vec);

	/*
	 * change @size_proc into vector_settings on @new_size_proc
	 */
	void (*resize)(solid_vector_t *vec, size_t new_size_proc);

	/*
	 * check @size_proc equal 0(true) or not(false)
	 */
	bool (*empty)(solid_vector_t *vec);

	/*
	 * return pointer to @index value
	 */
	void* (*at)(solid_vector_t *vec, size_t index);

	/*
	 * change @arr into vector_setting on range between @begin and @end;
	 * if @end == NULL then into @arr will be allocated array consists @repeater copy
	 *	of @begin value
	 */
	void (*assign)(solid_vector_t *vec, void *begin, void *end, size_t repeater);

	/*
	 * append @value in vector
	 */
	void (*push_back)(solid_vector_t *vec, void *value);

	/*
	 * delete last vector element
	 */
	void (*pop_back)(solid_vector_t *vec);

	/*
	 * insert block @begin-@end into vector on @index position
	 */
	void (*insert)(solid_vector_t *vec, void *begin, void *end, size_t index);

	/*
	 * removes from the vector a range of elements ([begin,end))
	 */
	void (*erase)(solid_vector_t *vec, void *begin, void *end);

	/*
	 * vec_1 exchanges vector_settings with vec_2
	 */
	void (*swap)(solid_vector_t *vec_1, solid_vector_t *vec_2);

	/*
	 * removes all elements from the vector (which are destroyed),
	 * leaving the container with a size of 0
	 */
	void (*clear)(solid_vector_t *vec);
} methods_solid_vector_t;

extern int initialized_solid_vector(solid_vector_t *vec,  methods_solid_vector_t *meth_vec,
									size_t size_instance, void (*clean_instance)(void *instance));

#endif
