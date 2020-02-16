//  Copyright (C) 2015-2020 Virgil Security, Inc.
//
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//      (1) Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//      (2) Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in
//      the documentation and/or other materials provided with the
//      distribution.
//
//      (3) Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
//  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
//  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
//  Lead Maintainer: Virgil Security Inc. <support@virgilsecurity.com>


import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Item {
    id: common

    enum ActiveMode {
        Units,
        Prices,
        Receipts,
        CalcResult
    }

    enum DbMode {
        DbInitializing,
        DbError,
        DbInitialized
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
