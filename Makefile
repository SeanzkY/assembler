main: main.o tokenizer.o symbolGenerator.o preprocess.o exception.o fileRead.o fileWrite.o firstPass.o binaryGenerator.o
	gcc -Wall -ansi -pedantic -g -o main main.o tokenizer.o symbolGenerator.o preprocess.o exception.o fileRead.o fileWrite.o firstPass.o binaryGenerator.o

main.o: main.c tokenization/tokenizer.h firstPass/firstPass.h
	gcc -Wall -ansi -pedantic -g -c main.c

tokenizer.o: tokenization/tokenizer.c tokenization/tokenizer.h tokenization/symbolGenerator.c tokenization/symbolGenerator.h
	gcc -Wall -ansi -pedantic -g -c tokenization/tokenizer.c

symbolGenerator.o: tokenization/symbolGenerator.c tokenization/symbolGenerator.h
	gcc -Wall -ansi -pedantic -g -c tokenization/symbolGenerator.c

preprocess.o: macros/preprocess.c macros/preprocess.h fileHandle/fileRead.c fileHandle/fileRead.h fileHandle/fileWrite.c fileHandle/fileWrite.h
	gcc -Wall -ansi -pedantic -g -c macros/preprocess.c

fileRead.o: fileHandle/fileRead.c fileHandle/fileRead.h exceptions/exception.c exceptions/exception.h
	gcc -Wall -ansi -pedantic -g -c fileHandle/fileRead.c

fileWrite.o: fileHandle/fileWrite.c fileHandle/fileWrite.h
	gcc -Wall -ansi -pedantic -g -c fileHandle/fileWrite.c

exception.o: exceptions/exception.c exceptions/exception.h
	gcc -Wall -ansi -pedantic -g -c exceptions/exception.c

firstPass.o: firstPass/firstPass.c firstPass/firstPass.h fileHandle/fileRead.h tokenization/symbolGenerator.h CodeGeneration/binaryGenerator.h tokenization/tokenizer.h CodeGeneration/binaryGenerator.h
	gcc -Wall -ansi -pedantic -g -c firstPass/firstPass.c

binaryGenerator.o: CodeGeneration/binaryGenerator.c CodeGeneration/binaryGenerator.h
	gcc -Wall -ansi -pedantic -g -c CodeGeneration/binaryGenerator.c


clean:
	rm -f main main.o tokenizer.o *.ob *.am *.ent *.ext




	
