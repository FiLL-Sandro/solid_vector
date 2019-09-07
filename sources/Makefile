include make_base.mk

CFLAGS += -D _GNU_SOURCE -iquote $(root_project_path)/headers
lib_path_move = $(root_project_path)/lib
lib_obj := mathematic.o solid_vector.o vector_func.o

vector_func.o: common_data.h mathematic.h
solid_vector.o: common_data.h vector_func.h

libsolid_vector.a: $(lib_obj)

clean:
	rm -fv *.o
