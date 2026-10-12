#include "Unit.h"
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

Unit::Unit(const Health& health, const Armor& armor, const Attack& attack,
    int speed, int x, int y)
    : health_(health),
      armor_(armor),
      attack_(attack),
      speed_(speed),
      x_(x),
      y_(y) {
    if (speed < 0) throw std::invalid_argument("Юнит: скорость не может быть меньше 0");
}

bool Unit::move(int dx, int dy, int fieldWidth, int fieldHeight) {
    // Длина шага — максимум из |dx| и |dy| (можно ходить по диагонали).
    // Например, (2, 1) — это шаг длиной 2, а не 3.
    int step = std::abs(dx);
    if (std::abs(dy) > step) step = std::abs(dy);

    // Скорость ограничивает длину шага.
    if (step > speed_) return false;

    // Сначала считаем новые координаты в локальных переменных.
    int nx = x_ + dx;
    int ny = y_ + dy;

    if (nx < 0 || nx >= fieldWidth) return false;
    if (ny < 0 || ny >= fieldHeight) return false;

    // Только теперь меняем состояние — все проверки уже пройдены.
    x_ = nx;
    y_ = ny;
    return true;
}

void Unit::takeDamage(int rawDamage) {
    // Броня сама решает, сколько урона она поглотит.
    int damage = armor_.reduceDamage(rawDamage);
    health_.decrease(damage);
}

void Unit::attackTarget(Unit& target) const {
    // target.armor_ доступен, потому что attackTarget — метод самого Unit.
    // Приватность/защищённость членов класса в C++ работает на уровне класса.
    int damage = attack_.calculateDamage(target.armor_);
    target.health_.decrease(damage);
}

void Unit::heal(int amount) {
    health_.increase(amount);
}

std::string Unit::describe() const {
    // ostringstream — как cout, но пишет в строку, а не в консоль.
    // getType()/getCategory() виртуальные: у Swordsman вернут своё значение.
    std::ostringstream out;
    out << getType() << " [" << getCategory() << "]"
        << " (" << x_ << "," << y_ << ")"
        << " ЗД " << health_.getValue() << "/" << health_.getMax()
        << " БРО " << armor_.getValue()
        << " АТК " << attack_.getValue()
        << " СКР " << speed_;
    return out.str();
}
