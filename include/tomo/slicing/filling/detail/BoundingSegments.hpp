namespace tomo
{
  namespace slicing
  {
    namespace filling
    {
      namespace detail
      {
        template
          <
          typename SEGMENT = tomo::geometry::base::Ray< typename BOUNDS::model_type >,
          typename BOUNDS = typename SEGMENT::bounds_type, 
          typename ANGLE = typename RAY::scalar_type 
          >
        std::pair<SEGMENT,SEGMENT> boundingSegments(const BOUNDS& _bounds, ANGLE _angle) 
        {
          typedef typename BOUNDS::scalar_type scalar_type;
          typedef typename BOUNDS::vec_type vec_type;

          scalar_type _radius = bounds().radius();
          scalar_type _cos = _radius * cos(_angle*M_PI/180);
          scalar_type _sin = _radius * sin(_angle*M_PI/180);
          vec_type _cross(-_sin,_cos);
          vec_type _dir(_cos,_sin);
          point_type _center = _bounds.center();

          std::pair<SEGMENT,SEGMENT> _result;
          _result.first[0] = _center - _cross - _dir;
          _result.first[1] = _result.first[0] + 2.0*_dir;
          _result.second[0] = _center + _cross - _dir; 
          _result.second[1] = _result.second[0] + 2.0*_dir;
          return _result;
        }
      }
    }
  }
}

