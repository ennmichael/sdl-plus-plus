#include "SDL2/SDL.h"
#include "resource.h"
#include "drawing.h"
#include "flow.h"
#include <iostream>

int main() 
{
    using namespace Sdl;
    
    try
    {
        Control _ { }; // Control the SDL subsystems
        
        Screen_properties props { }; // What our window will look like
        
        props.title = "Clock";
        props.width = 300;
        props.height = 300; // Set up...
        
        Screen scrn { props };
        Angle_rad angle { };
        
        Main_loop ml 
        {
            [&scrn, &angle]()
            {
                scrn.add_draw("pixel.bmp", { 0, 0 });
                scrn.add_draw("pixel.bmp", { 1, 0 });
                scrn.add_draw("pixel.bmp", { 0, 1 });
                scrn.add_draw("pixel.bmp", { 1, 1 });
                
                Line l { { 50, 50 }, angle, 20 };
                SDL_Color c { 255, 0, 0, 255 };
                
                scrn.add_draw(l, c); // Draw the line l with the color c
                
                scrn.redraw({ 0, 0, 255, 255 });
                
                angle.value += 0.005;
            }
        };
        
        ml.start();
    }
    catch (Sdl_exception& e)
    {
        std::cout << "Sdl exception: ";
        std::cout << e.what() << '\n';
    }
    catch (...)
    {
        throw; // Tell me what else could've gone wrong
    }
}
