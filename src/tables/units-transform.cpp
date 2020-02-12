//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <tables/units-transform.h>

const QString &PCCUnitsTransform::TableName() const {
    static const QString name{"UnitsTrasnform"};

    return name;
}

const std::wstring &PCCUnitsTransform::TableDescription() const {
    static const std::wstring descr{L"units transformation coefficients"s};
    return descr;
}

const PCCDbTable::TTableFields &PCCUnitsTransform::TableFields() const {
    static const TTableFields fields = {
            {"ID", "INTEGER UNIQUE"},
            {"ValueFrom", "REAL"},
            {"TypeFrom", "INTEGER"},
            {"ValueTo", "REAL"},
            {"TypeTo", "INTEGER"},
    };

    return fields;
}

const PCCDbTable::TTableData &PCCUnitsTransform::TableInitialData() const {
    static const TTableData initialData = {
            {QString::number(0), QString::number(1000), QString::number(1), QString::number(1), QString::number(2)}, // 1000g = 1kg
            {QString::number(1), QString::number(1000), QString::number(4), QString::number(1), QString::number(3)}  // 1000ml =1l
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
        unitData._fromType = row[2].toInt();
        unitData._toValue = row[3].toDouble();
        unitData._toType = row[4].toInt();

        _unitsTransform.emplace_back(std::move(unitData));
    }
}
