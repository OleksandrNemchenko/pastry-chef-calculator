//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <tables/metaInfo.h>
#include <tables/units.h>
#include <tables/units-transform.h>

namespace {
    static const QString gitHashField{"GitHash"};
    static const QString buildTimestampField{"BuildTimestamp"};
    static const QString databaseInterfaceVersionField{"DatabaseInterfaceVersion"};
    static const QString unitsInterfaceVersionField{"UnitsInterfaceVersion"};
    static const QString unitsTransformInterfaceVersionField{"UnitsTransformInterfaceVersion"};
}   // namespace

const QString &PCCMetaInformation::TableName() const {
    static const QString name{"MetaInfo"};

    return name;
}

const QString &PCCMetaInformation::TableDescription() const {
    static const QString descr{"meta data information"};
    return descr;
}

const PCCMetaInformation::TTableFields &PCCMetaInformation::TableFields() const {
    static const PCCMetaInformation::TTableFields fields = {
            {"Name", "TEXT NOT NULL UNIQUE"},
            {"Value", "TEXT NOT NULL"}
    };

    return fields;
}

const PCCDbTable::TTableData &PCCMetaInformation::TableInitialData() const {
    static const TTableData initialData = {
            { gitHashField, QString::fromLatin1(COMPILE_GIT_HASH) },
            { buildTimestampField, QString::fromLatin1(__DATE__ " " __TIME__) },
            { databaseInterfaceVersionField, QString::number(PCCDatabase::InterfaceVersion()) },
            { unitsInterfaceVersionField, QString::number(PCCUnits::MaxInterfaceVersion()) },
            { unitsTransformInterfaceVersionField, QString::number(PCCUnitsTransform::MaxInterfaceVersion()) },
    };

    return initialData;
}

void PCCMetaInformation::SetTableData(bool previouslyInitializedData, PCCDbTable::TTableData &&table) {
    for (const auto &row : table) {
        constexpr auto attribId = 0;
        constexpr auto valueId = 1;
        QString attrib{row[attribId]};
        QString value{row[valueId]};

        if (attrib == gitHashField) {
            _gitHash = value;
            if (previouslyInitializedData)
                logInfo(L"Database's GitHash : "s, _gitHash);
        } else if (attrib == buildTimestampField) {
            _buildTimestamp = value;
            if (previouslyInitializedData)
                logInfo(L"Database's build timestamp : "s, _buildTimestamp);
        } else if (attrib == databaseInterfaceVersionField) {
            _dbInterfaceVersion = value.toLongLong();
            if (previouslyInitializedData)
                logInfo(L"Database's interface : "s, _dbInterfaceVersion, L", current one : "s, PCCDatabase::InterfaceVersion());
        } else if (attrib == unitsInterfaceVersionField) {
            _unitsInterfaceVersion = value.toLongLong();
            if (previouslyInitializedData)
                logInfo(L"Units's interface : "s, _unitsInterfaceVersion);
        } else if (attrib == unitsTransformInterfaceVersionField) {
            _unitsTransformInterfaceVersion = value.toLongLong();
            if (previouslyInitializedData)
                logInfo(L"Units Transform's interface : "s, _unitsTransformInterfaceVersion);
        }
    }
}
