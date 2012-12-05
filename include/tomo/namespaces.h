
namespace tomo
{
  /// Contains all classes and method for geometry representation
  namespace geometry 
  {
    /// Contains basic classes like vectors, points and color
    namespace base
    {
    }

    /// Namespace for KDTree and associated objects
    namespace kd
    {
      /// Specializations of a kdtree whose leaves contain objects 
      namespace object
      {
      }

      /**@brief Specializations of a kdtree for finding nearest objects
       * @detail Leaves and inner nodes contain pointer to primitives
       */
      namespace nearest
      {
      }

      /// Specializations of a kdtree containing a data coeffient in its leaves
      namespace coeff
      {
      }

      /// Contains policies how to split (subdivide) a node of kdtree
      namespace split
      {
      }
    }

    /// Contains all geometric primitive types
    namespace prim
    {
    }

    /// Contains all geometrix complex types
    namespace comp
    {
    }

    /// Functors to determine the squared (!) distance between two primitives
    namespace distance
    {
    }

    /// Functors to determine an intersection between two primitives
    namespace intersect
    {
    }

    /// Functors for miscellaneous geometric algorithms
    namespace algorithm
    {
    }
  }

  /// All classes to slice a Mesh
  namespace slicing
  {
  }

  /// All classes for visualization of a scene
  namespace scene
  {
  }

  /// All classes for g-code representation 
  namespace gcode
  {
  }

  /// All classes for drawing geometry and specific wrappers
  namespace draw
  {
  }
}



