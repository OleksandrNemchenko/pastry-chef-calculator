
import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.5

ApplicationWindow {
    id: application
    visible: true
    title: qsTr("Pastry Chef Calculator")

    Common {
        id: common
    }

    function changeSelection(sel) {
        units.visible = sel == Common.ActiveMode.Units
        prices.visible = sel == Common.ActiveMode.Prices
        receipts.visible = sel == Common.ActiveMode.Receipts
        calcResult.visible = sel == Common.ActiveMode.CalcResult

        switch(sel){
        case Common.ActiveMode.Units : header.text = qsTr("Единицы измерения"); break
        case Common.ActiveMode.Prices : header.text = qsTr("Цены"); break
        case Common.ActiveMode.Receipts : header.text = qsTr("Рецепты"); break
        case Common.ActiveMode.CalcResult : header.text = qsTr("Расчеты"); break
        }
    }

    property int layoutsSpacing: common.dp(5)

    header: Header {
        id: header
    }

    DbStatusView {
        id: dbStatusView
    }

    Units {
        id: units
    }

    Prices {
        id: prices
    }

    Receipts {
        id: receipts
    }

    CalcResult {
        id: calcResult
    }

    footer: Footer {
        id: footer
    }

    Component.onCompleted: {
        changeSelection(Common.ActiveMode.Units)
        dbStatusView.showDbState(hasDbError, lastDbError)
    }

}