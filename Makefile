linker: main.cpp
	g++ -g -std=c++11 Helper.cpp Tokeniser.cpp main.cpp -o linker

clean:
	rm -f linker *~  
