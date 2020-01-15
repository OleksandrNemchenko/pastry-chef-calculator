//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#include <app.h>
#include <database.h>

PCCDatabase::PCCDatabase() {
    this->moveToThread(&_dbThread);
    _dbThread.start(QThread::LowPriority);
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

void PCCDatabase::TestRWAccess() {
}

void PCCDatabase::Initialize() {
    auto logTask = _log.AddTask();

    logInfo(L"Initialize database"s);
    emit databaseInitialization();

    try {
        InitConnection();
        TestRWAccess();

        logTask.Succeeded();

        emit databaseInitializedSuccessfully();
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

    bool exec = query.exec( query_str );
    if( !exec || ( !allow_error && query.lastError().type() != QSqlError::NoError )){
        logError( L"SQL error : "s, query.lastError().text().toStdWString() );
        return res;
    }

    result = true;

    return res;
}
