//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <database.h>
#include <tables/metaInfo.h>
#include <tables/units.h>

PCCDatabase::PCCDatabase(bool initEvent) {
    if(initEvent) {
        _initEventId = QEvent::registerEventType();
    }
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

    logDebug(L"Access parameters : database type \""s, dbType.toStdWString(),
             L"\", host name \""s, hostName.toStdWString(),
             L"\", database name \""s, databaseName.toStdWString(),
             L"\", user name \""s, userName.toStdWString(),
             L"\", password: "s, password.isEmpty() ? L"absent"s : L"present"s
    );

    _database = QSqlDatabase::addDatabase(dbType);
    if(!_database.isValid())
        throw L"Invalid database type."s;

    _database.setHostName(hostName);
    _database.setDatabaseName(databaseName);
    _database.setUserName(userName);
    _database.setPassword(password);
    if(!_database.open())
        throw L"Unable to open database."s;
}

template<typename TTable>
void PCCDatabase::InitTable(std::unique_ptr<TTable> &table, size_t detectedVersion, bool initPointer) {
    if(initPointer) {
        table.reset();
        table = std::make_unique<TTable>();
    }

    logInfo(L"Initialize "s, table->TableDescription());

    table->SetInterfaceVersion(detectedVersion);

    auto &tableFields = table->TableFields();
    ssize_t dataSize = 0;

    auto [getSizeSuccess, getSizeQuery] = ExecuteQuery(
            L"Get "s + table->TableDescription() + L" table size"s,
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
                L"Create "s + table->TableDescription() + L" table"s, query, false);
        if(!createTableSuccess)
            throw L"Unable to create table"s;
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

            qWarning("Query: %s", query.toStdString().c_str());
            auto [fillInitDataSuccess, fillInitDataQuery] = ExecuteQuery(
                L"Fill initial data to the "s + table->TableDescription() + L" table"s, query, false );

            if(!fillInitDataSuccess)
                throw L"Unable to fill initial data"s;
        }
    }

    auto [getDataSuccess, getDataQuery] = ExecuteQuery(
            L"Get "s + table->TableDescription() + L" table data"s,
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
//    auto logTask = _log.AddTask();

    logInfo(L"Initialize database"s);
    emit databaseInitialization();

    try {
        InitConnection();

        _metaInfo = std::make_unique<PCCMetaInformation>();
        InitTable(_metaInfo, -1, false);

        InitTable(_units, _metaInfo->UnitsInterfaceVersion(), true);

        emit databaseInitializedSuccessfully();

//        logTask.Succeeded();
    }
    catch(std::wstring errDescr) {
        logError(L"Unable to open database. "s, errDescr, L" Last SQL error: "s, _database.lastError().text().toStdWString());

        emit databaseInitializationError(QString::fromStdWString(errDescr) +
                                         QString::fromStdWString(L"\nSQL error : "s) +
                                         _database.lastError().text());
    }
}

PCCDatabase::TExecuteQuery PCCDatabase::ExecuteQuery(std::wstring &&descr, QString query_str, bool allow_error) const {
    TExecuteQuery res;
    auto &[result, query] = res;
    query = QSqlQuery( _database );
    result = false;

    logDebug( descr, L" query : "s, query_str.toStdWString() );
    qWarning("%s : %s", QString::fromStdWString(descr).toStdString().c_str(), query_str.toStdString().c_str());

    bool exec = query.exec( query_str );
    if( !exec || ( !allow_error && query.lastError().type() != QSqlError::NoError )){
        qWarning("ERROR : %s", query.lastError().text().toStdString().c_str());
        logError( L"SQL error : "s, query.lastError().text().toStdWString() );
        return res;
    }

    result = true;

    return res;
}

bool PCCDatabase::event(QEvent *event) {
    if(event->type() == _initEventId) {
        Initialize();
        return true;
    }
    return QObject::event(event);
}

PCCDatabaseQml::~PCCDatabaseQml(){
    _db.release();

    _dbThread.quit();

    constexpr auto waitThreadMSec = 1000;
    _dbThread.wait(waitThreadMSec);

    if (_dbThread.isRunning()) {
        logError(L"Unable to terminate database thread. Call QThread::terminate()"s);
        _dbThread.terminate();
    }
}

void PCCDatabaseQml::Initialize() {
    connect(&_dbThread, &QThread::finished, this, &QObject::deleteLater);

    _db = std::make_unique<PCCDatabase>(true);
    _db->moveToThread(&_dbThread);
    _dbThread.start(QThread::LowPriority);

    while(!_dbThread.isRunning()) {
        QThread::currentThread()->yieldCurrentThread();
    }

    QObject::connect(_db.get(), &PCCDatabase::databaseInitialization, this, &PCCDatabaseQml::databaseInitialization);
    QObject::connect(_db.get(), &PCCDatabase::databaseInitializedSuccessfully, this, &PCCDatabaseQml::databaseInitializedSuccessfully);
    QObject::connect(_db.get(), &PCCDatabase::databaseInitializationError, this, &PCCDatabaseQml::databaseInitializationError);

    if(_db->InitEventId() == -1) {
        logCritical(L"Unable to initialize database event"s);
    }

    QCoreApplication::postEvent(_db.get(), new QEvent(static_cast<QEvent::Type>(_db->InitEventId())) );
}
