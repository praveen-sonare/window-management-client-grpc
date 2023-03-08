import QtQuick 2.13
import QtQuick.Window 2.13

Window {
    id: background
    width: 800
    height: 600
    flags: Qt.FramelessWindowHint
    visible: true

    Image {
        anchors.fill: parent
        source: './images/AGL_HMI_Blue_Background_NoCar-01.png'
    }
}
