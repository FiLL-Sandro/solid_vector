root_project_path := $(realpath ./)
make_path := $(root_project_path)
VPATH := $(root_project_path)/headers:$(root_project_path)/sources

export root_project_path make_path VPATH

all:
	$(MAKE) -I $(make_path) -s -C ./sources libsolid_vector.a

clean:
	$(MAKE) -I $(make_path) -C ./sources clean
