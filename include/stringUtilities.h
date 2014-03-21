#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

namespace stringUtils {

#include <iostream>

// Taken from Jon Macey's OBJ importer : http://nccastaff.bournemouth.ac.uk/jmacey/PPP/index.html
void tokenize(const std::string& _str,std::vector<std::string>& _tokens,const std::string& _delimiters);

}

#endif // STRINGUTILITIES_H


