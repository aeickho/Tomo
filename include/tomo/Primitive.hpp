#pragma once

#include "tomo/Ray.hpp"
#include "tomo/Bounds.hpp"

namespace tomo 
{
  /// Struct which holds the result of an intersection test between split plane and primitive
  struct SplitPlaneIntersect
  {
  public:
    SplitPlaneIntersect(bool _left = false, bool _right = false) : result_(0) { set(_left,_right); }

    void set(bool _left, bool _right) { result_ = _left | (_right << 1); }

    bool left() { return result_ & 1; }
    bool right() { return result_ & 2; }
    bool both() { return result_ != 0; }
  private:
    unsigned char result_;
  };

  /// A primitive is an object which has an extent and for which an intersection point can be found 
  template<int DIMENSIONS, typename SCALAR = DEFAULT_TYPE>  
  struct Primitive
  {
    typedef SCALAR Scalar;
    typedef Bounds<DIMENSIONS,Scalar> bounds_type;
    typedef Point<DIMENSIONS,Scalar> point_type;
    typedef Vec<DIMENSIONS,Scalar> vector_type;
    typedef Ray<DIMENSIONS,Scalar> ray_type;

    /** @brief Virtual method to determine intersection point
     * @param _ray        Ray for intersection
     * @param _normal     Pointer to normal determined from intersection
     * @param _texCoords  Pointer to texCoords to be returned
     */
    inline bool intersect(ray_type& _ray, vector_type* _normal = NULL) const
    {
      return intersect(_ray,_ray.tNear(),_ray.tFar(),_normal);
    }
    
    /** @brief Virtual method to determine intersection point
     * @param _ray        Ray for intersection
     * @param _tNear      Ray segment nearest
     * @param _tFar       Ray segment farest
     * @param _normal     Pointer to normal determined from intersection
     * @param _texCoords  Pointer to texCoords to be returned
     */
    virtual bool intersect(ray_type& _ray, Scalar& _tNear, Scalar& _tFar, vector_type* _normal = NULL) const = 0;

    /** @brief Method to determine the intersection between primitive and split plane
     * @param _axis        Axis of split plane
     * @param _splitPos    Position of split plane
     * @param _boundsLeft  Left bounds
     * @param _boundsRight Right bounds
     */
    virtual SplitPlaneIntersect intersect(Axis _axis, float _splitPos, const bounds_type& _boundsLeft, const bounds_type& _boundsRight) const
    {
      if (bounds().max()[_axis] < _splitPos) return SplitPlaneIntersect(true,false);
      if (bounds().min()[_axis] > _splitPos) return SplitPlaneIntersect(false,true); 
      return SplitPlaneIntersect(true,true);
    }
 
    /** @brief Return bounds of primitive
     */
    virtual bounds_type bounds() const = 0;

    /** @brief Return pointer to object
     */
    void* pointer() const { return (void*)const_cast<Primitive*>(this); }
  };

  typedef Primitive<2,float> Primitive2f;
  typedef Primitive<3,float> Primitive3f;

}
