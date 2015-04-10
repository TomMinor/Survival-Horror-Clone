#include "Item.h"

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
