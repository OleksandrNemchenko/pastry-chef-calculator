//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_TABLES_UNITS_TRANSFORM_H_
#define PASTRY_CHEF_CALCULATOR_TABLES_UNITS_TRANSFORM_H_

#include <QtCore>
#include <QtSql>

struct SUnitData;

struct SUnitTransform{
    int _dbId;
    int _from;
    int _to;
    double _fromValue;
    double _toValue;
};

class PCCUnitsTransform : public QObject, public PCCDbTable {
Q_OBJECT

public:
    using TUnitsTransformArray = QVector<SUnitTransform>;

    const QString &TableName() const override;
    const QString &TableDescription() const override;
    const TTableFields &TableFields() const override;
    const TTableData &TableInitialData() const override;
    void SetTableData(PCCDatabase* db, bool previouslyInitializedData, TTableData &&table) override;
    bool DeleteRecord(size_t dbId) override;
    const SUnitTransform* AddTransform(uint idUnitFrom, uint idUnitTo, double fromValue, double toValue);

    static constexpr size_t MaxInterfaceVersion () { return _maxInterfaceVersion; }

    QJsonArray BuildModel(int dbFromUnit) const;
    const TUnitsTransformArray &Transforms() const { return _unitsTransform; }
    void DeleteUnitTransforms(int unitDbId);

    const SUnitTransform& UnitTransform(int dbUnit) const;
    SUnitTransform& UnitTransform(int dbUnit);

private:
    constexpr static size_t _maxInterfaceVersion = 1;
    TUnitsTransformArray _unitsTransform;

    void SetTableDataInterface1(PCCDatabase* db, bool previouslyInitializedData, TTableData &&table);

    template<typename TRet, typename TIt>
    static TRet& UnitTransformImpl(TIt begin, TIt end, int dbUnit);

};

#endif //PASTRY_CHEF_CALCULATOR_TABLES_UNITS_TRANSFORM_H_
