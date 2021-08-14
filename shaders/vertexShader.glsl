attribute vec2 position;   
uniform mat4 model;
uniform mat4 projection;

void main(){
    gl_Position =  model* vec4(position,0.0,1.0);  
}