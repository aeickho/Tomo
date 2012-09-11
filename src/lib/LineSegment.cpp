#include "tomo/LineSegment.hpp"
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/geometry/geometries/adapted/c_array.hpp>
#include <tbd/log.h>
#include <boost/geometry/multi/geometries/register/multi_polygon.hpp>


using namespace std;
using namespace boost::geometry;

BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian);

BOOST_GEOMETRY_REGISTER_MULTI_POLYGON(tomo::MultiPolygon);

namespace tomo
{
  

  LineSegment::LineSegment(Point2f _front, Point2f _back) :
    front_(_front), 
    back_(_back), 
    next_(NULL), 
    prev_(NULL)
  {
  }

  Bounds2f LineSegment::bounds() const
  {
    Bounds2f _bounds;
    _bounds.extend(back_);
    return _bounds;
  }

  SplitPlaneIntersect LineSegment::intersect(Axis _axis, float _splitPos,
                                          const Bounds2f& _boundsLeft, const Bounds2f& _boundsRight) const
  {
    float _p = front()[_axis];
    return SplitPlaneIntersect(_p <= _splitPos,
                               _p >= _splitPos);
  }

  float LineSegmentPlane::orientation(const LineSegment* _a, const LineSegment* _b) const
  {
    return (_a->front().x() - _a->back().x()) * (_b->front().y() - _a->front().y()) -
           (_a->front().y() - _a->back().y()) * (_b->front().x() - _a->front().x());
  }
  
  LineSegmentPlane::PolygonType LineSegmentPlane::asPolygon(
                          const LineSegment* _lineSegment, 
                          std::set<const LineSegment*>& _usedSegments,
                          Polygon& _polygon) 
  {
    const LineSegment* _curSegment = _lineSegment;

    /// This algorithm has an integrated orientation check for polygons 
    /// Inspired by:
    /// http://paulbourke.net/geometry/clockwise/source1.c

    int _flag = 0;
    float _orientationCoeff = 0.0;
    PolygonType _polygonType = PT_NONE;
    bool _polygonTypeDetermined = false;

    while (_usedSegments.find(_curSegment) == _usedSegments.end())
    {
      _polygon.outer().push_back(PointXYf(_curSegment->front().x(),_curSegment->front().y()));

      LineSegment* _next = _curSegment->next();
      if (!_next) return PT_NONE;
     
      if (_curSegment != _lineSegment)
      _usedSegments.insert(_curSegment);

      _orientationCoeff += orientation(_curSegment,_next);
      if (_orientationCoeff < 0) 
        _flag |= 1; 
      else if (_orientationCoeff > 0)
        _flag |= 2;
      if (_flag == 3 && !_polygonTypeDetermined) 
      {
        _polygonType = PT_CLOSURE;
        _polygonTypeDetermined = true;
      }

      _curSegment = _next;
    }

    if (_flag != 0 && !_polygonTypeDetermined)
      _polygonType = PT_HOLE;

    return _polygonType;
  }

  LineSegment* LineSegmentPlane::nearestSegment(LineSegment* _lineSegment)
  {
    /// @todo Improve this for handling of non-manifolds
    vector<LineSegment*> _kNearest = collectKNearest(_lineSegment,40);
   
    BOOST_FOREACH( LineSegment* _nearest , _kNearest )
      if (_nearest->front() != _nearest->back() && !_nearest->prev())
        return _nearest;

    LOG_MSG;
    return NULL;
  }

  LineSegmentPlane::LineSegmentPlane(Slice* _slice) : slice_(_slice) 
  {
  }

  float LineSegmentPlane::pos() const
  {
    BOOST_ASSERT(slice_);
    return slice_->pos();
  }



  MultiPolygon LineSegmentPlane::makePolygons(float _simplifyThreshold)
  {
    MultiPolygon _closures, _holes, _polygons;

    /// 1st step:
    /// Find nearest neighbor for each LineSegment
    BOOST_FOREACH( LineSegment& _lineSegment, objs() )
    {
      LineSegment* _nearest = nearestSegment(&_lineSegment);
      _lineSegment.next(_nearest);
      if (_nearest) _nearest->prev(&_lineSegment);
    }

    /// 2nd step:
    /// Iterate over line 
    std::set<const LineSegment*> _usedSegments;
    BOOST_FOREACH( const LineSegment& _lineSegment, objs() )
    {
      if (_usedSegments.find(&_lineSegment) != _usedSegments.end()) continue;
      Polygon _polygon;
      MultiPolygon _simplified(1);
      PolygonType _polygonType = asPolygon(&_lineSegment,_usedSegments,_polygon);
      if (_simplifyThreshold > 0.0)
      {
        boost::geometry::simplify(_polygon,_simplified[0],_simplifyThreshold);
      } else
      { 
        _simplified[0] = _polygon;
      }

      _polygons.push_back(_simplified[0]);
/*
      switch (_polygonType)
      {
        case PT_HOLE: 
          boost::geometry::union_(_holes,_simplified,_holes);
          break;
        case PT_CLOSURE: 
          boost::geometry::union_(_closures,_simplified,_closures);
          break;
        case PT_NONE: break;
      }
*/
    }

    /// 3nd step:
    /// Subtract holes from closures
//    boost::geometry::difference(_closures,_holes,_polygons);
    
    LOG_MSG << _polygons.size();

    return _polygons;
  }

  void LineSegmentPlane::addSegment(const Point2f& _p0, const Point2f& _p1)
  {
    if (objs().size() % 1024 == 0) objs().reserve(objs().size()+1024);
    objs().push_back(LineSegment(_p0,_p1));
  }

  void LineSegmentPlane::aggregate(const LineSegmentPlane& _lineSegmentPlane)
  {
    if (this->slice() != _lineSegmentPlane.slice()) 
    {
      LOG_WRN_(2) << "LineSegments are associated with different slices.";
      return;
    }

    Compound::aggregate(_lineSegmentPlane);
  }

  LineSegmentContainer::LineSegmentContainer(Slices& _slices) : slices_(_slices)
  {
    initializePlanes();
  }

  void LineSegmentContainer::initializePlanes()
  {
    vector< Slice* > _allSlices = slices_.fetch();
    BOOST_FOREACH( Slice* _slice, _allSlices )
    {
      LineSegmentPlane _lineSegmentPlane(_slice);
      planes_.insert(make_pair<float,LineSegmentPlane>(_slice->pos(),_lineSegmentPlane));
    }
  }

  void LineSegmentContainer::aggregate(const LineSegmentContainer& _lineSegmentContainer)
  {
    vector< LineSegmentPlane* > _thisPlanes = fetch();
    vector<const LineSegmentPlane* > _otherPlanes = _lineSegmentContainer.fetch();

    if (_thisPlanes.size() != _otherPlanes.size()) 
    {
      LOG_ERR << "Number of planes must be equal!";
      return;
    }

    for (size_t i = 0; i < _thisPlanes.size(); i++)
      _thisPlanes[i]->aggregate(*_otherPlanes[i]);
  }

  void LineSegmentContainer::produceSlices()
  {

  }

}
