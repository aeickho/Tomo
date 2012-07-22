#include "tomo/Slice.hpp"

namespace tomo 
{
  Slice::Slice(float _posZ, const Bounds3f& _bounds)
  {
        posZ_ = _posZ;
        anchor_(_bounds.min().x(),_bounds.min().y()); 
        size_(_bounds.size().x(),_bounds.size().y());     
  }

  void Slice::addSegment(const Point3f& _p0, const Point3f& _p1, const Vec3f& _normal)
  {
    LineSegment _seg(Point2f((_p0.x() - anchor_.x()) / size_.x(),
                             (_p0.y() - anchor_.y()) / size_.y()),
                      Point2f((_p1.x() - anchor_.x()) / size_.x(),
                           (_p1.y() - anchor_.y()) / size_.y()),
                      Vec2f(_normal.x(),_normal.y()));
    lineSegments_.push_back(_seg);    
  }

  Slices::Slices(unsigned _nSlices, Bounds3f _bounds)
  {
    make(_nSlices,_bounds);
  }

  void Slices::make(unsigned _nSlices, Bounds3f _bounds)
  {
    slices_.clear();
    float _sliceHeight = _bounds.size().z() / _nSlices;
    float _slicePos = _bounds.min().z();

    for (unsigned i = 0; i <= _nSlices; i++)
    {
      Slice _slice(_slicePos,_bounds);
      _slicePos += _sliceHeight;
      slices_.insert(_slice);
    }
  } 


  Slices::iterator Slices::get(float _posZ)
  {
    Slice _slice(_posZ); 
    return slices_.lower_bound(_slice);
  }

  Slices::const_iterator Slices::get(float _posZ) const
  {
    Slice _slice(_posZ); 
    return slices_.lower_bound(_slice);
  }

  Slices::iterator Slices::end() { return slices_.end(); }
  Slices::iterator Slices::begin() { return slices_.begin(); }

  Slices::const_iterator Slices::end() const { return slices_.end(); }
  Slices::const_iterator Slices::begin() const { return slices_.begin(); }

  std::vector<const Slice*> Slices::get() const
  {
    std::vector<const Slice*> _result;
    BOOST_FOREACH ( const Slice& _slice, slices_ )
      _result.push_back(&_slice);
    return _result;
  }
}
