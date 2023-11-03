
CC := gcc
AR := ar
CFLAGS :=  -c -w -g -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -O1 -DARDUINO=158 -D_86DUINO=210

INCLUDE := -I./

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %cpp, %o, $(SRCS))
MMDS := $(patsubst %o, %d, $(OBJS))
TARGETS := libio.a

.PHONY : everything clean all

everything: $(TARGETS)

clean:
	rm $(OBJS) $(MMDS) $(TARGETS)

all: clean everything

$(TARGETS): $(OBJS)
	$(AR) rcs $(TARGETS) $?

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@ $(INCLUDE)

%.d: %.cpp
	set -e; rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
    sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
    rm -f $@.$$$$

include $(MMDS)
