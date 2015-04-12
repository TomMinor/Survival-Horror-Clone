#include "stringUtilities.h"
#include <sstream>
#include <algorithm>

namespace util {

void tokenize(const std::string& _str, TokenStream &_tokens, const std::string& _delimiters)
{
  // Skip delimiters at beginning.
  std::string::size_type lastPos = _str.find_first_not_of(_delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos     = _str.find_first_of(_delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    std::string tmp = _str.substr(lastPos, pos - lastPos);

    // Skip empty strings
    if(tmp.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
    {
        _tokens.push_back(tmp);
    }

    // Skip delimiters.  Note the "not_of"
    lastPos = _str.find_first_not_of(_delimiters, pos);
    // Find next "non-delimiter"
    pos = _str.find_first_of(_delimiters, lastPos);
  }
}

float tokenToFloat(const std::string& _token)
{
  float output = 0.0f;

  if(!_token.empty())
  {
    std::istringstream ss(_token);
    if(!(ss >> output))
    {
      throw std::runtime_error("Could not convert token to float");
    }
  }
  else
  {
    throw std::runtime_error("Empty token when attempting to convert to float");
  }

  return output;
}

void removeEmptyTokens(TokenStream& _tokens)
{
    std::vector<std::string>::iterator it = remove_if(_tokens.begin(), _tokens.end(),mem_fun_ref(&std::string::empty));
    // erase the removed elements
    _tokens.erase(it, _tokens.end());
}

void stripCharacters(std::string& _string, const std::string& _stripChars)
{
    for(auto charToRemove : _stripChars)
    {
        _string.erase(std::remove(_string.begin(), _string.end(), charToRemove), _string.end());
    }
}

}

