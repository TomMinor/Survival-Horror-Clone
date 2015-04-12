#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include <vector>
#include <string>

#include "Item.h"

typedef std::vector<BaseItem*> ItemArray;

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
                                int _totalUses);

WeaponItem* registerWeaponItem( const std::string& _name,
                                const std::string& _description,
                                const std::string& _meshPath,
                                const std::string& _texturePath,
                                const std::string& _iconPath,
                                unsigned int _clipSize);

AmmoItem* registerAmmoItem(const std::string& _name,
                            const std::string& _description,
                            const std::string& _meshPath,
                            const std::string& _texturePath,
                            const std::string& _iconPath,
                            unsigned int _damage,
                            int _flags);

HealthItem* createHealthItem(const std::string& _name);

PuzzleItem* createPuzzleItem(const std::string& _name);

WeaponItem* createWeaponItem(const std::string& _name);

AmmoItem* createAmmoItem(const std::string& _name);

#endif // ITEMFACTORY_H
