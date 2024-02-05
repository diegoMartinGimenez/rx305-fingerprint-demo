import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

Window {
    id: root
    visible: true
    width:  500
    height:	620
    title: qsTr("Fingerprint GROW RX305 test")

    onWidthChanged: {
        if (root.width > root.height) {
            container.width = root.height
            container.height = root.width
            container.rotation = -90
            buttonsWidth = 250
            buttonsHeight = 100
            pixelSize = 32
        }
    }

    property int buttonsWidth: 140
    property int buttonsHeight: 40
    property int pixelSize: 14

    Item {
        id: container
        width:  500
        height:	620
        anchors.centerIn: parent

        SpinBox {
            id: id
            from: 0
            to: 1000
            stepSize: 1
            focus: true
            editable: true
            width: buttonsWidth
            height: buttonsHeight
            font.pixelSize: pixelSize
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

        Button {
            id: btnEnroll
            text: "Enroll"
            width: buttonsWidth
            height: buttonsHeight
            font.pixelSize: pixelSize
            anchors.top: id.bottom
            anchors.topMargin: 40
            anchors.left: id.left
            onClicked: {
                fingerId.visible = false
                fpMgr.enroll(id.value)
            }
        }

        Button {
            id: btnCancel
            text: "Cancel"
            width: buttonsWidth
            height: buttonsHeight
            font.pixelSize: pixelSize
            anchors.top: btnDelete.bottom
            anchors.left: btnEnroll.left
            anchors.topMargin: 40
            onClicked: {
                fingerId.visible = false
                fpMgr.cancel()
            }
        }

        Button {
            id: btnSearch
            text: "Search"
            width: buttonsWidth
            height: buttonsHeight
            font.pixelSize: pixelSize
            anchors.top: btnEnroll.bottom
            anchors.topMargin: 40
            anchors.left: btnEnroll.left
            onClicked: {
                fingerId.visible = false
                fpMgr.search()
            }
        }

        Button {
            id: btnDelete
            text: "Delete"
            width: buttonsWidth
            height: buttonsHeight
            font.pixelSize: pixelSize
            anchors.top: btnSearch.bottom
            anchors.topMargin: 40
            anchors.left: btnEnroll.left
            onClicked: {
                fingerId.visible = false
                fpMgr.deleteFingerprint(id.value)
            }
        }

        Text {
            id: info
            text: "Info"
            font.pixelSize: pixelSize
            anchors.bottom: parent.bottom
            anchors.right: parent.right
        }

        Text {
            id: fingerId
            font.pixelSize: pixelSize
            anchors.top: btnCancel.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: container.horizontalCenter
            visible: false
        }


    }


    Connections {
        target: fpMgr
        onCommandSuccess: {
            info.text = "OK"
        }
        onCommandError: {
            info.text = error
        }
        onFingerprintFound: {
            fingerId.text = "Fingerprint id: " + id
            fingerId.visible = true
        }

    }

}
