#include "UnitFactory.h"
#include <stdexcept>
#include "Infantry.h"
#include "Archer.h"
#include "Cavalry.h"
#include "Elf.h"

std::string toString(UnitKind kind) {
    if (kind == UnitKind::Swordsman)   return "Мечник";
    if (kind == UnitKind::Spearman)    return "Копейщик";
    if (kind == UnitKind::LightArcher) return "Лёгкий лучник";
    if (kind == UnitKind::HeavyArcher) return "Тяжёлый лучник";
    if (kind == UnitKind::Cataphract)  return "Катафракт";
    if (kind == UnitKind::HorseArcher) return "Конный лучник";
    if (kind == UnitKind::ElfRanger)  return "Эльфийский следопыт";
    if (kind == UnitKind::ElfDruid)   return "Эльфийский друид";
    return "Неизвестный";
}

// Каждая фабрика проверяет только «свои» виды. Для «чужих» бросает исключение —
// так попытка создать несовместимого юнита сразу видна, а не превращается в баг позже.
std::unique_ptr<Unit> InfantryFactory::createUnit(UnitKind kind, int x, int y) const {
    if (kind == UnitKind::Swordsman) return std::unique_ptr<Unit>(new Swordsman(x, y));
    if (kind == UnitKind::Spearman)  return std::unique_ptr<Unit>(new Spearman(x, y));
    throw std::invalid_argument(name() + " не умеет создавать " + toString(kind));
}

std::unique_ptr<Unit> ArcherFactory::createUnit(UnitKind kind, int x, int y) const {
    if (kind == UnitKind::LightArcher) return std::unique_ptr<Unit>(new LightArcher(x, y));
    if (kind == UnitKind::HeavyArcher) return std::unique_ptr<Unit>(new HeavyArcher(x, y));
    throw std::invalid_argument(name() + " не умеет создавать " + toString(kind));
}

std::unique_ptr<Unit> CavalryFactory::createUnit(UnitKind kind, int x, int y) const {
    if (kind == UnitKind::Cataphract)  return std::unique_ptr<Unit>(new Cataphract(x, y));
    if (kind == UnitKind::HorseArcher) return std::unique_ptr<Unit>(new HorseArcher(x, y));
    throw std::invalid_argument(name() + " не умеет создавать " + toString(kind));
}


// Фабрика эльфов создаёт только два класса категории Elf.
std::unique_ptr<Unit> ElfFactory::createUnit(UnitKind kind, int x, int y) const {
    if (kind == UnitKind::ElfRanger) return std::unique_ptr<Unit>(new ElfRanger(x, y));
    if (kind == UnitKind::ElfDruid)  return std::unique_ptr<Unit>(new ElfDruid(x, y));
    throw std::invalid_argument(name() + " не умеет создавать " + toString(kind));
}
