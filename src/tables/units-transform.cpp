//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <tables/units.h>
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

        unitData._dbId = row[0].toUInt();
        unitData._fromValue = row[1].toDouble();
        unitData._fromUnit = row[2].toInt();
        unitData._toValue = row[3].toDouble();
        unitData._toUnit = row[4].toInt();
        unitData._from = nullptr;
        unitData._to = nullptr;

        _unitsTransform.append(std::move(unitData));
    }
}

bool PCCUnitsTransform::DeleteField(size_t dbId)
{
    auto itUnitTransform = std::find_if(_unitsTransform.begin(), _unitsTransform.end(), [dbId](const SUnitTransform& unit) {
        return unit._dbId == dbId;
    } );
    if (itUnitTransform == _unitsTransform.end()) {
        logError(L"Unit transform ID = "s, dbId, L" has not been found"s);
        return false;
    }

    _unitsTransform.erase(itUnitTransform);

    if (!DeleteRecord(dbId))
        return false;

    return true;
}

void PCCUnitsTransform::SetTransformPointers(uint dbId, SUnitData* from, SUnitData* to)
{
    assert(from && to);

    auto itUnitTransform = std::find_if(_unitsTransform.begin(), _unitsTransform.end(), [dbId](const SUnitTransform& unit) {
        return unit._dbId == dbId;
    } );
    if (itUnitTransform == _unitsTransform.end()) {
        logError(L"Unit transform ID = "s, dbId, L" has not been found"s);
        return;
    }

    itUnitTransform->_from = from;
    itUnitTransform->_to = to;
}

QJsonArray PCCUnitsTransform::unitTransforms(uint dbFromUnit) const
{
    QJsonArray values;
    int indexUnitTransform = 0;

    for (const auto &transform : _unitsTransform) {

        if (transform._fromUnit != dbFromUnit)
            continue;

        QJsonObject value;

        value.insert("unitTransformIndex", indexUnitTransform);
        value.insert("idUnitTransform", static_cast<int>(transform._dbId));
        value.insert("thisValue", transform._fromValue);
        value.insert("toValue", transform._toValue);
        value.insert("toUnitAbbreviation", transform._to->_abbreviaton);

        values.append(value);

        ++indexUnitTransform;
    }

    return values;
}
