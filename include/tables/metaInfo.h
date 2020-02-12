//
// Created by Oleksandr Nemchenko on 12.01.2020.
//

#ifndef PASTRY_CHEF_CALCULATOR_META_INFORMATION_H_
#define PASTRY_CHEF_CALCULATOR_META_INFORMATION_H_

#include <QtCore>
#include <QtSql>

class PCCMetaInformation : public QObject, public PCCDbTable {
    Q_OBJECT

public:
    using TPtr = std::unique_ptr<PCCMetaInformation>;

    const QString &TableName() const override;
    const std::wstring &TableDescription() const override;
    const TTableFields &TableFields() const override;
    const TTableData &TableInitialData() const override;
    void SetTableData(bool previouslyInitializedData, TTableData &&table) override;
    static constexpr size_t MaxInterfaceVersion () { return _maxInterfaceVersion; }

    size_t UnitsInterfaceVersion() const            { return _unitsInterfaceVersion; }
    size_t UnitsTransformInterfaceVersion() const   { return _unitsTransformInterfaceVersion; }

private:
    std::wstring _gitHash;
    std::wstring _buildTimestamp;
    size_t _dbInterfaceVersion;
    size_t _unitsInterfaceVersion;
    size_t _unitsTransformInterfaceVersion;
    constexpr static size_t _maxInterfaceVersion = 1;
};

#endif //PASTRY_CHEF_CALCULATOR_META_INFORMATION_H_
