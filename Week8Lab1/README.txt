Reece Gerhart / Josh Nelson

Necessary programs/software:
Graphical program : sudo apt-get install gedit
Open GL  :  sudo apt-get install libglew-dev libglfw3-dev libglm-dev mesa-common-dev libglu1-mesa-dev

Download basic.cpp as well as shader files to your device (Shader.H, basic.vs, basic.frag).
run ls to ensure the files are accessible by the terminal (cd as needed)

RUN:  g++ basic.cpp -o basic -lGLEW -lglfw -lGL -lGLU -lSOIL
is the command to compile the basic.cpp file 

NOTE: The above command works for most machines

RUN: ./basic to run the file on your device

Camera Movement:

* W/S to move Up/Down
* A/D to move Left/Right
* Q/E to move foward/back
* Arrows to move Camera Angle Up/Down/Left/Right
* Ctrl + C to reset Camera Position
