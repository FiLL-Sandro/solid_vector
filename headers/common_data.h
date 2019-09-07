#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <stddef.h>
#include <stdbool.h>

typedef struct vector_settings
{
	size_t size_proc, size_real, size_instance;
	void *begin, *end, *arr;

	/*
	 * pointer to instance cleaner(may be NULL)
	 */
	void (*clean_instance)(void *instance);
} vector_settings_t;

typedef struct solid_vector
{
	/*
	 * @v_s is set of sizes(proccessing, real and of vector instance),
	 * pointers to the begin and to the end of vector and actually vector
	 */
	vector_settings_t v_s;

	//methods______________________________
	/*
	 * return value of @begin field into vector_settings_t;
	 */
	void* (*begin)(struct solid_vector *vec);

	/*
	 * return value of @end field into vector_settings_t
	 */
	void* (*end)(struct solid_vector *vec);

	/*
	 * return value of @size_proc field into vector_settings_t
	 */
	size_t (*size)(struct solid_vector *vec);

	/*
	 * change @size_proc into vector_settings on @new_size_proc
	 */
	void (*resize)(struct solid_vector *vec, size_t new_size_proc);

	/*
	 * check @size_proc equal 0(true) or not(false)
	 */
	bool (*empty)(struct solid_vector *vec);

	/*
	 * return pointer to @index value
	 */
	void* (*at)(struct solid_vector *vec, size_t index);

	/*
	 * change @arr into vector_setting on range between @begin and @end;
	 * if @end == NULL then into @arr will be allocated array consists @repeater copy
	 *	of @begin value
	 */
	void (*assign)(struct solid_vector *vec, void *begin, void *end, size_t repeater);

	/*
	 * append @value in vector
	 */
	void (*push_back)(struct solid_vector *vec, void *value);

	/*
	 * delete last vector element
	 */
	void (*pop_back)(struct solid_vector *vec);

	/*
	 * insert block @begin-@end into vector on @index position
	 */
	void (*insert)(struct solid_vector *vec, size_t index, void *begin, void *end);

	/*
	 * removes from the vector a range of elements ([begin,end))
	 */
	void (*erase)(struct solid_vector *vec, void *begin, void *end);

	/*
	 * vec_1 exchanges vector_settings with vec_2
	 */
	void (*swap)(struct solid_vector *vec_1, struct solid_vector *vec_2);

	/*
	 * removes all elements from the vector (which are destroyed),
	 * leaving the container with a size of 0
	 */
	void (*clear)(struct solid_vector *vec);

	//_____________________________________
} solid_vector_t;

#endif
