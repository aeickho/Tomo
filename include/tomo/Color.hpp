#pragma once

namespace tomo
{
  template<int CHANNELS, class VALUE> struct Color
  {
    static const int channels_ = CHANNELS;
    typedef VALUE Value;
    static const Value maxValue() { return valueMax(Value()); }
    Color()
    {
      BOOST_STATIC_ASSERT(channels_ >= 3 && channels_ <= 4);
    }
    Color(Value _r, Value _g, Value _b )
    {
      BOOST_STATIC_ASSERT(channels_ >= 3 && channels_ <= 4);
      v_[0] = _r;
      v_[1] = _g;
      v_[2] = _b;
      if( CHANNELS > 3 )
        v_[3] = maxValue();
    }
    Color(Value _r, Value _g, Value _b, Value _a )
    {
      BOOST_STATIC_ASSERT(channels_ == 4);
      v_[0] = _r;
      v_[1] = _g;
      v_[2] = _b;
      v_[3] = maxValue();
    }
    const Value& r() const
    {
      return v_[0];
    }
    void r( const Value& _value ) 
    { 
      v_[0] = _value;
    }
    const Value& g() const
    {
      return v_[1];
    }
    void g( const Value& _value ) 
    { 
      v_[1] = _value;
    }
    const Value& b() const
    {
      return v_[2];
    }
    void b( const Value& _value ) 
    { 
      v_[2] = _value;
    }
    const Value& a() const
    {
      BOOST_STATIC_ASSERT(channels_ >= 4);
      return v_[3];
    }
    void a( const Value& _value ) 
    { 
      BOOST_STATIC_ASSERT(channels_ >= 4);
      v_[3] = _value;
    }

    const Value* values() const { return v_; }
    Value* values() { return v_; }
    operator const Value*() const { return v_; }
    operator Value*() { return v_; }
  protected:
    static const float valueMax( float ) {
      return 1.0f;
    }
    static const double valueMax( double ) {
      return 1.0;
    }
    static const int valueMax( int ) {
      return 255;
    }
    Value v_[channels_];
  };
}

