app = cs
src_d = src
inc_d = include
src = $(wildcard $(src_d)/*.c)
obj = $(patsubst %.c, %.o, $(src))

CC = gcc
DFLAGS = -Wall -g -DDEBUG
CFLAGS = -I$(inc_d) $(DFLAGS)


$(app): $(obj)
	$(CC) $^ -o $@

# FIXME
#cs_server.o: $(inc_d)/*.h

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)


clean:
	-rm $(src_d)/*.o $(app)
