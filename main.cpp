#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>  
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 

#include "Shader.hpp"
#include "neuralNetwork.hpp"
#include "snakeGame.hpp"

#include<unistd.h>
unsigned int microsecond = 1000000;

bool showEvolution = 0;

const int numInputParams = 24;

#define NUM_INDIVIDUALS 100

std::vector<neuralNetwork> individuals;

std::vector<double> scores;




GLFWwindow *window;

std::vector<double> paramsInput;
std::vector<double> boardInput;
std::vector<double> res;
void avalia(){
    for(uint i=0;i<individuals.size();i++){
        double score = 0;
        for(int l=0;l<5;l++){//run 5 executions of every idividual
            
            int frames = 0;
            snakeGame game;
            while(!game.get_lost()){
                glfwPollEvents();
                /*
                int* board = game.get_board();
                for(int j=0;j<NUM_GRID_HORIZONTAL * NUM_GRID_VERTICAL;j++){
                    boardInput[j] = board[j];
                }
                res = individuals[i].forward(boardInput);
                */
                float params[numInputParams];
                game.get_params(params);

                //for(int j=0;j<numInputParams;j++){
                 //   printf("%lf ", params[j]);
                //}
                //printf("\n");

                for(int j=0;j<numInputParams;j++){
                    paramsInput[j] = params[j];
                }
                res = individuals[i].forward(paramsInput);
                double max = res[0];
                int dir=0;
                for(int j=1;j<4;j++){
                    if(res[j]>max){
                        max = res[j];
                        dir = j;
                    }
                }
                if(showEvolution) game.drawFrame(window);

                frames++;

                game.moveSnake(dir);
            }
            
            
            /*
            if(game.diedBy == 1){
                score += frames/4.0;
            }else{
                score += frames;
            }*/

            score += frames;
            score += 25*game.get_score()*game.get_score();
            
        }
        score /= 5.0;
        scores[i] = score;
    }

}

void show_best(){
    double bestScore = 0;
    for(uint i=0;i<individuals.size();i++){
        if(scores[i] > bestScore){
            bestScore = scores[i];
        }
    }
    printf("best score: %f\n", bestScore);
}


void elitismo(){
    double totalScore = scores[0];
    double bestScore = scores[0];
    int bestInd = 0;
    for(uint i=1;i<individuals.size();i++){
        totalScore += scores[i];
        if(scores[i] > bestScore){
            bestScore = scores[i];
            bestInd = i;
        }
    }
    std::vector<neuralNetwork> newIndividuals;
    newIndividuals.push_back(individuals[bestInd]);

    //mutation 
    for (uint i=1;i<individuals.size();i++){
        int ind1,ind2;
        //select snake 1
        double scoreValue = rand()%(int)totalScore;
        double sum =0;
        for(uint j=0;j<individuals.size();j++){
            sum += scores[j];
            if(sum >= scoreValue){
                ind1 = j;
                break;
            }
        }

        //select snake 2
        scoreValue = rand()%(int)totalScore;
        sum =0;
        for(uint j=0;j<individuals.size();j++){
            sum += scores[j];
            if(sum >= scoreValue){
                ind2 = j;
                break;
            }
        }

        neuralNetwork newInd = individuals[ind1];

        // Crossover
        newInd.mix(individuals[ind2]);

        // Mutacao
        newInd.mutate(0.1,0.01);

        newIndividuals.push_back(newInd);
    }

    individuals = newIndividuals;
}

/*
void elitismo(){
    double bestScore = scores[0];
    int bestInd = 0;
    for(uint i=1;i<individuals.size();i++){
        if(scores[i] > bestScore){
            bestScore = scores[i];
            bestInd = i;
        }
    }

    //mutation 
    for (uint i=0;i<individuals.size();i++){
        if(i == bestInd)
            continue;

        // Crossover
        individuals[i].mix(individuals[bestInd]);

        // Mutacao
        individuals[i].mutate(0.1,0.01);

    }

}*/

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS){
        showEvolution = !showEvolution;
        //std::cout << "showScreen: " << showScreen << std::endl;
    }
        
}


int main(){
    srand(10);

    //inicia ambiente de janelas
    glfwInit();

    glfwWindowHint(GLFW_VISIBLE, false);

    window = glfwCreateWindow(800, 800, "Cenario", NULL, NULL);

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    
    GLint GlewInitResult = glewInit();
    printf("GlewStatus: %s\n", glewGetErrorString(GlewInitResult));

    

    //INITIALIZING EVOLUTIVE ALGORITHM

    //initialize board input
    for(int i=0;i<NUM_GRID_HORIZONTAL * NUM_GRID_VERTICAL;i++){
        boardInput.push_back(0);
    }

    for(int i=0;i<numInputParams;i++){
        paramsInput.push_back(0);
    }

    std::vector<int> arch;
    arch.push_back(numInputParams);
    arch.push_back(32);
    arch.push_back(4);

    for(int i=0;i<NUM_INDIVIDUALS;i++){
        individuals.push_back(neuralNetwork(arch));
        scores.push_back(0.0);
    }
    glfwShowWindow(window);


    



    
    while(1){
        glfwPollEvents();
        avalia();
        show_best();
        elitismo();
    }
 

    return 0;
}
