#pragma once

#include "tomo/Compound.hpp"
#include "tomo/Triangle.hpp"

namespace tomo
{
  namespace mesh_detail 
  {
    /** @brief Base class for meshes *
     * @tparam TRIANGLE Triangle type
     */
    template<class TRIANGLE>
    struct MeshConcept : public Compound<TRIANGLE>
    {
      /// KDTree node type 
      typedef KDNode<TRIANGLE> Node;
      
      /** @brief Load mesh from file (virtual) 
       */
      virtual void read(const std::string& _filename) = 0;

      /** @brief return triangles **/
      const std::vector<TRIANGLE>& triangles() const { return Compound<TRIANGLE>::objs_; }

      bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const
      {
        bool found = false;
        recKDTreeTraverse(_ray,this->root(),_ray.tMin_,_ray.tMax_,found,_normal,_texCoords);
        return found;
      }

      bool slice(Slice& _slice) const 
      { 
        return false; // TODO: Implement slicing algo
      }

      private:
      /** @brief Traverses kd-tree along a ray recursively
       * @param _ray        Ray which traverses KDTree
       * @param _node       Current node
       * @param _tnear      Near ray section
       * @param _tfar       Far ray section
       * @param _found      Flag which determines if a ray was found
       * @param _normal     Normal of intersecting triangle
       * @param _texCoords  Texture coordinates of intersecting triangle
       */
      float recKDTreeTraverse(Ray& ray, const Node* node, float tnear, float tfar, bool& found,
          Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const
      {
        if (node->isLeaf())
        {
          BOOST_FOREACH( const Triangle* tri, node->leaf_.primitives(this->primLists_) )
            if (tri != ray.primitive_) found |= (tri->intersect(ray,_normal,_texCoords));
          return ray.tMax_;
        }

        int k = node->inner_.axis();
        float d = (node->inner_.splitPos() - ray.org_[k]) / ray.dir_[k];

        const KDNode<Triangle>* front = node->inner_.left(this->nodes_);
        const KDNode<Triangle>* back  = node->inner_.right(this->nodes_);
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

  /** @brief Vertex mesh which consists of VertexTriangles and a vertex list 
   */
  struct VertexMesh : public mesh_detail::MeshConcept<VertexTriangle>
  {
    std::vector<Vertex> vertices_;
  };

  /** @brief Mesh which consists of individual triangles 
   */
  struct TriangleMesh : public mesh_detail::MeshConcept<Triangle>
  {
    void read(const std::string& filename);

    /** @brief Split a mesh into halves along a split plane
     * @param _plane  Split plane
     * @returns Two new TriangleMeshes which represent halves of the original mesh 
     */
    std::pair<TriangleMesh,TriangleMesh> split(const Plane& plane);

    private:

    /** @brief Splits are triangle with splitting plane
     * @detail Adds triangles behind plane to behind and triangles in front of plane to front
     * @param _tri    Triangle to be split
     * @param _plane  Split plane
     * @param _halves Halves of the mesh where the resulting triangles are inserted 
     */
    void splitTriangle(const Triangle& tri, const Plane& plane, std::pair<TriangleMesh,TriangleMesh>& _halves);

  };

}
