import QtQuick 2.12
import QtQuick.Window 2.12
import "qrc:/CDs.js" as CDs
import "qrc:/qCDs.qml" as qCDs

Window {
    visible: true
    width: 450     //200 , 380, 450
    height: 170    //360, 420, 170

    //ListView
    /*
    Rectangle {
        id: mainrect
        color: "gray"
        width: 200
        height: 360

        Component {
            id: delegate
            Item {
                width: mainrect.width
                height: 70
                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        width: 64
                        height: 64
                        source: modelData.cover
                        smooth: true
                    }
                    Column {
                        Text {
                            color: "white"
                            text: modelData.artist
                            font.pointSize: 12
                        }
                        Text {
                            color: "lightblue"
                            text: modelData.album
                            font.pointSize: 10
                        }
                        Text {
                            color: "yellow"
                            text: modelData.year
                            font.pointSize: 8
                        }
                    }
                }
            }
        }

        ListView {
            focus: true
            header: Rectangle {
                width: parent.width
                height: 30
                gradient: Gradient {
                    GradientStop {position: 0; color: "gray";}
                    GradientStop {position: 0.7; color: "black";}
                }
                Text {
                    anchors.centerIn: parent;
                    color: "gray"
                    text: "CDs"
                    font.bold: true
                    font.pointSize: 20
                }
            }
            footer: Rectangle {
                width: parent.width
                height: 30
                gradient: Gradient {
                    GradientStop {position: 0; color: "gray";}
                    GradientStop {position: 0.7; color: "black";}
                }
            }
            highlight: Rectangle {
                width: parent.width
                color: "darkblue"
            }

            anchors.fill: parent
            model: CDs.jsonModel
            delegate: delegate
        }
    }
    */

    //Grid View
    /*
    Rectangle {
        id: mainrect
        color: "gray"
        width: 380
        height: 420
       Component {
           id:delegate
           Item {
               width: 120
               height: 120
               Column {
                   anchors.centerIn: parent
                   Image {
                       anchors.horizontalCenter: parent.horizontalCenter
                       width: 64
                       height: 64
                       source: modelData.cover
                       smooth: true
                   }
                   Text {color:"white";text: modelData.artist;font.pointSize: 12;}
                   Text {color:"lightBlue";text: modelData.album;font.pointSize: 10;}
                   Text {color:"yellow";text: modelData.year;font.pointSize: 8;}
               }
           }
       }

       GridView {
           cellHeight: 120
           cellWidth: 120
           focus: true
           header: Rectangle {
               width: parent.width
               height: 30
               gradient: Gradient {
                   GradientStop {position: 0; color: "gray";}
                   GradientStop {position: 0.7; color: "black";}
               }
               Text {
                   anchors.centerIn: parent;
                   color: "gray"
                   text: "CDs"
                   font.bold: true
                   font.pointSize: 20
               }
           }
           footer: Rectangle {
               width: parent.width
               height: 30
               gradient: Gradient {
                   GradientStop {position: 0; color: "gray";}
                   GradientStop {position: 0.7; color: "black";}
               }
            }
           highlight: Rectangle {
               width: parent.width
               color: "darkblue"
           }

           anchors.fill: parent
           model: CDs.jsonModel
           delegate: delegate
        }
    }
    */

    //PathView
    /*
    Rectangle {
        color: "gray"
        width: 450
        height: 270 //170
        Component {
            id:delegate
            Item {
                width: item.width
                height: item.height
                Column {
                    id: item
                    Image {
                        width: 90
                        height: 90
                        source: modelData.cover
                        smooth: true
                    }
                    Text {color:"white";text: modelData.artist;font.pointSize: 12;}
                    Text {color:"lightBlue";text: modelData.album;font.pointSize: 10;}
                    Text {color:"yellow";text: modelData.year;font.pointSize: 8;}
                }
            }
        }

        Path {
            id: itemsPath
        //In Line
            //startX: 0
            //startY: 80
            //PathLine {x: 500; y:80;}
        //3d Carusel
            startX: 150
            startY: 150
            PathAttribute {name: "iconScale"; value: 1.0}
            PathAttribute {name: "iconOpacity"; value: 1.0}
            PathQuad {x: 150; y:25; controlX: 460; controlY: 75;}
            PathAttribute {name: "iconScale"; value: 0.3;}
            PathAttribute {name: "iconOpacity"; value: 0.3;}
            PathQuad {x: 150; y: 150; controlX: -80; controlY: 75;}
        }
        PathView {
            id: itemsView
            anchors.fill: parent
            model: CDs.jsonModel
            delegate: delegate
            path: itemsPath
            pathItemCount: 5    //4
        }
    }
    */

    //Visualizing without delegate ??
    Rectangle {
        width: 250
        height: 250
        color: "darkSlateGray"

        Flickable {
            id:view
            width: 250
            height: 500
            contentWidth: 250
            contentHeight: column.height
            anchors.fill: parent

            Column {
                id: column
                anchors.fill: view
                spacing: 5
                Repeater {
                    model: qCDs{}
                }
            }
        }
    }
}
