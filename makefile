#!/usr/bin/make -f
# DEBUG={0|1}
TARGET_EXEC ?= $(shell basename "$$PWD")

SRCS := $(shell find . -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:./%=obj/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := ../
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

DEBUG=1

ifeq ($(DEBUG),1)
CFLAGS+=-O0 -g -ggdb3
else
CFLAGS+=-O2
endif

.PHONY: all
all: $(TARGET_EXEC)
	./$(TARGET_EXEC)

.PHONY: clean
clean:
	@echo removing all
	@rm -r $(TARGET_EXEC) obj

$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# assembly
obj/%.s.o: %.s | obj/
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
obj/%.c.o: %.c | obj/
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
obj/%.cpp.o: %.cpp | obj/
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# directories (add with | so only made if missing)
%/:
	mkdir -p $@

# Include #include rules for specific source files
-include $(DEPS)
