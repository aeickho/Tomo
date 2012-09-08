#include <iostream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <tbd/log.h>
#include <tbd/misc.hpp>
#include "tomo/GCode.hpp"
#include <boost/algorithm/string.hpp>

using namespace boost;
namespace po = program_options;

using namespace std;
using namespace tomo;

LOG_INIT;



union GCodeInstruction
{
    class GeneralInstruction
    {
      u16 flags_; // 0..4  : G-Code Letter
                  // 5..14 : Number
                  // 15    : Newline Flag
      u16 params_;
    };

    union Parameter
    {
      float float_;
      int   int_;
    } extParam_;

    bool newLine() const { return flags_ & (1 << 31); } 
    void newLine(bool _newLine)
    {
      flags_ = (flags_ & 0x7FFF) | (_newLine << 31);
    }

    char prefix() const
    {
      char _prefix = char((flags_ & 0xFFE0) + 65);
      return (_prefix == '[') ? 42 : _prefix;
    }

    void prefix(char _prefix)
    {
      if (_prefix < 'A' && _prefix > '[') return;
      if (_prefix == '*') _prefix = '[';
      flags_ = (flags_ & 0xFFE0) | (_prefix - 65); 
    }

    u16 number() const
    {
      return (flags_ >> 5) & 0x3FF;
    }

    void number(u16 _number) 
    {
      flags_ = (flags_ & 0x801F) | ((_number & 0x3FF) << 5);
    }

    std::string str() const
    {
      stringstream ss;
      if (newLine) ss << endl;
      ss << prefix();
      ss << number();
      return ss.str();
    }
};

class GCodeSequence
{
public:
  GCodeSequence() : lineNumbers_(false) {}

  void add(GCodeInstruction _instr) { instructions_.push_back(_instr); }

  friend std::ostream& operator<<(std::ostream& _os, const GCodeSequence& _seq)
  {
    lineNumber_ = 0;
    std::vector<GeneralInstruction>::const_iterator it = _seq.instructions().begin();

    while (it != _seq.instructions().end())
    {
      if (it->newLine()) { _os << endl; lineNumber_++; }
      if (lineNumbers()) _os << lineNumber_;
      _os << it->str();
      if (it->prefix() == 'G' && it->number() == '1')
      {
        for (int i = 0; i < 5; i++)
        {
          bool flagSet = (it->params_ & (1 << i)) != 0;
          ++it;
          if (!flagSet) continue;
          if (it == _seq.instructions().end()) return;
          switch (i)
          {
            case 0: _os << " X"; break;
            case 1: _os << " Y"; break;
            case 2: _os << " Z"; break;
            case 3: _os << " E"; break;
            case 4: _os << " F"; break;
          } 
          os << it->extParam_.float_;
        }
      }
      _os << ' ';
      ++it;
    }

    return _os;
  }


  friend std::istream& operator>>(std::istream& _is, GCodeSequence& _seq)
  {
    while (_is.good())
    {
      char _buf[4096];
      _is.getline(_buf,4096);
     
      std::string _splitStr(_buf);
      std::vector<std::string> _splitVec;
      boost::trim(_splitStr);
     
      // Split off comments
      boost::split(_splitVec,_splitStr, boost::is_any_of(";"), boost::token_compress_on);

      if (_splitVec.size() == 0) continue;

      _splitStr = _splitVec[0];
      _splitVec.clear();
      boost::split(_splitVec,_splitStr, boost::is_any_of(" \t"), boost::token_compress_on);

      int _instrNum = 0;
      bool _g1 = false; 
      if (_splitVec.size() == 0) continue;

      BOOST_FOREACH( std::string& s, _splitVec )
      {
        GCodeInstruction _instr;
        if (_instrNum == 0) _instr.newLine(true);

        char _prefix = s[0];

        if (_g1) 
        {
          float _param = atof(s.substr(1,s.length()-1));
          switch (_prefix)
          {
            case 'X': case 'Y': case 'Z': case 'E': case 'F':
              _instr.extParam_.float_ = _param; 
            break;
            default:
          }
        }

        if (!_g1)
        {
          _instr.prefix(_prefix);
          if (_prefix=='M' || _prefix == 'G')
          {
            u16 _number = atoi(s.substr(1,s.length()-1));
            _instr.number(_number);

            _g1 = _number == 1 && _prefix == 'G';
          }
        }
        _instrNum++;
        _seq.add(_instr);
      }
      

    }
    
  }


  TBD_PROPERTY(bool,lineNumbers);
  TBD_PROPERTY_REF(std::vector<GCodeInstruction>,instructions);

private:
  unsigned lineNumber_;
};



int main(int ac, char* av[])
{
  cout << "GCodeTest -- written by Wilston Oreo." << endl;

  stringstream descStr;
  descStr << "Allowed options";

  string inputFile, outputFile;
  int nSlices = 100, resX = 100, resY = 100;

  // Declare the supported options.
  po::options_description desc(descStr.str());

  desc.add_options()
    ("help,h", "Display help message.")
    ("input,i", po::value<string>(&inputFile), "Input OFF file")
    ("output,o", po::value<string>(&outputFile), "Output PPM image file")
    ;

  // Parse the command line arguments for all supported options
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    cout << desc << endl;
    return 1;
  }

  LOG->level(2);

  using namespace tomo::gcode;

  LOG_MSG;

  G<1> G1;

  std::cout << G1;


  return EXIT_SUCCESS;
}

