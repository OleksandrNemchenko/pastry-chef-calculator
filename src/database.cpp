//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <database.h>
#include <tables/metaInfo.h>
#include <tables/units.h>
#include <tables/units-transform.h>

PCCDatabase::PCCDatabase() {
    _metaInfo = std::make_unique<PCCMetaInformation>();
    _units = std::make_unique<PCCUnits>();
    _unitsTransform = std::make_unique<PCCUnitsTransform>();

    Initialize();
}

PCCDatabase::~PCCDatabase() {
}

void PCCDatabase::InitConnection() {
    QSettings settings;

    settings.beginGroup("Database");

    QString dbType = settings.value("dbType", "QSQLITE").toString();
    QString hostName = settings.value("hostName").toString();
    QString databaseName = settings.value("databaseName", "pastry-chef-calculator.db3").toString();
    QString userName = settings.value("userName").toString();
    QString password = settings.value("password").toString();

    logDebug(L"Access parameters : database type \""s, dbType,
             L"\", host name \""s, hostName,
             L"\", database name \""s, databaseName,
             L"\", user name \""s, userName,
             L"\", password: "s, password.isEmpty() ? L"absent"s : L"present"s
    );

    _database = QSqlDatabase::addDatabase(dbType);
    if(!_database.isValid())
        throw QString("Invalid database type");

    _database.setHostName(hostName);
    _database.setDatabaseName(databaseName);
    _database.setUserName(userName);
    _database.setPassword(password);
    if(!_database.open())
        throw QString("Unable to open database");
}

template<typename TTable>
void PCCDatabase::InitTable(std::unique_ptr<TTable> &table, size_t detectedVersion) {
    logDebug(L"Initialize "s, table->TableDescription());

    table->SetInterfaceVersion(detectedVersion);

    auto &tableFields = table->TableFields();
    ssize_t dataSize = 0;

    auto [getSizeSuccess, getSizeQuery] = ExecuteQuery(
            QString("Get %1 table size").arg(table->TableDescription()),
            QString(R"(SELECT count(*) from %1;)"). arg(table->TableName()),
            true);

    if(!getSizeSuccess || !getSizeQuery.next()) {
        QString query = "CREATE TABLE \"" + table->TableName() + "\" (";

        bool firstField = true;
        for(auto &field : table->TableFields()) {
            if(firstField)  firstField = false;
            else            query += ", ";

            query += QString("\"%1\" %2"). arg(field.name). arg(field.creationParameters);
        }
        query += ");";

        auto [createTableSuccess, createTableQuery] = ExecuteQuery(
                QString("Create %1 table").arg(table->TableDescription()), query, false);
        if(!createTableSuccess)
            throw QString("Unable to create table");
    } else
        dataSize = getSizeQuery.value(0).toInt();

    bool previouslyInitializedData = true;
    if(dataSize <= 1 )
    {
        const auto fillData = table->TableInitialData();

        if(fillData.size() > 0 ){
            dataSize = fillData.size();
            previouslyInitializedData = false;

            QString query = QString("INSERT INTO %1 ("). arg(table->TableName());

            bool firstField = true;
            for(auto &field : tableFields) {
                if(firstField)  firstField = false;
                else            query += ", ";

                query += field.name;
            }

            query += ") VALUES (";

            firstField = true;
            for(auto &data : fillData) {
                if(firstField)  firstField = false;
                else            query += ", (";

                assert(data.size() == tableFields.size());

                bool firstValue = true;
                for(auto &value: data) {
                    if(firstValue)  firstValue = false;
                    else            query += ", ";

                    query += QString("'%1'"). arg(value);
                }
                query += ")";
            }

            auto [fillInitDataSuccess, fillInitDataQuery] = ExecuteQuery(
                QString("Fill initial data to the %1 table").arg(table->TableDescription()), query, false );

            if(!fillInitDataSuccess)
                throw QString("Unable to fill initial data");
        }
    }

    auto [getDataSuccess, getDataQuery] = ExecuteQuery(
            QString("Get %1 table data").arg(table->TableDescription()),
            QString(R"(SELECT * from %1;)"). arg(table->TableName()),
            true);

    PCCDbTable::TTableData tableData;
    if(dataSize > 0)
        tableData.reserve(dataSize);
    logDebug(L"Loaded records : "s, dataSize);

    while(getDataQuery.next()) {
        PCCDbTable::TTableRow tableRow;
        tableRow.reserve(tableFields.size());

        for(auto columnName : tableFields){
            int fieldId = getDataQuery.record().indexOf(columnName.name);
            tableRow.emplace_back(getDataQuery.value(fieldId).toString());
        }

        tableData.emplace_back(tableRow);
    }

    table->SetTableData(previouslyInitializedData, std::move(tableData));
}

void PCCDatabase::Initialize() {
    auto logTask = _log.addTask();

    logInfo(L"Initialize database"s);

    try {
        InitConnection();

        InitTable(_metaInfo, -1);

        InitTable(_unitsTransform, _metaInfo->UnitsTransformInterfaceVersion());

        InitTable(_units, _metaInfo->UnitsInterfaceVersion());
        _units->SetUnitsTransform(_unitsTransform.get());

        logTask.succeeded();
    }
    catch(QString errDescr) {
        _lastError = _database.lastError().text();
        logError(L"Unable to open database. "s, errDescr, L" Last SQL error: "s, _lastError);
    }
}

PCCDatabase::TExecuteQuery PCCDatabase::ExecuteQuery(QString &&descr, QString query_str, bool allow_error) const {
    TExecuteQuery res;
    auto &[result, query] = res;
    query = QSqlQuery( _database );
    result = false;

    logDebug(descr.toStdWString(), L" query : "s, query_str);

    bool exec = query.exec( query_str );
    if (!exec || ( !allow_error && query.lastError().type() != QSqlError::NoError)) {
        logError(L"SQL error : "s, query.lastError().text());
        return res;
    }

    result = true;

    return res;
}

bool PCCDbTable::DeleteRecord (size_t dbId)
{
    auto logTask = _log.addTask();
    QString queryStr = QString("DELETE FROM %1 WHERE %2 = '%3'"). arg(TableName()). arg(IDFieldName()). arg(dbId);

    auto [result, query] = _db->ExecuteQuery(QString("Delete record from %1 table").arg(TableDescription()), queryStr);

    if (!result) {
        logError(L"Unable to delete record from table "s, TableName());
        return false;
    }

    logTask.succeeded();
    return true;
}