
CC := gcc
AR := ar
CFLAGS := -c -w -g -MMD -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -O1 -DARDUINO=158 -D_86DUINO=210

INCLUDE := ./

SRCS := $(wildcard *.cpp)
INIT_OBJS := 86Duino_Init.o
MAIN_OBJS := main.o
OBJS := $(filter-out $(INIT_OBJS) $(MAIN_OBJS), $(patsubst %cpp, %o, $(SRCS)))
TARGETS := libcore.a

.PHONY : everything clean all

everything: $(TARGETS) $(INIT_OBJS) $(MAIN_OBJS)

clean:
	rm $(OBJS) $(INIT_OBJS) $(MAIN_OBJS) $(TARGETS)

all: clean everything

$(TARGETS): $(OBJS)
	$(AR) rcs $(TARGETS) $?

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@ -I $(INCLUDE)

-include $(OBJS:.o=.d)

