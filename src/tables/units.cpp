//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <tables/units.h>
#include <tables/units-transform.h>

/* static */const PCCUnits::TTypeMap PCCUnits::_typeMap = {
    { EUnitType::CURRENCY, "Валюта" },
    { EUnitType::WEIGHT,   "Вес"    },
    { EUnitType::VOLUME,   "Объем"  },
    { EUnitType::PIECE,    "Штуки"  },
};

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

void PCCUnits::SetTableData(PCCDatabase *db, bool previouslyInitializedData, PCCDbTable::TTableData &&table) {
    (void) db;

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
        case static_cast<int>(EUnitRoles::TYPE) :          return _typeMap[unit._type];
        case static_cast<int>(EUnitRoles::TYPE_ID) :       return static_cast<uint>(unit._type);
        case static_cast<int>(EUnitRoles::TITLE) :         return unit._title;
        case static_cast<int>(EUnitRoles::ABBREVIATION) :  return unit._abbreviaton;
        case static_cast<int>(EUnitRoles::IS_SINGLE) :     return isSingleUnitForType(unit);
        default: assert(false && "Unsupported unit type"); return QString();
    }
}

QHash<int, QByteArray> PCCUnits::roleNames() const
{
    static const QHash<int, QByteArray> roles{
            { static_cast<int>(EUnitRoles::DB_ID),        "idUnit"       },
            { static_cast<int>(EUnitRoles::TYPE),         "type"         },
            { static_cast<int>(EUnitRoles::TYPE_ID),      "typeId"       },
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

Q_INVOKABLE QJsonArray PCCUnits::unitTransforms(int dbId)
{
    return _db->unitTransforms().BuildModel(dbId);
}

Q_INVOKABLE void PCCUnits::unitTransformDelete(int idUnit, int idUnitTransform)
{
    auto unitTransform = _db->unitTransforms().UnitTransform(idUnitTransform);

    if (!_db->unitTransforms().DeleteRecord(unitTransform._dbId))
        return;

    Refresh();
}

Q_INVOKABLE void PCCUnits::unitDelete(int idUnit)
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

    // TODO : need to refresh UnitsTransform in QML. Now it is emulated as 1) delete all, 2) insert all
    beginRemoveRows(QModelIndex(), unitIndex, unitIndex);
    DeleteRecord(unitIndex);
    _units.remove(unitIndex);
    endRemoveRows();

    Refresh();
//    beginInsertRows(QModelIndex(), 0, _units.size());
//    endInsertRows();
}

QJsonArray PCCUnits::buildUnitTypesModel()
{
    QJsonArray unitTypes;

    for (EUnitType unit = EUnitType::FIRST; unit != EUnitType::UNITS_AMOUNT; ) {
        QJsonObject value;

        value.insert("type", static_cast<int>(unit));
        value.insert("title", _typeMap[unit]);

        unitTypes.append(value);
        unit = static_cast<EUnitType>(static_cast<size_t>(unit) + 1);
    }

    return unitTypes;
}

Q_INVOKABLE void PCCUnits::unitAdd(const QString& typeTitle, const QString& title, const QString& abbreviation)
{
    int newDbId = 0;

    if (_units.size() >= 1) {
        newDbId = _units[0]._dbId;

        for (auto &unit : _units)
            newDbId = std::max(newDbId, unit._dbId);

        ++newDbId;
    }

    SUnitData unit;

    unit._dbId = newDbId;
    unit._type = _typeMap.keys(typeTitle).at(0);
    unit._title = title;
    unit._abbreviaton = abbreviation;

    size_t index;
    for (index = 0; index < _units.size(); ++index) {
        const SUnitData &curUnit = _units[index];

        if (unit < curUnit)
            break;
    }

    TMapRecordValue record;
    record["ID"] = QString::number(unit._dbId);
    record["Title"] = unit._title;
    record["Abbreviation"] = unit._abbreviaton;
    record["Type"] = QString::number(static_cast<int>(unit._type));
    AddRecord(record);

    beginInsertRows(QModelIndex(), index, index);
    _units.insert(index, std::move(unit));
    endInsertRows();

}

Q_INVOKABLE QJsonArray PCCUnits::unitTransformsToSelection(int idFromUnit)
{
    QJsonArray model;

    EUnitType fromType = _db->units().Unit(idFromUnit)._type;

    for (const auto &unit : _units) {
        if (unit._dbId == idFromUnit || unit._type != fromType)
            continue;

        bool alreadyExists = false;

        for (const auto &transform : _db->unitTransforms().Transforms())
            if ((transform._from == idFromUnit && transform._to == unit._dbId) ||
                (transform._from == unit._dbId && transform._to == idFromUnit)) {
                alreadyExists = true;
                break;
            }

        if (alreadyExists)
            continue;

        QJsonObject value;

        value.insert("unitId", static_cast<int>(unit._dbId));
        value.insert("title", unit._title);
        value.insert("abbreviation", unit._abbreviaton);

        model.append(value);
    }

    return model;
}

void PCCUnits::Refresh()
{
    emit dataChanged(index(0, 0), index(_units.size() - 1, 0));
}

Q_INVOKABLE void PCCUnits::unitTransformAdd(int idUnitFrom, int idUnitTo, QString valueFrom, QString valueTo)
{
    _db->unitTransforms().AddTransform(idUnitFrom, idUnitTo, valueFrom.toDouble(), valueTo.toDouble());
}

const SUnitData& PCCUnits::Unit(int dbUnit) const
{
    return UnitImpl<const SUnitData>(_units.cbegin(), _units.cend(), dbUnit);
}

SUnitData& PCCUnits::Unit(int dbUnit)
{
    return UnitImpl<SUnitData>(_units.begin(), _units.end(), dbUnit);
}

template<typename TRet, typename TIt>
/* static */ TRet& PCCUnits::UnitImpl(TIt begin, TIt end, int dbUnit)
{
    auto itUnit = std::find_if(begin, end, [dbUnit](const auto &unit) {
        return unit._dbId == dbUnit;
    } );
    if (itUnit == end) {
        logError(L"Unable to find unit with ID = "s, dbUnit);
        assert(false);
        static SUnitData dummy;
        return dummy;
    }

    return *itUnit;
}
