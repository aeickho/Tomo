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
        typename SEGMENT = tomo::geometry::prim::Segment,
                 typename BOUNDS = tomo::geometry::base::Bounds2f,
                 template<class,class> class SEG_CONTAINER = std::vector
                 >
        struct Pattern
        {
          typedef SEGMENT Segment;
          typedef typename Segment::vec_type vec_type;
          typedef typename Segment::scalar_type scalar_type;
          typedef SEG_CONTAINER<Segment,std::allocator<Segment>> Segments;

          void operator()(const BOUNDS& _bounds,
                          Segment& _segments)
          {
            std::pair<Segment,Segment> _boundingSegments = boundingSegments(_bounds,angle_);
            vec_type _fillingDirection = _boundingSegments.second[0] - _boundingSegments.first[0];

            unsigned _nLines = _fillingDirection.length() / gap_;
            vec_type _inc = _fillingDirection * (1.0 / _nLines);

            Segment _segment(_boundingSegments.first[0] + 0.5*_inc,
                             _boundingSegments.first[1] + 0.5*_inc);

            for (unsigned i = 1; i < _nLines; ++i)
            {
              _segments.push_back(_segment);
              _segment[0] += _inc;
              _segment[1] += _inc;
            }
          }

          TBD_PROPERTY(scalar_type,angle);
          TBD_PROPERTY(scalar_type,gap);
        };
      }
    }
  }
}
