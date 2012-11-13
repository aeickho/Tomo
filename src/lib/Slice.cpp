#include "tomo/slicing/Slice.hpp"

using namespace std;

namespace tomo
{
  namespace slicing
  {
    Slice::Slice(float _pos) : pos_(_pos)
    {
    }

    void Slice::optimize(float _distance)
    {

    }

    void Slice::add(Polygon& _polygon)
    {
      polygons_.push_back(_polygon);
    }

    void Slice::clear()
    {
      polygons_.clear();
    }
  }
}
