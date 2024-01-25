STATION_PATH = ../station

FONT = font.psf
SONG = song.wav

HEADER = still-alive.h
SOURCE = still-alive.c

OUT_APPLICATION = still-alive

CFLAGS = -march=native -pipe -std=c17 -Wall -Wextra -Wpedantic -pedantic-errors \
		 -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition \
		 $(shell pkg-config --with-path "${STATION_PATH}" --cflags station-app) -O2 # -g3 -ggdb
LFLAGS = $(shell pkg-config --with-path "${STATION_PATH}" --libs station-app)

OBJCOPY = objcopy


${OUT_APPLICATION}: ${SOURCE}.o ${FONT}.o ${SONG}.o
	${CC} -o ${OUT_APPLICATION} ${SOURCE}.o ${FONT}.o ${SONG}.o ${CFLAGS} ${LFLAGS} -Wl,-znoexecstack

${SOURCE}.o: ${HEADER} ${SOURCE}
	${CC} -c -o ${SOURCE}.o ${SOURCE} ${CFLAGS}

${FONT}.o:
	${OBJCOPY} -I binary -O elf64-x86-64 -B i386:x86-64 --rename-section ".data"=".rodata" ${FONT} ${FONT}.o

${SONG}.o:
	${OBJCOPY} -I binary -O elf64-x86-64 -B i386:x86-64 --rename-section ".data"=".rodata" ${SONG} ${SONG}.o

clean:
	rm ./*.o "./${OUT_APPLICATION}"

.phony: clean

