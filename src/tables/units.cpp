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
            {"DefaultUnit", "INTEGER"}
    };

    return fields;
}

const PCCUnits::TTableData &PCCUnits::TableInitialData() const {
    static const QString DEFAULT = "1";
    static const QString NOT_DEFAULT = "0";
    static const TTableData initialData = {
            {QString::number(0), tr("Украинская гривня"), tr("грн"), QString::number(static_cast<int>(EUnitType::CURRENCY)), DEFAULT    },
            {QString::number(1), tr("Грамм"),             tr("г"),   QString::number(static_cast<int>(EUnitType::WEIGHT)),   NOT_DEFAULT},
            {QString::number(2), tr("Килограмм"),         tr("кг"),  QString::number(static_cast<int>(EUnitType::WEIGHT)),   DEFAULT    },
            {QString::number(3), tr("Литр"),              tr("л"),   QString::number(static_cast<int>(EUnitType::VOLUME)),   NOT_DEFAULT},
            {QString::number(4), tr("Миллилитр"),         tr("мл"),  QString::number(static_cast<int>(EUnitType::VOLUME)),   DEFAULT    },
            {QString::number(5), tr("Штука"),             tr("шт."), QString::number(static_cast<int>(EUnitType::PIECE)),    DEFAULT    },
            {QString::number(6), tr("Фунт"),              tr("lb"),  QString::number(static_cast<int>(EUnitType::WEIGHT)),   NOT_DEFAULT},
            {QString::number(7), tr("Унция"),             tr("oz"),  QString::number(static_cast<int>(EUnitType::WEIGHT)),   NOT_DEFAULT},
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
    constexpr size_t DEFAULT = TYPE + 1;
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

        unitData._dbId = row[DB_ID].toULongLong();
        unitData._title = row[TITLE];
        unitData._abbreviaton = row[ABBREV];
        int type = row[TYPE].toInt();
        unitData._default = row[DEFAULT].toInt() != 0;

        if (type >= static_cast<int>(EUnitType::UNITS_AMOUNT)) {
            logError (L"Unsupported unit type "s, type);
            continue;
        }

        unitData._type = static_cast<EUnitType>(type);

        logDebug(L"Loaded unit : \""s, unitData._title, L"\" ("s, unitData._abbreviaton, L") : db id = "s,
                unitData._dbId, L", type = "s, type, unitData._default ? L", default"s : L""s);

        _units.emplace_back(std::move(unitData));
    }

    endInsertRows();

    emit dataChanged(index(0, 0), index(correctRows, 0));
}

void PCCUnits::SetUnitsTransform(const PCCUnitsTransform& unitsTransforms) {

    for (const PCCUnitsTransform::SUnitTransform &transform : unitsTransforms.Transforms() ) {
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

        logDebug(L"Units transformation : "s, transform._fromValue, itUnitFrom->_abbreviaton, L" (db ID "s, itUnitFrom->_dbId, L") = "s,
                 transform._toValue, itUnitTo->_abbreviaton, L" (db ID "s, itUnitTo->_dbId, L")"s);

        SUnitData::STranform unitDataTransform(*itUnitTo, transform._fromValue, transform._toValue);

        itUnitFrom->_transform.emplace_back(std::move(unitDataTransform));
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
        case static_cast<int>(EUnitRoles::DB_ID) :         return static_cast<uint>(unit._dbId);
        case static_cast<int>(EUnitRoles::TYPE) :          return typeDescription(unit._type);
        case static_cast<int>(EUnitRoles::TITLE) :         return unit._title;
        case static_cast<int>(EUnitRoles::ABBREVIATION) :  return unit._abbreviaton;
        case static_cast<int>(EUnitRoles::IS_DEFAULT) :    return unit._default;
        default: assert(false && "Unsupported unit type"); return QString();
    }
}

QHash<int, QByteArray> PCCUnits::roleNames() const
{
    static const QHash<int, QByteArray> roles{
            {static_cast<int>(EUnitRoles::DB_ID), "dbId"},
            {static_cast<int>(EUnitRoles::TYPE), "type"},
            {static_cast<int>(EUnitRoles::TITLE), "title"},
            {static_cast<int>(EUnitRoles::ABBREVIATION), "abbreviation"},
            {static_cast<int>(EUnitRoles::IS_DEFAULT), "isDefault"}
    };

    return roles;
}

uint PCCUnits::unitTransformsAmount(uint dbId)
{
    for (const auto &unit : _units) {
        if (unit._dbId == dbId)
            return unit._transform.size();
    }

    logError(L"Incorrect dbId = "s, dbId, L" unitTransformsAmount request from QML"s);
    return 0;
}

Q_INVOKABLE QJsonObject PCCUnits::unitTransform(uint dbId, uint transformStep)
{
    QJsonObject json;

    if (transformStep >= unitTransformsAmount(dbId)) {
        logError(L"Incorrect step = "s, transformStep, L" for dbId = "s, dbId);
        return json;
    }

    for (const auto &unit : _units) {
        if (unit._dbId != dbId)
            continue;

        const SUnitData::STranform &transform = unit._transform[transformStep];

        json.insert("thisValue", transform._thisValue);
        json.insert("toValue", transform._toValue);
        json.insert("toUnitAbbreviation", transform._toTransform._abbreviaton);

        return json;
    }

    logError(L"Incorrect dbId = "s, dbId, L" unitTransforms request from QML"s);

    return json;
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
