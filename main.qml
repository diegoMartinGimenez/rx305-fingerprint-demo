import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQml.StateMachine 1.0 as DSM


Window {
    visible: true
    width: 500
    height: 250
    title: qsTr("Hello World")
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

    DSM.StateMachine {
        id: stateMachine
        initialState: idle
        running: true

        DSM.State {
            id: idle
            DSM.SignalTransition {
                signal: btnEnroll.clicked
                targetState: enrollingState
            }
        }
        DSM.State {
            id: enrollingState
            onEntered: info.text = "Enrolling"
        }
    }

//    DSM.StateMachine {
//        id: stateMachine
//        initialState: idle
//        running: true

//        DSM.State {
//            id: idle
//            DSM.SignalTransition {
//                signal: updateButton.clicked
//                targetState: validatingUrlLang
//            }
//        }
//        DSM.State {
//            id: validatingUrlLang
//            DSM.SignalTransition {
//                signal: _http.errorChanged
//                targetState: idle
//                onTriggered: showError(_http.error)
//            }
//            DSM.SignalTransition {
//                signal: _http.success
//                targetState: updatingLang
//                onTriggered: {
//                    //httpWindow.reqFinished.connect(LangModel.update)
//                }
//            }
//            onEntered: {
//            }
//        }
//        DSM.State {
//            id: updatingLang
//            DSM.SignalTransition {
//                signal: _http.errorChanged
//                targetState: idle
//                onTriggered: showError(_http.error)
//            }
//            DSM.SignalTransition {
//                signal: _http.finished
//                targetState: delay
//                onTriggered: LangModel.update(code, content)
//            }
//        }
//        DSM.State {
//            id: delay
//            DSM.TimeoutTransition {
//                timeout: 100
//                targetState: validatingUrlContrib
//            }
//        }
//        DSM.State {
//            id: validatingUrlContrib
//            DSM.SignalTransition {
//                signal: _http.errorChanged
//                targetState: idle
//                onTriggered: showError(_http.error)
//            }
//            DSM.SignalTransition {
//                signal: _http.success
//                targetState: updatingContrib
//                onTriggered: console.log("to updatingContrib")
//            }
//            onEntered: {
//                var url = "https://api.github.com/repos/" + orgTextField.text.trim() + "/" +
//                          repoTextField.text.trim() + "/contributors"
//                /*var url = "https://api.github.com/repos/vortexmakes/RKH" + "/stats/contributors"*/
//                var header = "Accept: application/vnd.github.v3+json"
//                _http.executeGet(url, header, tokenTextField.text)
//            }
//        }
//        DSM.State {
//            id: updatingContrib
//            DSM.SignalTransition {
//                signal: _http.errorChanged
//                targetState: idle
//                onTriggered: showError(_http.error)
//            }
//            DSM.SignalTransition {
//                signal: _http.finished
//                targetState: idle
//                onTriggered: {
//                    ContribModel.update(code, content)

//                    var obj = JSON.parse(content)
//                    var contrib = "Contributions: ["
//                    var total = 0.0

//                    // Calculates the total contributions
//                    for (var n in obj) {
//                        var cont = obj[n]
//                        total += cont["contributions"]
//                    }
//                    // Makes a list storing information about each contributor
//                    for (var k in obj) {
//                        var author = obj[k]
//                        var per = (author["contributions"] * 100)/total
//                        contrib += " " + author["login"] + ": " + per.toFixed(2) + "% "
//                                + "(" + author["contributions"] + ")"
//                    }
//                    contrib += " ]"
//                    statusLabel.text = ""
//                    statusLabel.text = contrib + "\n\n"
//                    updateButton.focus = true
//                }
//            }
//        }
//    }

}
