#pragma once

#include "tomo/geometry/prim/LineSegment.hpp"
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
      typedef RESULT_CONTAINER<Ring,std::allocator<Ring>> Rings;
      typedef geometry::prim::LineSegment LineSegment;
      typedef geometry::comp::Compound<LineSegment> LineCompound;

      Rings operator()(LineCompound& _compound)
      {
        // ensure that the compound has a valid index
        _compound.validate();
        // multiple rings as result
        Rings _result;
        ///@todo Implement connecting algorithm
        return _result;
      }
    };
  }
}
