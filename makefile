#esta regla compila el programa principal:
#Makefile sigue la sitaxis:
# programa: dependencias
# 	instrucciones

main.o: main.c
	gcc -o main main.c
	./main

clean: 
	rm -f *.o
	rm -f main