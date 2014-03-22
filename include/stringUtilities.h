#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

#include <iostream>
#include <stdexcept>
#include <vector>

namespace stringUtils {

// Taken from Jon Macey's OBJ importer : http://nccastaff.bournemouth.ac.uk/jmacey/PPP/index.html
void tokenize(const std::string& _str,std::vector<std::string>& _tokens,const std::string& _delimiters);

float tokenToFloat(const std::string& _token);

}

#endif // STRINGUTILITIES_H


