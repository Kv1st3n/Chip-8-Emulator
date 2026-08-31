#include "renderer.h"
#include "chip8.h"
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

    if (argc != 2) 
    {
        cout << "Invalid input! To use the emulator do: chip8 rom-file-name" << endl;
        return 1;
    }

    int width = 640, height = 320;
    int texture_width = 64, texture_height = 32;
    Renderer Renderer(width, height, texture_width, texture_height);


    Renderer.init(argv[1]);

}