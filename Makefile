CC = gcc
CFLAGS = -pthread -g
DAMYNOMPI = main.o show.o argument.o error.o finalize.o hasegawa.o history.o initial_path.o initialize.o mallocer.o mannerism.o math.o parameter.o pole.o read_data.o search.o tabu.o terminate.o timer.o damy_tozaki.o visualize.o

a.out: $(DAMYNOMPI)

clean:
	rm -f $(DAMYNOMPI) *.o
