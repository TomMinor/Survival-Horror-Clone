#ifndef ITEM_H
#define ITEM_H

#include <string>

#include "Texture.h"
#include "md2mesh.h"
#include "FileSystem.h"

#include <unordered_map>

///@todo
/// Factory class for all items read from file
/// Read items from file
/// Read recipes from file

//---------------------------------------- Shared Item Data---------------------------------------

/**
 * @brief The SharedItemData struct is intended to represent the unique shareable data of an ITEM_H
 */
struct SharedItemData
{
    Md2::Mesh* m_mesh;
    Texture* m_icon;
    const std::string m_name;
    const std::string m_description;
    bool m_isStackable;

    SharedItemData(
            const std::string& _name,
            const std::string& _description,
            const std::string& _meshPath,
            const std::string& _texturePath,
            const std::string& _iconPath,
            bool _isStackable
            )
        : m_mesh(new Md2::Mesh( FileSystem::itemPath(_name) + '/' + _meshPath, FileSystem::itemPath(_name) + '/' + _texturePath)),
          m_icon(new Texture( FileSystem::itemPath(_name) + '/' + _iconPath)),
          m_name(_name),
          m_description(_description),
          m_isStackable(_isStackable)
    {;}

    // Default ctor
    SharedItemData()
        : m_mesh(nullptr),
          m_icon(nullptr),
          m_name("NullItem"),
          m_description("NullItem"),
          m_isStackable(false)
    {;}

    ~SharedItemData()
    {
        delete m_icon; m_icon = nullptr;
        delete m_mesh; m_mesh = nullptr;
    }
};

std::ostream & operator<<(std::ostream &_output, const SharedItemData &_v);

//---------------------------------------- Base Item Item ---------------------------------------

class BaseItem
{
    friend class Inventory;

public:
    BaseItem( const SharedItemData* _itemData )
        : m_itemData(_itemData)
    {;}

    virtual ~BaseItem()
    {
        delete m_itemData;
    }

    virtual void use() = 0;

    virtual bool useWith(const BaseItem* _item) = 0;

    inline const SharedItemData* itemData() const { return m_itemData; }

    virtual void print() const;

protected:
    const SharedItemData* m_itemData;
};

//---------------------------------------- Health Item ---------------------------------------

class HealthItem : public BaseItem
{
public:
    HealthItem( unsigned int _healthPoints, const SharedItemData* _itemData )
        : BaseItem(_itemData), m_healthPoints(_healthPoints)
    {;}

    virtual void use();

    virtual bool useWith(const BaseItem* _item);

    virtual void print() const;

private:
    unsigned int m_healthPoints;
};

//---------------------------------------- Weapon Item ---------------------------------------

class WeaponItem : public BaseItem
{
public:
    WeaponItem( unsigned int _clipSize, const SharedItemData* _itemData )
        : BaseItem(_itemData), m_clipSize(_clipSize), m_ammoRemaining(_clipSize)
    {;}

    virtual void use();

    virtual bool useWith(const BaseItem* _item);

    virtual void print() const;

private:
    unsigned int m_clipSize;
    unsigned int m_ammoRemaining;
};

//---------------------------------------- Ammo Item ---------------------------------------

class AmmoItem : public BaseItem
{
public:
    enum AmmoFlags
    {
      NONE          = 0x00,
      HITSCAN       = 0x01,
      EXPLOSIVE     = 0x02,
      INCENDIARY    = 0x04,
      ACID          = 0x08,
      ARROW         = 0x10,
    };

    static const std::unordered_map<std::string, AmmoItem::AmmoFlags> StringToAmmoFlags;

    AmmoItem( unsigned int _damage, int _flags, const SharedItemData* _itemData )
        : BaseItem(_itemData), m_damage(_damage), m_flags(_flags)
    {;}

    virtual void use();

    virtual bool useWith(const BaseItem* _item);

    virtual void print() const;

private:
    unsigned int m_damage;
    int m_flags;
};

//---------------------------------------- Puzzle Item ---------------------------------------

class PuzzleItem : public BaseItem
{
public:
    PuzzleItem( unsigned int _uses, const SharedItemData* _itemData )
        : BaseItem(_itemData), m_usesRemaining(_uses)
    {;}

    virtual void use();

    virtual bool useWith(const BaseItem* _item);

    virtual void print() const;

private:
    int m_usesRemaining;
};


#endif // ITEM_H
