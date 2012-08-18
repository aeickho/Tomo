#include "tomo/Slice.hpp"

using namespace std;

namespace tomo 
{
  /*
     bool LineSegment::intersect(Ray2f _ray, float _tNear, float _tFar, Vec2f _normal)
     {
  // compute slopes, note the cludge for infinity, however, this will
  // be close enough
  float m1,m2,a1,a2,b1,b2,c1,c2,det_inv;

  float x0 = v_.x(), x1 = next_->v_.x();
  float y0 = v_.y(), y1 = next_->v_.y();
  float x2 = _ray.org_.x(), x3 = _ray.org_.x() + _ray.dir_.x();
  float y2 = _ray.org_.y(), y3 = _ray.org_.y() + _ray.dir_.y();

  if ((x1-x0)!=0)
  m1 = (y1-y0)/(x1-x0);
  else
  m1 = (float)1e+10;   // close enough to infinity

  if ((x3-x2)!=0)
  m2 = (y3-y2)/(x3-x2);
  else
  m2 = (float)1e+10;   // close enough to infinity

  // compute constants

  a1 = m1;
  a2 = m2;
  b1 = -1;
  b2 = -1;

  c1 = (y0-m1*x0);
  c2 = (y2-m2*x2);

  // compute the inverse of the determinate
  det_inv = 1/(a1*b2 - a2*b1);

  // use Kramers rule to compute xi and yi
  xi=((b1*c2 - b2*c1)*det_inv);
  yi=((a2*c1 - a1*c2)*det_inv);

*/
  /*
     SplitPlaneIntersect LineSegment::intersect(Axis _axis, float _splitPos, 
     const Bounds2f& _boundsLeft, const Bounds2f& _boundsRight) const;
     {
     if (!next_)
     {
     return SplitPlaneIntersect();
     }

     if (v_[_axis] < _splitPos && next_->v_[_axis] < _splitPos) return SplitPlaneIntersect(true,false); else
     if (v_[_axis] > _splitPos && next_->v_[_axis] > _splitPos) return SplitPlaneIntersect(false,true); else
     {
     Axis _k = (_axis + 1 ) % 2;



     return SplitPlaneIntersect(true,true);
     }
     }

     Bounds2f LineSegment::bounds() const
     {

     }
     */
  Point2f SliceVertex::getCoords(const Bounds2f& _bounds)
  {
    return Point2f(x()/float(Slice::resolution_)*_bounds.size().x()+_bounds.min().x(),
                   y()/float(Slice::resolution_)*_bounds.size().y()+_bounds.min().y());
  }


  Slice::Slice(float _posZ, const Bounds3f& _bounds)
  {
    posZ_ = _posZ;
    bounds_(Point2f(_bounds.min().x(),_bounds.min().y()),
        Point2f(_bounds.max().x(),_bounds.max().y()));
  }

  void Slice::addSegment(const Point3f& _p0, const Point3f& _p1)
  {
    SliceVertex _ps0, _ps1;
    if (!makePoint(_p0,_ps0)) return;
    if (!makePoint(_p1,_ps1)) return;

//    if (_ps0 == _ps1) return;

    graph_.insert(std::make_pair<SliceVertex,SliceVertex>(_ps0,_ps1));
  }

  void Slice::makePolylines()
  {
    while (!graph_.empty())
    {
      Polyline _polyline;

      Graph::iterator it = graph_.begin();
      if (it != graph_.end()) 
        _polyline.push_back((it->first));

      while (it != graph_.end())
      {
        SliceVertex _next = it->second;
        _polyline.push_back(_next);
        graph_.erase(it);
        it = graph_.find(_next);
      }

      if (_polyline.size() > 1)
        polylines_.push_back(_polyline);
    }
  }

