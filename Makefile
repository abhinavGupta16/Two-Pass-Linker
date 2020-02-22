linker_abhinav_gupta: main.cpp
	g++ -g -std=c++11 Helper.cpp Tokeniser.cpp main.cpp -o linker_abhinav_gupta; module load gcc-6.3.0;
clean:
	rm -f linker_abhinav_gupta *~
