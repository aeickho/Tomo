#pragma once

#include "tomo/BoundingBox.hpp"

#include <vector>
#include <stack>

namespace tomo 
{
  template <typename PRIMITIVE> 
  union KDNode
  {
    bool isLeaf() const { return !inner_.leafFlag(); }

    /// Inner node
    struct Inner
    {
      typedef std::vector<KDNode> NodeCont;

    public:
      inline bool leafFlag() const { return data_ & (1 << 31); }
      inline Axis axis() const { return Axis(data_ & 3); }
      inline void axis(Axis _axis) { data_  |= ((~3) | (_axis & 3)); }

      /// Insert children
      inline void insert(NodeCont& _nodes, KDNode*& _left, KDNode*& _right)
      {
        data_ = (0x7FFFFFFC & (_nodes.size() << 2)) | (1 << 31);
        _nodes.push_back(KDNode());
        _nodes.push_back(KDNode());
        _left = &_nodes.at(_nodes.size()-2);
        _right = &_nodes.at(_nodes.size()-1);

//        LOG_MSG << fmt("% %") % ptrdiff_t(_right - _left) % sizeof(KDNode);
      }

      inline const KDNode* left(const NodeCont& _nodes) const { return &_nodes[(data_ & 0x7FFFFFFC) >> 2]; }
      inline const KDNode* right(const NodeCont& _nodes) const { return &_nodes[((data_ & 0x7FFFFFFC) >> 2) + 1]; }
      inline KDNode* left(NodeCont& _nodes) { return &_nodes[(data_ & 0x7FFFFFFC) >> 2]; }
      inline KDNode* right(NodeCont& _nodes) { return &_nodes[((data_ & 0x7FFFFFFC) >> 2) + 1]; }

      TBD_PROPERTY(float,splitPos);

    private:
      unsigned int data_;
    };

    /// Leaf node
    struct Leaf
    {
    public:
      typedef std::vector<PRIMITIVE*> PrimCont;

      /// Insert Primitive pointers from _src into _dest
      inline void insert(const PrimCont& _src, PrimCont& _dest)
      {
        // Make leaf node
        offset_ = _dest.size();
        size_ = _src.size();
        //_dest.resize(offset_ + size_);
        for (unsigned i = 0; i < size_; i++)
          _dest.push_back(_src[i]);
      }

      inline PrimCont primitives(const PrimCont& _primLists) const
      {
        PrimCont _primList;
        _primList.reserve(size_);
        for (unsigned i = offset_, j = 0; i < offset_ + size_; i++, j++)
          _primList[j] = _primLists[i];
        return _primList;
      }

    private:
      unsigned int size_;
      unsigned int offset_;
    };

    Inner inner_;
    Leaf leaf_;
  };

  template <typename PRIMITIVE, unsigned int PRIMITIVES_PER_NODE = 10, unsigned int MAX_DEPTH = 32> 
  struct KDTree
  {
  protected:
    /// Node type
    typedef KDNode<PRIMITIVE> Node;
    typedef typename KDNode<PRIMITIVE>::Inner NodeInner;
    
    /// Node container
    typedef std::vector<Node> NodeCont;
    
    /// Primitive container
    typedef std::vector<PRIMITIVE*> PrimCont;


    NodeCont nodes_;
    PrimCont primLists_;
    
    struct NodeStackElement
    {
      Bounds bounds_;
      PrimCont primLists_;
      Node* node_;
    };

    const Node* root() const { return &nodes_[0]; }
    Node* root() { return &nodes_[0]; }

    void build(std::vector<PRIMITIVE>& _objs, const Bounds& _bounds)
    {
      std::vector<NodeStackElement> _stack;

      nodes_.clear();
      nodes_.reserve(2*MAX_DEPTH*_objs.size()/PRIMITIVES_PER_NODE);
      nodes_.push_back(Node());

      primLists_.clear();

      // Initialize root node
      NodeStackElement _node;
      _node.bounds_ = _bounds;  // Object's bounds
      _node.node_ = &nodes_[0];

      // Init primitive lists
      _node.primLists_.reserve(_objs.size());
      for (unsigned i = 0; i < _objs.size(); i++)
        _node.primLists_.push_back(&_objs[i]);

      while (1)
      {
        // Perform depth-in first search until leaf node
        while (_node.primLists_.size() > PRIMITIVES_PER_NODE && _stack.size() < MAX_DEPTH) 
        {
          NodeStackElement _right;
          _right.primLists_.reserve(_node.primLists_.size());

          // Setup node
          NodeInner& _inner = _node.node_->inner_;
          _inner.axis(_node.bounds_.dominantAxis());
          _inner.splitPos(splitPos(_node.primLists_,_inner,_node.bounds_));          
          _inner.insert(nodes_,_node.node_,_right.node_);
          
          // Make bounding boxes
          _node.bounds_.split(_inner.splitPos(),_inner.axis(),_node.bounds_,_right.bounds_);
        
          // Test which primitives intersect which nodes
          unsigned _leftNodeCount = 0, _rightNodeCount = 0;
          for (unsigned i = 0; i < _node.primLists_.size(); i++)
          {
            PRIMITIVE* _prim = _node.primLists_[i];
            SplitPlaneIntersect _result = _prim->intersect(_inner.axis(),_inner.splitPos(),_node.bounds_,_right.bounds_);
        
            // Store primtive pointers in right node, if intersection
            if (_result.right()) 
            { 
              _right.primLists_.push_back(_prim); 
              _rightNodeCount++; 
            }
            
            // Optimization: We can use the original root node array for storing left node primitive pointers
            if (_result.left()) 
            {  
              _node.primLists_[_leftNodeCount]; 
              _leftNodeCount++; 
            }
          }

          _node.primLists_.resize(_leftNodeCount);
          _stack.push_back(_right);
        }

        // Make leaf node
        _node.node_->leaf_.insert(_node.primLists_,primLists_);
         
        _node = _stack.front();
        if (!_stack.empty()) _stack.pop_back(); else return;
      }
    }

    virtual float splitPos(const PrimCont& _primList, const NodeInner& _inner, const Bounds& _bounds) const
    {
      return 0.5*(_bounds.min()[_inner.axis()] + _bounds.max()[_inner.axis()]);
    }

  };
}

