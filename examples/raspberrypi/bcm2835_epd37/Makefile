
CUR_DIR := $(shell pwd)
DIR_NAME := $(shell basename $(CUR_DIR))
OBJ_PATH := $(BUILD_PATH)/$(DIR_NAME)
EXEC := bcm2835_epd37

SRCS := $(wildcard *.cpp)
OBJS := $(addprefix $(OBJ_PATH)/, $(patsubst %.cpp, %.o, $(SRCS)))

INCLUDEFLAGS := -I$(ROOT_DIR)/libepd37
LIBFLAGS := $(ROOT_DIR)/build/libepd37/libepd37.a \
            -lbcm2835

all : BUILD_DIR $(OBJS) $(EXEC)

$(EXEC):
	$(CXX) $(INCLUDEFLAGS) $(OBJS) -o $(BIN_PATH)/$@ $(LIBFLAGS)

BUILD_DIR:
	@[ -d $(OBJ_PATH) ] || mkdir -p $(OBJ_PATH)

$(OBJ_PATH)/%.o : %.cpp
	$(CXX) -c $(INCLUDEFLAGS) $(CFLAGS) $< -o $@

clean:
	rm $(OBJ_PATH)/*.o
