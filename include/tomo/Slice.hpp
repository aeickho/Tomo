#pragma once

#include <vector>
#include "tomo/Plane.hpp"
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
    LineSegment(Point2f _p0, Point2f _p1, Vec2f _normal) :
      p0_(_p0), p1_(_p1), normal_(_normal) {}

    Point2f p0_, p1_;
    Vec2f normal_;
  };

  struct Slice 
  {
    Slice(float _posZ = 0.0) : posZ_(_posZ) {}
    Slice(float _posZ, const Bounds3f& _bounds);
    
    void addSegment(const Point3f& _p0, const Point3f& _p1, const Vec3f& _normal);

    Point2f anchor_;
    Vec2f size_;
    float posZ_;
    std::vector<LineSegment> lineSegments_; 
  };

  struct Slices
  {
  private:
      struct CompareSlice
      {
        bool operator()(const Slice& lhs, const Slice& rhs) const
        {
          return lhs.posZ_ < rhs.posZ_;
        }
      };

      std::set<Slice,CompareSlice> slices_;
  public:
      Slices() {}
      Slices(unsigned _nSlices, Bounds3f _bounds);

      typedef std::set<Slice,CompareSlice>::iterator iterator;
      typedef std::set<Slice,CompareSlice>::const_iterator const_iterator;

      void make(unsigned _nSlices, Bounds3f _bounds);

      /// Get slice by Z position
      iterator get(float _posZ);
      const_iterator get(float _posZ) const;
      
      std::vector<const Slice*> get() const;
   
      iterator end();
      iterator begin();
      const_iterator end() const;
      const_iterator begin() const;
  };

}
