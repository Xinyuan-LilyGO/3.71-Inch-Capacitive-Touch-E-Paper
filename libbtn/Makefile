
CUR_DIR := $(shell pwd)
DIR_NAME := $(shell basename $(CUR_DIR))
OBJ_PATH := $(BUILD_PATH)/$(DIR_NAME)
LIB := $(DIR_NAME).a

# SRCS := $(wildcard *.c)
# OBJS := $(addprefix $(OBJ_PATH)/, $(patsubst %.c, %.o, $(SRCS)))

OBJS := $(OBJ_PATH)/button.o

all : BUILD_DIR $(OBJS) $(OBJ_PATH)/$(LIB)

BUILD_DIR:
	@[ -d $(OBJ_PATH) ] || mkdir -p $(OBJ_PATH)

$(OBJ_PATH)/$(LIB): $(OBJS)
	ar crv $@ $^

$(OBJ_PATH)/button.o: button.cpp button.h
	$(CXX) -c $< -o $@

clean:
	rm $(OBJ_PATH)/*.o $(OBJ_PATH)/$(LIB)
