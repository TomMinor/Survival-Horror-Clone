#include "Item.h"

const std::unordered_map<std::string, AmmoItem::AmmoFlags> AmmoItem::StringToAmmoFlags = {
    { "NONE",         AmmoItem::NONE },
    { "HITSCAN",      AmmoItem::HITSCAN },
    { "EXPLOSIVE",    AmmoItem::EXPLOSIVE },
    { "INCENDIARY",   AmmoItem::INCENDIARY },
    { "ACID",         AmmoItem::ACID },
    { "ARROW",        AmmoItem::ARROW }
};

std::ostream & operator<<(std::ostream &_output, const SharedItemData &_v)
{
    _output <<
    "[" << _v.m_name << "]\n" <<
    "Description : " << _v.m_description << "\n" <<
    "Stackable? " << (_v.m_isStackable ? "Yes" : "No");

    return _output;
}

void BaseItem::print() const
{
    std::cout << *m_itemData;
}

void WeaponItem::print() const
{
    BaseItem::print();

    std::cout << "\n" <<
    "Clip Size : " << m_clipSize << "\n" <<
    "Remaining Ammo : " << m_ammoRemaining << std::endl;
}

void AmmoItem::print() const
{
    BaseItem::print();

    std::cout << "\n" <<
    "Damage: " << m_damage << "\n"
    "Flags : " << m_flags << std::endl;;
}

void HealthItem::print() const
{
    BaseItem::print();

    std::cout << "\n" <<
    "Heal Points : " << m_healthPoints << std::endl;;
}

void PuzzleItem::print() const
{
    BaseItem::print();

    std::cout << "\n" <<
    "Uses : " << m_usesRemaining << std::endl;;
}


void WeaponItem::use()
{
    // Equip
}

void AmmoItem::use()
{
    // Can't be used on it's own
}

void HealthItem::use()
{
    // Heal player
}

void PuzzleItem::use()
{

}

bool WeaponItem::useWith(const BaseItem *_item)
{

}

bool AmmoItem::useWith(const BaseItem *_item)
{

}

bool HealthItem::useWith(const BaseItem *_item)
{

}

bool PuzzleItem::useWith(const BaseItem *_item)
{

}
