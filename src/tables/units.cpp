//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <tables/units.h>
#include <tables/units-transform.h>

const QString &PCCUnits::TableName() const {
    static const QString name{"Units"};

    return name;
}

const QString &PCCUnits::TableDescription() const {
    static const QString descr{"units"};
    return descr;
}

const PCCDbTable::TTableFields &PCCUnits::TableFields() const {
    static const TTableFields fields = {
            {"ID", "INTEGER UNIQUE"},
            {"Title", "TEXT NOT NULL"},
            {"Abbreviation", "TEXT NOT NULL"},
            {"Type", "INTEGER"},
    };

    return fields;
}

const PCCUnits::TTableData &PCCUnits::TableInitialData() const {
    static const TTableData initialData = {
            { QString::number(0), tr("Украинская гривня"), tr("грн"), QString::number(static_cast<int>(EUnitType::CURRENCY)) },
            { QString::number(1), tr("Грамм"),             tr("г"),   QString::number(static_cast<int>(EUnitType::WEIGHT)) },
            { QString::number(2), tr("Килограмм"),         tr("кг"),  QString::number(static_cast<int>(EUnitType::WEIGHT)) },
            { QString::number(3), tr("Литр"),              tr("л"),   QString::number(static_cast<int>(EUnitType::VOLUME)) },
            { QString::number(4), tr("Миллилитр"),         tr("мл"),  QString::number(static_cast<int>(EUnitType::VOLUME)) },
            { QString::number(5), tr("Штука"),             tr("шт."), QString::number(static_cast<int>(EUnitType::PIECE)) },
            { QString::number(6), tr("Фунт"),              tr("lb"),  QString::number(static_cast<int>(EUnitType::WEIGHT)) },
            { QString::number(7), tr("Унция"),             tr("oz"),  QString::number(static_cast<int>(EUnitType::WEIGHT)) },
    };

    return initialData;
}

void PCCUnits::SetTableData(bool previouslyInitializedData, PCCDbTable::TTableData &&table) {

    beginResetModel();
    _units.clear();
    endResetModel();

    switch (_currentInterfaceVersion) {
        case 1 : SetTableDataInterface1 (previouslyInitializedData, std::move (table)); break;
        default: assert(false && "Unsupported interface");  break;
    }

}

void PCCUnits::SetTableDataInterface1(bool previouslyInitializedData, PCCDbTable::TTableData &&table) {
    constexpr size_t DB_ID = 0;
    constexpr size_t TITLE = DB_ID + 1;
    constexpr size_t ABBREV = TITLE + 1;
    constexpr size_t TYPE = ABBREV + 1;
    (void) previouslyInitializedData;
    size_t correctRows = 0;

    for (const TTableRow &row : table) {
        int type = row[TYPE].toInt();

        if (type >= static_cast<int>(EUnitType::FIRST) && type < static_cast<int>(EUnitType::UNITS_AMOUNT))
            ++correctRows;
    }

    beginInsertRows(QModelIndex(), 0, correctRows);

    for (const TTableRow &row : table) {
        SUnitData unitData;

        unitData._dbId = row[DB_ID].toUInt();
        unitData._title = row[TITLE];
        unitData._abbreviaton = row[ABBREV];
        int type = row[TYPE].toInt();

        if (type >= static_cast<int>(EUnitType::UNITS_AMOUNT)) {
            logError (L"Unsupported unit type "s, type);
            continue;
        }

        unitData._type = static_cast<EUnitType>(type);

        logDebug(L"Loaded unit : \""s, unitData._title, L"\" ("s, unitData._abbreviaton, L") : db id = "s,
                unitData._dbId, L", type = "s, type);

        _units.append(std::move(unitData));
    }

    std::sort(_units.begin(), _units.end());

    endInsertRows();

    emit dataChanged(index(0, 0), index(correctRows, 0));
}

bool SUnitData::operator<(const SUnitData& right) const
{
    if (static_cast<size_t>(_type) == static_cast<size_t>(right._type))
        return _title < right._title;
    else
        return static_cast<size_t>(_type) < static_cast<size_t>(right._type);

}

