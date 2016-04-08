PROGRAM = egl.exe
SRCS = main.c egl.c
PACKAGES = x11-xcb xcb egl glesv2

###########################################################################

objs = $(SRCS:%.c=%.o)
pkg-cflags = $(shell pkg-config --cflags $(PACKAGES))
pkg-libs = $(shell pkg-config --libs $(PACKAGES))

.PHONY : all

all : $(PROGRAM)

$(PROGRAM) : $(objs)
	$(CC) -flto -o $@ $^ $(pkg-libs)

$(objs) : %.o : %.c
	$(CC) -O2 -flto --std=c11 -D_POSIX_C_SOURCE=200112L -c -o $@ $< $(pkg-cflags)
