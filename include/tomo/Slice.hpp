#pragma once

#include <vector>
#include "tomo/Plane.hpp"
#include "tomo/Vertex.hpp"
#include "tomo/KDTree.hpp"
#include <boost/foreach.hpp>
#include <set>
#include <list>

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

/*
  struct LineSegment : Primitive2f
  {
    LineSegment(Point2us _v) : v_(_v), next_(NULL) {}

    Point2us v_;
    LineSegment* next_;

    bool isLine() const
    {
      if (!next_) return false;
      return next_->next_ == this;
    }


    bool intersect(Ray2f _ray, float _tNear, float _tFar, Vec2f _normal = NULL);
    SplitPlaneIntersect intersect(Axis _axis, float _splitPos, 
            const Bounds2f& _boundsLeft, const Bounds2f& _boundsRight) const;
    Bounds2f bounds() const;

    KDTree<LineSegment,2,float> kdTree_;
  };
*/
  struct SliceVertex : public Point2us
  {
    Point2f getCoords(const Bounds2f& _bounds);

    friend bool operator<(const SliceVertex& lhs, const SliceVertex& rhs) 
    {
      return (lhs.x() * 65536 + lhs.y()) < (rhs.x() * 65536 + rhs.y());
    }
  };

  struct Polyline : private std::list<SliceVertex>
  {
    typedef std::list<SliceVertex> Cntr;

    using Cntr::iterator;
    using Cntr::const_iterator;
    using Cntr::push_back;
    using Cntr::push_front;
    using Cntr::size;
    using Cntr::end;
    using Cntr::begin;
    using Cntr::front;
    using Cntr::back;

    void optimize(float _threshold);
  };

  struct Slice 
  {
    typedef std::multimap<SliceVertex,SliceVertex> Graph; 

    Slice(float _posZ, const Bounds3f& _bounds);
    
    void addSegment(const Point3f& _p0, const Point3f& _p1);
    //bool getSegment(const LineSegment& _lineSeg, Point2f& _p0, Point2f& _p1) const;

//    LineSegments::iterator get(LineSegment* _lineSeg);

    void connect();

    Bounds2f bounds_;
    float posZ_;

    void makePolylines();
    void optimize(float _threshold);

    friend bool operator<(const Slice& lhs, const Slice& rhs)
    {
      return lhs.posZ_ < rhs.posZ_;
    }


    std::vector<Polyline> polylines_;
    Graph graph_;
    static int resolution_;
    
    private:
      bool makePoint(const Point3f& _p, SliceVertex& _v) const;

  };

  struct Slices
  {
  public:
      Slices() {}
      Slices(float _thickness, Bounds3f _bounds);

      typedef std::set<Slice>::iterator iterator;
      typedef std::set<Slice>::const_iterator const_iterator;

      void make(float _thickness, Bounds3f _bounds);

      std::vector<Slice*> get();

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
