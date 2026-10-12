#pragma once
#include <cstddef>
#include "Unit.h"

class GameField;  


// не хранит копию списка, а хранит указатель на поле и текущий индекс.
class GameFieldIterator {
public:
    GameFieldIterator(GameField* field, size_t index) {
        field_ = field;
        index_ = index;
    }

    Unit& operator*() const;
    Unit* operator->() const;

    // префиксный ++it — вернуть себя после увеличения
    GameFieldIterator& operator++() {
        index_ = index_ + 1;
        return *this;
    }

    // копия нужна потому, что вызывающий ждёт именно старое значение
    GameFieldIterator operator++(int) {
        GameFieldIterator copy = *this;
        index_ = index_ + 1;
        return copy;
    }

    // два итератора равны, если указывают на одно поле и один индекс
    bool operator==(const GameFieldIterator& other) const {
        return field_ == other.field_ && index_ == other.index_;
    }

    bool operator!=(const GameFieldIterator& other) const {
        return !(*this == other);
    }

private:
    GameField* field_;
    size_t index_;
};
