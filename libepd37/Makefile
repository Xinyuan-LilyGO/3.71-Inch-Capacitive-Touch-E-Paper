
CUR_DIR := $(shell pwd)
DIR_NAME := $(shell basename $(CUR_DIR))
OBJ_PATH := $(BUILD_PATH)/$(DIR_NAME)
LIB := $(DIR_NAME).a

# SRCS := $(wildcard *.c)
# SRCS += $(wildcard *.cpp)
# OBJS := $(addprefix $(OBJ_PATH)/, $(patsubst %.c, %.o, $(SRCS)))
# OBJS += $(addprefix $(OBJ_PATH)/, $(patsubst %.cpp, %.o, $(SRCS)))
OBJS := $(OBJ_PATH)/epd2in13b.o \
		$(OBJ_PATH)/epdif.o \
		$(OBJ_PATH)/font8.o \
		$(OBJ_PATH)/font12.o \
		$(OBJ_PATH)/font16.o \
		$(OBJ_PATH)/font20.o \
		$(OBJ_PATH)/font24.o \
		$(OBJ_PATH)/epdpaint.o

all : BUILD_DIR $(OBJS) $(OBJ_PATH)/$(LIB)

BUILD_DIR:
	@[ -d $(OBJ_PATH) ] || mkdir -p $(OBJ_PATH)

$(OBJ_PATH)/$(LIB): $(OBJS)
	ar crv $@ $^

$(OBJ_PATH)/epd2in13b.o: epd2in13b.cpp epd2in13b.h
	$(CXX) -c $< -o $@
$(OBJ_PATH)/epdif.o: epdif.cpp epdif.h
	$(CXX) -c $< -o $@
$(OBJ_PATH)/font8.o: font8.c fonts.h
	$(CC) -c $< -o $@
$(OBJ_PATH)/font12.o: font12.c fonts.h
	$(CC) -c $< -o $@
$(OBJ_PATH)/font16.o: font16.c fonts.h
	$(CC) -c $< -o $@
$(OBJ_PATH)/font20.o: font20.c fonts.h
	$(CC) -c $< -o $@
$(OBJ_PATH)/font24.o: font24.c fonts.h
	$(CC) -c $< -o $@
$(OBJ_PATH)/epdpaint.o: epdpaint.cpp epdpaint.h
	$(CXX) -c $< -o $@

clean:
	rm $(OBJ_PATH)/*.o $(OBJ_PATH)/$(LIB)
