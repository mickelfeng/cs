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


sinclude $(src:.c=.d)

%.d: %.c
	@set -e; \
    rm -f $@; \
    $(CC) -MM $(CFLAGS) $< > $@.$$$$; \
    sed 's,\($*\)\.o[:]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
    rm -f $@.$$$$


%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)


clean:
	-rm $(src_d)/*.o $(app)
