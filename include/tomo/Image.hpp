#pragma once

#include "tomo/Vector.hpp"
#include "tomo/misc.hpp"
#include "tomo/Color.hpp"
#include <vector>

namespace tomo
{
  struct Image
  {
    typedef tomo::Color<3,float> Color;
    Image(unsigned _width = 0, unsigned _height = 0)
      : width_(_width), height_(_height)
    {
      resize(_width,_height);
    }


    Image(const std::string& filename)
    {
      load_ppm(filename);
    }

    void load_ppm(const std::string& filename);
    void save_ppm(const std::string& filename);

    void resize(unsigned _width, unsigned _height)
    {
      data.resize(_width*_height);
      for (size_t i = 0; i < data.size(); i++)
        data[i] = Color();
    }

    inline Color& operator() (int x, int y)
    {
      return data[x + y*width_];
    }

    inline const Color& operator() (int x, int y) const
    {
      return data[x + y*width_];
    }

    inline const Color& get(int x, int y) 
    {
      return data[x + y*width_];
    }

    inline void set(int x, int y, Color color)
    {
      data[x + y*width_] = color; 
    }

    inline bool valid() const
    {
      return !data.empty();
    }

    std::vector<Color> data;
    
    TBD_PROPERTY(unsigned,width);
    TBD_PROPERTY(unsigned,height);
    
  };
}

