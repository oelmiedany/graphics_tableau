#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include <QGLWidget>
#include <QSlider>
#include <QTimer>
#include <QLabel>
#include <QBoxLayout>
#include "SceneWidget.h"

class SceneWindow: public QWidget
    {
    //remember to insert comments

    public:
    SceneWindow(QWidget *parent);
    ~SceneWindow();

    QBoxLayout *windowLayout;
    SceneWidget *scene;
    //inserts sliders
    QTimer *timer;
    QSlider *angle;
    QSlider *speed;
    QSlider *rotation;
    QLabel *text[4];

    void ResetInterface();

    };

#endif // SCENEWINDOW_H
