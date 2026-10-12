#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Unit.h"
#include "UnitFactory.h"
#include "GameFieldIterator.h"

// игровое поле это контейнер юнитов на прямоугольной сетке
//
// Хранение: vector<unique_ptr<Unit>>. unique_ptr выбран потому, что поле
// единолично владеет юнитами, а абстрактный Unit
// нельзя хранить по значению только через указатель.

class GameField {
public:
    GameField(int width, int height, size_t maxUnits);

    // Правило пяти.
    ~GameField();
    GameField(const GameField& other);
    GameField& operator=(const GameField& other);
    GameField(GameField&& other);
    GameField& operator=(GameField&& other);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    size_t size() const { return units_.size(); }
    size_t maxUnits() const { return maxUnits_; }
    bool empty() const { return units_.empty(); }

    bool inBounds(int x, int y) const {
        return x >= 0 && x < width_ && y >= 0 && y < height_;
    }

 
    bool isCellFree(int x, int y) const;

  
    void addUnit(std::unique_ptr<Unit> unit);

    // создать юнита через фабрику и сразу поставить
    void addUnit(const UnitFactory& factory, UnitKind kind, int x, int y);

    // удаление по индексу и по координатам
    void removeUnitAt(size_t index);
    bool removeUnitAt(int x, int y);

    // удалить всех погибших за один проход
    void removeDead();


    bool moveUnit(size_t index, int dx, int dy);

    // полностью удалить всех юнитов
    void clear();

    Unit& getUnit(size_t index);
    const Unit& getUnit(size_t index) const;

    void print(std::ostream& os) const;


    GameFieldIterator begin() { return GameFieldIterator(this, 0); }
    GameFieldIterator end() { return GameFieldIterator(this, units_.size()); }

private:
    int width_;
    int height_;
    size_t maxUnits_;
    std::vector<std::unique_ptr<Unit>> units_;

    // итератору нужен прямой доступ к units_
    friend class GameFieldIterator;
};
