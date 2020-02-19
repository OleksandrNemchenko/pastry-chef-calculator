
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4

Item {
    id: common

    enum ActiveMode {
        Units,
        Prices,
        Receipts,
        CalcResult
    }

    property int dpi: Screen.pixelDensity * 25.4
    property int desktopDPI: 120
    property int dip2pixels: 160
    function dp(x) {
        if(dpi < desktopDPI) {
            return x;
        } else {
            return x * (dpi / dip2pixels);
        }
    }

    property int smallButtonSide: dp(20)
    property int listMargin: dp(5)
    property int sectionTitleMargin: dp(5)
    property int unitMargin: dp(5)
    property int unitTransformMargin: dp(5)
    property int buttonsMargin: dp(5)

    property color titleColor: "darkGray"
    property color titleBackground: "lightGray"

    property color sectionTitleBackground: "white"
    property color sectionTitleColor: "darkBlue"

    property color unitBackground: "white"
    property color unitColor: "darkRed"

    property color unitTransformBackground: "white"
    property color unitTransformColor: "darkGreen"

    property bool titleFontBold: true
    property int titleFontPointSize: 30

    property bool sectionTitleBold: true
    property int sectionTitlePointSize: 14

    property bool unitBold: false
    property int unitPointSize: 14

    property bool unitTransformBold: false
    property int unitTransformPointSize: 14

    property int unitTransformHeight: dp(14)

    property int indent: dp(20)
}
