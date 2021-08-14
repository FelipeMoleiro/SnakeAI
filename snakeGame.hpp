#include <deque>

#include <GL/glew.h>
#include <GL/gl.h>  
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 
#include "Shader.hpp"

#define NUM_GRID_HORIZONTAL 20
#define NUM_GRID_VERTICAL 20

class snakeGame
{ 
private:
    std::deque<std::pair<int,int>> snake;
    int curDir;
    int foodX,foodY;
    int size;
    int energy;
    int lost;
    int board[NUM_GRID_HORIZONTAL][NUM_GRID_VERTICAL];
    void generate_food();
    bool collide(int x,int y);
  
    double score;
public:
    bool diedBy; //0 wall,1 energy, 2 snake
    snakeGame(/* args */);
    void moveSnake(int dir);
    int *get_board();
    int get_lost();
    double get_score();
    void get_params(float* params);
    int get_curDir();
    ~snakeGame();
    void drawFrame(GLFWwindow* window);
};

