//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_TABLES_UNITS_H_
#define PASTRY_CHEF_CALCULATOR_TABLES_UNITS_H_

#include <QtCore>
#include <QtSql>

class PCCUnits : public QObject, public PCCDbTable {
Q_OBJECT

public:
    enum EUnitType{
        CURRENCY = 0,
        WEIGHT,
        VOLUME,
        PIECE
    };

    const QString &TableName() const override;
    const std::wstring &TableDescription() const override;
    const std::vector<Field> &TableFields() const override;
    const TTableData &TableInitialData() const override;
    void SetTableData(bool previouslyInitializedData, TTableData &&table) override;
    static constexpr size_t MaxInterfaceVersion () { return _maxInterfaceVersion; }

private:
    constexpr static size_t _maxInterfaceVersion = 1;
    struct SUnitData{
        size_t _dbId;
        std::wstring _name;
        bool _default;
    };

    void SetTableDataInterface1(bool previouslyInitializedData, TTableData &&table);

    using TUnitsArray = std::vector<SUnitData>;
    TUnitsArray _currencies;
    TUnitsArray _weights;
    TUnitsArray _volumes;
    TUnitsArray _pieces;
};

#endif //PASTRY_CHEF_CALCULATOR_TABLES_UNITS_H_
