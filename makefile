all:
	g++ main.cpp Shader.cpp snakeGame.cpp neuralNetwork.cpp -o main -lglfw -lGL -lGLEW -Wall -std=c++11 
run:
	./main