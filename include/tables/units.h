//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_TABLES_UNITS_H_
#define PASTRY_CHEF_CALCULATOR_TABLES_UNITS_H_

#include <app.h>

class PCCUnitsTransform;
struct SUnitTransform;

enum class EUnitType{
    FIRST = 0,

    CURRENCY = FIRST,
    WEIGHT,
    VOLUME,
    PIECE,

    UNITS_AMOUNT
};

struct SUnitData {
    int _dbId;
    EUnitType _type;
    QString _title;
    QString _abbreviaton;

    bool operator<(const SUnitData& right) const;

};


class PCCUnits : public QAbstractListModel, public PCCDbTable {
Q_OBJECT

public:
    static constexpr size_t MaxInterfaceVersion () { return _maxInterfaceVersion; }
    bool isSingleUnitForType(const SUnitData& unit) const;

    static QJsonArray buildUnitTypesModel();

    void AddField(const SUnitData &unit);

    const SUnitData& Unit(int dbUnit) const;
    SUnitData& Unit(int dbUnit);
    void Refresh();

//    PCCDbTable implementations
    const QString &TableName() const override;
    const QString &TableDescription() const override;
    const TTableFields &TableFields() const override;
    const TTableData &TableInitialData() const override;
    void SetTableData(PCCDatabase* db, bool previouslyInitializedData, TTableData &&table) override;

//    QAbstractListModel implementations
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

//    Access from QML
    Q_INVOKABLE QJsonArray unitTransforms(int dbId);
    Q_INVOKABLE QJsonArray unitTransformsToSelection(int idFromUnit);
    Q_INVOKABLE void unitTransformDelete(int idUnit, int idUnitTransform);
    Q_INVOKABLE void unitTransformAdd(int idUnitFrom, int idUnitTo, QString valueFrom, QString valueTo);
    Q_INVOKABLE void unitDelete(int idUnit);
    Q_INVOKABLE void unitAdd(const QString& typeTitle, const QString& title, const QString& abbreviation);

    enum class EUnitRoles : int {
        DB_ID = Qt::UserRole + 1,
        TYPE,
        TYPE_ID,
        TITLE,
        ABBREVIATION,
        IS_SINGLE,
    };

private:

    using TUnitsArray = QVector<SUnitData>;
    using TTypeMap = QMap<EUnitType, QString>;

    TUnitsArray _units;
    static const TTypeMap _typeMap;
    constexpr static size_t _maxInterfaceVersion = 1;

    void SetTableDataInterface1(bool previouslyInitializedData, TTableData &&table);

    template<typename TRet, typename TIt>
    static TRet& UnitImpl(TIt begin, TIt end, int dbUnit);

};

#endif //PASTRY_CHEF_CALCULATOR_TABLES_UNITS_H_
