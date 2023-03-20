CC=gcc

LIBDIR=./lib/
MAINDIR=./
CDIR=$(LIBDIR) $(MAINDIR)
ODIR=./bin/
DEPDIR=./depfile/
FINAL=main

# -MT $@ -MMD -MP -MF $(DEPDIR)$*.d
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)$*.d 

CFILES=$(foreach D,$(CDIR),$(wildcard $(D)*.c))
OBJECTS=$(foreach D,$(CFILES),$(patsubst %.c,$(ODIR)%.o,$(shell basename $(D))))



all: $(FINAL)

$(FINAL) : $(OBJECTS)
	$(CC) -o $@ $^

$(ODIR)%.o :: $(MAINDIR)%.c 
	$(CC) $(DEPFLAGS) -c -o $@ $<
	
$(ODIR)%.o :: $(LIBDIR)%.c 
	$(CC) $(DEPFLAGS) -c -o $@ $<
	

DEPFILES=$(foreach D,$(DEPDIR),$(wildcard $(D)*.d))

-include $(DEPFILES)

clean:
	rm -f  $(OBJECTS) $(DEPFILES)