void PCCUnits::SetUnitsTransform(PCCUnitsTransform* unitsTransforms) {

    for (const SUnitTransform &transform : unitsTransforms->Transforms() ) {
        decltype(_units)::iterator itUnitFrom = _units.end();
        decltype(_units)::iterator itUnitTo = _units.end();

        for (auto itUnit = _units.begin(); itUnit != _units.end(); ++itUnit) {
            if (transform._fromUnit == itUnit->_dbId)
                itUnitFrom = itUnit;
            if (transform._toUnit == itUnit->_dbId)
                itUnitTo = itUnit;
        }

        if (itUnitFrom == _units.end() || itUnitTo == _units.end()) {
            logError(L"Incorrect units transformation : ID = "s, transform._dbId,
                     L", from value = "s, transform._fromValue,
                     L", from unit = "s, transform._fromUnit,
                     L", to value = "s, transform._toValue,
                     L", to unit = "s, transform._toUnit );
            continue;
        }

        logDebug(L"Units transformation ("s, transform._dbId, L"): "s, transform._fromValue, itUnitFrom->_abbreviaton, L" (db ID "s, itUnitFrom->_dbId, L") = "s,
                 transform._toValue, itUnitTo->_abbreviaton, L" (db ID "s, itUnitTo->_dbId, L")"s);

        unitsTransforms->SetTransformPointers(transform._dbId, &*itUnitFrom, &*itUnitTo);
        itUnitFrom->_transform.append(&transform);
    }

}

int PCCUnits::rowCount(const QModelIndex &parent) const
{
    return _units.size();
}

QVariant PCCUnits::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _units.size())
        return QVariant();

    const SUnitData &unit = _units.at(index.row());

    switch (role) {
        case static_cast<int>(EUnitRoles::DB_ID) :         return unit._dbId;
        case static_cast<int>(EUnitRoles::TYPE) :          return typeDescription(unit._type);
        case static_cast<int>(EUnitRoles::TITLE) :         return unit._title;
        case static_cast<int>(EUnitRoles::ABBREVIATION) :  return unit._abbreviaton;
        case static_cast<int>(EUnitRoles::IS_SINGLE) :     return isSingleUnitForType(unit);
        default: assert(false && "Unsupported unit type"); return QString();
    }
}

QHash<int, QByteArray> PCCUnits::roleNames() const
{
    static const QHash<int, QByteArray> roles{
            { static_cast<int>(EUnitRoles::DB_ID),        "idUnit"      },
            { static_cast<int>(EUnitRoles::TYPE),         "type"         },
            { static_cast<int>(EUnitRoles::TITLE),        "title"        },
            { static_cast<int>(EUnitRoles::ABBREVIATION), "abbreviation" },
            { static_cast<int>(EUnitRoles::IS_SINGLE),    "isSingle"     }
    };

    return roles;
}

bool PCCUnits::isSingleUnitForType(const SUnitData& unit) const
{
    auto itUnit = std::find_if(_units.begin(), _units.end(), [unit](const SUnitData& testUnit) {
        return unit._dbId != testUnit._dbId && testUnit._type == unit._type;
    } );

    return itUnit == _units.end();
}

Q_INVOKABLE QJsonArray PCCUnits::unitTransforms(uint dbId)
{
    return _db->unitTransforms().unitTransforms(dbId);
}

/* static */ const QString& PCCUnits::typeDescription(EUnitType unitType)
{
    static const std::array<QString, static_cast<size_t>(EUnitType::UNITS_AMOUNT)> description{
        tr("Денежная единица"), // EUnitType::CURRENCY
        tr("Вес"),              // EUnitType::WEIGHT
        tr("Количество"),       // EUnitType::VOLUME
        tr("Штука"),            // EUnitType::PIECE
    };

    assert(static_cast<size_t>(unitType) < static_cast<size_t>(EUnitType::UNITS_AMOUNT));

    return description[static_cast<size_t>(unitType)];
}

Q_INVOKABLE void PCCUnits::unitTransformDelete(uint idUnit, uint idUnitTransform)
{
    auto itUnit = std::find_if(_units.begin(), _units.end(), [idUnit](const SUnitData& unit) {
        return unit._dbId == idUnit;
    } );
    if (itUnit == _units.end()) {
        logError(L"Unit ID = "s, idUnit, L" has not been found"s);
        return;
    }

    auto &transform = itUnit->_transform;
    auto itUnitTransform = std::find_if(transform.begin(), transform.end(), [idUnitTransform](const SUnitTransform* transform) {
        return transform->_dbId == idUnitTransform;
    } );
    if (itUnitTransform == transform.end()) {
        logError(L"Unit transform ID = "s, idUnitTransform, L" for unit ID = "s, idUnit, L" has not been found"s);
        return;
    }

    if (!_db->unitTransforms().DeleteField(idUnitTransform))
        return;

    // list model changes
}

Q_INVOKABLE void PCCUnits::unitDelete(uint idUnit)
{
    int unitIndex;

    for (unitIndex = 0; unitIndex < _units.size(); ++unitIndex) {
        if (_units.at(unitIndex)._dbId == idUnit)
            break;
    }

    if (unitIndex == _units.size()) {
        logError(L"Unit ID = "s, idUnit, L" has not been found"s);
        return;
    }

    _db->unitTransforms().DeleteUnitTransforms(idUnit);

    beginRemoveRows(QModelIndex(), unitIndex, unitIndex);
    DeleteRecord(unitIndex);
    _units.remove(unitIndex);
    endRemoveRows();
}
