#ifndef _SDPP__DRAWING__HEADRER__
#define _SDPP__DRAWING__HEADRER__

#include "SDL2/SDL.h"
#include "resource.h"
#include <string>
#include <vector>
#include <utility>
#include <map>

namespace Sdl
{
    template <class T>
    struct Basic_point;
    struct Canvas;
    struct Line;
    
    using Point = Basic_point<int>;
    using Precise_point = Basic_point<double>;
    // More aliases like these?
    
    template <class T>
    struct Basic_point { // A 2D point, as simple as they make them
        T x { };
        T y { };
    };
    
    struct Screen_properties { // Properties of a window
        int width { 800 };
        int height { 600 };
        Point position { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED };
        std::string title { };
    };
    
    struct Canvas { // Canvas is what you need in order to draw something
      // Should this class even exist?
        Canvas() = default;
        explicit Canvas(const Screen_properties&); 
        // Creates a canvas based on the given window properties
        
        Unique_window window { nullptr };
        Unique_renderer renderer { nullptr };
    };
    
    struct Angle_rad { // Angle in radians
        double value { };
    };
    
    struct Line {
        Line() = default;
        Line(Point, Angle_rad, int) noexcept;
        Line(Point, Point) noexcept;
        
        Point from { };
        Point to { };
    };
    
    struct Drawable_texture {
        Shared_texture texture { nullptr };
        int width { };
        int height { };
    };
    
    class Texture_factory {
    public:
        void add(const std::string&, Unique_renderer&);
        bool exists(const std::string&) const noexcept;
        Drawable_texture get(const std::string&) const noexcept;
        
    private:
        std::map<std::string, Drawable_texture> m_map { };
    };
    
    class Screen { // Manipulates a canvas
    public:
        explicit Screen(const Screen_properties&); 
        
        void add_draw(const std::string&, Point);
        void add_draw(Line, SDL_Color);
        
        void redraw(SDL_Color); // Clear, draw, render present
        
    private:
        void set_renderer_color(SDL_Color);
        
        void redraw_textures();
        void redraw_lines();
        
        struct Texture_to_draw
        {
            Drawable_texture drawable { };
            Point point { };
        };
        
        struct Line_to_draw
        {
            Line line { };
            SDL_Color color { };
        };
        
        Canvas m_canvas { };
        Texture_factory m_factory { };
        
        // Stuff to draw
        std::vector<Texture_to_draw> m_textures { };
        std::vector<Line_to_draw> m_lines { };
    };
}

#endif
