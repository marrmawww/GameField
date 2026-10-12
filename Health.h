#pragma once
#include <stdexcept>

// здоровье юнита: текущее значение + максимум, выше которого лечение не работает
// класс изменяемый (в отличие от Armor/Attack), потому что здоровье меняется по ходу игры
class Health {
public:
    explicit Health(int value) {
        if (value < 0) throw std::invalid_argument("Здоровье: значение не может быть меньше 0");
        value_ = value;
        max_ = value;
    }

    void decrease(int amount) {
        if (amount < 0) throw std::invalid_argument("Здоровье: урон не может быть меньше 0");
        value_ = value_ - amount;
        if (value_ < 0) value_ = 0;
    }


    void increase(int amount) {
        if (amount < 0) throw std::invalid_argument("Здоровье: лечение не может быть меньше 0");
        value_ = value_ + amount;
        if (value_ > max_) value_ = max_;
    }

    bool isZero() const { return value_ == 0; }
    int getValue() const { return value_; }
    int getMax() const { return max_; }

private:
    int value_;
    int max_;
};
