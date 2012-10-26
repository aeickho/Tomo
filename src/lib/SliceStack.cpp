#include "tomo/slicing/SliceStack.hpp"

namespace tomo
{
  namespace slicing
  {
    SliceStack::SliceStack(float _thickness, const bounds_type& _bounds)
    {
      build(_thickness,_bounds);
    }

    void SliceStack::build(float _thickness, const bounds_type& _bounds)
    {
      planes_.clear();
      float _slicePos = _bounds.min().z() + _thickness / 2.0 ;

      while (_slicePos <= _bounds.max().z())
       {
        Slice _slice(_slicePos);
        planes_.insert(std::make_pair<>(_slicePos,_slice));
        _slicePos += _thickness;
      }
      LOG_MSG << fmt("Made % slices.") % planes_.size();
    }
  }
}
