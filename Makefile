SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
DOC_DIR := docs

PROG_NAME := demo.out

EXE := $(BIN_DIR)/$(PROG_NAME)
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude -MMD -MP
CXXFLAGS := -std=c++17
LDFLAGS  := -Llib
LDLIBS   := -lGL -lGLU -lglut

all: $(EXE)

docs:
	@doxygen
	@xdg-open $(DOC_DIR)/html/index.html

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean-all: clean clean-docs

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

clean-docs:
	@$(RM) -rv $(DOC_DIR)

-include $(OBJ:.o=.d)

.PHONY: all clean-all clean docs clean-docs run