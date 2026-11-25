
OBJS = main.o analyzer.o
target = analyzer
cc = gcc
exe : ${OBJS}
	${cc} ${OBJS} -o ${target}
main.o : main.c
	${cc} -c $^
analyzer.o : analyzer.c
	${cc} -c $^
clean:
	@echo "Clearing please wait..."
	-del /Q *.o analyzer.exe