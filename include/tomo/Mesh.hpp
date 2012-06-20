#pragma once

#include "tomo/Compound.hpp"
#include "tomo/Triangle.hpp"

namespace tomo
{
  namespace mesh_detail 
  {
    template<class TRIANGLE>
      struct MeshConcept : public Compound<TRIANGLE>
    {
      virtual void read(const std::string& _filename) = 0;

      typedef KDNode<TRIANGLE> Node;

      const std::vector<TRIANGLE>& triangles() const { return Compound<TRIANGLE>::objs_; }
      
      bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const
      {
        bool found = false;
        recKDTreeTraverse(_ray,this->root_,_ray.tMin_,_ray.tMax_,found,_normal,_texCoords);
        return found;
      }

      bool slice(Slice& _slice) const 
      { 
        return false; // TODO: Implement slicing algo
      }

    private:

      // Method to define how KDTree is constructed 
      void divideNode(Node* node, const BoundingBox& box, int depth)
      {
        if (depth > 15 || node->objs.size() < 10)
        {   // We have a leaf node!
          node->left = NULL; node->right = NULL;
          return;
        }
        node->left = new Node;
        node->right = new Node;
        node->axis = box.dominantAxis();

        // TODO: Surface Area Heuristic here!
        node->splitPos = 0.5*(box.min()[node->axis] + box.max()[node->axis]);
        BoundingBox boxLeft, boxRight;
        box.split(node->splitPos,node->axis,boxLeft,boxRight);

        BOOST_FOREACH( Triangle* tri, node->objs )
        {
          int result = tri->intersect(node->splitPos,node->axis);
          if (result & 1) node->left->objs.push_back(tri);
          if (result & 2) node->right->objs.push_back(tri);
        }
        node->objs.clear();
        divideNode(node->left,boxLeft,depth+1);
        divideNode(node->right,boxRight,depth+1);

      }

      // Traverses kd-tree along a ray recursively
      float recKDTreeTraverse(Ray& ray, Node* node, float tnear, float tfar, bool& found,
          Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const
      {
        if (node->isLeaf())
        {
          BOOST_FOREACH( Triangle* tri, node->objs )
            if (tri != ray.primitive_) found |= (tri->intersect(ray,_normal,_texCoords));
          return ray.tMax_;
        }

        int k = node->axis;
        float d = (node->splitPos - ray.org_[k]) / ray.dir_[k];

        KDNode<Triangle>* front = node->left;
        KDNode<Triangle>* back  = node->right;
        if (ray.dir_[k] < 0) std::swap(front,back); 

        if (d <= tnear)
        {
          recKDTreeTraverse(ray,back,tnear,tfar,found);
        } else
          if (d >= tfar)
          {
            recKDTreeTraverse(ray,front,tnear,tfar,found);
          } else
          {
            float t_hit = recKDTreeTraverse(ray,front,tnear,d,found);
            if (t_hit <= d) return t_hit;
            return recKDTreeTraverse(ray,back,d,tfar,found,_normal,_texCoords);
          }
        return INF;
      }

    };
  }

  struct VertexMesh : public mesh_detail::MeshConcept<VertexTriangle>
  {
    std::vector<Vertex> vertices_;
  };

  struct TriangleMesh : public mesh_detail::MeshConcept<Triangle>
  {
    void read(const std::string& filename);

    // Split a mesh into halves along a split plane
    std::pair<TriangleMesh,TriangleMesh> split(const Plane& plane);

    protected:

    // Iterate over triangles and determine maximum  
    private:

    // Splits are triangle with splitting plane
    // Adds triangles behind plane to behind and triangles in front of plane to front 
    void splitTriangle(const Triangle& tri, const Plane& plane, TriangleMesh& behind, TriangleMesh& front);
  };

}