  void Polyline::optimize(float _threshold)
  {
    unsigned int _size = size(),
                 _iteration = 0;
    typedef multimap<float,Polyline::iterator> ErrorMap;

    // Repeat this loop until there is no vertex to be deleted
    while(1)
    {
      ErrorMap _errorMap; 
      Polyline::iterator it, _prev,_next, _itBegin = begin();
      ++_itBegin;
      if (_iteration % 2 == 1) ++_itBegin; 
      _next = _itBegin; ++_next;
      it = _itBegin;

      if (size() <= 2) break;
      unsigned int _counter = 0;

      while (_next != end())
      {
        // Calculate Error //TODO: Put this into separate method
        _next = it; ++_next; _prev = it; --_prev;
        Vec2f _dir(_next->x() - _prev->x(),_next->y() - _prev->y());
        Vec2f _normal(-_dir.y(),_dir.x());
        Vec2f _p(it->x() - _prev->x(),it->y() - _prev->y());

        float _error = dot(_p,_normal);
        _error *= _error;
        _errorMap.insert(make_pair<float,Polyline::iterator>(_error,it));

        ++it; 
        ++it; if (it == end()) break;
      }

      /// Erase those line vertices with the lowest errors
      for (ErrorMap::iterator _mapIt = _errorMap.begin(); 
          _mapIt != _errorMap.lower_bound(_threshold); ++_mapIt)
      {
        _counter++;
        erase(_mapIt->second);
      }
      if (_counter == 0) break;
      _iteration++;
    }

    LOG_MSG_(2) << fmt("Ratio: %, Size: %") % (float(size())/float(_size)) % size();
  }

  void Slice::optimize(float _threshold)
  {
    BOOST_FOREACH( Polyline& _polyline, polylines_ )
      _polyline.optimize(_threshold);
  }


  std::vector<Slice*> Slices::get() 
  { 
    std::vector<Slice*> _result;
    BOOST_FOREACH( const Slice& _slice, slices_ )
      _result.push_back(const_cast<Slice*>(&_slice));
    return _result;

  }

  /*
     bool Slice::getSegment(const LineSegment& _lineSeg, Point2f& _p0, Point2f& _p1) const
     {
     if (!_lineSeg.next_ || _lineSeg.normal_.sqrLength() == 0.0 ) return false;

     _p0(bounds_.min().x() + bounds_.size().x() / 65535.0 * _lineSeg.v_.x(),
     bounds_.min().y() + bounds_.size().y() / 65535.0 * _lineSeg.v_.y());
     _p1(bounds_.min().x() + bounds_.size().x() / 65535.0 * _lineSeg.next_->v_.x(),
     bounds_.min().y() + bounds_.size().y() / 65535.0 * _lineSeg.next_->v_.y());
     return true;
     }
     */
  /*
     Slice::LineSegments::iterator Slice::get(LineSegment* _lineSeg)
     {
     LineSegments::iterator it = lineSegments_.lower_bound(*_lineSeg);
     size_t _count = lineSegments_.count(it);
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

     }
     return lineSegments_.end();
     }
     */
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

  int Slice::resolution_ = 4096;

  bool Slice::makePoint(const Point3f& _p, SliceVertex& _v) const 
  {
    //LOG_MSG << fmt("% %") % bounds_.min().x()% bounds_.min().y() % bounds_.max().x() % bounds_.max().y();  
    if (!bounds_.inside(Point2f(_p.x(),_p.y()))) return false;

    //    LOG_MSG << fmt("% %") % _v.x() % _v.y();

    _v.x(resolution_*(_p.x() - bounds_.min().x()) / bounds_.size().x());
    _v.y(resolution_*(_p.y() - bounds_.min().y()) / bounds_.size().y());
    return true;
  }

  Slices::Slices(float _thickness, Bounds3f _bounds)
  {
    make(_thickness,_bounds);
  }


  void Slices::make(float _thickness, Bounds3f _bounds)
  {
    slices_.clear();
    float _slicePos = _bounds.min().z() + _thickness / 2.0 ;

    while (_slicePos < _bounds.max().z())
    {
      if (_slicePos > _bounds.max().z()) break;
      Slice _slice(_slicePos,_bounds);
      _slicePos += _thickness;
      slices_.insert(_slice);
    }

    LOG_MSG << fmt("Made % slices.") % slices_.size();
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
