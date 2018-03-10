IDIR = .
ODIR = .
LDIR = .

CC     = gcc
CFLAGS = -I$(IDIR)
LIBS   = -static -lncursesw

_DEPS = io.h still-alive.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o still-alive.o io.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

TARGET_NAME = still-alive

$(TARGET_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o

.PHONY: clean-all
clean-all:
	rm -f $(ODIR)/*.o $(TARGET_NAME)

