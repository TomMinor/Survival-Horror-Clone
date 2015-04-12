#include <map>
#include <string>

#include "ItemFactory.h"

namespace // Anonymous namespace
{

//---------------------------------------- Static Global Datastructures ---------------------------------------
typedef std::unordered_map<std::string, SharedItemData*> UniqueItemMap;

static UniqueItemMap s_uniqueItems;
static ItemArray s_itemTypes;

/**
 * @brief insertItemIfUnique
 * @param _name
 * @param _value
 */
void insertItemIfUnique(const std::string& _name,  SharedItemData* _value)
{
    if( s_uniqueItems.find(_name) != s_uniqueItems.end() )
    {
        std::cerr << "Item name " << _name << " is not unique, ignoring" << std::endl;
    }
    else
    {
        s_uniqueItems.insert( std::map<std::string, SharedItemData*>::value_type(_name, _value));
    }
}


/**
 * @brief createItem
 * @param _name
 * @return
 */
BaseItem* createItem(const std::string& _name)
{
    for(ItemArray::iterator item = s_itemTypes.begin(); item != s_itemTypes.end(); item++)
    {
        if( (*item)->itemName() == _name )
        {
            return (*item);
        }
    }

    return nullptr;
}

}

//----------------------------------------------- Health Item -----------------------------------------------

HealthItem* registerHealthItem(
        const std::string& _name,
        const std::string& _description,
        const std::string& _meshPath,
        const std::string& _texturePath,
        const std::string& _iconPath,
        unsigned int _healthPoints
        )
{
    SharedItemData* value = new SharedItemData(_name, _description, _meshPath, _texturePath, _iconPath, false);
    insertItemIfUnique(_name, value);

    HealthItem* result = new HealthItem(_healthPoints, s_uniqueItems[_name]);

    s_itemTypes.push_back(result);

    return result;
}

//----------------------------------------------- Puzzle Item -----------------------------------------------

PuzzleItem* registerPuzzleItem(const std::string& _name,
        const std::string& _description,
        const std::string& _meshPath,
        const std::string& _texturePath,
        const std::string& _iconPath,
        bool _isStackable,
        int _totalUses
        )
{
    SharedItemData* value = new SharedItemData(_name, _description, _meshPath, _texturePath, _iconPath, _isStackable);
    insertItemIfUnique(_name, value);

    PuzzleItem* result = new PuzzleItem(_totalUses, s_uniqueItems[_name]);

    s_itemTypes.push_back(result);

    return result;
}

//----------------------------------------------- Weapon Item -----------------------------------------------

WeaponItem* registerWeaponItem(
        const std::string& _name,
        const std::string& _description,
        const std::string& _meshPath,
        const std::string& _texturePath,
        const std::string& _iconPath,
        unsigned int _clipSize
        )
{
    SharedItemData* value = new SharedItemData(_name, _description, _meshPath, _texturePath, _iconPath, false);
    insertItemIfUnique(_name, value);

    WeaponItem* result = new WeaponItem(_clipSize, s_uniqueItems[_name]);

    s_itemTypes.push_back(result);

    return result;
}

//----------------------------------------------- Ammo Item -----------------------------------------------

AmmoItem* registerAmmoItem(const std::string& _name,
        const std::string& _description,
        const std::string& _meshPath,
        const std::string& _texturePath,
        const std::string& _iconPath,
        unsigned int _damage,
        int _flags
        )
{
    SharedItemData* value = new SharedItemData(_name, _description, _meshPath, _texturePath, _iconPath, true);
    insertItemIfUnique(_name, value);

    AmmoItem* result = new AmmoItem(_damage, _flags, s_uniqueItems[_name]);

    s_itemTypes.push_back(result);

    return result;
}

//------------------------------------------ Item Creation Wrappers -------------------------------------

HealthItem* createHealthItem(const std::string& _name)
{
    return dynamic_cast<HealthItem*>(createItem(_name));
}

PuzzleItem* createPuzzleItem(const std::string& _name)
{
    return dynamic_cast<PuzzleItem*>(createItem(_name));
}

WeaponItem* createWeaponItem(const std::string& _name)
{
    return dynamic_cast<WeaponItem*>(createItem(_name));
}

AmmoItem* createAmmoItem(const std::string& _name)
{
    return dynamic_cast<AmmoItem*>(createItem(_name));
}
