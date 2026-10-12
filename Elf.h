#pragma once
#include "Unit.h"


class Elf : public Unit {
public:
    std::string getCategory() const override { return "Elf"; }

protected:
    Elf(const Health& h, const Armor& a, const Attack& at,
        int speed, int x, int y)
        : Unit(h, a, at, speed, x, y) {
    }
};

// следопыт — быстрый эльф с хорошей атакой и умеренной броней
class ElfRanger final : public Elf {
public:
    ElfRanger(int x = 0, int y = 0)
        : Elf(Health(85), Armor(3), Attack(19), 3, x, y) {
    }

    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new ElfRanger(*this));
    }
    std::string getType() const override { return "ElfRanger"; }
    char getSymbol() const override { return 'E'; }
};

// друид — более живучий эльф с меньшей скоростью
class ElfDruid final : public Elf {
public:
    ElfDruid(int x = 0, int y = 0)
        : Elf(Health(110), Armor(5), Attack(16), 2, x, y) {
    }

    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new ElfDruid(*this));
    }
    std::string getType() const override { return "ElfDruid"; }
    char getSymbol() const override { return 'D'; }
};
