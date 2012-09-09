#include "tomo/Slice.hpp"
#include "tomo/Primitive.hpp"
#include "tomo/Compound.hpp"

namespace tomo
{
  struct LineSegment : public Primitive2f
  {
    LineSegment(Point2f _front, Point2f _back) : front_(_front), back_(_back), next_(NULL) {}

  /*  bool isLine() const;
    bool isHole() const;
    bool closed() const;*/

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
  };

  /// The class LineSegmentContainer is used to store line segments to make polygons out of them
  struct LineSegmentPlane : Compound<LineSegment,2,float>
  {
    LineSegmentPlane(Slice* _slice = NULL); 

    std::vector<Polygon> makePolygons();

    float pos() const;
    void addSegment(const Point2f& _p0, const Point2f& _p1);
    void aggregate(const LineSegmentPlane& _lineSegmentPlane);

    TBD_PROPERTY_RO(Slice*,slice);

  private:
    Polygon asPolygon(LineSegment* _lineSegment);
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
