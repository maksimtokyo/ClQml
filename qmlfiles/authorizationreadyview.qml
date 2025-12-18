import QtQuick
import QtQuick.Controls
import QtMultimedia

Item {
    id: aut
    width: 400
    height: 400


    Rectangle{
        color: "black"
        anchors.fill: parent
    }


    Loader{
        anchors.centerIn:  parent
        sourceComponent:  authorizationready
    }

    Component{
        id: authorizationready

        ListView{
            id: lw
            width: aut.width
            height: aut.height

            model: roottgcontoller.listmodel
            delegate: Rectangle{
                width: parent.width
                height: 50
                color: "yellow"
                Text{
                    text: model.text
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        roottgcontoller.installMusicSignal(index);
                        playMusic.source = "file://" + model.music;
                        console.log(model.music);
                        console.log(playMusic.source);
                        playMusic.play();
                    }

                }

            }
        }
    }

    MediaPlayer {
        id: playMusic
        source: "music.wav"
        audioOutput: AudioOutput { volume: 1.0 }
    }
}

