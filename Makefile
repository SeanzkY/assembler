main: main.o tokenizer.o 
	gcc -Wall -ansi -pedantic -g -o main main.o tokenizer.o

main.o: main.c tokenization/tokenizer.h
	gcc -Wall -ansi -pedantic -g -c main.c

tokenizer.o: tokenization/tokenizer.c tokenization/tokenizer.h
	gcc -Wall -ansi -pedantic -g -c tokenization/tokenizer.c

clean:
	rm -f main main.o tokenizer.o *.ob *.am *.ent *.ext



	
