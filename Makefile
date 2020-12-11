all: tempo tarefa1 tarefa2 tarefa3 tarefa4

tempo: tempo.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic tempo.o smpl.o rand.o -lm

tarefa1: tarefa1.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic tarefa1.o smpl.o rand.o -lm

tarefa2: tarefa2.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic tarefa2.o smpl.o rand.o -lm

tarefa3: tarefa3.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic tarefa3.o smpl.o rand.o -lm

tarefa4: tarefa4.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic tarefa4.o smpl.o rand.o -lm

smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

tempo.o: tempo.c smpl.h
	$(COMPILE.c) -g  tempo.c

rand.o: rand.c
	$(COMPILE.c) -g rand.c

clean:
	$(RM) *.o tempo relat saida tarefa1 tarefa2 tarefa3 tarefa4

