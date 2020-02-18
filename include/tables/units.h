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
    uint _dbId;
    EUnitType _type;
    QString _title;
    QString _abbreviaton;
    bool _default;

    bool operator<(const SUnitData& right) const;

    QVector<const SUnitTransform*> _transform;
};


class PCCUnits : public QAbstractListModel, public PCCDbTable {
Q_OBJECT

public:
    static constexpr size_t MaxInterfaceVersion () { return _maxInterfaceVersion; }
    void SetUnitsTransform(PCCUnitsTransform* unitsTransforms);

    // PCCDbTable implementations
    const QString &TableName() const override;
    const QString &TableDescription() const override;
    const TTableFields &TableFields() const override;
    const TTableData &TableInitialData() const override;
    void SetTableData(bool previouslyInitializedData, TTableData &&table) override;

//    QAbstractListModel implementations
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

//  Access from QML
    Q_INVOKABLE QJsonArray unitTransforms(uint dbId);
    Q_INVOKABLE void unitTransformDelete(uint idUnit, uint idUnitTransform);

    enum class EUnitRoles : int {
        DB_ID = Qt::UserRole + 1,
        TYPE,
        TITLE,
        ABBREVIATION,
        IS_DEFAULT
    };

private:

    constexpr static size_t _maxInterfaceVersion = 1;


    void SetTableDataInterface1(bool previouslyInitializedData, TTableData &&table);
    static const QString& typeDescription(EUnitType unitType);

    using TUnitsArray = QVector<SUnitData>;
    TUnitsArray _units;
};

#endif //PASTRY_CHEF_CALCULATOR_TABLES_UNITS_H_
