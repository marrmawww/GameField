#include <iostream>
#include <string>
#include <utility>
#include <windows.h>

#include "GameField.h"
#include "Infantry.h"
#include "Archer.h"
#include "Cavalry.h"
#include "Elf.h"
#include "GameFieldIterator.h"

// Печатает карту и число юнитов, чтобы сравнивать состояние ДО и ПОСЛЕ операции.
void printField(const std::string& title, const GameField& field)
{
    std::cout << "\n" << title << "\n";
    field.print(std::cout);
    std::cout << "Юнитов: " << field.size() << "\n";
}

// Выводит название сценария.
void printScenario(const std::string& title)
{
    std::cout << "\n" << title << "\n";
}

int main()
{
    // Включаем UTF-8 для корректного отображения русского текста в консоли Windows.
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    std::cout << std::boolalpha;

    // СЦЕНАРИЙ 1. Создание поля и фабрики
    printScenario("СЦЕНАРИЙ 1. Создание поля и фабрики");

    GameField field(8, 5, 10);

    InfantryFactory infantryFactory;
    ArcherFactory archerFactory;
    CavalryFactory cavalryFactory;
    ElfFactory elfFactory;

    printField("До создания юнитов:", field);

    // Каждая фабрика создаёт только юнитов своей категории.
    field.addUnit(infantryFactory, UnitKind::Swordsman, 0, 0);
    field.addUnit(infantryFactory, UnitKind::Spearman, 1, 0);
    field.addUnit(archerFactory, UnitKind::LightArcher, 2, 1);
    field.addUnit(archerFactory, UnitKind::HeavyArcher, 3, 1);
    field.addUnit(cavalryFactory, UnitKind::Cataphract, 4, 2);
    field.addUnit(cavalryFactory, UnitKind::HorseArcher, 5, 2);
    // Четвёртая категория: эльфы, два новых конкретных класса.
    field.addUnit(elfFactory, UnitKind::ElfRanger, 6, 3);
    field.addUnit(elfFactory, UnitKind::ElfDruid, 7, 3);

    printField("После создания восьми юнитов из четырёх категорий:", field);

    // Проверяем, что фабрика пехоты не создаёт юнита кавалерии.
    try
    {
        infantryFactory.createUnit(UnitKind::Cataphract, 0, 0);
    }
    catch (const std::exception& e)
    {
        std::cout << "Ожидаемая ошибка фабрики: " << e.what() << "\n";
    }

    // СЦЕНАРИЙ 2. Глубокое копирование
    printScenario("СЦЕНАРИЙ 2. Глубокое копирование");

    // describe() показывает тип, категорию, координаты и характеристики юнита.
    std::cout << "\nХарактеристики юнитов:\n";
    for (size_t i = 0; i < field.size(); ++i)
    {
        std::cout << field.getUnit(i).describe() << "\n";
    }

    // Копирующий конструктор создаёт независимые объекты через clone().
    GameField fieldCopy(field);

    std::cout << "\nSwordsman в копии до изменения:\n";
    std::cout << fieldCopy.getUnit(0).describe() << "\n";

    // Урон наносится только копии, оригинал не должен измениться.
    fieldCopy.getUnit(0).takeDamage(35);

    std::cout << "\nSwordsman в копии после урона:\n";
    std::cout << fieldCopy.getUnit(0).describe() << "\n";

    std::cout << "\nSwordsman в оригинале:\n";
    std::cout << field.getUnit(0).describe() << "\n";
    std::cout << "Если характеристики отличаются, глубокое копирование работает.\n";

    // СЦЕНАРИЙ 3. Перемещение
    printScenario("СЦЕНАРИЙ 3. Перемещение");

    // Первое состояние: поле до передачи ресурсов.
    printField("Поле до move-конструктора:", fieldCopy);

    // Move-конструктор передаёт контейнер юнитов в новый объект.
    GameField movedField(std::move(fieldCopy));

    // Второе состояние: те же юниты теперь принадлежат movedField.
    printField("Поле после move-конструктора:", movedField);

    // Проверяем move-присваивание без дополнительного вывода третьей карты.
    GameField anotherField(1, 1, 1);
    anotherField = std::move(movedField);
    std::cout << "После operator=(&&) получатель содержит "
              << anotherField.size() << " юнитов.\n";

    // СЦЕНАРИЙ 4. Собственный итератор
    printScenario("СЦЕНАРИЙ 4. Собственный GameFieldIterator");

    // begin() возвращает итератор на первый элемент, end() — позицию после последнего.
    std::cout << "Результат обхода:\n";
    for (GameFieldIterator it = anotherField.begin();
         it != anotherField.end();
         ++it)
    {
        // operator-> позволяет вызвать метод текущего юнита.
        std::cout << it->getType() << "\n";
    }

    // Отдельно демонстрируем оба оператора доступа к текущему элементу.
    if (!anotherField.empty())
    {
        GameFieldIterator it = anotherField.begin();
        std::cout << "operator*: " << (*it).getType() << "\n";
        std::cout << "operator->: " << it->getType() << "\n";
    }

    std::cout << "Обход итератором не изменяет поле.\n";

    // СЦЕНАРИЙ 5. Операции GameField
    printScenario("СЦЕНАРИЙ 5. Операции GameField");

    GameField operationsField(8, 5, 10);
    operationsField.addUnit(infantryFactory, UnitKind::Swordsman, 0, 0);
    operationsField.addUnit(infantryFactory, UnitKind::Spearman, 1, 0);
    operationsField.addUnit(archerFactory, UnitKind::LightArcher, 2, 1);
    operationsField.addUnit(cavalryFactory, UnitKind::Cataphract, 4, 2);

    printField("До операций:", operationsField);

    // Удаляем LightArcher по координатам.
    operationsField.removeUnitAt(2, 1);

    // Убиваем Swordsman и удаляем его методом removeDead().
    operationsField.getUnit(0).takeDamage(1000);
    operationsField.removeDead();

    // После удаления Swordsman индекс 0 принадлежит Spearman.
    // Перемещаем его из (1, 0) в (2, 0).
    operationsField.moveUnit(0, 1, 0);

    printField("После операций:", operationsField);

    // clear() удаляет оставшихся Spearman и Cataphract.
    operationsField.clear();
    std::cout << "После clear(): " << operationsField.size() << " юнитов\n";
    std::cout << "empty(): " << operationsField.empty() << "\n";


    printScenario("СЦЕНАРИЙ 6. Удаление во время обхода итератором");

    GameField iteratorField(8, 5, 10);

    iteratorField.addUnit(infantryFactory, UnitKind::Swordsman, 0, 0);
    iteratorField.addUnit(infantryFactory, UnitKind::Spearman, 1, 0);
    iteratorField.addUnit(archerFactory, UnitKind::LightArcher, 2, 1);
    iteratorField.addUnit(archerFactory, UnitKind::HeavyArcher, 3, 1);
    iteratorField.addUnit(cavalryFactory, UnitKind::Cataphract, 4, 2);

    iteratorField.getUnit(1).takeDamage(1000); // Spearman
    iteratorField.getUnit(3).takeDamage(1000); // HeavyArcher

    printField("До удаления погибших:", iteratorField);

    size_t index = 0;
    GameFieldIterator it = iteratorField.begin();

    while (it != iteratorField.end())
    {
        if (!it->isAlive())
        {
            std::cout << "Удаляем: " << it->getType() << "\n";

            iteratorField.removeUnitAt(index);

            it = GameFieldIterator(&iteratorField, index);
        }
        else
        {
            // если удаления не было, переходим дальше
            ++it;
            ++index;
        }
    }

    printField("После удаления погибших:", iteratorField);
    std::cout << "Осталось юнитов: "
        << iteratorField.size() << "\n";

    std::cout << "\nВсе демонстрационные сценарии завершены.\n";
    return 0;
}
