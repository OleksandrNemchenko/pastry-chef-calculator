
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Item {
    anchors.fill: parent

    ListView {
        id: unitsView
        property int rightMargin: unitsViewScrollBar.width

        anchors.margins: common.listMargin
        anchors.fill: parent

        model: PCCUnits

        ScrollBar.vertical: ScrollBar {
            id: unitsViewScrollBar
            active: true
        }

        section {
            property: "type"

            delegate: UnitSection {
                id: typeSection
            }
        }   // section

        delegate: UnitDelegate {
            id: unitDelegate
        }   // delegate, id: unitDelegate
    }   // ListView, id: unitsView
}
