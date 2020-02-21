SOURCES = sierpinski.c

PROGRAM = sierpinski

CC65_TARGET = c64

# provided by the VICE emulator
X64   ?= x64sc
C1541 ?= c1541

CC      = cl65
CFLAGS  = -t $(CC65_TARGET) --create-dep $(<:.c=.d) -Ors --codesize 500 -T -g
LDFLAGS = -t $(CC65_TARGET) -Ln labels -m $(PROGRAM).map

ifneq ($(shell echo),)
  CMD_EXE = 1
endif

ifdef CMD_EXE
  NULLDEV = nul:
  DEL = -del /f
else
  NULLDEV = /dev/null
  DEL = $(RM)
endif

########################################

.SUFFIXES:
.PHONY: all mostlyclean clean disk run

all: compile disk

compile: $(PROGRAM)

ifneq ($(MAKECMDGOALS),clean)
-include $(SOURCES:.c=.d)
endif

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(PROGRAM): $(SOURCES:.c=.o)
	$(CC) $(LDFLAGS) -o $@ $^

mostlyclean:
	$(DEL) $(SOURCES:.c=.o) $(SOURCES:.c=.d) $(PROGRAM) $(PROGRAM).map

clean: mostlyclean
	$(DEL) $(PROGRAM).d64

disk: compile
	@$(C1541) -format $(PROGRAM),AA d64 $(PROGRAM).d64 >$(NULLDEV)
	$(C1541) -attach $(PROGRAM).d64 -write $(PROGRAM) >$(NULLDEV)

run: compile disk
	@$(X64) $(PROGRAM).d64

debug: compile disk
	@$(X64) -moncommands labels -8 $(PROGRAM).d64
