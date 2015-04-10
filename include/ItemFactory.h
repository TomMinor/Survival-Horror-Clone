#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include <string>

#include "Item.h"

HealthItem* registerHealthItem(const std::string& _name,
                               const std::string& _description,
                               const std::string& _meshPath,
                               const std::string& _texturePath,
                               const std::string& _iconPath,
                               unsigned int _healthPoints);

PuzzleItem* registerPuzzleItem( const std::string& _name,
                                const std::string& _description,
                                const std::string& _meshPath,
                                const std::string& _texturePath,
                                const std::string& _iconPath,
                                bool _isStackable,
                                unsigned int _totalUses);

WeaponItem* registerWeaponItem( const std::string& _name,
                                const std::string& _description,
                                const std::string& _meshPath,
                                const std::string& _texturePath,
                                const std::string& _iconPath,
                                unsigned int _healthPoints);

AmmoItem* registerAmmoItem( const std::string& _name,
                            const std::string& _description,
                            const std::string& _meshPath,
                            const std::string& _texturePath,
                            const std::string& _iconPath,
                            unsigned int _damage,
                            AmmoItem::AmmoFlags _flags);

HealthItem* createHealthItem(const std::string& _name);

PuzzleItem* createPuzzleItem(const std::string& _name);

WeaponItem* createWeaponItem(const std::string& _name);

AmmoItem* createAmmoItem(const std::string& _name);

#endif // ITEMFACTORY_H
