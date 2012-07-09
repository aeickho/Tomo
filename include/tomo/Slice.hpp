#pragma once

#include <vector>
#include "tomo/Plane.hpp"
#include <boost/foreach.hpp>
#include <set>

namespace tomo
{
  using std::vector;

  typedef vector<Point2f> Polyline;
  typedef vector<Polyline> Polylines;
  typedef vector<Polyline*> PolylineList;

  class Track : public Polyline
  {
  };

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

    Slice(float _posZ, const Bounds& _bounds)
    {
        posZ_ = _posZ;
        anchor_(_bounds.min().x(),_bounds.min().y()); 
        size_(_bounds.size().x(),_bounds.size().y()); 
    }

    void addSegment(const Point3f& _p0, const Point3f& _p1, const Vec3f& _normal)
    {
      LineSegment _seg(Point2f((_p0.x() - anchor_.x()) / size_.x(),
                               (_p0.y() - anchor_.y()) / size_.y()),
                       Point2f((_p1.x() - anchor_.x()) / size_.x(),
                               (_p1.y() - anchor_.y()) / size_.y()),
                       Vec2f(_normal.x(),_normal.y()));
      lineSegments_.push_back(_seg);
    }
    


    Point2f anchor_;
    Vec2f size_;
    float posZ_;
    std::vector<LineSegment> lineSegments_;

 /*   SplitPlane plane_;
    std::vector<Isle> isles_;

    Ray generateRay(float _pos)
    {

    }

    vector<Ray> generateRays(unsigned _numberOfRays)
    {
      std::vector<Ray> _rays; _rays.reserve(_numberOfRays);
      Vec3f _dir(plane_.u_.cross(plane_.n_)), 
            _org(plane_.v_.cross(plane_.n_));
      diff = (_org - plane.center_) * (0.5 / _numberOfRays);

      for (unsigned i = 0; i < _numberOfRays; i++)
       {
        _rays.push_back(Ray(_org,_dir,0.0,1.0));
        _org += diff; 
      }
    }
*/

   

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
    Slices(unsigned _nSlices, Bounds _bounds)
    {
      make(_nSlices,_bounds);
    }

    typedef std::set<Slice,CompareSlice>::iterator iter;
    typedef std::set<Slice,CompareSlice>::const_iterator const_iter;

      void make(unsigned _nSlices, Bounds _bounds)
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

      iter get(float _posZ)
      {
        Slice _slice(_posZ); 
        return slices_.lower_bound(_slice);
      }

      const_iter get(float _posZ) const
      {
        Slice _slice(_posZ); 
        return slices_.lower_bound(_slice);
      }

      std::vector<const Slice*> get() const
      {
        std::vector<const Slice*> _result;
        BOOST_FOREACH ( const Slice& _slice, slices_ )
          _result.push_back(&_slice);
        return _result;
      }
   
      iter end() { return slices_.end(); }
      iter begin() { return slices_.begin(); }

  };

}
