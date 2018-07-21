CC				= gcc
CFLAGS			:= -g -O1 -std=c99 -Wall -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26 -Wno-int-conversion
LDLIBS			:= -losxfuse
OUTPUT_OPTION	= -MMD -MP -o $@

BIN 			:= p4fs
SRC				:= main.c getattr.c open.c read.c readdir.c p4.c util.c
OBJ				:= $(SRC:.c=.o)
DEP				:= $(SRC:.c=.d)

all: $(BIN)
$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(DEP) $(BIN)

-include $(DEP)
