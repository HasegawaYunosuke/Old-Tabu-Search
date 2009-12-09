CC = gcc
CFLAGS = -pthread -g
NOMPI = main.o show.o argument.o error.o finalize.o hasegawa.o history.o initial_path.o initialize.o mallocer.o mannerism.o math.o parameter.o pole.o read_data.o search.o tabu.o terminate.o timer.o tozaki.o visualize.o
MPI = $(NOMPI) parallel.o

a.out: $(NOMPI)

clean:
	rm *.o

mpi:
	mpicc -o $@ $(MPI) $(CFLAGS)
