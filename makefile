snake:
	clang++ main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o snake.o

format:
	clang-format -i main.cpp
