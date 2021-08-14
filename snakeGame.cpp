#include "snakeGame.hpp"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


bool snakeGame::collide(int x,int y){
    if(x >= NUM_GRID_VERTICAL || x < 0 || y >= NUM_GRID_HORIZONTAL || y < 0){
        diedBy = 0;
        return true;
    }for(unsigned int i=0;i<snake.size();i++){
        if(x == snake[i].first && y == snake[i].second){
            diedBy = 2;
            return true;
        }
    }
    return false;
}

void snakeGame::generate_food(){
    int x,y;
    do{
        x = rand()%NUM_GRID_VERTICAL;
        y = rand()%NUM_GRID_HORIZONTAL;
    }while(collide(x,y)); //ARRUMAR
    foodX = x;
    foodY = y;
}

double dist_euclid(double ax,double ay,double bx,double by){
    return  fabs(ax-bx) + fabs(ay-by);
}

void snakeGame::moveSnake(int dir){
    int oldPosX = snake[0].first,oldPosY=snake[0].second;
    int newPosx = snake[0].first,newPosy=snake[0].second;
    if(dir == 0 && curDir != 2){
        newPosx = (newPosx - 1);
        curDir = dir;
    }else if(dir == 1 && curDir != 3){
        newPosy = (newPosy+1);
        curDir = dir;
    }else if(dir == 2 && curDir != 0){
        newPosx = (newPosx +1);
        curDir = dir;
    }else if(dir == 3 && curDir != 1){
        newPosy = (newPosy- 1);
        curDir = dir;
    }else{
        if(curDir == 0 ){
            newPosx = (newPosx - 1);
        }else if(curDir == 1){
            newPosy = (newPosy+1);
        }else if(curDir == 2){
            newPosx = (newPosx +1);
        }else if(curDir == 3 ){
            newPosy = (newPosy- 1);
        }
    }
    if(collide(newPosx,newPosy)){
        lost = 1;
    }else{
        if(dist_euclid(oldPosX,oldPosY,foodX,foodY) > dist_euclid(newPosx,newPosy,foodX,foodY)){
            score += 1;
        }else{
            score -= 1.5;
        }

        if(newPosx == foodX && newPosy == foodY){
            size++;
            snake.push_front(std::make_pair(newPosx,newPosy));
            generate_food();
            energy = NUM_GRID_HORIZONTAL*NUM_GRID_VERTICAL;
        }else{
            snake.push_front(std::make_pair(newPosx,newPosy));
            snake.pop_back();
            energy--;
            if(energy == 0){
                diedBy = 1;
                lost = 1;
            }
        }
    }

    
}



bool initializedShader = false;


// variaveis globais
typedef struct{
    float x, y;
} coordenadas;

GLuint loc_mat,loc_color;

