import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

Window {
    visible: true
    width: 500
    height: 250
    title: qsTr("Fingerprint GROW RX305 test")

    property int counter: 1

    SpinBox {
        id: id
        from: 0
        to: 1000
        stepSize: 1
        focus: true
        editable: true
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
    }

    Button {
        id: btnEnroll
        text: "Enroll"
        anchors.top: id.bottom
        anchors.topMargin: 10
        anchors.left: id.left
        onClicked: {
            fingerId.visible = false
            fpMgr.enroll(id.value)
        }

    }

    Button {
        text: "Cancel"
        anchors.top: btnEnroll.top
        anchors.left: btnEnroll.right
        anchors.leftMargin: 10
        onClicked: {
            fingerId.visible = false
            fpMgr.cancel()
        }

    }

    Button {
        id: btnSearch
        text: "Search"
        anchors.top: btnEnroll.bottom
        anchors.topMargin: 10
        anchors.left: btnEnroll.left
        onClicked: {
            fingerId.visible = false
            fpMgr.search()
        }
    }

    Text {
        id: info
        text: "Info"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }

    Text {
        id: fingerId
        anchors.centerIn: parent
        visible: false
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
