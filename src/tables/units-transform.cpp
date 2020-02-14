//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <tables/units-transform.h>

const QString &PCCUnitsTransform::TableName() const {
    static const QString name{"UnitsTrasnform"};

    return name;
}

const QString &PCCUnitsTransform::TableDescription() const {
    static const QString descr{"units transformation coefficients"};
    return descr;
}

const PCCDbTable::TTableFields &PCCUnitsTransform::TableFields() const {
    static const TTableFields fields = {
            {"ID", "INTEGER UNIQUE"},
            {"ValueFrom", "REAL"},
            {"UnitFrom", "INTEGER"},
            {"ValueTo", "REAL"},
            {"UnitTo", "INTEGER"},
    };

    return fields;
}

const PCCDbTable::TTableData &PCCUnitsTransform::TableInitialData() const {
    static const TTableData initialData = {
            {QString::number(0), QString::number(1000), QString::number(1), QString::number(1), QString::number(2)}, // 1000g = 1kg
            {QString::number(1), QString::number(1000), QString::number(4), QString::number(1), QString::number(3)}, // 1000ml = 1l
            {QString::number(2), QString::number(1),    QString::number(6), QString::number(453.59237), QString::number(1)},  // 1lb = 453,59237g
            {QString::number(3), QString::number(1),    QString::number(7), QString::number(28.349523125), QString::number(1)},  // 1oz = 28,349523125g
    };

    return initialData;
}

void PCCUnitsTransform::SetTableData(bool previouslyInitializedData, PCCDbTable::TTableData &&table) {
    switch (_currentInterfaceVersion) {
        case 1 : SetTableDataInterface1 (previouslyInitializedData, std::move (table)); break;
        default: assert(false && "Unsupported interface");  break;
    }
}

void PCCUnitsTransform::SetTableDataInterface1(bool previouslyInitializedData, PCCDbTable::TTableData &&table) {
    (void) previouslyInitializedData;

    for (const TTableRow &row : table) {
        SUnitTransform unitData;

        unitData._dbId = row[0].toULongLong();
        unitData._fromValue = row[1].toDouble();
        unitData._fromUnit = row[2].toInt();
        unitData._toValue = row[3].toDouble();
        unitData._toUnit = row[4].toInt();

        _unitsTransform.emplace_back(std::move(unitData));
    }
}
