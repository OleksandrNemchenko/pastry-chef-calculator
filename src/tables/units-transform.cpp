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

void PCCUnitsTransform::SetTableData(PCCDatabase* db, bool previouslyInitializedData, PCCDbTable::TTableData &&table) {
    switch (_currentInterfaceVersion) {
        case 1 : SetTableDataInterface1 (db, previouslyInitializedData, std::move (table)); break;
        default: assert(false && "Unsupported interface");  break;
    }
}

void PCCUnitsTransform::SetTableDataInterface1(PCCDatabase* db, bool previouslyInitializedData, PCCDbTable::TTableData &&table) {
    (void) previouslyInitializedData;

    for (const TTableRow &row : table) {
        SUnitTransform unitData;

        unitData._dbId = row[0].toUInt();
        unitData._fromValue = row[1].toDouble();
        unitData._from = row[2].toInt();
        unitData._toValue = row[3].toDouble();
        unitData._to = row[4].toInt();

        _unitsTransform.append(std::move(unitData));
    }
}

bool PCCUnitsTransform::DeleteRecord(size_t dbId)
{
    auto itUnitTransform = std::find_if(_unitsTransform.begin(), _unitsTransform.end(), [dbId](const SUnitTransform& unit) {
        return unit._dbId == dbId;
    } );
    if (itUnitTransform == _unitsTransform.end()) {
        logError(L"Unit transform ID = "s, dbId, L" has not been found"s);
        return false;
    }

    _unitsTransform.erase(itUnitTransform);

    if (!DeleteDbRecord(dbId))
        return false;

    return true;
}

QJsonArray PCCUnitsTransform::BuildModel(int dbFromUnit) const
{
    QJsonArray values;
    int indexUnitTransform = 0;

    for (const auto &transform : _unitsTransform) {

        if (transform._from != dbFromUnit)
            continue;

        QJsonObject value;

        value.insert("unitTransformIndex", indexUnitTransform);
        value.insert("idUnitTransform", static_cast<int>(transform._dbId));
        value.insert("thisValue", transform._fromValue);
        value.insert("toValue", transform._toValue);
        value.insert("toUnitAbbreviation", _db->units().Unit(transform._to)._abbreviaton);

        values.append(value);

        ++indexUnitTransform;
    }

    return values;
}

void PCCUnitsTransform::DeleteUnitTransforms(int unitDbId)
{
    for (auto it = _unitsTransform.begin(); it != _unitsTransform.end();) {
        if (it->_from == unitDbId || it->_to == unitDbId) {
            DeleteDbRecord(it->_dbId);
            it = _unitsTransform.erase(it);
        }
        else
            ++it;
    }
}

const SUnitTransform* PCCUnitsTransform::AddTransform(uint idUnitFrom, uint idUnitTo, double fromValue, double toValue)
{
    int newDbId = 0;

    if (_unitsTransform.size() >= 1) {
        newDbId = _unitsTransform.at(0)._dbId;

        for (const auto &transform : _unitsTransform)
            newDbId = std::max(newDbId, transform._dbId);

        ++newDbId;
    }

    SUnitTransform transform;

    transform._dbId = newDbId;
    transform._from = idUnitFrom;
    transform._to = idUnitTo;
    transform._fromValue = fromValue;
    transform._toValue = toValue;

    _unitsTransform.append(std::move(transform));

    TMapRecordValue record;
    record["ID"] = QString::number(transform._dbId);
    record["ValueFrom"] = QString::number(transform._fromValue);
    record["UnitFrom"] = QString::number(transform._from);
    record["ValueTo"] = QString::number(transform._toValue);
    record["UnitTo"] = QString::number(transform._to);
    AddRecord(record);

    _db->units().Refresh();

    return &_unitsTransform.last();
}

const SUnitTransform& PCCUnitsTransform::UnitTransform(int dbUnit) const
{
    return UnitTransformImpl<const SUnitTransform>(_unitsTransform.cbegin(), _unitsTransform.cend(), dbUnit);
}

SUnitTransform& PCCUnitsTransform::UnitTransform(int dbUnit)
{
    return UnitTransformImpl<SUnitTransform>(_unitsTransform.begin(), _unitsTransform.end(), dbUnit);
}

template<typename TRet, typename TIt>
/* static */ TRet& PCCUnitsTransform::UnitTransformImpl(TIt begin, TIt end, int dbUnit)
{
    auto itUnit = std::find_if(begin, end, [dbUnit](const auto &unit) {
        return unit._dbId == dbUnit;
    } );
    if (itUnit == end) {
        logError(L"Unable to find unit transform with ID = "s, dbUnit);
        assert(false);
        static SUnitTransform dummy;
        return dummy;
    }

    return *itUnit;
}
