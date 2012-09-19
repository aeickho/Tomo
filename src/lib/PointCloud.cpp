#include "tomo/geometry/PointCloud.hpp"
#include "tomo/geometry/aux/Median.hpp"

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <boost/foreach.hpp>

#include <cassert>

#include "tbd/log.h"

using namespace std;

namespace tomo
{
  namespace geometry
  {
    /*
      static bool compareX(const Vertex3f* a, const Vertex3f* b) { return a->v.x() < b->v.x(); }
      static bool compareY(const Vertex3f* a, const Vertex3f* b) { return a->v.y() < b->v.y(); }
      static bool compareZ(const Vertex3f* a, const Vertex3f* b) { return a->v.z() < b->v.z(); }

      float PointCloud::splitPos(const PrimCont& _primList, NodeInner* _inner, const Bounds3f& _bounds) const
      {
        switch (_inner->axis())
        {
          case X: return median(_primList,compareX)->v.x();
          case Y: return median(_primList,compareY)->v.y();
          case Z: return median(_primList,compareZ)->v.z();
          default: return 0.0;
        }
      }
    */

    void PointCloud::read(const string& _filename)
    {
      ///@todo implement read function
      update();
    }

  }
}
