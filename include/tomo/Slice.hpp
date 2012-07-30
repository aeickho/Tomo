#pragma once

#include <vector>
#include "tomo/Plane.hpp"
#include "tomo/Vertex.hpp"
#include "tomo/KDTree.hpp"
#include <boost/foreach.hpp>
#include <set>

namespace tomo
{
  using std::vector;

/*  typedef vector<Point2f> Polyline;
  typedef vector<Polyline> Polylines;
  typedef vector<Polyline*> PolylineList;

  class Track : public Polyline
  {
  };
*/

  struct LineSegment
  {
    LineSegment(Point2us _v, Vec2f _normal = Vec2f()) : v_(_v), normal_(_normal), next_(NULL) {}

    Point2us v_;
    Vec2f normal_;
    LineSegment* next_;

    bool isLine() const
    {
      if (!next_) return false;
      return next_->next_ == this;
    }

    friend bool operator<(const LineSegment& lhs, const LineSegment& rhs) 
    {
      return (lhs.v_.x() * 65536 + lhs.v_.y()) < (rhs.v_.x() * 65536 + rhs.v_.y());
    }
  };

  struct Slice 
  {
    typedef std::multiset<LineSegment> LineSegments;
    Slice(float _posZ, const Bounds3f& _bounds);
    
    void addSegment(const Point3f& _p0, const Point3f& _p1, const Vec3f& _normal);
    bool getSegment(const LineSegment& _lineSeg, Point2f& _p0, Point2f& _p1, Vec3f& _normal ) const;

    LineSegments::iterator get(LineSegment* _lineSeg);

    void connect();

    Bounds2f bounds_;
    float posZ_;
    LineSegments lineSegments_;

    friend bool operator<(const Slice& lhs, const Slice& rhs)
    {
      return lhs.posZ_ < rhs.posZ_;
    }

    private:
      bool makePoint(const Point3f& _p, Point2us& _v) const;
  };

  struct Slices
  {
  public:
      Slices() {}
      Slices(unsigned _nSlices, Bounds3f _bounds);

      typedef std::set<Slice>::iterator iterator;
      typedef std::set<Slice>::const_iterator const_iterator;

      void make(unsigned _nSlices, Bounds3f _bounds);

      /// Get slice by Z position
      iterator get(float _posZ);
      const_iterator get(float _posZ) const;
     
      std::vector<const Slice*> get() const;

      iterator end();
      iterator begin();
      const_iterator end() const;
      const_iterator begin() const;
  private:
      std::set<Slice> slices_;
  };

}
