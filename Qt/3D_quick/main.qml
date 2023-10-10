import QtQuick 2.12
import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.12
import QtQuick.Scene3D 2.13

Rectangle {
    color: "black"
    Scene3D {
        anchors.fill: parent
        focus: true
        aspects: ["input", "logic"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio
        Entity {
            Camera {    //Камера
                id: camera
                nearPlane: 0.1
                farPlane: 1000.0
                position: Qt.vector3d(0.0,0.0,50.0)
            }
            FirstPersonCameraController {   //Управление камерой
                camera: camera
                linearSpeed: 1000.0
                acceleration: 0.1
                deceleration: 1.0
            }
            components: [
                RenderSettings {
                    activeFrameGraph:
                        ForwardRenderer {
                            camera: camera
                            clearColor: "transparent"
                    }
                },
                InputSettings {}
            ]
            DirectionalLight {  //Свет
                color: "#afafff"
            }

            //Ex 1 Простой шарик
            /*
            Entity {        //Шар
                PhongMaterial {
                    id: phongMaterial
                    ambient: Qt.rgba(0.3,0.3,0.3,1.0)
                    diffuse: Qt.rgba(1,1,1,1)
                }
                SphereMesh {
                    id: sphereMesh
                    radius: 6
                }
                components: [sphereMesh,phongMaterial]
            }
            */
            //

            //Test (snowman lul)
            /*
            Entity {        //Шар 2
                PhongMaterial {
                    id: phongMaterial2
                    ambient: Qt.rgba(0.3,0.3,0.3,1.0)
                    diffuse: Qt.rgba(1,1,1,1)
                }
                Transform {
                    id: transform2
                    translation: Qt.vector3d(0,10,0);
                }
                SphereMesh {
                    id: sphereMesh2
                    radius: 4
                }
                components: [sphereMesh2,phongMaterial2,transform2]
            }

            Entity {        //Шар 3
                PhongMaterial {
                    id: phongMaterial3
                    ambient: Qt.rgba(0.3,0.3,0.3,1.0)
                    diffuse: Qt.rgba(1,1,1,1)
                }
                Transform {
                    id: transform3
                    translation: Qt.vector3d(0,17,0);
                }
                SphereMesh {
                    id: sphereMesh3
                    radius: 3
                }
                components: [sphereMesh3,phongMaterial3,transform3]
            }
            */
            //

            //Ex 2 Анимация
            Entity {
                PhongMaterial {
                    id: phongMaterial
                    ambient: Qt.rgba(0.3,0.3,0.3,1.0);
                    diffuse: Qt.rgba(1,1,1,1)
                }
                SphereMesh {
                    id: sphereMesh
                    radius: 6
                }
                Transform {
                    id: sphereTransform
                    property real myParam: 0
                    matrix: {
                        var mat = Qt.matrix4x4();
                        mat.rotate(myParam,Qt.vector3d(0,1,0));
                        mat.translate(Qt.vector3d(24,0,0));
                        return mat;
                    }
                }
                components: [sphereMesh, phongMaterial, sphereTransform]
                NumberAnimation {
                    target: sphereTransform
                    property: "myParam"
                    duration: 10000
                    from: 0; to: 360;
                    loops: Animation.Infinite
                    running: true
                }
            }

            Entity {    //Pyramid
                GoochMaterial {
                    id: goochMaterial
                    diffuse: Qt.rgba(1, 1, 1, 1)
                }
                Mesh {
                    id: pyramidMesh
                    source: "qrc:/pyramid.obj"
                }
                Transform {
                    id: pyramidTransform
                    property real myRotation: 0
                    matrix: {
                        var mat = Qt.matrix4x4();
                        mat.rotate(myRotation, Qt.vector3d(1,1,1))
                        mat.scale(Qt.vector3d(10,10,10));
                        return mat;
                    }
                }
                components: [pyramidMesh,goochMaterial,pyramidTransform]
                NumberAnimation {
                    target: pyramidTransform
                    property: "myRotation"
                    duration: 10000
                    from: 0; to: 360;
                    loops: Animation.Infinite
                    running: true
                }
            }

            //
        }
    }
}
