linker: main.cpp
	module load gcc-6.3.0; g++ -g -std=c++11 Helper.cpp Tokeniser.cpp main.cpp -o linker;
clean:
	rm -f linker *~  
