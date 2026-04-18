main: main.o tokenizer.o symbolGenerator.o preprocess.o exception.o fileRead.o
	gcc -Wall -ansi -pedantic -g -o main main.o tokenizer.o symbolGenerator.o preprocess.o exception.o fileRead.o

main.o: main.c tokenization/tokenizer.h
	gcc -Wall -ansi -pedantic -g -c main.c

tokenizer.o: tokenization/tokenizer.c tokenization/tokenizer.h tokenization/symbolGenerator.c tokenization/symbolGenerator.h
	gcc -Wall -ansi -pedantic -g -c tokenization/tokenizer.c

symbolGenerator.o: tokenization/symbolGenerator.c tokenization/symbolGenerator.h
	gcc -Wall -ansi -pedantic -g -c tokenization/symbolGenerator.c

preprocess.o: macros/preprocess.c macros/preprocess.h fileHandle/fileRead.c fileHandle/fileRead.h
	gcc -Wall -ansi -pedantic -g -c macros/preprocess.c

fileRead.o: fileHandle/fileRead.c fileHandle/fileRead.h exceptions/exception.c exceptions/exception.h
	gcc -Wall -ansi -pedantic -g -c fileHandle/fileRead.c

exception.o: exceptions/exception.c exceptions/exception.h
	gcc -Wall -ansi -pedantic -g -c exceptions/exception.c


clean:
	rm -f main main.o tokenizer.o *.ob *.am *.ent *.ext



	
