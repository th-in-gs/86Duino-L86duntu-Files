CC := gcc
CFLAGS := -c -w -g -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -O1 -DARDUINO=158 -D_86DUINO=210
LINK_FLAGS := -w -Wl,--gc-sections -O1
86DUINO_DIR := /home/dmp/86Duino_Linux_SDK

LIB_PATH := $(86DUINO_DIR)/cores
LIB_INCLUDE := -I$(86DUINO_DIR)/cores
LIB := libcore.a
INIT_OBJS := 86Duino_Init.o

THIRD_LIB_NAME := 
THIRD_LIB_INCLUDE := $(addprefix -I$(86DUINO_DIR)/libraries/,$(THIRD_LIB_NAME))
THIRD_LIB_PATH := $(addprefix -L$(86DUINO_DIR)/libraries/,$(THIRD_LIB_NAME))
THIRD_LIB := $(addprefix -l,$(THIRD_LIB_NAME))
EXTEN_LIB :=

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %cpp,%o,$(SRCS))
TARGETS := 86adc

.PHONY : everything clean all $(LIB)

everything: $(LIB) $(TARGETS)

clean:
	rm $(OBJS) $(TARGETS)

all: clean everything

$(LIB):
	make -C $(LIB_PATH)

$(TARGETS): $(OBJS) $(LIB_PATH)/$(LIB) $(LIB_PATH)/$(INIT_OBJS)
	$(CC) $(LINK_FLAGS) -Wl,--start-group $^ $(THIRD_LIB) $(EXTEN_LIB) -Wl,--end-group -o $@ $(THIRD_LIB_PATH) -lstdc++ -pthread -lrt

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@ $(LIB_INCLUDE) $(THIRD_LIB_INCLUDE)
