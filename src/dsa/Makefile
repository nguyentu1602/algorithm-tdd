# := evaluate only once; = evaluate everytime $() is used
CXX:= g++
PROJECT := dsa
WARNINGS := -Wall -Wextra
COMMON_FLAGS := -g -std=c++0x

# pretty print Makefile
Q := @

########################
# directory listing
########################
SRC_DIR = src
BUILD_DIR = build
BUILD_INCLUDE_DIR := $(BUILD_DIR)/src
INCLUDE_DIRS += $(BUILD_INCLUDE_DIR) ./include ./src


########################
# Get all sources files
# CXX_SRCS are the source files excluding test ones
CXX_SRCS := $(shell find $(SRC_DIR)/$(PROJECT) ! -name "Test*.cpp" \
	! -name "*_sol.cpp" -name "*.cpp")
# TEST_SRCS are the test source files
# TEST_MAIN_SRC := $(shell find $(SRC_DIR)/$(PROJECT)/test -name "test_main.cc")
TEST_SRCS := $(shell find $(SRC_DIR)/$(PROJECT)/test -name "Test*.cpp")
TEST_SRCS := $(filter-out $(TEST_MAIN_SRC), $(TEST_SRCS))
# GTEST_SRC := $(SRC_DIR)/gtest/gtest-all.cc


########################
# Derive generated files
# what does the {} do?
CXX_OBJS := $(addprefix $(BUILD_DIR)/,  ${CXX_SRCS:.cpp=.o})
TEST_OBJS := $(addprefix $(BUILD_DIR)/, ${TEST_SRCS:.cpp=.o})
# GTEST_OBJ := $(addprefix $(BUILD_DIR)/, ${GTEST_SRC:.cc=.o})

# Gather all objects files that needed to be built
OBJS := $(CXX_OBJS)


# Output files for automatic dependency generation
# each .d file shows the dependencies for the associated .o file
DEPS := ${CXX_OBJS:.o=.d} ${TEST_OBJS:.o=.d}
# The target shared library name
LIB_BUILD_DIR := $(BUILD_DIR)/lib
LIBRARY_DIRS += $(LIB_BUILD_DIR)

STATIC_NAME := $(LIB_BUILD_DIR)/lib$(PROJECT).a
DYNAMIC_NAME := $(LIB_BUILD_DIR)/lib$(PROJECT).so

# IT IS VERY TRICKY HOW ORIGIN IS USED IN HERE!!!!
ORIGIN := \$$ORIGIN


#########################
# compiling (debug) flags and linking flags
#########################
COMMON_FLAGS += $(foreach includedir, $(INCLUDE_DIRS), -I$(includedir))

CFLAGS += -pthread -fPIC $(COMMON_FLAGS) $(WARNINGS)
LFLAGS += -pthread -fPIC $(COMMON_FLAGS) $(WARNINGS)
LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))

# Automatic dependency generation - it will create lots of .ld files
# one per .o file. These .d files will be picked up by the -include
# directive in the Makefile later
CFLAGS += -MMD -MP


TEST_BIN_DIR := $(BUILD_DIR)/test
# TEST_ALL_BIN := $(TEST_BIN_DIR)/test_all.testbin
TEST_BINS := $(addsuffix .testbin, $(addprefix $(TEST_BIN_DIR)/,\
	$(foreach obj, $(TEST_OBJS), $(basename $(notdir $(obj))))))

TEST_BUILD_DIR := $(BUILD_DIR)/$(SRC_DIR)/$(PROJECT)/test
# Get all directory containing code. Later we mimic the structure of the directory
# in the build folder
SRC_DIRS := $(shell find * -type d -exec bash -c "find {} -maxdepth 1 \
	\( -name '*.cpp' -o -name '*.cpp' \) | grep -q ." \; -print)

ALL_BUILD_DIRS := $(sort $(BUILD_DIR) $(addprefix $(BUILD_DIR)/, $(SRC_DIRS)) \
	$(TEST_BIN_DIR) $(LIB_BUILD_DIR) $(TEST_BUILD_DIR))





.PHONY: all test runtest clean

all: $(OBJS)
	$(info CXX_SRCS= $(CXX_SRCS))
	$(info CXX_OBJS= $(CXX_OBJS))
	$(info TEST_OBJS= $(TEST_OBJS))
	$(info TEST_BINS= $(TEST_BINS))
	$(info SRC_DIRS= $(SRC_DIRS))
	$(info ALL_BUILD_DIRS= $(ALL_BUILD_DIRS))


test: $(TEST_BINS)

# run all the tests in the test folder:
runtest: $(TEST_BINS)
	for test in $(TEST_BINS); do ./$$test; done

clean:
	rm -rf build

#######################
# Implicit rules:
#######################

# create all needed build directories. -p avoids hierachial dependency
$(ALL_BUILD_DIRS):
	@ mkdir -p $@

$(DYNAMIC_NAME): $(OBJS) | $(LIB_BUILD_DIR)
	@ echo LD -o $@
	$(Q)$(CXX) -shared -o $@ $(OBJS) $(LFLAGS) $(LDFLAGS)

