#ifndef ITEM_H
#define ITEM_H

#include <string>

#include "include/Texture.h"
#include "include/md2mesh.h"

#include <unordered_map>

///@todo
/// Factory class for all items read from file
/// Read items from file
/// Read recipes from file

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
        : m_mesh(new Md2::Mesh(_meshPath, _texturePath)),
          m_icon(new Texture(_iconPath)),
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


class BaseItem
{
    friend class Inventory;

public:
    BaseItem( const SharedItemData& _itemData )
        : m_itemData(_itemData)
    {;}

    virtual void use() = 0;

    virtual bool useWith(const BaseItem* _item) = 0;

    inline std::string itemName() const { return m_itemData.m_name; }

private:
    const SharedItemData& m_itemData;
};


class HealthItem : public BaseItem
{
public:
    HealthItem( unsigned int _healthPoints, const SharedItemData& _itemData )
        : BaseItem(_itemData), m_healthPoints(_healthPoints)
    {;}

    virtual void use();

    virtual bool useWith(const BaseItem* _item);

private:
    unsigned int m_healthPoints;
};


class WeaponItem : public BaseItem
{
public:
    WeaponItem( unsigned int _clipSize, const SharedItemData& _itemData )
        : BaseItem(_itemData), m_clipSize(_clipSize), m_ammoRemaining(_clipSize)
    {;}

    virtual void use();

    virtual bool useWith(const BaseItem* _item);

private:
    unsigned int m_clipSize;
    unsigned int m_ammoRemaining;
};


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
      ARROW         = 0x10
    };

    AmmoItem( unsigned int _damage, AmmoFlags _flags, const SharedItemData& _itemData )
        : BaseItem(_itemData), m_damage(_damage), m_flags(_flags)
    {;}

    virtual void use();

    virtual bool useWith(const BaseItem* _item);

private:
    unsigned int m_damage;
    AmmoFlags m_flags;
};


class PuzzleItem : public BaseItem
{
public:
    PuzzleItem( unsigned int _uses, const SharedItemData& _itemData )
        : BaseItem(_itemData), m_usesRemaining(_uses)
    {;}

    virtual void use();

    virtual bool useWith(const BaseItem* _item);

private:
    unsigned int m_usesRemaining;
};


#endif // ITEM_H
