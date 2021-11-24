LIBBTN := libbtn
LIBEPD37 := libepd37
LIBFT6336 := libft6336
EXAMPLES_DIR := examples/raspberrypi

BUILD_PATH := $(shell pwd)/build
BIN_PATH := $(shell pwd)/bin
ROOT_DIR := $(shell pwd)

export BIN_PATH BUILD_PATH ROOT_DIR

.PHONY: prepare all clean dirclean distclean

prepare:
	@[ -d $(BUILD_PATH) ] || mkdir -p $(BUILD_PATH)
	@[ -d $(ROOT_DIR)/bin ] || mkdir -p $(ROOT_DIR)/bin

all: prepare
	$(MAKE) -C $(LIBBTN) all
	$(MAKE) -C $(LIBEPD37) all
	$(MAKE) -C $(LIBFT6336) all
	@for dir in `ls $(EXAMPLES_DIR)`; do \
		$(MAKE) -C $(EXAMPLES_DIR)/$$dir all; \
	done

clean:
	$(MAKE) -C $(LIBBTN) clean
	$(MAKE) -C $(LIBEPD37) clean
	$(MAKE) -C $(LIBFT6336) clean
	@for dir in `ls $(EXAMPLES_DIR)`; do \
		$(MAKE) -C $(EXAMPLES_DIR)/$$dir clean; \
	done

dirclean:
distclean:
	rm build/* -rf
