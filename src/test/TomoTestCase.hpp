#pragma once

#include "tomo/draw/magick/Wrapper.hpp"
#include "tomo/misc.hpp"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

struct TomoTestCase
{
  typedef tomo::geometry::prim::Ring Ring;
  typedef tomo::geometry::prim::Polygon Polygon;
  typedef tomo::geometry::prim::MultiPolygon MultiPolygon;
  typedef tomo::geometry::prim::MultiLineString MultiLineString;
  typedef tomo::geometry::base::Point2f Point2f;
  typedef tomo::geometry::base::Vec2f Vec2f;
  typedef tomo::geometry::base::Point3f Point3f;
  typedef tomo::draw::magick::Wrapper Wrapper;

#define RES 1024
  TomoTestCase() : wrapper_(RES,RES), n_(0)
  {
    wrapper_.scale(tomo::geometry::base::Vec2f(RES,RES));
  }

  std::string name()
  {
    std::vector<std::string> _splitVec;
    std::string _name(boost::unit_test::framework::current_test_case().p_name);
    boost::algorithm::split( _splitVec, _name, boost::is_any_of("_"), boost::token_compress_on );
    TOMO_ASSERT(_splitVec.size() > 0);
    return _splitVec[0];
  }

  std::string outputDir()
  {
    return std::string("testdata/");
  }

  void writeImage(std::string _name = std::string(), bool _numbers = true)
  {
    if (wrapper_.image().rows() * wrapper_.image().columns() > 1)
      wrapper_.image().write(testOutputName(_name+".png",_numbers));
  }

  TBD_PROPERTY_REF(Wrapper,wrapper)
  TBD_PROPERTY_REF(int,n)
private:
  std::string testOutputName(std::string _ext, bool _numbers)
  {
    std::stringstream ss;
    ss << outputDir() << name();
    if (_numbers)
    {
      ss << std::setw(3) << std::setfill('0') << ++n_;
    }
    ss << _ext;
    return ss.str();
  }
};

#define TOMO_TEST_CASE( test_name, resolution ) \
  BOOST_FIXTURE_TEST_CASE( test_name##_test, TomoTestCase ) \
  {\
    do {\
      typedef tomo::test::generators::test_name generator_type;\
      generator_type _generator;\
      generator_type::ObjectList&& _objs = _generator.generate();\
      int _nObj = 0;\
      int _res = resolution;\
      for ( const generator_type::Object& _obj : _objs )\
      {\
        Wrapper& _w = wrapper();\
        _w.clear(_res,_res);\
        _generator.draw(_w,_obj);\
        std::stringstream ss; ss << "_testobj" << std::setw(3) << std::setfill('0') << ++_nObj;\
        writeImage(ss.str(),false);\
        _w.clear(_res,_res);

#define TOMO_TEST_CASE_END }} while(0); }

