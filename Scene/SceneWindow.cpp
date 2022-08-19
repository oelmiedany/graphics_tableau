#include "SceneWindow.h"

SceneWindow::SceneWindow(QWidget *parent): QWidget(parent)
{
    windowLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    scene = new SceneWidget(this);
    windowLayout->addWidget(scene);

    timer = new QTimer(this);
    timer->start(20);
    connect(timer, SIGNAL(timeout()),  scene, SLOT(objectRotation()));

    text[0] = new QLabel();
    text[0]->setStyleSheet("QLabel {color : white; }");
    text[0]->setText("Interactive Rendition of Kandinsky Composition 8");
    text[0]->setMaximumHeight(20);
    windowLayout->addWidget(text[0]);

    text[1] = new QLabel();
    text[1]->setStyleSheet("QLabel {color : white; }");
    text[1]->setText("Set Viewing Angle");
    text[1]->setMaximumHeight(20);
    windowLayout->addWidget(text[1]);

    angle = new QSlider(Qt::Horizontal);
    angle ->setRange(-14,14);
    angle -> setValue(0);
    connect(angle, SIGNAL(valueChanged(int)), scene, SLOT(updateAngle(int)));
    windowLayout->addWidget(angle);

    text[2] = new QLabel();
    text[2]->setStyleSheet("QLabel {color : white; }");
    text[2]->setText("Set Angle of the Centre Piece");
    text[2]->setMaximumHeight(20);
    windowLayout->addWidget(text[2]);

    rotation = new QSlider(Qt::Horizontal);
    rotation -> setRange(-22,23);
    rotation -> setValue(-22);
    connect(rotation, SIGNAL(valueChanged(int)), scene, SLOT(updateRotation(int)));
    windowLayout->addWidget(rotation);

    text[3] = new QLabel();
    text[3]->setStyleSheet("QLabel {color : white; }");
    text[3]->setText("Set speed of movement");
    text[3]->setMaximumHeight(20);
    windowLayout->addWidget(text[3]);

    speed = new QSlider(Qt::Horizontal);
    speed ->setRange(0,6);
    speed -> setValue(3);
    connect(speed, SIGNAL(valueChanged(int)), scene, SLOT(updateSpeed(int)));
    windowLayout->addWidget(speed);

}

SceneWindow::~SceneWindow()
{
    delete scene;
    delete windowLayout;
}

void SceneWindow::ResetInterface()
{
    scene->update();
    update();
}
