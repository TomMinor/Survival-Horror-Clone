#include <fstream>

#include "ItemParser.h"
#include "ItemFactory.h"
#include "stringUtilities.h"
#include <algorithm>

struct GeneralItemData
{
    std::string name;
    std::string description;
    std::string iconFileName;
    std::string meshFileName;
    std::string meshTextureFileName;
};

GeneralItemData readItemData(util::TokenStream::iterator& _tokenStream, const util::TokenStream::iterator& _end, unsigned int _line)
{
    if( (_tokenStream + 5) > _end)
    {
        std::string message("Line " + std::to_string(_line) + " Couldn't load item data : too few tokens");
        throw std::runtime_error(message);
    }

    GeneralItemData data;
    data.name = (*_tokenStream++);
    data.description = (*_tokenStream++);
    data.iconFileName = (*_tokenStream++);
    data.meshFileName = (*_tokenStream++);
    data.meshTextureFileName = (*_tokenStream++);

    return data;
}

void parseItemManifest(const std::string& _manifestFilePath, ItemArray &_items)
{
    std::ifstream file;
    file.open(_manifestFilePath, std::ios::in);

    if( !file.is_open() )
    {
      throw std::runtime_error("Error opening file");
    }

    unsigned int lineCounter = 0;
    std::string line;
    while(std::getline(file, line))
    {
        lineCounter++;

        util::TokenStream tokens;
        util::tokenize(line, tokens, "'\t");

        for(auto& token : tokens)
        {
            util::trim(token);
        }

        // Ignore comments and empty lines
        if(tokens.size() > 0 && tokens[0][0] != '#')
        {
            util::TokenStream::iterator token = tokens.begin();

            if(*token == "HEALTH")
            {
                GeneralItemData itemData = readItemData( ++token, tokens.end(), lineCounter);

                unsigned int healthPoints;

                try
                {
                    healthPoints = util::tokenToFloat( *token++ );
                }
                catch(std::runtime_error& e)
                {
                    throw e;
                }

                auto tmp = registerHealthItem(itemData.name,
                                               itemData.description,
                                               itemData.meshFileName,
                                               itemData.meshTextureFileName,
                                               itemData.iconFileName,
                                               healthPoints);

                if(tmp)
                    _items.push_back(tmp);
                else
                    std::cerr << "Error constructing health item" << std::endl;
            }
            else if(*token == "KEY")
            {
                GeneralItemData itemData = readItemData( ++token, tokens.end(), lineCounter);

                unsigned int maximumUses;

                try
                {
                    maximumUses = util::tokenToFloat( *token++ );
                }
                catch(std::runtime_error& e)
                {
                    throw e;
                }

                auto tmp = registerPuzzleItem(itemData.name,
                                              itemData.description,
                                              itemData.meshFileName,
                                              itemData.meshTextureFileName,
                                              itemData.iconFileName,
                                              false, ///@todo Modify file format to store isStackable
                                              maximumUses);

                if(tmp)
                    _items.push_back(tmp);
                else
                    std::cerr << "Error constructing puzzle item" << std::endl;
            }
            else if(*token == "AMMO")
            {
                GeneralItemData itemData = readItemData( ++token, tokens.end(), lineCounter);

                int flags = AmmoItem::NONE;
                unsigned int damage;

                try
                {
                    damage = util::tokenToFloat( *token++ );

                    for( util::TokenStream::iterator flagToken = token; flagToken != tokens.end(); flagToken++ )
                    {
                        std::cout << *flagToken << std::endl;
                        if( AmmoItem::StringToAmmoFlags.count(*flagToken) > 0 )
                        {
                            flags |= AmmoItem::StringToAmmoFlags.at(*token);
                        }
                    }
                }
                catch(std::runtime_error& e)
                {
                    throw e;
                }

                auto tmp = registerAmmoItem(itemData.name,
                                            itemData.description,
                                            itemData.meshFileName,
                                            itemData.meshTextureFileName,
                                            itemData.iconFileName,
                                            damage,
                                            flags);
                if(tmp)
                    _items.push_back(tmp);
                else
                    std::cerr << "Error constructing ammo item" << std::endl;
            }
            else if(*token == "WEAPON")
            {
                GeneralItemData itemData = readItemData( ++token, tokens.end(), lineCounter);

                unsigned int clipSize;

                try
                {
                    clipSize = util::tokenToFloat( *token++ );
                }
                catch(std::runtime_error& e)
                {
                    throw e;
                }

                auto tmp = registerWeaponItem(itemData.name,
                                              itemData.description,
                                              itemData.meshFileName,
                                              itemData.meshTextureFileName,
                                              itemData.iconFileName,
                                              clipSize);

                if(tmp)
                    _items.push_back(tmp);
                else
                    std::cerr << "Error constructing weapon item" << std::endl;
            }
            else
            {
                std::cerr << "Unknown line type on line " << lineCounter << std::endl;
            }
        }
    }
}
