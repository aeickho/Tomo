#include "tomo/LineSegment.hpp"
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/geometry/geometries/adapted/c_array.hpp>
#include <tbd/log.h>

using namespace std;
using namespace boost::geometry;

BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian);

namespace tomo
{
//  BOOST_GEOMETRY_REGISTER_MULTI_POLYGON(vector<Polygon>);


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

  Polygon LineSegmentPlane::asPolygon(LineSegment* _lineSegment) 
  {
    std::set<const LineSegment*> _usedSegments;

    Polygon _polygon;
    LineSegment* _curSegment = _lineSegment;

    while (_usedSegments.find(_curSegment) != _usedSegments.end())
    {
      _curSegment->next(nearest(_curSegment));
      _polygon.outer().push_back(PointXYf(_curSegment->front().x(),_curSegment->front().y()));
      _usedSegments.insert(_curSegment);
      _curSegment = _curSegment->next();

      if (!_curSegment) break;
    }

    return _polygon;
  }

  LineSegmentPlane::LineSegmentPlane(Slice* _slice) : slice_(_slice) 
  {
  }

  float LineSegmentPlane::pos() const
  {
    BOOST_ASSERT(slice_);
    return slice_->pos();
  }

  vector<Polygon> LineSegmentPlane::makePolygons()
  {
    vector<Polygon> _closures;
    vector<Polygon> _holes;

    update();

    BOOST_FOREACH( LineSegment& _lineSegment, objs() )
      if (!_lineSegment.next())
      {
        LOG_MSG << fmt("(% %), number of segments = %") % _lineSegment.front().x() % _lineSegment.front().y() % objs().size();
        Polygon _polygon = asPolygon(&_lineSegment);
    /*    if (_polygon.isHole())
        {
          _holes.push_back(_polygon);
        } else
        {
          _closures.push_back(_polygon);
        }*/
      }

    vector<Polygon> _polygons;
/*
    BOOST_FOREACH( Polygon& _polygon, _polygons )
    {
      boost::geometry::union_(_polygon,_slicePolygons,_slicePolygons);
    }
    BOOST_FOREACH( Polygon& _hole, _holes )
    {
      boost::geometry::difference(_slicePolygons,_hole,_slicePolygons);
    }
    BOOST_FOREACH( Polygon& _polygon, _slicePolygons )
      slice_->add(_polygon);
*/

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
