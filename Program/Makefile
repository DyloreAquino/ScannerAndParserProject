
all: scanner.exe parser.exe

token.o: token.c token.h
	gcc -c token.c

scan.o: scan.c scan.h token.h
	gcc -c scan.c

scanit.o: scanit.c scan.h token.h
	gcc -c scanit.c

scanner.exe: scanit.o scan.o token.o
	gcc -o scanner.exe scanit.o scan.o token.o

rdparse.o: rdparse.c scan.h token.h
	gcc -c rdparse.c

parser.exe: rdparse.o scan.o token.o
	gcc -o parser.exe rdparse.o scan.o token.o

clean:
	rm *.exe *.o

test-scanner: scanner.exe
	./scanner.exe Inputs/*.txt
	
test-parser: parser.exe
	./parser.exe Inputs/*.txt

test: test-scanner test-parser