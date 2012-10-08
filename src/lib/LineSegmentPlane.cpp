#include "tomo/slicing/LineSegmentPlane.hpp"

#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <tbd/log.h>

using namespace std;
using namespace tomo::geometry::prim;

namespace tomo
{
  namespace slicing
  {
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

      Ring _ring;
      while (_usedSegments.find(_curSegment) == _usedSegments.end())
      {
        _ring.add(_curSegment->front());

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
    /*
      vector<LineSegment*> _kNearest = collectKNearest(_lineSegment,1);
      BOOST_FOREACH( LineSegment* _nearest , _kNearest )
      if (!_nearest->prev())
        return _nearest;

      LOG_MSG;*/
      return nearest(_lineSegment);
    }

    LineSegmentPlane::LineSegmentPlane(Slice* _slice) : slice_(_slice)
    {
    }

    float LineSegmentPlane::pos() const
    {
      BOOST_ASSERT(slice_);
      return slice_->pos();
    }



    void LineSegmentPlane::makeRings(std::vector<Ring>& _rings)
    {
      /// 1st step:
      /// Find nearest neighbor for each LineSegment
      BOOST_FOREACH( LineSegment& _lineSegment, lineSegments() )
      {
        LineSegment* _nearest = nearestSegment(&_lineSegment);
        _lineSegment.next(_nearest);
        if (_nearest) _nearest->prev(&_lineSegment);
      }

      /// 2nd step:
      /// Iterate over line
      std::set<const LineSegment*> _usedSegments;
      BOOST_FOREACH( const LineSegment& _lineSegment, lineSegments() )
      {
        if (_usedSegments.find(&_lineSegment) != _usedSegments.end()) continue;
       /*
        Polygon _polygon;
        MultiPolygon _simplified(1);
        PolygonType _polygonType = asPolygon(&_lineSegment,_usedSegments,_polygon);
        if (_simplifyThreshold > 0.0)
        {
          boost::geometry::simplify(_polygon(),_simplified[0](),_simplifyThreshold);
        }
        else
        {
          _simplified[0] = _polygon;
        }

        _polygons.push_back(_simplified[0]);
      
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
    }

    void LineSegmentPlane::addSegment(const point_type& _p0, const point_type& _p1)
    {
      if (lineSegments().size() % 1024 == 0) lineSegments().reserve(lineSegments().size()+1024);
      lineSegments().push_back(LineSegment(_p0,_p1));
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
}
