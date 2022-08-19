#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QGLWidget>
#include <string>

struct materialStruct;

class SceneWidget: public QGLWidget
    {
    Q_OBJECT

    public:
    SceneWidget(QWidget *parent);

    public slots:
    void objectRotation();
    void updateAngle(int input);
    void updateSpeed(int input);
    void updateRotation(int input);

    protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    private:
    void loadImages(std::string file,int width, int height, int index);
    void loadTextures(int index, int width, int height);
    //landscaping
    void backgroundTriangles(const materialStruct*);
    void octagon(const materialStruct*);
    void circle();
    void lines(const materialStruct*);
    void wave();
    void curve();
    //centre piece
    void texturedCuboids();
    void triangularPrism();
    void fillerShapes(const materialStruct*);
    void shapeBase();
    void centrePiece();
    //rotating system
    void rotatingSpheres(const materialStruct*, double radius, double xposition, double yposition, double zposition);
    void rotatingSystem();

    void sceneConstructor();
    double angleClockwise;
    double angleAntiClockwise;
    double constantRotationClockwise;
    double constantRotationAntiClockwise;
    int counter;
    int viewingAngle;
    int rotationSpeed;
    int rotationAngle;

    };

#endif // SCENEWIDGET_H
