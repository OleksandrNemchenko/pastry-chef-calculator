//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <tables/metaInfo.h>
#include <tables/units.h>

namespace {
    static const std::wstring gitHashField{L"GitHash"s};
    static const std::wstring buildTimestampField{L"BuildTimestamp"s};
    static const std::wstring databaseInterfaceVersionField{L"DatabaseInterfaceVersion"s};
    static const std::wstring unitsInterfaceVersionField{L"UnitsInterfaceVersion"s};
}   // namespace

const QString &PCCMetaInformation::TableName() const {
    static const QString name{"MetaInfo"};

    return name;
}

const std::wstring &PCCMetaInformation::TableDescription() const {
    static const std::wstring descr{L"meta data information"s};
    return descr;
}

const std::vector<PCCMetaInformation::Field> &PCCMetaInformation::TableFields() const {
    static const std::vector<PCCMetaInformation::Field> fields = {
            {"Name", "TEXT NOT NULL UNIQUE"},
            {"Value", "TEXT NOT NULL"}
    };

    return fields;
}

const PCCMetaInformation::TTableData &PCCMetaInformation::TableInitialData() const {
    static const TTableData initialData = {
            { QString::fromStdWString(gitHashField), QString::fromLatin1(COMPILE_GIT_HASH) },
            { QString::fromStdWString(buildTimestampField), QString::fromLatin1(__DATE__ " " __TIME__) },
            { QString::fromStdWString(databaseInterfaceVersionField), QString::number(PCCDatabase::InterfaceVersion()) },
            { QString::fromStdWString(unitsInterfaceVersionField), QString::number(PCCUnits::MaxInterfaceVersion()) }
    };

    return initialData;
}

void PCCMetaInformation::SetTableData(bool previouslyInitializedData, PCCMetaInformation::TTableData &&table) {
    for (const auto &row : table) {
        constexpr auto attribId = 0;
        constexpr auto valueId = 1;
        std::wstring attrib = row[attribId].toStdWString();
        std::wstring value = row[valueId].toStdWString();

        if (attrib == gitHashField) {
            _gitHash = value;
            if (previouslyInitializedData)
                logInfo(L"Database's GitHash : "s, _gitHash);
        } else if (attrib == buildTimestampField) {
            _buildTimestamp = value;
            if (previouslyInitializedData)
                logInfo(L"Database's build timestamp : "s, _buildTimestamp);
        } else if (attrib == databaseInterfaceVersionField) {
            _dbInterfaceVersion = std::stol(value);
            if (previouslyInitializedData)
                logInfo(L"Database's interface : "s, _dbInterfaceVersion, L", current one : "s, PCCDatabase::InterfaceVersion());
        } else if (attrib == unitsInterfaceVersionField) {
            _unitsInterfaceVersion = std::stol(value);
            if (previouslyInitializedData)
                logInfo(L"Units's interface : "s, _unitsInterfaceVersion);
        }
    }
}