$(STATIC_NAME): $(OBJS) | $(LIB_BUILD_DIR)
	@ echo AR -o $@
	$(Q)ar rcs $@ $(OBJS)

# For the .o objects in /build
$(BUILD_DIR)/%.o: %.cpp | $(ALL_BUILD_DIRS)
	@ echo CXX $<
	$(Q) $(CXX) $(CFLAGS) -c $< -o $@

# # Link the aggregate test file dynamically. It uses -rpath and require a libproj.so fie
# # in the location specified by rpath. $(ORIGIN) is resolved once done
# $(TEST_ALL_BIN): $(TEST_MAIN_SRC) $(TEST_OBJS) $(GTEST_OBJ) \
# 	| $(DYNAMIC_NAME) $(TEST_BIN_DIR)
# 	@ echo CXX/LD -o $@ $<
# 	$(CXX) $(TEST_MAIN_SRC) $(TEST_OBJS) $(GTEST_OBJ) -o $@ \
# 		$(LFLAGS) $(LDFLAGS) -l$(PROJECT) -Wl,-rpath,$(ORIGIN)/../lib

# link the aggregate test file staticly. You need the libproj.a file when linking, but
# once it's done you can run it as a standalone against libproj.a. I use -Wl flag in gcc
# to pass flag to the linker. It's REQUIRED that '-Wl -Bdynamic' is trailing because
# it let all the other libraries compiled dynamically.
# $(TEST_ALL_BIN): $(TEST_MAIN_SRC) $(TEST_OBJS) $(GTEST_OBJ) \
# 	| $(STATIC_NAME) $(TEST_BIN_DIR)
# 	@ echo CXX/LD -o $@ $<
# 	$(Q) $(CXX) $(TEST_MAIN_SRC) $(TEST_OBJS) $(GTEST_OBJ) -o $@ \
# 		$(LFLAGS) $(LDFLAGS) -Wl,-Bstatic -l$(PROJECT) -Wl,-Bdynamic

# use static linking to libproj.a here
$(TEST_BINS): $(TEST_BIN_DIR)/%.testbin: $(TEST_BUILD_DIR)/%.o \
	| $(STATIC_NAME) $(TEST_BIN_DIR)
	@ echo LD $<
	$(Q) $(CXX) $< -o $@ \
		$(LFLAGS) $(LDFLAGS) -Wl,-Bstatic -l$(PROJECT) -Wl,-Bdynamic


# for automatic dependency generation:
# it will include all the .d files that generate by the -MMD flag of gcc
-include $(DEPS)
















# # search all deps and sources, exclusing _sol.
# DEPS := $(wildcard $(INCDIR)/*.*)
# SOURCES := $(wildcard $(SRCDIR)/*.cpp)
# EXCLUDE_SOURCES := $(wildcard $(SRCDIR)/*_sol.cpp)
# SOURCES := $(filter-out $(EXCLUDE_SOURCES), $(SOURCES)) # must pass a list of file

# EXEC_SOURCES := $(wildcard $(SRCDIR)/Test*.cpp)
# EXEC_SOURCES := $(filter-out $(EXCLUDE_SOURCES), $(EXEC_SOURCES))
# EXCLUDE_EXECS := $(addprefix $(SRCDIR)/, TestCuckooHashTable)
# EXEC_SOURCES := $(filter-out $(EXCLUDE_SOURCES), $(EXEC_SOURCES))


# EXEC_OBJS := $(addprefix $(BINDIR)/, $(notdir $(EXEC_SOURCES:.cpp=.o)))
# # make a list of object files
# OBJS := $(addprefix $(BINDIR)/, $(notdir $(SOURCES:.cpp=.o)))

# # make a list of test file (all binary)
# EXECS := $(basename $(EXEC_OBJS))


# all: main
# main: $(OBJS) $(EXECS)
# 	$(info OBJS = $(OBJS))
# 	@echo 'Start building a list of binary files with prefix Test*'
# 	$(info EXEC_SOURCES = $(EXEC_SOURCES))
# 	$(info EXEC_OBJS = $(EXEC_OBJS))
# 	$(info EXECS = $(EXECS))
# # automatically generate a list of dependency
# # List of automatic variables used
# build/%.o: src/%.cpp
# 	$(CXX) $(CFLAGS) -o $@ $<

# build/%: build/%.o
# 	$(CXX) $(LFLAGS) $@ $^


# 	# $(CXX) -std=c++0x -Wall SeparateChaining.cpp TestSeparateChaining.cpp -o build/testSeparateChaining
# 	# $(CXX) -std=c++0x -Wall QuadraticProbing_sol.cpp TestQuadraticProbing.cpp -o build/testQuadraticProbing
# 	# $(CXX) -std=c++0x -Wall myTestSort.cpp -o build/myTestSort
# 	# $(CXX) -std=c++0x -Wall RadixSort.cpp -o build/RadixSort


# test:
# 	# ./build/testList
# 	# ./build/testBST
# 	# ./build/testVector
# 	# ./build/testBinaryHeap
# 	# ./build/testBinomialQueue
# 	#./build/testSeparateChaining
# 	#./build/testQuadraticProbing
# 	#./build/myTestSort
# 	# ./build/RadixSort

# clean:
# 	rm -rf *~
# 	rm -rf build
