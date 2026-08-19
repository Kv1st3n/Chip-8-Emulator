#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>

class Renderer
{
    public:

        // constructor
        Renderer();
        // deconstructor
        ~Renderer();

        void init();
        void handle_sound();
        void update_screen();
        bool input_handler();

    private:
};

#endif