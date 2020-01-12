import QtQuick 2.0
import QtQuick.Layouts 1.5

Rectangle {
    property int buttonSide : common.dp(40)
    width: parent.width
    height: common.dp(50)
    color: "gray"

    RowLayout {
        anchors.fill: parent

        Image {
            id: unitsButton
            source: "pics/Units.svg"
            sourceSize.width: buttonSide
            sourceSize.height: buttonSide
            MouseArea {
                anchors.fill: parent
                onClicked: { changeSelection(Common.ActiveMode.Units); }
            }        }

        Image {
            id: pricesButton
            source: "pics/Prices.svg"
            sourceSize.width: buttonSide
            sourceSize.height: buttonSide
            MouseArea {
                anchors.fill: parent
                onClicked: { changeSelection(Common.ActiveMode.Prices); }
            }
        }

        Image {
             id: receiptsButton
             source: "pics/Receipts.svg"
             sourceSize.width: buttonSide
             sourceSize.height: buttonSide
            MouseArea {
                anchors.fill: parent
                onClicked: { changeSelection(Common.ActiveMode.Receipts); }
            }        }

        Image {
             id: calcResultButton
             source: "pics/CalcResult.svg"
             sourceSize.width: buttonSide
             sourceSize.height: buttonSide
            MouseArea {
                anchors.fill: parent
                onClicked: { changeSelection(Common.ActiveMode.CalcResult); }
            }        }
    }
}