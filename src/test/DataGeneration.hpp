#pragma once

struct 



/// create test polygon
void makeCircle(Ring& _ring,
                 Point2f _center,
                 float _radius,
                 int _numSegments = 24,
                 bool _inverse = false)
{
  _ring.clear();
  for (int i = 0; i <= _numSegments; i++)
  {
    float _angle = float(i)/float(_numSegments)*M_PI*2.0;
    float _sin = _radius*sin(_angle),
          _cos = _radius*cos(_angle);

    if (_inverse) _sin = -_sin;
    _ring.push_back(Point2f(_cos,_sin) + _center);
  }
  boost::geometry::correct(_ring);
}

/// create test polygon
void makeIrregularRing(Ring& _ring,
                 Point2f _center,
                 float _radius,
                 int _numSegments = 24,
                 bool _inverse = false)
{
  _ring.clear();
  for (int i = 0; i <= _numSegments; i++)
  {
    float _angle = float(i)/float(_numSegments)*M_PI*2.0;
    float _sin = _radius*sin(_angle),
          _cos = _radius*cos(_angle);

    if (i < _numSegments/4 || (i > _numSegments/2 && i <= 3*_numSegments/4))
    {
      _sin *= 0.5;
      _cos *= 0.5;
    }

    if (i == 0)
    {
      _sin *= 0.1;
      _cos *= 0.1;
    }

    if (_inverse) _sin = -_sin;
    _ring.add(Point2f(_cos,_sin) + _center);
  }

  boost::geometry::correct(_ring);
}

void makePolygonWithHoles(Polygon& _polygon)
{
  Ring _ring;
  makePolygon(_ring,Point2f(0.5,0.5),0.4);

  Polygon _polygon(_ring);
  
  makeCircle(_ring,Point2f(0.3,0.7),0.1);
  _polygon.holes().push_back(_ring);
  makeCircle(_ring,Point2f(0.6,0.35),0.07);
  _polygon.holes().push_back(_ring);

}

