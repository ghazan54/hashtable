CC := gcc
CFLAGS := -Wall -Wextra -g
CPPFLAGS := -I src/ -I thirdparty/
TARGET := build/hashtable_test.out
LIB := build/hashtable.a

SRC_DIR := src
OBJ_DIR := build
TEST_DIR := test
THIRDPARTY_DIR := thirdparty

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_EXE := $(TEST_DIR)/test.c
DEPS := $(OBJ:.o=.d)

.PHONY: all clean

all: create_build $(TARGET)

create_build:
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi

$(TARGET): $(OBJ) $(TEST_EXE) $(LIB)
	$(CC) $(CFLAGS) $^ -o $@ $(CPPFLAGS)

$(LIB): $(OBJ)
	ar rcs $@ $^

-include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(CPPFLAGS)
	$(CC) $(CFLAGS) -MM -MT $@ -MF $(OBJ_DIR)/$*.d $<

test: $(TARGET)
	build/hashtable_test.out

clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(TARGET) $(LIB)
