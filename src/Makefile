export BUILD_TYPE = debug

#############################################
#              set for compiler             #
#############################################
CFILES_COUNT := $(shell find $(SRC_DIR) -name "*.c" -print | wc -l)
CXXFILES_COUNT := $(shell find $(SRC_DIR) \( -name "*.cc" -o -name "*.cpp" -o -name "*.cxx" \) -print | wc -l)
ifeq (0, $(CFILES_COUNT))
  ifneq (0, $(CXXFILES_COUNT)) #cpp project
    CLANG_EXIST := $(shell which clang++)
    ifneq (, $(CLANG_EXIST)) #if CLANG_EXIST isn't null string
      CC := clang++
    else
      CC := g++
    endif
  endif
else#is C project
  CLANG_EXIST := $(shell which clang)
  ifeq (, $(CLANG_EXIST))
    CC := gcc
  else
    CC := clang
  endif
endif

###################################################
#  Common Flags for gcc, g++, clang and clang++   #
###################################################
COMMON_WARN = -Wall -Wextra -Werror -ansi -pedantic -Wconversion -Wpointer-arith \
  -Wcast-qual -Wcast-align -Wshadow -Wwrite-strings
CWARN_FLAGS = $(COMMON_WARN) -Wstrict-prototypes -Wmissing-prototypes -Wtraditional
CXXWARN_FLAGS = $(COMMON_WARN) -Woverloaded-virtual

DEBUG_FLAGS = -DDEBUG -g

LDFLAGS = -Wl,-lm -lmemusage -lmcheck
EFENCE_TOOL := $(shell dpkg -l | grep -w electric-fence)
ifneq (, $(EFENCE_TOOL))
  LDFLAGS += -lefence
endif
GOOGLE_PERFTOOLS := $(shell dpkg -l | grep -w google-perftools)
ifneq (, $(GOOGLE_PERFTOOLS))
  LDFLAGS += -ltcmalloc
endif
LIB = -L.

#####################################################
#    gcc or g++ options                             #
#####################################################
CFLGAS = $(CWARN_FLAGS) -std=c11 -lmemusage
CXXFLAGS = $(CXXWARN_FLAGS) -std=c++14 -lmemusage

####################################################
#   clang or clang++ options                       #
####################################################
CLANG_WARN = -Weverything -Werror -Wno-c++98-compat -Wno-c++98-compat-pedantic \
 -Wno-documentation -Wno-padded

ADDRESS = address,object-size,bounds,vla-bound
#PTRREF was pointer and reference
PTRREF= alignment,null
INTEGER = integer,enum,bool
#vptr was only for C++
FUNCTION = undefined,return,vptr
OVERFLOW = float-cast-overflow,signed-integer-overflow,unsigned-integer-overflow,shift
DIVIDE = float-divide-by-zero,integer-divide-by-zero

SANITIZE = -fsanitize=$(ADDRESS),$(PTRREF),$(INTEGER),$(FUNCTION),$(OVERFLOW),$(DIVIDE)

CLANG_FLAGS = -std=c11 $(CLANG_WARN) $(SANITIZE)
CLANGPP_FLAGS = -std=c++14 $(CLANG_WARN) $(SANITIZE)

#################################################
#  Chosen GNU compiler or LLVM/Clang comiler    #
#################################################
ifeq ($(CC), gcc)
  FLAGS := $(CFLGAS)
else ifeq ($(CC), g++)
	FLAGS := $(CXXFLAGS)
else ifeq ($(CC), clang)
	FLAGS := $(CLANG_FLAGS)
else
  FLAGS := $(CLANGPP_FLAGS)
endif

ifeq (, $(findstring release, $(BUILD_TYPE)))
  FLAGS += $(DEBUG_FLAGS)
endif

ifneq (, $(findstring release, $(BUILD_TYPE)))
	OPT_FLAGS := -O2
else
	OPT_FLAGS := -O0
endif
FLAGS += $(OPT_FLAGS)
##################################################
#         The project files settings             #
##################################################
SRC_DIR = .
OBJ_DIR = ../obj
BIN_DIR = ../bin
LIB_DIR = ../lib
DIRS = $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)
MK_DIRS = \
  for dir in $(DIRS); do\
    if [ ! -d $$dir ]; then\
      echo "$$dir not exist, create it!";\
      mkdir $$dir;\
    fi;\
  done
EXE = $(BIN_DIR)/a.out
OBJ = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cc))

#link all needed object files to binary file.
$(EXE): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(EXE) $(LIB) $(LDFLAGS)

#then I should set most object files. Attention, I should use %.o, %.c, not *.o, *.c.
#obj/%.o : $(SRC_DIR)/%.cc
#    $(CC) $(FLAGS) -c $< -o $@
$(OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean all
clean:
	rm -fv $(OBJ_DIR)/*.o $(EXE)
	echo "remove all object files and sort"
all:
	$(MK_DIRS)
	make
