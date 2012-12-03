#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace node
      {
        struct StaticPrimitivePtrContainer
        {
          void leafRange(const NodeLeaf& _leaf, prim_iterator& _begin, prim_iterator& _end)
          {
            _begin = primLists_.begin() + _leaf.begin();
            _end = primLists_.begin() + _leaf.end();
          }

          void leafRange(const NodeLeaf& _leaf, 
                         prim_const_iterator& _begin, 
                         prim_const_iterator& _end) const
          {
            _begin = primLists_.begin() + _leaf.begin();
            _end = primLists_.begin() + _leaf.end();
          }
        /// Insert Primitive pointers from _primList into primLists_
        NodeLeaf& leafNodeSetup(unsigned _nodeIndex, const prim_cntr_type& _primList)
        {
          nodes_[_nodeIndex].leaf_.begin(primLists_.size());
          nodes_[_nodeIndex].leaf_.end(primLists_.size()+_primList.size());
          primLists_.insert(primLists_.end(),_primList.begin(),_primList.end());
          return nodes_[_nodeIndex].leaf_;
        }



        };
      }
    }
  }
}