void initializeShader(){
    Shader programShader("shaders/vertexShader.glsl","shaders/fragmentShader.glsl");
        // Preparando dados para enviar a GPU
    coordenadas vertices[8] = {
            {0,0},
            {0,2.0/NUM_GRID_VERTICAL},
            {2.0/NUM_GRID_HORIZONTAL,0},
            {2.0/NUM_GRID_HORIZONTAL,2.0/NUM_GRID_VERTICAL}
    };

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Abaixo, nós enviamos todo o conteúdo da variável vertices.
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
    GLint loc = glGetAttribLocation(programShader.getID(), "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

    // Associando variávels do programa GLSL (Fragment Shader) para definir cores
    loc_color = glGetUniformLocation(programShader.getID(), "color");

    loc_mat = glGetUniformLocation(programShader.getID(), "model");
    // Associando nossa janela com eventos de teclado

}

snakeGame::snakeGame()
{
    if(!initializedShader){
        initializeShader();
        initializedShader = true;
    }
    curDir = 0;
    snake.push_back(std::make_pair(NUM_GRID_HORIZONTAL/2.0,NUM_GRID_VERTICAL/2.0));
    generate_food();
    size = 1;
    energy=NUM_GRID_HORIZONTAL*NUM_GRID_VERTICAL;
    lost = 0;
    diedBy = -1;
}

snakeGame::~snakeGame()
{
}

int *snakeGame::get_board(){
    for(int i=0;i<NUM_GRID_HORIZONTAL;i++){
        for(int j=0;j<NUM_GRID_VERTICAL;j++){ 
            board[i][j] = 0;
        }
    }
    for(unsigned int i=0;i<snake.size();i++){
        board[snake[i].first][snake[i].second] = 1;
    }
    board[foodX][foodY] = 2;
    return &board[0][0];
}

int snakeGame::get_lost(){
    return lost;
}

double snakeGame::get_score(){
    return size;
    //return (size * NUM_GRID_HORIZONTAL*NUM_GRID_VERTICAL ) + (energy - NUM_GRID_HORIZONTAL*NUM_GRID_VERTICAL)/10.0;
}




void lookAt(int inix,int iniy,int x,int y,int board[NUM_GRID_HORIZONTAL][NUM_GRID_VERTICAL],int* res){
    res[0] = 0;
    res[1] = 0;
    res[2] = 0;

    bool foodFound = 0;
    bool snakeFound = 0;

    inix += x;
    iniy += y;
    double dist = 1;
    while(inix < NUM_GRID_HORIZONTAL && inix >= 0 && iniy < NUM_GRID_VERTICAL && iniy >= 0){
        if(!snakeFound && board[inix][iniy] == 1){//snake body
            snakeFound = 1;
            res[1] = 1.0/dist;
        }
        if(!foodFound && board[inix][iniy] == 2){//food body
            foodFound = 1;
            res[2] = 1;
        }
        inix += x;
        iniy += y;
        dist++;
        //printf("%d %d\n",inix, iniy);
    }
    res[0] = 1.0/dist;
}

void snakeGame::get_params(float* params)
{
    
    get_board();
    int tmpres[3];
    
    lookAt(snake[0].first,snake[0].second,0,1,board,tmpres);
    params[0+0*8] = tmpres[0];
    params[0+1*8] = tmpres[1];
    params[0+2*8] = tmpres[2];

    lookAt(snake[0].first,snake[0].second,1,1,board,tmpres);
    params[1+0*8] = tmpres[0];
    params[1+1*8] = tmpres[1];
    params[1+2*8] = tmpres[2];

    lookAt(snake[0].first,snake[0].second,1,0,board,tmpres);
    params[2+0*8] = tmpres[0];
    params[2+1*8] = tmpres[1];
    params[2+2*8] = tmpres[2];

    lookAt(snake[0].first,snake[0].second,1,-1,board,tmpres);
    params[3+0*8] = tmpres[0];
    params[3+1*8] = tmpres[1];
    params[3+2*8] = tmpres[2];

    lookAt(snake[0].first,snake[0].second,0,-1,board,tmpres);
    params[4+0*8] = tmpres[0];
    params[4+1*8] = tmpres[1];
    params[4+2*8] = tmpres[2];

    lookAt(snake[0].first,snake[0].second,-1,-1,board,tmpres);
    params[5+0*8] = tmpres[0];
    params[5+1*8] = tmpres[1];
    params[5+2*8] = tmpres[2];

    lookAt(snake[0].first,snake[0].second,-1,0,board,tmpres);
    params[6+0*8] = tmpres[0];
    params[6+1*8] = tmpres[1];
    params[6+2*8] = tmpres[2];

    lookAt(snake[0].first,snake[0].second,-1,1,board,tmpres);
    params[7+0*8] = tmpres[0];
    params[7+1*8] = tmpres[1];
    params[7+2*8] = tmpres[2];

}

int snakeGame::get_curDir(){
    return curDir;
}

void draw_quad(int x,int y,int type){ //0 - snake, 1-food
	if(x >= NUM_GRID_HORIZONTAL || x < 0) return;
	if(y >= NUM_GRID_VERTICAL || y < 0) return;

	float dx =  -1 + (x * 2.0/NUM_GRID_HORIZONTAL);
	float dy =  -1 + (y * 2.0/NUM_GRID_VERTICAL);

    float model[16] = {
        1.0f, 0.0f, 0.0f, dx,
        0.0f, 1.0f, 0.0f, dy,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // enviando a matriz de transformacao para a GPU
    glUniformMatrix4fv(loc_mat, 1, GL_TRUE, model);

    if(type == 0){
        glUniform4f(loc_color,0,1,0,1);
    }else{
        glUniform4f(loc_color,1,0,0,1);
    }
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0,4);
}


void snakeGame::drawFrame(GLFWwindow* window){
    glClearColor(1.0,1.0,1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    for(unsigned int i=0;i<snake.size();i++){
        draw_quad( snake[i].first,snake[i].second,0);
    }
    
    draw_quad(foodX,foodY,1);
    //printf("oi\n");



    glfwSwapBuffers(window);
}