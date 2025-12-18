import QtQuick
import QtQuick.Controls

Item {

    width: 200
    height: 200


    Loader{
        anchors.centerIn:  parent
        sourceComponent:  phoneNumber
    }

    Component{
        id: phoneNumber

        TextField{
            id: root
            font.pixelSize: 16
            width: 100
            height: 50
            padding: 10
            color: "white"
            placeholderTextColor: "#AAAAAA"

            background: Rectangle {

                radius: 8
                color: "#1d1d1d"
                border.color: root.activeFocus ? "#4da3ff" : "#444"
                border.width: 1
            }

            onAccepted: {
                console.log("text: ", text)
                roottgcontoller.phoneNumber = text
            }
        }
    }
}
