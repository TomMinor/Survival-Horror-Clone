#ifndef ITEMPARSER_H
#define ITEMPARSER_H

#include <string>

#include "ItemFactory.h"

void parseItemManifest(const std::string& _manifestFilePath, ItemArray& _items);

#endif // ITEMPARSER_H
