linker_abhinav: main.cpp
	g++ -g -std=c++11 Helper.cpp Tokeniser.cpp main.cpp -o linker_abhinav; module load gcc-6.3.0;
clean:
	rm -f linker *~  
