CC = gcc
CFLAGS = -pthread -g
LIBS = -lm
NOMPIC = main.c show.c argument.c error.c finalize.c hasegawa.c history.c initial_path.c initialize.c mallocer.c mannerism.c math.c parameter.c pole.c read_data.c search.c tabu.c terminate.c timer.c tozaki.c visualize.c
NOMPIO = main.o show.o argument.o error.o finalize.o hasegawa.o history.o initial_path.o initialize.o mallocer.o mannerism.o math.o parameter.o pole.o read_data.o search.o tabu.o terminate.o timer.o tozaki.o visualize.o
MPIC = $(NOMPIC) parallel.c
MPIO = $(NOMPIO) parallel.o

a.out: $(NOMPIO)

clean:
	rm *.o

mpi:
	echo "#define MPIMODE" > header.buf
	cat header.h | grep -v "define MPIMODE" >> header.buf
	mv header.buf header.h
	mpicc -c $(MPIC)
	mpicc -o a.out $(MPIO) $(CFLAGS) $(LIBS)
	echo "/* #define MPIMODE */" > header.buf
	cat header.h | grep -v "define MPIMODE" >> header.buf
	mv header.buf header.h
