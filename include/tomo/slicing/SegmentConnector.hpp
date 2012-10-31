#pragma once

#include "tomo/geometry/prim/Ring.hpp"

namespace tomo
{
  namespace slicing
  {
    /// Connects line segments from a compound into several rings
    template<template<class,class> class RESULT_CONTAINER=std::vector>
    struct SegmentConnector
    {
      typedef geometry::prim::Ring Ring;
      typedef geometry::prim::Segment Segment;
      typedef RESULT_CONTAINER<Ring,std::allocator<Ring>> Rings;
      typedef geometry::comp::Compound<Segment> LineCompound;
      typedef geometry::kd::visitor::Nearest<
          LineCompound::kdtree_type,
          geometry::kd::visitor::SegmentSegmentSqrDistance,
          geometry::kd::visitor::SegmentBoundsSqrDistance> Nearest;      

      Rings operator()(LineCompound& _compound)
      {
        using geometry::kd::visitor::SegmentNodeIntersectPointOnly;
        // ensure that the compound has a valid index
        _compound.validate<SegmentNodeIntersectPointOnly>();
        // multiple rings as result
        Rings _result;
        ///@todo Implement connecting algorithm

        Nearest _nearest(_compound.kdTree());

        BOOST_FOREACH( const Segment& _segment, _compound.objs() )
        {
          Ring _ring;
          if (makeRing(_segment,_nearest,_ring))
          {
            _result.push_back(_ring);
          }
        }

        return _result;
      }

    private:
      bool makeRing(const Segment& _segment, Nearest& _nearest, Ring& _ring)
      {
        Segment* _currentSegment = const_cast<Segment*>(&_segment);
      //  _ring().push_back(_currentSegment->p0().as());

        while (1)
        {
          _currentSegment = _nearest.find(_currentSegment);
          if (!_currentSegment)
          {
            std::cout << _ring().size() << " " << _currentSegment << std::endl;
            return false;
          }
          _ring().push_back(_currentSegment->p0().as());
        }

        return true;
      }
    };
  }
}
