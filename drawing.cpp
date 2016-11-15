#include "drawing.h"
#include <cmath>
#include <iostream>

namespace Sdl
{ // TODO think a bit more about your exception throwing.
    Canvas::Canvas(const Screen_properties& properties)
        : window 
          {
              SDL_CreateWindow(
                  properties.title.c_str(),
                  properties.position.x,
                  properties.position.y,
                  properties.width,
                  properties.height,
                  SDL_WINDOW_SHOWN)
          }
        , renderer { SDL_CreateRenderer(window.get(), -1 , 0) }
    {
        check_pointer(window);
        check_pointer(renderer);
    }
    
    Line::Line(Point new_from, Angle_rad rads, int length) noexcept
        : from { new_from }
        ,to
         {
             static_cast<int>(std::sin(rads.value) * length + from.x),
             static_cast<int>(std::cos(rads.value) * length + from.y)
         } // Points are integers in the SDL library, so narrowing
    {      // has to happen
    }
    
    Line::Line(Point new_from, Point new_to) noexcept
        : from { new_from }
        , to { new_to }
    {
    }
    
    void Texture_factory::add(const std::string& name, 
                              Unique_renderer& renderer)
    {
        Drawable_texture drawable = {
            load_texture(name.c_str(), renderer)
        };
        
        SDL_QueryTexture(drawable.texture.get(), nullptr, nullptr,
                         &drawable.width, &drawable.height);
        
        m_map[name] = std::move(drawable);
    }
    
    bool Texture_factory::exists(const std::string& name) const noexcept
    {
        return m_map.count(name) != 0;
    }
    
    Drawable_texture
    Texture_factory::get(const std::string& name) const noexcept
    {
        return m_map.at(name);
    }
    
    Screen::Screen(const Screen_properties& properties)
        : m_canvas { properties }
    {
    }
    
    void Screen::add_draw(const std::string& name, Point where)
    {
        if (!m_factory.exists(name))
            m_factory.add(name, m_canvas.renderer);
        
        m_textures.push_back({ m_factory.get(name), where });
    }
    
    void Screen::add_draw(Line line, SDL_Color color)
    {
        m_lines.push_back({ line, color });
    }
    
    void Screen::redraw(SDL_Color color)
    {
        set_renderer_color(color);
        check_function(
            SDL_RenderClear(m_canvas.renderer.get())
        );
        
        redraw_textures();
        redraw_lines();
        
        SDL_RenderPresent(m_canvas.renderer.get());
    }
    
    void Screen::set_renderer_color(SDL_Color color)
    {
        check_function(
            SDL_SetRenderDrawColor(m_canvas.renderer.get(),
                                   color.r, color.g, color.b, color.a)
        );
    }
    
    void Screen::redraw_textures()
    {
        SDL_Rect src { };
        SDL_Rect dst { };
        
        src.x = 0;
        src.y = 0;
        
        for (auto& to_draw : m_textures)
        {
            src.w = dst.w = to_draw.drawable.width;
            src.h = dst.h = to_draw.drawable.height;
            
            dst.x = to_draw.point.x;
            dst.y = to_draw.point.y;
            
            check_function(
                SDL_RenderCopy(m_canvas.renderer.get(), 
                               to_draw.drawable.texture.get(),
                               &src, &dst)
            );
        }
    
        m_textures.clear();
    }
    
    void Screen::redraw_lines()
    {
        for (const auto& to_draw : m_lines)
        {
            set_renderer_color(to_draw.color);
            
            check_function(
                SDL_RenderDrawLine(m_canvas.renderer.get(),
                                   to_draw.line.from.x, to_draw.line.from.y, 
                                   to_draw.line.to.x, to_draw.line.to.y)
            );
        }
        
        m_lines.clear();
    }
}
