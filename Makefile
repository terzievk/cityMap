SRC_DIR := sources
OBJ_DIR := objects
INC_DIRS := includes doctest

# f is fast to type - maybe change later
EXE := f
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# where to search for headers
CPPFLAGS := $(addprefix -I,$(INC_DIRS))
# generate dependencies makefiles,
# which we read at the bottom with `include`
CPPFLAGS := $(CPPFLAGS) -MMD -MP

CXXFLAGS := -Wall -Werror -std=c++20

.PHONY: all clean test

all: $(EXE)
## these two lines will save some typing for a while
## remove later
	@echo
	@echo ==================================================================
	@echo executing ./$(EXE):
	@echo
	./$(EXE)

clean:
	@$(RM) -rv $(EXE) $(OBJ_DIR)

test: $(EXE)
	@./$(EXE) -e

$(EXE): $(OBJ)
	$(CXX) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	echo $(CPPFLAGS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

-include $(OBJ:.o=.d)
