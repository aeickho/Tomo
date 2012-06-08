#pragma once

#include "tomo/BoundingBox.hpp"

namespace tomo 
{

  template <typename T> 
  struct KDNode
  {
    KDNode(Axis _axis = X, float _splitPos = 0)
    {
      axis = _axis;
      splitPos = _splitPos;
      left = NULL; right = NULL;
    }

    bool isLeaf() const { return (!left && !right); }

    KDNode<T>* left;
    KDNode<T>* right;

    Axis axis;
    float splitPos;
    vector<T*> objs;

    void free() 
    { 
      left->free(); 
      delete left; 
      left = NULL; 
      right->free();
      delete right;
      right = NULL;
    }

    void draw(Color color, const BoundingBox& box, int depth, int maxDepth) const
    {
      if (isLeaf() || depth >= maxDepth) { box.draw(color); return; }

      BoundingBox boxLeft, boxRight;
      box.split(splitPos,axis,boxLeft,boxRight);
      if (left) left->draw(color,boxLeft,depth+1,maxDepth);
      if (right) right->draw(color,boxRight,depth+1,maxDepth);
    }
  };

  template <typename T> 
  struct KDTree
  {
    KDTree() : root(NULL) {}

    typedef KDNode<T> Node;
    Node* root;
    void clear()
    {
      if (!root) return;
      root->free();
      delete root;
      root = NULL;
    }

    void draw(Color color, const BoundingBox& box) const
    {
      if (root) root->draw(color,box,0,12);
    }

    void build(vector<T>& objs, const BoundingBox& boundingBox)
    {
      clear();
      root = new Node;
      root->objs.reserve(objs.size());
      
      for (unsigned i = 0; i < objs.size(); i++)
        root->objs.push_back(&objs[i]);
      
      divideNode(root,boundingBox,0);
    }


  private:
    virtual void divideNode(Node* node, const BoundingBox& boundingBox, int depth) = 0;
  };
}

