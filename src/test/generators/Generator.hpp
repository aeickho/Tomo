#pragma once

#include "tomo/draw/magick/Wrapper.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      template<typename OBJECT_TYPE>
      struct Generator
      {
        typedef OBJECT_TYPE Object;
        typedef std::list< Object > ObjectList;
        typedef draw::magick::Wrapper Wrapper;

        Generator(bool _writeImages = true) :
          writeImages_(_writeImages)
        {}

        virtual ObjectList generate() const = 0;
        virtual std::string name() const = 0;
        virtual void draw(Wrapper& _wrapper, const Object& _object) = 0;

        void write(std::string _outputDir)
        {
          ObjectList _objs = generate();

          int i = 0;
          for ( const Object& _obj : _objs )
          {
            std::ofstream _ofs(filenamePrefix(_outputDir,++i)+".dat");
            boost::archive::text_oarchive _oa(_ofs);
            ///@todo Make all tested types serializable
            //_oa << _obj;

            if (writeImages_)
            {
              Wrapper _w(1024,1024);
              _w.scale(tomo::geometry::base::Vec2f(1024,1024));
              draw(_w,_obj);
              _w.image().write(filenamePrefix(_outputDir,i) + ".png");
            }
          }
        }

        TBD_PROPERTY(bool,writeImages);
      private:
        std::string filenamePrefix(std::string _outputDir, int _n)
        {
          std::stringstream ss;
          ss << _outputDir << "/" << name();
          ss << std::setw(3) << std::setfill('0') << _n;
          return ss.str();
        }
      };

#define TOMO_TEST_GENERATOR_DECL(generator_name,object_name) \
    public:\
      std::string name() const { return #generator_name; } \
      typedef object_name Object;\
      typedef std::list< Object > ObjectList;\
      typedef draw::magick::Wrapper Wrapper;\
      generator_name(bool _writeImages = true) : Generator(_writeImages) {}\
      generator_name(bool _writeImages, std::string _outputDir) :\
        Generator(_writeImages) { write(_outputDir); }

    }
  }
}
