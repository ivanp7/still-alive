TARGET_NAME = still-alive

IDIR = .
ODIR = .
LDIR = .

CC     = gcc
CFLAGS = -I$(IDIR) -Wall `pkg-config --cflags ncursesw`
LIBS   = `pkg-config --libs ncursesw` -ldl

_DEPS = io.h still-alive.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o still-alive.o io.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean clean-all
clean:
	rm -f $(ODIR)/*.o

clean-all: clean
	rm -f $(TARGET_NAME)

