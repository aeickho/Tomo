#pragma once
#include <tbd/property.h>
#include <iostream>
#include <boost/ptr_container/ptr_vector.hpp>

namespace tomo
{
  namespace gcode
  {
    typedef  double DEFAULT_PARAMETER;

    struct Token
    {
      virtual char letter() const = 0;
      virtual std::string parameter_str() const = 0;

      std::string str() const
      {
        std::stringstream ss; 
        ss << letter() << parameter_str(); 
        return ss.str();
      }
    };

    template <char LETTER>
    struct CodeField : public Token
    {
      char letter() const
      {
        return LETTER;
      }
    };
    
    template <char LETTER, typename PARAMETER>
    struct Parameter : public CodeField<LETTER>
    {
      Parameter() {}
      Parameter(PARAMETER _parameter) : parameter_(_parameter) {}

      friend std::ostream& operator<<(std::ostream& _os, Parameter<LETTER,PARAMETER>& _i) 
      {
        return _os << _i.str() << ' ';
      }
   
      std::string parameter_str() const 
      { 
        std::stringstream ss; 
        ss << parameter();
        return ss.str();
      }

      TBD_PROPERTY(PARAMETER,parameter);
    };

    typedef Parameter<'X',DEFAULT_PARAMETER> X;
    typedef Parameter<'Y',DEFAULT_PARAMETER> Y;
    typedef Parameter<'Z',DEFAULT_PARAMETER> Z;
    typedef Parameter<'E',DEFAULT_PARAMETER> E;
    typedef Parameter<'F',DEFAULT_PARAMETER> F;
    typedef Parameter<'P',DEFAULT_PARAMETER> P;
    typedef Parameter<'R',DEFAULT_PARAMETER> R;
    typedef Parameter<'S',DEFAULT_PARAMETER> S;
    typedef Parameter<'N',int> N;

    struct Line
    { 
      TBD_PROPERTY_REF(std::vector<Token*>,parameters);
    };

    template <char LETTER, int NUMBER>
    struct Command : public CodeField<LETTER>, Line
    { 
      typedef std::vector<Token*> ParamCtnr;

      int number() const
      {
        return NUMBER;
      };

      std::string parameter_str() const 
      { 
        std::stringstream ss; 
        ss << number();
        return ss.str();
      }

      friend std::ostream& operator<<(std::ostream& _os, Command<LETTER,NUMBER>& _i) 
      {
        _os << _i.str();
        ParamCtnr& _parameters = _i.parameters();
        for (ParamCtnr::iterator it = _parameters.begin(); it != _parameters.end(); ++it)
          _os << ' ' << (*it)->str();
        _os << "\r\n";
        return _os;
      }
    };

    // General standard G-Code command
    template <int NUMBER>
    struct G : Command<'G',NUMBER> {};

    // Reprap specific command
    template <int NUMBER>
    struct M : Command<'M',NUMBER> {};
  }

  typedef boost::ptr_vector<gcode::Line> GCodeInstructions;
}

