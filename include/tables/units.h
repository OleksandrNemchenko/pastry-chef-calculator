//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_TABLES_UNITS_H_
#define PASTRY_CHEF_CALCULATOR_TABLES_UNITS_H_

#include <app.h>

class PCCUnitsTransform;

class PCCUnits : public QAbstractListModel, public PCCDbTable {
Q_OBJECT

public:
    static constexpr size_t MaxInterfaceVersion () { return _maxInterfaceVersion; }
    void SetUnitsTransform(const PCCUnitsTransform& unitsTransforms);

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

    enum class EUnitRoles : int {
        TYPE = Qt::UserRole + 1,
        NAME,
        IS_DEFAULT
    };

private:
    enum class EUnitType{
        FIRST = 0,

        CURRENCY = FIRST,
        WEIGHT,
        VOLUME,
        PIECE,

        UNITS_AMOUNT
    };

    constexpr static size_t _maxInterfaceVersion = 1;
    struct SUnitData {
        size_t _dbId;
        EUnitType _type;
        QString _name;
        bool _default;

        struct STranform {
            SUnitData &_toTransform;
            double _thisValue;
            double _toValue;

            STranform(SUnitData &toTransform, double thisValue, double toValue): _toTransform(toTransform),
                                                                                 _thisValue(thisValue), _toValue(toValue) {}
        };

        std::vector<STranform> _transform;
    };

    void SetTableDataInterface1(bool previouslyInitializedData, TTableData &&table);
    static const QString& typeDescription(EUnitType unitType);

    using TUnitsArray = std::vector<SUnitData>;
    TUnitsArray _units;
};

#endif //PASTRY_CHEF_CALCULATOR_TABLES_UNITS_H_
