# 
# Reqired CXX 
# Optioal CXX_FLAGS, FLAGS_FAST, FLAGS_OPENMP
#



# path #
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin
LD_LIBRARY_PATH+=lib
# executable # 
BIN_NAME = pwp

SOURCES = $(shell find $(SRC_PATH) -name "*.cpp")
OBJECTS = $(SOURCES:$(SRC_PATH)/%.cpp=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

# flags #
COMPILE_FLAGS = -std=c++14 -Wall -Wextra -pedantic $(FLAGS_OPENMP)
LIBS = -lgmp -lgmpxx $(FLAGS_OPENMP)
CXXFLAGS += $(COMPILE_FLAGS)
INCLUDES = -Iinclude


# checks the executable and symlinks to the output
.PHONY: all
all: dirs $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

.PHONY: dirs
dirs:
	mkdir -p $(BUILD_PATH)
	mkdir -p $(BIN_PATH)

.PHONY: debug
debug: clean
debug: COMPILE_FLAGS += -g
debug: all

.PHONY: optimized
optimized: clean
optimized: COMPILE_FLAGS += $(FLAGS_FAST) -qopt-report=5
optimized: all

.PHONY: clean
clean:
	@$(RM) $(BIN_NAME)
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)


# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LIBS)

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
