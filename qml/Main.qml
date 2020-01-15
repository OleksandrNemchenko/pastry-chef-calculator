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
        db.Initialize()
    }
}