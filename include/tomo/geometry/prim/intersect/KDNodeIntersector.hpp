
namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        template <class PRIMITIVE>
        struct KDNodeIntersector :
            Intersector<PRIMITIVE, typename aux::KDNodePrimitive< PRIMITIVE::model_type > >
        {
          typedef KDNode<PRIMITIVE> node_type;
        };
      }
    }
  }
}

