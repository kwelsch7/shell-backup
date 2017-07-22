msh:	cscd340Lab6.c ./linkedlist/listUtils.o ./linkedlist/linkedList.o ./tokenize/makeArgs.o ./utils/myUtils.o ./utils/shellUtils.o ./process/process.o ./pipes/pipes.o ./alias/alias.o ./history/history.o ./utils/rcAndHistUtils.o
	gcc -g cscd340Lab6.c ./linkedlist/listUtils.o ./linkedlist/linkedList.o ./tokenize/makeArgs.o ./utils/myUtils.o ./utils/shellUtils.o ./process/process.o ./pipes/pipes.o ./alias/alias.o ./history/history.o ./utils/rcAndHistUtils.o -o msh

linkedList.o:	./linkedlist/linkedList.c ./linkedlist/linkedList.h
	gcc -g -c ./linkedlist/linkedList.c

listUtils.o:	./linkedlist/listUtils.c ./linkedlist/listUtils.h
	gcc -g -c ./linkedlist/listUtils.c

makeArgs.o:	./tokenize/makeArgs.c ./tokenize/makeArgs.h
	gcc -g -c ./tokenize/makeArgs.c

myUtils.o:	./utils/myUtils.c ./utils/myUtils.h
	gcc -g -c ./utils/myUtils.c

shellUtils.o:	./utils/shellUtils.c ./utils/shellUtils.h
	gcc -g -c ./utils/shellUtils.c

process.o:	./process/process.c ./process/process.h
	gcc -g -c ./process/process.c	

pipes.o:	./pipes/pipes.c ./pipes/pipes.h
	gcc -g -c ./pipes/pipes.c

alias.o:	./alias/alias.c ./alias/alias.h
	gcc -g -c ./alias/alias.c

history.o:	./history/history.c ./history/history.h
	gcc -g -c ./history/history.c

rcAndHistUtils.o:	./utils/rcAndHistUtils.c ./utils/rcAndHistUtils.h
	gcc -g -c ./utils/rcAndHistUtils.c

clean:
	rm ./utils/rcAndHistUtils.o
	rm ./history/history.o
	rm ./alias/alias.o
	rm ./pipes/pipes.o
	rm ./process/process.o
	rm ./utils/shellUtils.o
	rm ./utils/myUtils.o
	rm ./tokenize/makeArgs.o
	rm ./linkedlist/listUtils.o
	rm ./linkedlist/linkedList.o
	rm msh