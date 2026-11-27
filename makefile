snake: src/*.cpp inc/*.h
	clang++ -std=c++23 -Iinc src/*.cpp -o snake.o -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -O3

format:
	clang-format -i src/*.cpp inc/*.h
