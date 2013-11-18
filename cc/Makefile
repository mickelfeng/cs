app = cc
src_d = src
inc_d = include
src = $(wildcard $(src_d)/*.c)
obj = $(patsubst %.c, %.o, $(src))

CC = gcc
DFLAGS = -Wall -g -DDEBUG
CFLAGS = -I$(inc_d) $(DFLAGS)


$(app): $(obj)
	$(CC) $^ -o $@


ifneq ($(MAKECMDGOALS), clean)
sinclude $(src:.c=.d)

%.d: %.c
	@set -e; \
	rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[:]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
endif


%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)


.PHONY: clean
clean:
	-rm $(src_d)/*.o $(src_d)/*.d $(app)
