INCLUDE:=-I/home/kabilan/Downloads/c/kubernetes -I/home/kabilan/Desktop/FYP/implementation/implementation-2/paas-fuzzer/include -I/home/kabilan/Desktop/FYP/implementation/implementation-2/paas-fuzzer/vcpkg/installed/x64-linux/include
LIBS:=-L/usr/local/lib -lyaml -lwebsockets -lkubernetes -L/usr/local/lib
CFLAGS:=-g
BIN:=ClusterAccess

.PHONY : all clean test memcheck
all:
	g++ ClusterAccess.cpp $(CFLAGS) $(INCLUDE) $(LIBS) -o $(BIN)

test:
	./$(BIN)

memcheck:
	valgrind --tool=memcheck --leak-check=full ./$(BIN)

clean:
	rm ./$(BIN)
