import QtQuick
import QtQuick.Controls
import Cl


Window {
    id: w
    width: 640
    height: 480
    visible: true

    onClosing:{
        roottgcontoller.finishworkthread();
    }


    Rectangle{
        color: "#1d586b"
        anchors.fill:  parent
    }

    TgController{
        id: roottgcontoller
        onViewChanged:{
            pageLoader.source = view
        }
    }


    Button{
        anchors.left: parent.left
        anchors.top: parent.top

        onClicked: {
            roottgcontoller.loadmusicchat();

        }
    }

    Loader {
        id: pageLoader
        anchors.centerIn: parent
        source: "qmlfiles/phoneview.qml"
    }
}

