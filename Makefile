linker: main.cpp
	g++ -g -std=c++11 Helper.cpp main.cpp -o linker

clean:
	rm -f linker *~  
