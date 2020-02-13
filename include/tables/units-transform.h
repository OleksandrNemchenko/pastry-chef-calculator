//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_TABLES_UNITS_TRANSFORM_H_
#define PASTRY_CHEF_CALCULATOR_TABLES_UNITS_TRANSFORM_H_

#include <QtCore>
#include <QtSql>

class PCCUnitsTransform : public QObject, public PCCDbTable {
Q_OBJECT

public:
    struct SUnitTransform{
        size_t _dbId;
        size_t _fromType;
        size_t _toType;
        long double _fromValue;
        long double _toValue;
    };
    using TUnitsTransformArray = std::vector<SUnitTransform>;

    const QString &TableName() const override;
    const QString &TableDescription() const override;
    const TTableFields &TableFields() const override;
    const TTableData &TableInitialData() const override;
    void SetTableData(bool previouslyInitializedData, TTableData &&table) override;
    static constexpr size_t MaxInterfaceVersion () { return _maxInterfaceVersion; }

    const TUnitsTransformArray &Transforms() const { return _unitsTransform; }

private:
    constexpr static size_t _maxInterfaceVersion = 1;

    void SetTableDataInterface1(bool previouslyInitializedData, TTableData &&table);

    TUnitsTransformArray _unitsTransform;
};

#endif //PASTRY_CHEF_CALCULATOR_TABLES_UNITS_TRANSFORM_H_
