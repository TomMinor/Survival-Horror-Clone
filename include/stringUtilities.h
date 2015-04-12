#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace util {

typedef std::vector<std::string> TokenStream;

// Taken from Jon Macey's OBJ importer : http://nccastaff.bournemouth.ac.uk/jmacey/PPP/index.html
void tokenize(const std::string& _str, TokenStream& _tokens,const std::string& _delimiters);

float tokenToFloat(const std::string& _token);

void removeEmptyTokens(TokenStream& _tokens);

void stripCharacters(std::string& _string, const std::string &_stripChars);

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}


}

int main2();

#endif // STRINGUTILITIES_H


