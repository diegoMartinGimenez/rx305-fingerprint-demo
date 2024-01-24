import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQml.StateMachine 1.0 as DSM


Window {
    visible: true
    width: 500
    height: 250
    title: qsTr("Hello World")

    property int counter: 1

    Rectangle {
        id: rect
        width: 200
        height: 40
        border.color: "black"
        border.width: 2
        TextEdit {
            id: txtEdit
            anchors.fill: parent
            padding: 5
            focus: true
        }
    }
    Button {
        id: btnConnect
        text: "Connect"
        onClicked: fpMgr.loop()
        anchors.top: rect.bottom
        anchors.left: rect.left

    }

//    Connections {
//        target: fpMgr
//        onCommandSuccess: {
//            info.text = "OK" + counter
//            counter += 1
//        }


//    }

    Button {
        text: "Cancel"
        onClicked: fpMgr.cancel()
        anchors.top: rect.bottom
        anchors.left: btnConnect.right
        anchors.leftMargin: 10
    }

    Button {
        id: btnEnroll
        text: "Enroll"
        anchors.top: btnConnect.bottom
        anchors.topMargin: 10
        anchors.left: btnConnect.left
    }

    Text {
        id: info
        text: "Info"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }

//    DSM.StateMachine {
//        id: stateMachine
//        initialState: idle
//        running: true

//        DSM.State {
//            id: idle
//            DSM.SignalTransition {
//                signal: btnEnroll.clicked
//                targetState: enrollingState
//            }
//        }
//        DSM.State {
//            id: enrollingState
//            initialState: enrolling_State1
//            onEntered: {
//                info.text = "Enrolling"
//            }
//            DSM.State {
//                id: enrolling_State1
//                onEntered: fpMgr.loop()
//                DSM.SignalTransition {
//                    signal: fpMgr.commandSuccess
//                    targetState: enrolling_State2
//                }
//            }
//            DSM.State {
//                id: enrolling_State2
//                onEntered: {
//                    info.text = "Step 2"
//                }
//            }
//        }
//    }

}
