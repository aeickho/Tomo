#pragma once

#include "tomo/geometry/algorithm/PolygonOffset.hpp"

namespace tomo
{
  namespace slicing
  {
    /// concept: Procedure 
    template<class STATE>
    class Perimeter 
    {
    public:
      typedef STATE State;
      typedef geometry::prim::Polygon Polygon;
      typedef geometry::prim::MultiPolygon MultiPolygon;

      Perimeter() : number_(3), nozzleWidth_(0.02) {}

      MultiPolygon operator()( const MultiPolygon& _in, 
                               MultiPolygon& _out)
      {
        MultiPolygon _output;
        using geometry::algorithm::PolygonOffset;

        TOMO_ASSERT(nozzleWidth_ > 0);
        TOMO_ASSERT(number_ > 0);

        for (int i = 1; i <= number_; i++)
        {
          for (const Polygon& _polygon : _in)
          {
            PolygonOffset()(_polygon,nozzleWidth_*0.5-nozzleWidth_*i,_output);
          }
        }

        for (const Polygon& _polygon : _in)
        {
          PolygonOffset()(_polygon,-nozzleWidth_*number_,_out);
        }

        _output.update();
        return _output;
      }

      TBD_PROPERTY(int,number)
      TBD_PROPERTY(float,nozzleWidth)
    };
  }
}


