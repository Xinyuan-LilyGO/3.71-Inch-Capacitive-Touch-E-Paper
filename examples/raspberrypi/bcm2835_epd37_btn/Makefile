
CUR_DIR := $(shell pwd)
DIR_NAME := $(shell basename $(CUR_DIR))
OBJ_PATH := $(BUILD_PATH)/$(DIR_NAME)
EXEC := bcm2835_epd37_btn

SRCS := $(wildcard *.cpp)
OBJS := $(addprefix $(OBJ_PATH)/, $(patsubst %.cpp, %.o, $(SRCS)))

INCLUDEFLAGS := -I$(ROOT_DIR)/libepd37
INCLUDEFLAGS += -I$(ROOT_DIR)/libbtn
LIBFLAGS := $(ROOT_DIR)/build/libepd37/libepd37.a \
			$(ROOT_DIR)/build/libbtn/libbtn.a \
			-lpthread \
			-lbcm2835

all : BUILD_DIR $(OBJS)  $(EXEC)

$(EXEC):
	$(CXX) $(INCLUDEFLAGS) $(OBJS) $(FONTS_OBJ) -o $(BIN_PATH)/$@ $(LIBFLAGS)

BUILD_DIR:
	@[ -d $(OBJ_PATH) ] || mkdir -p $(OBJ_PATH)

$(OBJ_PATH)/%.o : %.cpp
	$(CXX) -c $(INCLUDEFLAGS) $(CFLAGS) $< -o $@

# $(OBJ_PATH)/font12.o: font12.c
# 	$(CXX) -c $(INCLUDEFLAGS) $(CFLAGS) $< -o $@

clean:
	rm $(OBJ_PATH)/*.o
