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
    switch (_currentInterfaceVersion) {
        case 1 : SetTableDataInterface1 (previouslyInitializedData, std::move (table)); break;
        default: assert(false && "Unsupported interface");  break;
    }
}

void PCCUnits::SetTableDataInterface1(bool previouslyInitializedData, PCCDbTable::TTableData &&table) {
    (void) previouslyInitializedData;

    for (const TTableRow &row : table) {
        SUnitData unitData;

        unitData._dbId = row[0].toULongLong();
        unitData._name = row[1];
        unitData._default = row[3].toInt() != 0;

        int unitType = row[2].toInt();

        if (unitType >= static_cast<int>(EUnitType::UNITS_AMOUNT))
            logError (L"Unsupported unit type "s, unitType);
        else
            _units[unitType].emplace_back(std::move(unitData));
    }
}

void PCCUnits::SetUnitsTransform(const PCCUnitsTransform& unitsTransforms) {

    for (const PCCUnitsTransform::SUnitTransform &transform : unitsTransforms.Transforms() ) {
        TUnitsArray::iterator from;
        TUnitsArray::iterator to;

        auto tryToFind = [this] (size_t id) {
            EUnitType elemType;
            TUnitsArray::iterator itValue;

            for (size_t type = static_cast<size_t>(EUnitType::FIRST); type < static_cast<size_t>(EUnitType::UNITS_AMOUNT); ++type)
                for (itValue = _units[type].begin(); itValue != _units[type].end(); ++itValue) {
                    if (itValue->_dbId == id) {
                        elemType = static_cast<EUnitType>(type);
                        return std::tuple(true, elemType, itValue);
                    }
                }

            return std::tuple(false, EUnitType::UNITS_AMOUNT, _units[0].end());
        };

        auto [resFrom, elemTypeFrom, itValueFrom] = tryToFind(transform._fromType);
        auto [resTo,   elemTypeTo,   itValueTo]   = tryToFind(transform._toType  );

        if (!resFrom || !resTo) {
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
