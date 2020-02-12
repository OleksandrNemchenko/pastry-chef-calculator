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
    virtual const std::wstring &TableDescription () const = 0;
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
    PCCDatabase(bool initEvent);
    ~PCCDatabase() override;

    using TExecuteQuery = std::pair<bool, QSqlQuery>;
    TExecuteQuery ExecuteQuery( std::wstring &&descr, QString query_str, bool allow_error = false ) const;

    constexpr static size_t InterfaceVersion() { return 1; }
    int InitEventId() const { return _initEventId; }

signals:
    void databaseInitialization();
    void databaseInitializedSuccessfully();
    void databaseInitializationError(QString errorDescription);

protected:
    bool event(QEvent *event) override;

private:
    QSqlDatabase _database;
    std::unique_ptr<PCCMetaInformation> _metaInfo;
    std::unique_ptr<PCCUnits> _units;
    std::unique_ptr<PCCUnitsTransform> _unitsTransform;
    int _initEventId = -1;

    void Initialize();
    void InitConnection();

    template<typename TTable>
    void InitTable(std::unique_ptr<TTable> &table, size_t neededVersion, bool initPointer = true);

};

class PCCDatabaseQml : public QObject {
    Q_OBJECT

public:
    ~PCCDatabaseQml();

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

#endif //PASTRY_CHEF_CALCULATOR_DATABASE_H_
