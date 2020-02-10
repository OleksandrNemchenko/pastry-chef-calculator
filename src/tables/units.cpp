//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <tables/units.h>

const QString &PCCUnits::TableName() const {
    static const QString name{"Units"};

    return name;
}

const std::wstring &PCCUnits::TableDescription() const {
    static const std::wstring descr{L"units"s};
    return descr;
}

const std::vector<PCCUnits::Field> &PCCUnits::TableFields() const {
    static const std::vector<PCCUnits::Field> fields = {
            {"ID", "INTEGER UNIQUE"},
            {"Name", "TEXT NOT NULL"},
            {"Type", "INTEGER"},
            {"Default", "BOOLEAN"}
    };

    return fields;
}

const PCCUnits::TTableData &PCCUnits::TableInitialData() const {
    static const TTableData initialData = {
            {"0", QString(tr("грн")), "0", "1"},
            {"1", QString(tr("г")),   "1", "0"},
            {"2", QString(tr("кг")),  "1", "1"},
            {"3", QString(tr("л")),   "2", "0"},
            {"4", QString(tr("мл")),  "2", "1"},
            {"5", QString(tr("шт.")), "3", "1"}
    };

    return initialData;
}

void PCCUnits::SetTableData(bool previouslyInitializedData, PCCUnits::TTableData &&table) {
    switch (_currentInterfaceVersion) {
        case 1 : SetTableDataInterface1 (previouslyInitializedData, std::move (table)); break;
        default: assert(false && "Unsupported interface");  break;
    }
}

void PCCUnits::SetTableDataInterface1(bool previouslyInitializedData, PCCUnits::TTableData &&table) {
    (void) previouslyInitializedData;

    for (const TTableRow &row : table) {
        SUnitData unitData;

        unitData._dbId = row[0].toULongLong();
        unitData._name = row[1].toStdWString();
        unitData._default = row[3].toInt() != 0;

        switch (row[2].toLong()) {
            case CURRENCY:  _currencies.push_back (std::move(unitData)); break;
            case WEIGHT:    _weights.push_back    (std::move(unitData)); break;
            case VOLUME:    _volumes.push_back    (std::move(unitData)); break;
            case PIECE:     _pieces.push_back     (std::move(unitData)); break;
        }
    }
}
