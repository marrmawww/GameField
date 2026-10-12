#pragma once
#include <algorithm>
#include <stdexcept>

// броня уменьшает входящий урон
class Armor {
public:
    explicit Armor(int value) : value_(value) {
        if (value < 0) throw std::invalid_argument("Броня: значение не может быть меньше 0");
    }

    // урон после брони не может стать отрицательным
    int reduceDamage(int damage) const {
        if (damage < 0) throw std::invalid_argument("Броня: урон не может быть меньше 0");
        return (std::max)(0, damage - value_);
    }

    int getValue() const { return value_; }

private:
    const int value_;
};
