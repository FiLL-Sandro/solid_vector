#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>

#define VOID_OFFSET(_ptr, _bytes) \
	_ptr = (void*)((u_int8_t*)_ptr + _bytes)

#define LOG_DEBUG(_format, ...) \
	printf("%s: %s: %d: "_format "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define EASY_IF(_cond, _action)		\
	do {							\
		if (_cond)					\
		{							\
			_action					\
		}							\
	} while(0);

#define EASY_IFEL(_cond, _action_then, _action_else) \
	do {					\
		if (_cond)	\
		{					\
			_action_then	\
		}					\
		else				\
		{					\
			_action_else	\
		}					\
	} while(0);

#define EASY_IFELIF(_cond_1, _action_1, _cond_2, _action_2) \
	do {					\
		if (_cond_1)		\
		{					\
			_action_1		\
		}					\
		else if (_cond_2)	\
		{					\
			_action_2		\
		}					\
	} while(0);

#define EASY_MALLOC(_result, _size) \
	EASY_REALLOC(_result, NULL, _size)

#define EASY_REALLOC(_result, _arr, _size) \
	do {											\
		_result = realloc(_arr,_size);					\
		EASY_IF(_size != 0, assert(_result != NULL);)	\
	} while(0);

#endif
