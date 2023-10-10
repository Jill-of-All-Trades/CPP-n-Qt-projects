import QtQuick 2.12

VisualItemModel {
    Row {
        Image {
            width: 64
            height: 64
            source: "qrc:/img_1.jfif"
            smooth: true
        }
        Column {
            Text {color:"white";text: "OtAkU";font.pointSize: 12;}
            Text {color:"lightBlue";text: "NerD";font.pointSize: 10;}
            Text {color:"yellow";text: "2003";font.pointSize: 8;}
        }
    }
    Rectangle {
        width: parent.width
        height: 64
        color: "yellow"
        Text {
            anchors.centerIn: parent
            color: "red"
            text: "Blank!"
        }
    }
    Row {
        Image {
            width: 64
            height: 64
            source: "qrc:/img_2.jfif"
            smooth: true
        }
        Column {
            Text {color:"white";text: "Va-11 Hall-a";font.pointSize: 12;}
            Text {color:"lightBlue";text: "Meet the stuff";font.pointSize: 10;}
            Text {color:"yellow";text: "2016";font.pointSize: 8;}
        }
    }
}
