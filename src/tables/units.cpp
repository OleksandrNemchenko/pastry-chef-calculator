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
            {"Name", "TEXT NOT NULL"},
            {"Type", "INTEGER"},
            {"DefaultUnit", "INTEGER"}
    };

    return fields;
}

const PCCUnits::TTableData &PCCUnits::TableInitialData() const {
    static const QString DEFAULT = "1";
    static const QString NOT_DEFAULT = "1";
    static const TTableData initialData = {
            {QString::number(0), QString(tr("грн")), QString::number(static_cast<int>(EUnitType::CURRENCY)), DEFAULT    },
            {QString::number(1), QString(tr("г")),   QString::number(static_cast<int>(EUnitType::WEIGHT)),   NOT_DEFAULT},
            {QString::number(2), QString(tr("кг")),  QString::number(static_cast<int>(EUnitType::WEIGHT)),   DEFAULT    },
            {QString::number(3), QString(tr("л")),   QString::number(static_cast<int>(EUnitType::VOLUME)),   NOT_DEFAULT},
            {QString::number(4), QString(tr("мл")),  QString::number(static_cast<int>(EUnitType::VOLUME)),   DEFAULT    },
            {QString::number(5), QString(tr("шт.")), QString::number(static_cast<int>(EUnitType::PIECE)),    DEFAULT    }
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
    (void) previouslyInitializedData;
    size_t correctRows = 0;

    for (const TTableRow &row : table) {
        int type = row[2].toInt();

        if (type >= static_cast<int>(EUnitType::FIRST) && type < static_cast<int>(EUnitType::UNITS_AMOUNT))
            ++correctRows;
    }

    beginInsertRows(QModelIndex(), 0, correctRows);

    for (const TTableRow &row : table) {
        SUnitData unitData;

        unitData._dbId = row[0].toULongLong();
        unitData._name = row[1];
        unitData._default = row[3].toInt() != 0;

        int type = row[2].toInt();

        if (type >= static_cast<int>(EUnitType::UNITS_AMOUNT)) {
            logError (L"Unsupported unit type "s, type);
            continue;
        }

        unitData._type = static_cast<EUnitType>(type);

        logDebug(L"Loaded unit : \""s, unitData._name, L"\" ("s, unitData._dbId, L") : type = "s, type,
                 unitData._default ? L", default"s : L""s);

        _units.emplace_back(std::move(unitData));
    }

    endInsertRows();

    emit dataChanged(index(0, 0), index(correctRows, 0));
}

void PCCUnits::SetUnitsTransform(const PCCUnitsTransform& unitsTransforms) {

    for (const PCCUnitsTransform::SUnitTransform &transform : unitsTransforms.Transforms() ) {
        TUnitsArray::iterator from;
        TUnitsArray::iterator to;

        auto tryToFind = [this] (size_t id) {
            EUnitType elemType;
            TUnitsArray::iterator itValue;

            for (auto itUnit = _units.begin(); itUnit != _units.end(); ++itUnit) {
                if (itUnit->_dbId == id)
                    return itUnit;
            }

            return _units.end();
        };

        auto itValueFrom = tryToFind(transform._fromType);
        auto itValueTo   = tryToFind(transform._toType  );

        if (itValueFrom == _units.end() || itValueTo == _units.end()) {
            logError(L"Incorrect units transformation : ID = "s, transform._dbId,
                     L", from value = "s, transform._fromValue,
                     L", from type = "s, transform._fromType,
                     L", to value = "s, transform._toValue,
                     L", to type = "s, transform._toType );
            continue;
        }

        logDebug(L"Units transformation : "s, transform._fromValue, itValueFrom->_name, L" (db ID "s, itValueFrom->_dbId, L") = "s,
                 transform._toValue, itValueTo->_name, L" (db ID "s, itValueTo->_dbId, L")"s);

        SUnitData::STranform directTransform(*itValueTo, transform._fromValue, transform._toValue);
        SUnitData::STranform reverseTransform(*itValueFrom, transform._toValue, transform._fromValue);

        itValueFrom->_transform.emplace_back(std::move(directTransform));
        itValueTo->_transform.emplace_back(std::move(reverseTransform));
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
        case static_cast<int>(EUnitRoles::TYPE) :       return typeDescription(unit._type);
        case static_cast<int>(EUnitRoles::NAME) :       return unit._name;
        case static_cast<int>(EUnitRoles::IS_DEFAULT) : return unit._default;
        default: assert(false && "Unsupported unit type"); return QString();
    }
}

QHash<int, QByteArray> PCCUnits::roleNames() const
{
    static const QHash<int, QByteArray> roles{
            {static_cast<int>(EUnitRoles::TYPE), "type"},
            {static_cast<int>(EUnitRoles::NAME), "name"},
            {static_cast<int>(EUnitRoles::IS_DEFAULT), "isDefault"}
    };

    return roles;
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
