#pragma once
#include <stdexcept>
#include "Armor.h"

// сила атаки
class Attack {
public:
    explicit Attack(int value) : value_(value) {
        if (value < 0) throw std::invalid_argument("Атака: значение не может быть меньше 0");
    }

   
    int calculateDamage(const Armor& targetArmor) const {
        return targetArmor.reduceDamage(value_);
    }

    int getValue() const { return value_; }

private:
    const int value_;
};
