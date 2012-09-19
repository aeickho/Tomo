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

    Slices::Slices(float _thickness, geometry::aux::Bounds3f _bounds)
    {
      build(_thickness,_bounds);
    }

    void Slices::build(float _thickness, geometry::aux::Bounds3f _bounds)
    {
      planes_.clear();
      float _slicePos = _bounds.min().z() + _thickness / 2.0 ;

      while (_slicePos <= _bounds.max().z())
      {
        Slice _slice(_slicePos);
        planes_.insert(make_pair<float,Slice>(_slicePos,_slice));
        _slicePos += _thickness;
      }
      LOG_MSG << fmt("Made % slices.") % planes_.size();
    }

  }
}
