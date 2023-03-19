CC=gcc

CDIR=./ ./lib/
ODIR=./bin/
DEPDIR=./depfile/
FINAL=main

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)$*.d
CFLAGS=$(DEPFLAGS)

CFILES=$(foreach D,$(CDIR),$(wildcard $(D)*.c))
OBJECTS=$(foreach D,$(CFILES),$(patsubst %.c,$(ODIR)%.o,$(shell basename $(D))))

all: $(FINAL)

$(FINAL) : $(OBJECTS)
	$(CC) -o $@ $^

$(ODIR)%.o :: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^

$(ODIR)%.o :: $(CDIR)%.c 
	$(CC) $(CFLAGS) -c -o $@ $^

DEPFILES=$(foreach D,$(DEPDIR),$(wildcard $(D)*.d))

include $(DEPFILES)

clean:
	rm -f  $(OBJECTS) $(DEPFILES)
