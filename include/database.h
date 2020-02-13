//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_DATABASE_H_
#define PASTRY_CHEF_CALCULATOR_DATABASE_H_

#include <QtCore>
#include <QtSql>

class PCCMetaInformation;
class PCCUnits;
class PCCUnitsTransform;

class PCCDbTable {
public:
    struct TField {
        QString name;
        QString creationParameters;
    };

    using TTableRow = std::vector<QString>;
    using TTableData = std::vector<TTableRow>;
    using TTableFields = std::vector<TField>;

    virtual const QString &TableName () const = 0;
    virtual const QString &TableDescription () const = 0;
    virtual const TTableFields &TableFields () const = 0;
    virtual const TTableData &TableInitialData () const { static const TTableData emptyData; return emptyData; }
    virtual void SetInterfaceVersion (size_t version) { _currentInterfaceVersion = version; }
    virtual size_t CurrentInterfaceVersion () const { return _currentInterfaceVersion; }
    virtual void SetTableData (bool previouslyInitializedData, TTableData &&table) = 0;

protected:
    size_t _currentInterfaceVersion;
};

class PCCDatabase : public QObject {
    Q_OBJECT

public:
    PCCDatabase();
    ~PCCDatabase();
    PCCDatabase(const PCCDatabase&) = delete;
    PCCDatabase(PCCDatabase&&) = delete;
    PCCDatabase& operator=(const PCCDatabase&) = delete;
    PCCDatabase& operator=(PCCDatabase&&) = delete;

    using TExecuteQuery = QPair<bool, QSqlQuery>;
    TExecuteQuery ExecuteQuery(QString &&descr, QString query_str, bool allow_error = false ) const;

    constexpr static size_t InterfaceVersion()  { return 1; }

    bool hasDbError() const                     { return !_lastError.isEmpty(); }
    const QString& lastDbError() const          { return _lastError; }

    PCCUnits& units()                           { return *_units.get(); }

private:
    QSqlDatabase _database;
    std::unique_ptr<PCCMetaInformation> _metaInfo;
    std::unique_ptr<PCCUnits> _units;
    std::unique_ptr<PCCUnitsTransform> _unitsTransform;
    QString _lastError;

    void Initialize();
    void InitConnection();

    template<typename TTable>
    void InitTable(std::unique_ptr<TTable> &table, size_t neededVersion);

};
/*
class PCCDatabaseQml : public QObject {
    Q_OBJECT

public:
    PCCDatabaseQml();
    ~PCCDatabaseQml();

    PCCDatabase &dataBase() { return *_db.get(); }
    QAbstractListModel* unitsModel();

signals:
    void databaseInitialization();
    void databaseInitializedSuccessfully();
    void databaseInitializationError(QString errorDescription);

public slots:
    void Initialize();

private:
    std::unique_ptr<PCCDatabase> _db;
    QThread _dbThread;
};
*/
#endif //PASTRY_CHEF_CALCULATOR_DATABASE_H_
