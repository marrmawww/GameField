#pragma once
#include <memory>
#include <string>
#include "Health.h"
#include "Armor.h"
#include "Attack.h"

class Unit {
public:
    // виртуальный деструктор нужен при удалении объекта через указатель Unit*
    virtual ~Unit() {}

    // виртуальные методы, каждый конкретный наследник обязан их реализовать
   
    virtual std::unique_ptr<Unit> clone() const = 0;
    virtual std::string getType() const = 0;
    virtual std::string getCategory() const = 0;
    virtual char getSymbol() const = 0;

    int getX() const { return x_; }
    int getY() const { return y_; }
    int getSpeed() const { return speed_; }
    bool isAlive() const { return !health_.isZero(); }

    // используется GameField при добавлении/перемещении юнита
    void setPosition(int x, int y) {
        x_ = x;
        y_ = y;
    }

    bool move(int dx, int dy, int fieldWidth, int fieldHeight);

    // сначала броня уменьшает урон, потом здоровье уменьшается на остаток
    void takeDamage(int rawDamage);

    // атакующий не меняется, меняется только цель, поэтому метод const
    void attackTarget(Unit& target) const;

    void heal(int amount);

    std::string describe() const;

protected:
    
    Unit(const Health& health, const Armor& armor, const Attack& attack,
        int speed, int x, int y);

    Unit(const Unit&) = default;

    Health health_;
    Armor armor_;
    Attack attack_;
    int speed_;
    int x_;
    int y_;
};
