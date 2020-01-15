//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_DATABASE_H_
#define PASTRY_CHEF_CALCULATOR_DATABASE_H_

#include <QtCore>
#include <QtSql>

class PCCDatabase : public QObject {
    Q_OBJECT

public:
    PCCDatabase();
    ~PCCDatabase() override;

signals:
    void databaseInitialization();
    void databaseInitializedSuccessfully();
    void databaseInitializationError(QString errorDescription);

public slots:
    void Initialize();

private:
    QSqlDatabase _database;
    QThread _dbThread;

    void InitConnection();
    void TestRWAccess();

    using TExecuteQuery = std::pair<bool, QSqlQuery>;
    TExecuteQuery ExecuteQuery( std::wstring &&descr, QString query_str, bool allow_error = false ) const;
};

class PCCDatabaseQml : public QObject {
    Q_OBJECT

signals:
    void databaseInitialization();
    void databaseInitializedSuccessfully();
    void databaseInitializationError(QString errorDescription);
    void Initialize();

};

#endif //PASTRY_CHEF_CALCULATOR_DATABASE_H_
