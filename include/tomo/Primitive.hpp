#pragma once

#include "tomo/Ray.hpp"
#include "tomo/Bounds.hpp"

namespace tomo 
{
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

  /* @brief A primitive is an object which has an extent and for which an intersection point can be found 
  */
  struct Primitive
  {
    /** @brief Virtual method to determine intersection point
     * @param _ray        Ray for intersection
     * @param _normal     Pointer to normal determined from intersection
     * @param _texCoords  Pointer to texCoords to be returned
     */
    inline bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const
    {
      return intersect(_ray,_ray.tNear(),_ray.tFar(),_normal,_texCoords);
    }
    
    /** @brief Virtual method to determine intersection point
     * @param _ray        Ray for intersection
     * @param _tNear      Ray segment nearest
     * @param _tFar       Ray segment farest
     * @param _normal     Pointer to normal determined from intersection
     * @param _texCoords  Pointer to texCoords to be returned
     */
    virtual bool intersect(Ray& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const = 0;

    /** @brief Method to determine the intersection between primitive and split plane
     * @param _axis        Axis of split plane
     * @param _splitPos    Position of split plane
     * @param _boundsLeft  Left bounds
     * @param _boundsRight Right bounds
     */
    virtual SplitPlaneIntersect intersect(Axis _axis, float _splitPos, const Bounds& _boundsLeft, const Bounds& _boundsRight) const
    {
      if (bounds().max()[_axis] < _splitPos) return SplitPlaneIntersect(true,false);
      if (bounds().min()[_axis] > _splitPos) return SplitPlaneIntersect(false,true); 
      return SplitPlaneIntersect(true,true);
    }
 
    /** @brief Return bounds of primitive
     */
    virtual Bounds bounds() const = 0;

    /** @brief Return pointer to object
     */
    Primitive* pointer() const { return const_cast<Primitive*>(this); }
  };
}
