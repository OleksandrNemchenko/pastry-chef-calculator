//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_TABLES_UNITS_TRANSFORM_H_
#define PASTRY_CHEF_CALCULATOR_TABLES_UNITS_TRANSFORM_H_

#include <QtCore>
#include <QtSql>

struct SUnitData;

struct SUnitTransform{
    uint _dbId;
    uint _fromUnit;
    uint _toUnit;
    const SUnitData* _from;
    const SUnitData* _to;
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
    void SetTableData(bool previouslyInitializedData, TTableData &&table) override;
    bool DeleteField(size_t dbId) override;

    static constexpr size_t MaxInterfaceVersion () { return _maxInterfaceVersion; }

    QJsonArray unitTransforms(uint dbFromUnit) const;
    const TUnitsTransformArray &Transforms() const { return _unitsTransform; }
    void SetTransformPointers(uint dbId, SUnitData* from, SUnitData* to);

private:
    constexpr static size_t _maxInterfaceVersion = 1;

    void SetTableDataInterface1(bool previouslyInitializedData, TTableData &&table);

    TUnitsTransformArray _unitsTransform;
};

#endif //PASTRY_CHEF_CALCULATOR_TABLES_UNITS_TRANSFORM_H_
