#include <map>
#include <string>
#include <vector>

#include "ItemFactory.h"

namespace // Anonymous namespace
{

static std::unordered_map<std::string, SharedItemData*> s_uniqueItems;

static std::vector<BaseItem*> s_itemTypes;

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

BaseItem* createItem(const std::string& _name)
{
    for(std::vector<BaseItem*>::iterator item = s_itemTypes.begin();
        item != s_itemTypes.end();
        item++
        )
    {
        if( (*item)->itemName() == _name )
        {
            return (*item);
        }
    }

    return nullptr;
}

}

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

    HealthItem* result = new HealthItem(_healthPoints, *s_uniqueItems[_name]);

    s_itemTypes.push_back(result);

    return result;
}

PuzzleItem* registerPuzzleItem(
        const std::string& _name,
        const std::string& _description,
        const std::string& _meshPath,
        const std::string& _texturePath,
        const std::string& _iconPath,
        bool _isStackable,
        unsigned int _totalUses
        )
{
    SharedItemData* value = new SharedItemData(_name, _description, _meshPath, _texturePath, _iconPath, _isStackable);
    insertItemIfUnique(_name, value);

    PuzzleItem* result = new PuzzleItem(_totalUses, *s_uniqueItems[_name]);

    s_itemTypes.push_back(result);

    return result;
}

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

    WeaponItem* result = new WeaponItem(_clipSize, *s_uniqueItems[_name]);

    s_itemTypes.push_back(result);

    return result;
}

AmmoItem* registerAmmoItem(const std::string& _name,
        const std::string& _description,
        const std::string& _meshPath,
        const std::string& _texturePath,
        const std::string& _iconPath,
        unsigned int _damage,
        AmmoItem::AmmoFlags _flags
        )
{
    SharedItemData* value = new SharedItemData(_name, _description, _meshPath, _texturePath, _iconPath, true);
    insertItemIfUnique(_name, value);

    AmmoItem* result = new AmmoItem(_damage, _flags, *s_uniqueItems[_name]);

    s_itemTypes.push_back(result);

    return result;
}

HealthItem* createHealthItem(const std::string& _name)
{
    return static_cast<HealthItem*>(createItem(_name));
}

PuzzleItem* createPuzzleItem(const std::string& _name)
{
    return static_cast<PuzzleItem*>(createItem(_name));
}

WeaponItem* createWeaponItem(const std::string& _name)
{
    return static_cast<WeaponItem*>(createItem(_name));
}

AmmoItem* createAmmoItem(const std::string& _name)
{
    return static_cast<AmmoItem*>(createItem(_name));
}
