# SnakeAI

Neural network learns to play Snake.

The Neural Net has 24 input parameters, for each of the 8 directions it takes the distance to the wall, distance to the snake body and if there is a food in that direction. The architecture is one dense layer of 32 neurons and 4 output neurons. You can add or remove layers and neurons by editing the main file, just leave the inputs and outputs with it's original numbers.

Here is one evolution of the snakeAI.

![](https://github.com/FelipeMoleiro/SnakeAI/blob/main/AIPlaying.gif)

To run this program you just need g++, glfw and glew. Then run the make command and make run.

The only keybord command the program accepts is the key 'p', that starts and stops the animation of the AI training. It starts turned off by default, so the window that póps up is black until the 'p' key is pressed.

TODO(Ideas of ways to get better results):
- Add variable mutation(eiher by keyboard or by score plateauing) 
- Test different metrics of score
- Test different input informations
  
