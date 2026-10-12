#include "GameField.h"
#include <stdexcept>
#include <utility>

GameField::GameField(int width, int height, size_t maxUnits) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Поле: ширина и высота должны быть положительными");
    }
    if (maxUnits == 0) {
        throw std::invalid_argument("Поле: максимальное число юнитов должно быть больше 0");
    }
    width_ = width;
    height_ = height;
    maxUnits_ = maxUnits;
}

// Деструктор. vector<unique_ptr<Unit>> и сам бы всё освободил,
// но явный clear() показывает: поле владеет юнитами и удаляет их.
GameField::~GameField() {
    units_.clear();
}

// Копирование: unique_ptr копировать нельзя, поэтому для каждого юнита
// зовём clone() — получаем независимую копию нужного класса.
GameField::GameField(const GameField& other) {
    width_ = other.width_;
    height_ = other.height_;
    maxUnits_ = other.maxUnits_;
    for (size_t i = 0; i < other.units_.size(); ++i) {
        units_.push_back(other.units_[i]->clone());
    }
}

// Присваивание копированием. Проверка this == &other обязательна:
// без неё при a = a мы бы сначала очистили свои юниты, а потом
// попытались клонировать их же — но их уже нет.
GameField& GameField::operator=(const GameField& other) {
    if (this == &other) return *this;

    units_.clear();
    width_ = other.width_;
    height_ = other.height_;
    maxUnits_ = other.maxUnits_;
    for (size_t i = 0; i < other.units_.size(); ++i) {
        units_.push_back(other.units_[i]->clone());
    }
    return *this;
}

// Перемещение: vector забирает свой буфер у other без копирования элементов.
// other остаётся в валидном, но пустом состоянии.
GameField::GameField(GameField&& other) {
    width_ = other.width_;
    height_ = other.height_;
    maxUnits_ = other.maxUnits_;
    units_ = std::move(other.units_);

    other.width_ = 0;
    other.height_ = 0;
    other.maxUnits_ = 0;
}

GameField& GameField::operator=(GameField&& other) {
    if (this == &other) return *this;

    width_ = other.width_;
    height_ = other.height_;
    maxUnits_ = other.maxUnits_;
    units_ = std::move(other.units_);

    other.width_ = 0;
    other.height_ = 0;
    other.maxUnits_ = 0;
    return *this;
}

bool GameField::isCellFree(int x, int y) const {
    for (size_t i = 0; i < units_.size(); ++i) {
        if (units_[i]->getX() == x && units_[i]->getY() == y) return false;
    }
    return true;
}

void GameField::addUnit(std::unique_ptr<Unit> unit) {
    if (unit == nullptr) throw std::invalid_argument("Поле: юнит пустой");
    if (units_.size() >= maxUnits_) throw std::length_error("Поле: достигнут лимит юнитов");
    if (!inBounds(unit->getX(), unit->getY())) throw std::out_of_range("Поле: юнит за границей поля");
    if (!isCellFree(unit->getX(), unit->getY())) throw std::logic_error("Поле: клетка уже занята");
    units_.push_back(std::move(unit));
}

// Отдельный setPosition не нужен: фабрика уже передала x, y в конструктор юнита.
void GameField::addUnit(const UnitFactory& factory, UnitKind kind, int x, int y) {
    std::unique_ptr<Unit> unit = factory.createUnit(kind, x, y);
    addUnit(std::move(unit));
}

void GameField::removeUnitAt(size_t index) {
    if (index >= units_.size()) throw std::out_of_range("Поле: неправильный индекс");
    units_.erase(units_.begin() + index);
}

// Возвращает false, если в такой клетке никого нет — это не ошибка,
// а нормальный результат поиска.
bool GameField::removeUnitAt(int x, int y) {
    for (size_t i = 0; i < units_.size(); ++i) {
        if (units_[i]->getX() == x && units_[i]->getY() == y) {
            units_.erase(units_.begin() + i);
            return true;
        }
    }
    return false;
}

void GameField::removeDead() {
    // Обратный цикл: после erase индексы справа не сдвигаются,
    // значит элементы слева остаются на своих местах.
    for (size_t i = units_.size(); i-- > 0; ) {
        if (!units_[i]->isAlive()) {
            units_.erase(units_.begin() + i);
        }
    }
}

bool GameField::moveUnit(size_t index, int dx, int dy) {
    Unit& unit = getUnit(index);
    int nx = unit.getX() + dx;
    int ny = unit.getY() + dy;

    // Проверяем до изменения состояния: иначе при неудаче
    // пришлось бы откатывать уже сделанный шаг.
    if (!inBounds(nx, ny)) return false;
    if (!isCellFree(nx, ny)) return false;

    return unit.move(dx, dy, width_, height_);
}

void GameField::clear() {
    // unique_ptr автоматически уничтожит всех юнитов.
    units_.clear();
}

Unit& GameField::getUnit(size_t index) {
    if (index >= units_.size()) throw std::out_of_range("Поле: неправильный индекс");
    return *units_[index];
}

const Unit& GameField::getUnit(size_t index) const {
    if (index >= units_.size()) throw std::out_of_range("Поле: неправильный индекс");
    return *units_[index];
}

void GameField::print(std::ostream& os) const {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            char symbol = '.';
            // Ищем, кто стоит на клетке (x, y). Если никто — печатаем '.'.
            for (size_t i = 0; i < units_.size(); ++i) {
                if (units_[i]->getX() == x && units_[i]->getY() == y) {
                    symbol = units_[i]->getSymbol();
                    break;
                }
            }
            os << symbol << ' ';
        }
        os << '\n';
    }
}

// Реализация итератора. Здесь GameField уже определён полностью,
// поэтому можно обращаться к его приватному полю units_.
Unit& GameFieldIterator::operator*() const {
    return *field_->units_[index_];
}

Unit* GameFieldIterator::operator->() const {
    return field_->units_[index_].get();
}
