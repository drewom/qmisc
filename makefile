#!/usr/bin/make -f
TARGET_EXEC ?= $(shell basename "$$PWD")

SRCS := $(shell find . -name *.cpp -or -name *.c)

INC_DIRS := ../
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

DEBUG=1
ifneq ($(DEBUG),0)
	TARGET := .dbg
	_CPPFLAGS += $(INC_FLAGS) -DDEBUG -MMD -MP
	_CFLAGS   += -O0 -g
	_CXXFLAGS += -O0 -g
else
	TARGET :=
	_CPPFLAGS += $(INC_FLAGS) -DNDEBUG -MMD -MP
	_CFLAGS   += -O2 -g
	_CXXFLAGS += -O2 -g
endif

OBJS := $(SRCS:./%=obj/%$(TARGET).o)
DEPS := $(OBJS:.o=.d)


# Merge flags with user flags. We ensure this way that user options follow our
# options, which should give user's options preference while not completely
# replacing our own. This allows us to harden our compile while still allowing
# the user to override us (for testing different -On values for instance).
override CPPFLAGS := $(_CPPFLAGS) $(CPPFLAGS)
override CFLAGS := $(_CFLAGS) $(CFLAGS)
override CXXFLAGS := $(_CXXFLAGS) $(CXXFLAGS)
override LDFLAGS := $(_LDFLAGS) $(LDFLAGS)

.PHONY: all
all: $(TARGET_EXEC)$(TARGET)
	@echo "Running tests..."
	@./$(TARGET_EXEC)$(TARGET) && echo "Success!" || echo "Failure!"

.PHONY: debugger
debugger: $(TARGET_EXEC)$(TARGET)
	@lldb -f ./$(TARGET_EXEC)$(TARGET) -o "break set -n main" -o run

.PHONY: clean
clean:
	@echo Removing all make generated files
	@rm -rf obj
	@rm -f $(TARGET_EXEC) $(TARGET_EXEC).dbg tags

$(TARGET_EXEC)$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c source
obj/%.c$(TARGET).o: %.c | obj/
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
obj/%.cpp$(TARGET).o: %.cpp | obj/
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# directories (add with | so only made if missing)
%/:
	mkdir -p $@

# Include #include rules for specific source files
-include $(DEPS)
