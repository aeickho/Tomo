#include "tomo/Slice.hpp"

namespace tomo 
{
  Slice::Slice(float _posZ, const Bounds3f& _bounds)
  {
        posZ_ = _posZ;
        bounds_(Point2f(_bounds.min().x(),_bounds.min().y()),
                Point2f(_bounds.max().x(),_bounds.max().y()));
  }

  void Slice::addSegment(const Point3f& _p0, const Point3f& _p1, const Vec3f& _normal)
  {
    LineSegment _ls0(Point2us(),Vec2f(_normal.x(),_normal.y()));
    if (!makePoint(_p0,_ls0.v_)) return;
    
    LineSegment _ls1(Point2us(),Vec2f(_normal.x(),_normal.y()));
    if (!makePoint(_p1,_ls1.v_)) return;

    _ls0.next_ = const_cast<LineSegment*>(&(*lineSegments_.insert(_ls1)));    
    _ls1.next_ = const_cast<LineSegment*>(&(*lineSegments_.insert(_ls0)));    
  }

  bool Slice::getSegment(const LineSegment& _lineSeg, Point2f& _p0, Point2f& _p1, Vec3f& _normal ) const
  {
    if (!_lineSeg.next_ || _lineSeg.normal_.sqrLength() == 0.0 ) return false;

    _p0(bounds_.min().x() + bounds_.size().x() / 65535.0 * _lineSeg.v_.x(),
        bounds_.min().y() + bounds_.size().y() / 65535.0 * _lineSeg.v_.y());
    _p1(bounds_.min().x() + bounds_.size().x() / 65535.0 * _lineSeg.next_->v_.x(),
        bounds_.min().y() + bounds_.size().y() / 65535.0 * _lineSeg.next_->v_.y());
    _normal(_lineSeg.normal_.x(),_lineSeg.normal_.y(),0.0);
    return true;
  }
  
  Slice::LineSegments::iterator Slice::get(LineSegment* _lineSeg)
  {
    LineSegments::iterator it = lineSegments_.lower_bound(*_lineSeg);
    /*size_t _count = lineSegments_.count(it);
    if (_count == 1)
    {
      return it;
    }
    else if (_count > 1)
    {
      map<float,LineSegments::iterator> _candidates; // Map from scalar product of normals to corresponding LineSegment
      pair<LineSegment::iterator,LineSegment::iterator> _range = lineSegments_.equal_range(*_lineSeg);
      LineSegment::iterator _it;

      for (_it = _range.first ; _it != _range.second ; ++it )
      {
        if (&(*_it) == _lineSeg) continue;
        _candidates.insert(std::make_pair(dot(_it->normal_,_lineSeg->normal_),_it));
      }

    }*/
    return lineSegments_.end();
  }

  void Slice::connect()
  {
    /*
    LineSegments _lineSeg;

    while (it != lineSegments_.end())
    {

      _lineSeg.insert(
    }

    while (it != lineSegments_.end())
    {
      if (lineSegments_.count(it) > 1)
      {
        it->next = 
        ++it;
        lineSegments_.erase(it++);
      } else
        ++it;
    }*/
  }

  bool Slice::makePoint(const Point3f& _p, Point2us& _v) const 
  {
    //LOG_MSG << fmt("% %") % bounds_.min().x()% bounds_.min().y() % bounds_.max().x() % bounds_.max().y();  
    if (!bounds_.inside(Point2f(_p.x(),_p.y()))) return false;

//    LOG_MSG << fmt("% %") % _v.x() % _v.y();
    
    _v.x(65535*(_p.x() - bounds_.min().x()) / bounds_.size().x());
    _v.y(65535*(_p.y() - bounds_.min().y()) / bounds_.size().y());
    return true;
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
    Slice _slice(_posZ,Bounds3f()); 
    return slices_.lower_bound(_slice);
  }

  Slices::const_iterator Slices::get(float _posZ) const
  {
    Slice _slice(_posZ,Bounds3f()); 
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
