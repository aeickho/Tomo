#include "tomo/Slice.hpp"
#include "tomo/Primitive.hpp"
#include "tomo/Compound.hpp"

namespace tomo
{
  struct LineSegment : public Primitive2f
  {
    LineSegment(Point2f _front, Point2f _back);

    bool intersect(Ray2f& _ray, float& _tNear, float& _tFar, Vec2f* _normal = NULL) const
    {
      return false;
    }


    Vec2f distanceVec(const Primitive& _p) const
    {
      return _p.center() - front();
    }

    Bounds2f bounds() const;
    SplitPlaneIntersect intersect(Axis _axis, float _splitPos,
                                  const Bounds2f& _boundsLeft, const Bounds2f& _boundsRight) const;

    TBD_PROPERTY_REF(Point2f,front);
    TBD_PROPERTY_REF(Point2f,back);
    TBD_PROPERTY(LineSegment*,next);
    TBD_PROPERTY(LineSegment*,prev);
  };

  /// The class LineSegmentContainer is used to store line segments to make polygons out of them
  struct LineSegmentPlane : Compound<LineSegment,2,float>
  {
    LineSegmentPlane(Slice* _slice = NULL); 

    std::vector<Polygon> makePolygons(float _simplifyThreshold = 0.0);

    float pos() const;
    void addSegment(const Point2f& _p0, const Point2f& _p1);
    void aggregate(const LineSegmentPlane& _lineSegmentPlane);

    TBD_PROPERTY_RO(Slice*,slice);

  private:
    typedef enum { PT_NONE, PT_CLOSURE, PT_HOLE } PolygonType; 

    /// Calculates the cross product of front and back and is used to determine orientation of the segment (CW or CCW)
    float orientation(const LineSegment* _a, const LineSegment* _b) const;

    PolygonType asPolygon(const LineSegment* _lineSegment, 
                          std::set<const LineSegment*>& _usedSegments, 
                          Polygon& _polygon);

    LineSegment* nearestSegment(LineSegment* _lineSegment);
  };

  struct LineSegmentContainer : PlaneStack<float,LineSegmentPlane>
  {
    LineSegmentContainer(Slices& _slices);
    void produceSlices();

    void aggregate(const LineSegmentContainer& _lineSegmentContainer);

  private:
    void initializePlanes();

    Slices& slices_;
  };
}
