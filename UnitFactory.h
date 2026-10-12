#pragma once
#include <memory>
#include <string>
#include "Unit.h"

// Список всех видов юнитов. enum class — чтобы значения не попадали в int.
enum class UnitKind {
    Swordsman, Spearman,
    LightArcher, HeavyArcher,
    Cataphract, HorseArcher,
    ElfRanger, ElfDruid
};

// Превратить значение перечисления в строку — нужно для сообщений об ошибках.
std::string toString(UnitKind kind);

// Базовый класс для всех фабрик.
//
// Выбран Фабричный метод, а не Абстрактная фабрика:
// у нас нет нескольких семейств взаимосвязанных объектов, есть категории юнитов.
class UnitFactory {
public:
    virtual ~UnitFactory() {}

    virtual std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const = 0;

    virtual std::string name() const = 0;
};

// Фабрика пехоты — умеет создавать только Swordsman и Spearman.
class InfantryFactory final : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const override;
    std::string name() const override { return "InfantryFactory"; }
};

// Фабрика лучников — только LightArcher и HeavyArcher.
class ArcherFactory final : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const override;
    std::string name() const override { return "ArcherFactory"; }
};

// Фабрика конницы — только Cataphract и HorseArcher.
class CavalryFactory final : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const override;
    std::string name() const override { return "CavalryFactory"; }
};

// Фабрика эльфов — создаёт ElfRanger и ElfDruid.
class ElfFactory final : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const override;
    std::string name() const override { return "ElfFactory"; }
};
