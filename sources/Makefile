include make_base.mk

CFLAGS += -D _GNU_SOURCE -iquote $(root_project_path)/headers
lib_path_move = $(root_project_path)/lib
lib_obj := mathematic.o vector_func.o init_solid_vector.o

vector_func.o: solid_vector.h mathematic.h definitions.h
init_solid_vector.c: vector_func.h

libsolid_vector.a: $(lib_obj)

clean:
	rm -fv *.o
