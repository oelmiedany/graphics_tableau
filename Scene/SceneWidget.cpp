#include <GL/glu.h>
#include <QGLWidget>
#include <cmath>
#include "SceneWidget.h"
#include "SOIL.h"
#include <fstream>

GLubyte *texture[3]; // The texture image
GLuint textureID[3];

typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} materialStruct;


static materialStruct shinyBlack = {
                       {0.1f,0.1f,0.1f,1.0f },
                       {0.1f,0.1f,0.1f,1.0f },
                       {0.50f,0.50f,0.50f,0.3f },
                       50.0f};

static materialStruct shinyPurple = {
                       {0.7f,0.0f,0.7f,1.0f },
                       {0.7f,0.0f,0.7f,1.0f },
                       {0.90f,0.90f,0.90f,0.9f },
                       90.0f};

static materialStruct shinyRed = {
                  { 1.0f, 0.0f, 0.0f, 1.0f },
                  {1.0f, 0.0f, 0.0f, 1.0f },
                  {0.5f, 0.5f, 0.5f, 0.5f },
                  90.0f};

static materialStruct matteLightBlue = {
                  { 0.47f, 0.65f, 1.0f, 0.5f },
                  {0.47f, 0.65f, 1.0f, 0.5f },
                  {0.5f, 0.5f, 0.5f, 0.0f },
                  00.0f};

static materialStruct matteBlue = {
                  { 0.f, 0.f, 1.0f, 0.5f },
                  {0.f, 0.f, 1.0f, 0.5f },
                  {0.5f, 0.5f, 0.5f, 0.0f },
                  00.0f};

static materialStruct matteOffWhite = {
                  { 1.0f, 1.0f, 0.9f, 1.0f },
                  {1.0f, 1.0f, 0.9f, 1.0f },
                  {0.5f, 0.5f, 0.5f, 0.0f },
                 00.0f};

static materialStruct matteYellow = {
                       {0.5f,0.5f,0.0f,1.0f },
                       {0.5f,0.5f,0.0f,1.0f },
                       {0.3f,0.3f,0.3f,0.3f },
                       00.0f};

static materialStruct shinyOrange = {
                       {1.0f,0.67f,0.0f,1.0f },
                       {1.0f,0.67f,0.0f,1.0f },
                       {0.1f,0.1f,0.1f,0.1f },
                       70.0f};


static materialStruct shinyGreen = {
                  { 0.0f, 0.5f, 0.0f, 0.5f },
                  {0.0f, 0.5f, 0.1f, 1.0f },
                  {0.5f, 0.5f, 0.5f, 0.5f },
                  70.0f};

static materialStruct shinyBlue = {
                  { 0.2f, 0.3f, 1.0f, 0.5f },
                  {0.2f, 0.3f, 1.0f, 1.0f },
                  {0.7f, 0.6f, 0.6f, 0.8f },
                  70.0f};

static materialStruct shinyPink = {
                  { 0.8f, 0.45f, 0.45f, 1.0f },
                  {0.8f, 0.45f, 0.45f, 1.0f },
                  {0.2f, 0.2f, 0.2f, 0.0f },
                  70.0f};

static materialStruct matteWhite = {
                  { 0.1f, 1.0f, 1.0f, 1.0f },
                  {1.0f, 1.0f, 1.0f, 1.0f },
                  {0.0f, 0.7f, 0.7f, 0.8f },
                  100.0f};

static materialStruct mattePink = {
                  { 1.0f, 0.45f, 0.45f, 1.0f },
                  {1.0f, 0.45f, 0.45f, 1.0f},
                  {0.1f, 0.1f, 0.1f, 0.1f },
                  10.0f};

void SceneWidget::loadImages(std::string file,int width, int height, int index)
{
    // load texture
    std::ifstream  textureFile (file, std::ifstream::in);//open files
    std::string temp;
    std::getline(textureFile, temp);//reads over unnecessary comments
    std::getline(textureFile, temp);

    //textures are stored in a Glubyte array
    texture[index] = new GLubyte[width*height*4];//creates an array equivalent to the size of the image

    int i,j,colour;
    for(i=height; i>=0; i--)//loops through the height of the image
    {
        for(j=0;j<width;j++)//loops through the width of the image
        {
            //each colour ratio consits of three numbers, rgb, as such for every pixel three numbers are read
            textureFile >> colour;
            texture[index][(i*width+j)*4]=(GLubyte) colour;
            textureFile >> colour;
            texture[index][(i*width+j)*4+1]=(GLubyte) colour;
            textureFile >> colour;
            texture[index][(i*width+j)*4+2]=(GLubyte) colour;
            texture[index][(i*width+j)*4+3]=(GLubyte) 255;//manually includes an alpha chanel missing in ppm for the colours to appear correct
        }
    }
    textureFile.close();

}

void SceneWidget::loadTextures(int index, int width,int height)
{
    glGenTextures(1, &textureID[index]);//generates a new texture ID to identify the texture
    glBindTexture(GL_TEXTURE_2D, textureID[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );//if texture is too small it will repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);//necessary for the image to be mapped correctly
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,0, GL_RGBA, GL_UNSIGNED_BYTE, texture[index]);//creates a two dimensional texture
}

SceneWidget::SceneWidget(QWidget *parent):QGLWidget(parent),angleClockwise(0.0),angleAntiClockwise(0.0)
{
    viewingAngle = 0;
    rotationAngle = -22;
    rotationSpeed = 3;

    loadImages("Marc_Dekamps.ppm",400,400,0);
    loadImages("markus.ppm",250,302,1);
    loadImages("Mercator-projection.ppm",700,543,2);
}

void SceneWidget::initializeGL()
{
    loadTextures(0,400,400);
    loadTextures(1,250,302);
    loadTextures(2,700,543);
    //background colour
    glClearColor(0.9, 0.85, 0.8, 1.0);
}

void SceneWidget::resizeGL(int width, int height)
    { // resizeGL()
    // set the viewport to the entire widget
    glViewport(0, 0, width, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat light_pos[] = {0., 0.,15., 1.0};//lighting position
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);



    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-14.0, 14.0, -14.0, 14.0, -5.0, 30.0);

    }

void SceneWidget::circle()
{
    materialStruct* material = &mattePink;

    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);

    //utilises glutobject disk
    GLUquadric *quad;
    quad = gluNewQuadric();
    gluDisk(quad,0,5,25,25);
}

void SceneWidget::octagon(const materialStruct *material)
{

    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);

    glTranslatef(0,-3,0);

    //front
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-1.0,0.0,1.0);
    glVertex3f(1.0,0.0,1.0);
    glVertex3f(3.0,2.0,1.0);
    glVertex3f(3.0,4.0,1.0);
    glVertex3f(1.0,6.0,1.0);
    glVertex3f(-1.0,6.0,1.0);
    glVertex3f(-1.0,6.0,1.0);
    glVertex3f(-3.0,4.0,1.0);
    glVertex3f(-3.0,2.0,1.0);
    glEnd();

    //back
    glNormal3f(0.0,0.0,-1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-1.0,0.0,0.0);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(3.0,2.0,0.0);
    glVertex3f(3.0,4.0,0.0);
    glVertex3f(1.0,6.0,0.0);
    glVertex3f(-1.0,6.0,0.0);
    glVertex3f(-1.0,6.0,0.0);
    glVertex3f(-3.0,4.0,0.0);
    glVertex3f(-3.0,2.0,0.0);
    glVertex3f(-1.0,0.0,0.0);
    glEnd();

    //sides
    //bottom left
    glNormal3f(-0.5547,-0.5547,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-3.0,2.0,1.0);
    glVertex3f(-3.0,2.0,0.0);
    glVertex3f(-1.0,0.0,0.0);
    glVertex3f(-1.0,0.0,1.0);
    glEnd();

    //bottom
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-1.0,0.0,1.0);
    glVertex3f(-1.0,0.0,0.0);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(1.0,0.0,1.0);
    glEnd();

    //bottom right
    glNormal3f(0.5547,-0.5547,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(1.0,0.0,1.0);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(3.0,2.0,0.0);
    glVertex3f(3.0,2.0,1.0);
    glEnd();

    //right
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(3.0,2.0,1.0);
    glVertex3f(3.0,2.0,0.0);
    glVertex3f(3.0,4.0,0.0);
    glVertex3f(3.0,4.0,1.0);
    glEnd();

    //upper right
    glNormal3f(0.5547,0.5547,0);
    glBegin(GL_POLYGON);
    glVertex3f(3.0,4.0,1.0);
    glVertex3f(3.0,4.0,0.0);
    glVertex3f(1.0,6.0,0.0);
    glVertex3f(1.0,6.0,1.0);
    glEnd();

    //top
    glNormal3f(0.0,1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(1.0,6.0,1.0);
    glVertex3f(1.0,6.0,0.0);
    glVertex3f(-1.0,6.0,0.0);
    glVertex3f(-1.0,6.0,1.0);
    glEnd();

    //upper left
    glNormal3f(-0.5547,0.5547,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-1.0,6.0,1.0);
    glVertex3f(-1.0,6.0,0.0);
    glVertex3f(-3.0,4.0,0.0);
    glVertex3f(-3.0,4.0,1.0);
    glEnd();

    //left
    glNormal3f(-1.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-3.0,4.0,1.0);
    glVertex3f(-3.0,4.0,0.0);
    glVertex3f(-3.0,2.0,0.0);
    glVertex3f(-3.0,2.0,1.0);
    glEnd();

}

void SceneWidget::lines(const materialStruct *material)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);

    glNormal3f(0,0,1);
    glBegin(GL_POLYGON);
    glVertex3f(0.,0.5,15.);
    glVertex3f(0.,0.0,15.);
    glVertex3f(20,0.0,15);
    glVertex3f(20,0.5,15);
    glEnd();
}

void SceneWidget::wave()
{
    const materialStruct *material = &shinyBlack;

    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);

    //simulates the scribble in Kandinsky's composition using a geometric approach
    for (double x=0; x<20; x=x+0.25){
        glNormal3f(0,0,1);
        glBegin(GL_POLYGON);
        double y1=sin(x)*2.5;
        double y2=sin(x+0.25)*2.5;
        glVertex3f(x,y1+0.5,16.);
        glVertex3f(x,y1,16.);
        glVertex3f(x+1,y2,16.);
        glVertex3f(x+1,y2+0.5,16.);
        glEnd();
    }


}

void SceneWidget::curve()
{
    const materialStruct *material = &shinyBlack;

    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);

    //creates a curve that gradualls slims down
    for (double x=0; x<25; x=x+0.25){
        glNormal3f(0,0,1);
        glBegin(GL_POLYGON);
        double y1=log(x);
        double y2=log(x+0.25);
        glVertex3f(x,y1+0.5,16.);
        glVertex3f(x,y1,16.);
        glVertex3f(x+1,y2,16.);
        glVertex3f(x+1,y2+0.5,16.);
        glEnd();
    }

}

void SceneWidget::backgroundTriangles(const materialStruct *material)
{

    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);

    //the trianges are triangular pyramids with a flat surface that faces away form the user
    //angled base
    glNormal3f(0,0.7,0.7);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,2.0,2.0);
    glVertex3f(-2.0,0.0,0.0);
    glVertex3f(2.0,0.0,0.0);
    glEnd();

    //right
    glNormal3f(0.7,0,0.7);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,8.0,0.0);
    glVertex3f(0.0,2.0,2.0);
    glVertex3f(2.0,0.0,0.0);
    glEnd();

    //left
    glNormal3f(-0.7,0,0.7);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,8.0,0.0);
    glVertex3f(-2.0,0.0,0.0);
    glVertex3f(0.0,2.0,2.0);
    glEnd();

    //back
    glNormal3f(0,0,-1);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,8.0,0.0);
    glVertex3f(-2.0,0.0,0.0);
    glVertex3f(2.0,0.0,0.0);
    glEnd();
}

void SceneWidget::texturedCuboids()
{
    //all untextutured sides are green
    materialStruct* green = &shinyGreen;

    glMaterialfv(GL_FRONT, GL_AMBIENT,    green->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    green->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   green->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   green->shininess);

    glRotatef(angleAntiClockwise, 0.0, 1.0, 0.0);//incorporates the animation
    glTranslatef(-1.,0.,0.);

    //front   
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//informs openGL to replace surface with the texture
    glEnable(GL_TEXTURE_2D);
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);

    glTexCoord3f(0.0,2.0,1.0);
    glVertex3f(0.0,3.0,1.0);

    glTexCoord3f(0.0,1.0,1.0);
    glVertex3f(0.0,0.0,1.0);

    glTexCoord3f(1.0,1.0,1.0);
    glVertex3f(2.0,0.0,1.0);

    glTexCoord3f(1.0,2.0,1.0);
    glVertex3f(2.0,3.0,1.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //right-side
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);

    glTexCoord3f(0.0,2.0,1.0);
    glVertex3f(2.0,3.0,1.0);

    glTexCoord3f(0.0,1.0,1.0);
    glVertex3f(2.0,0.0,1.0);

    glTexCoord3f(1.0,1.0,1.0);
    glVertex3f(2.0,0.0,-1.0);

    glTexCoord3f(1.0,2.0,1.0);
    glVertex3f(2.0,3.0,-1.0);

    glEnd();
    glDisable(GL_TEXTURE_2D);


    //back
    glBindTexture(GL_TEXTURE_2D, textureID[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glNormal3f(0.0,0.0,-1.0);
    glBegin(GL_POLYGON);

    glTexCoord3f(0.0,2.0,1.0);
    glVertex3f(0.0,3.0,-1.0);

    glTexCoord3f(0.0,1.0,1.0);
    glVertex3f(0.0,0.0,-1.0);

    glTexCoord3f(1.0,1.0,1.0);
    glVertex3f(2.0,0.0,-1.0);

    glTexCoord3f(1.0,2.0,1.0);
    glVertex3f(2.0,3.0,-1.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //left-side
    glNormal3f(-1.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,3.0,1.0);
    glVertex3f(0.0,3.0,-1.0);
    glVertex3f(0.0,0.0,-1.0);
    glVertex3f(0.0,0.0,1.0);
    glEnd();

    //top
    glNormal3f(0.0,1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,3.0,-1.0);
    glVertex3f(0.0,3.0,1.0);
    glVertex3f(2.0,3.0,1.0);
    glVertex3f(2.0,3.0,-1.0);
    glEnd();

    //bottom
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,0.0,-1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(2.0,0.0,1.0);
    glVertex3f(2.0,0.0,-1.0);
    glEnd();
}

void SceneWidget::triangularPrism()
{
    materialStruct* pink=&shinyPink;
    materialStruct* orange = &shinyOrange;
    materialStruct* blue=&shinyBlue;

    glRotatef(angleClockwise, 0.0, 1.0, 0.0);//incorporates the animation
    glTranslatef(0.,0.,-0.6);

    //front
    glMaterialfv(GL_FRONT, GL_AMBIENT,    pink->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    pink->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   pink->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   pink->shininess);

    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5,4.0,1.0);
    glVertex3f(-0.5,0.0,1.0);
    glVertex3f(0.5,0.0,1.0);
    glVertex3f(0.5,4.0,1.0);
    glEnd();

    //right-side
    glMaterialfv(GL_FRONT, GL_AMBIENT,    orange->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    orange->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   orange->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   orange->shininess);

    glNormal3f(0.447,0.0,-0.89442);
    glBegin(GL_POLYGON);
    glVertex3f(0.5,4.0,1.0);
    glVertex3f(0.5,0.0,1.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,4.0,0.0);
    glEnd();

    //left-side
    glMaterialfv(GL_FRONT, GL_AMBIENT,    blue->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    blue->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   blue->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   blue->shininess);

    glNormal3f(-0.447,0.0,-0.89442);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,4.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(-0.5,0.0,1.0);
    glVertex3f(-0.5,4.0,1.0);
    glEnd();

    //top
    glNormal3f(0.0,1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,4.0,0.0);
    glVertex3f(-0.5,4.0,1.0);
    glVertex3f(0.5,4.0,1.0);
    glEnd();

    //bottom
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(-0.5,0.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glEnd();

}

void SceneWidget::fillerShapes(const materialStruct *material)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);

    //front
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(2.0,0.0,1.0);
    glVertex3f(2.0,1.0,1.0);
    glEnd();

    //right-side
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(2.0,1.0,1.0);
    glVertex3f(2.0,0.0,1.0);
    glVertex3f(2.0,0.0,-1.0);
    glVertex3f(2.0,1.0,-1.0);
    glEnd();

    //back
    glNormal3f(0.0,0.0,-1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,1.0,-1.0);
    glVertex3f(0.0,0.0,-1.0);
    glVertex3f(2.0,0.0,-1.0);
    glVertex3f(2.0,1.0,-1.0);
    glEnd();

    //left-side
    glNormal3f(-1.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,-1.0);
    glVertex3f(0.0,1.0,-1.0);
    glEnd();

    //bottom
    glNormal3f(0.0,-1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,0.0,-1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(2.0,0.0,1.0);
    glVertex3f(2.0,0.0,-1.0);
    glEnd();

    //top
    glNormal3f(0.0,1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0,1.0,-1.0);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(2.0,1.0,1.0);
    glVertex3f(2.0,1.0,-1.0);
    glEnd();


}

void SceneWidget::shapeBase()
{
    //The shape itself is a trapezoidal prism
    materialStruct* material = &shinyBlack;

    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);

    //front
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-9.0,0.0,2.0);
    glVertex3f(-9.0,-0.5,2.0);
    glVertex3f(5.0,-1.0,2.0);
    glVertex3f(5.0,0.0,2.0);
    glEnd();

    //right-side
    glNormal3f(0.0,1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(5.0,0.0,2.0);
    glVertex3f(5.0,-1.0,2.0);
    glVertex3f(5.0,-1.0,-2.0);
    glVertex3f(5.0,0.0,-2.0);
    glEnd();

    //back
    glNormal3f(0.0,0.0,-1.0);
    glBegin(GL_POLYGON);
    glVertex3f(5.0,0.0,-2.0);
    glVertex3f(5.0,-1.0,-2.0);
    glVertex3f(-9.0,-0.5,-2.0);
    glVertex3f(-9.0,0.0,-2.0);
    glEnd();

    //left-side
    glNormal3f(-1.0,0.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-9.0,0.0,-2.0);
    glVertex3f(-9.0,-0.5,-2.0);
    glVertex3f(-9.0,-0.5,2.0);
    glVertex3f(-9.0,0.0,2.0);
    glEnd();

    //top
    glNormal3f(0.0,1.0,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-9.0,0.0,-2.0);
    glVertex3f(-9.0,0.0,2.0);
    glVertex3f(5.0,0.0,2.0);
    glVertex3f(5.0,0.0,-2.0);
    glEnd();

    //bottom
    glNormal3f(-0.04993,-0.99995,0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-9.0,-0.5,-2.0);
    glVertex3f(-9.0,-0.5,2.0);
    glVertex3f(5.0,-1.0,2.0);
    glVertex3f(5.0,-1.0,-2.0);
    glEnd();

    //

}

void SceneWidget::centrePiece()
{
    //This object is constructed from the aforementioned objects
    //left of middle
    glPushMatrix();
    glTranslatef(-2,0,0);
    glScalef(1,2,1);
    fillerShapes(&shinyOrange);
    glTranslatef(0,1,0);
    glScalef(1,0.5,1);
    fillerShapes(&matteWhite);
    glTranslatef(1,1,-2);
    texturedCuboids();
    glPopMatrix();

    //long-vertical filler shapes
    glPushMatrix();
    glScalef(0.5,4,1);
    fillerShapes(&shinyBlack);
    glPopMatrix();

    //chequred filler shapes
    glPushMatrix();
    glTranslatef(1,0,0);
    glScalef(1,1.5,1);
    fillerShapes(&matteWhite);
    glTranslatef(2,1,0);
    fillerShapes(&matteWhite);
    glTranslatef(-2,0,-1);
    fillerShapes(&shinyBlack);
    glPopMatrix();

    //small horizontal textured cuboid
    glPushMatrix();
    glTranslatef(3,0.75,-2);
    glScalef(0.67,0.75,1);
    glRotatef(180,1,1,0);
    texturedCuboids();
    glPopMatrix();

    //horizontal rotating triangular prism
    glPushMatrix();
    glTranslatef(1,3.5,-1);
    glRotatef(180,1,1,0);
    glRotatef(60,0,1,0);
    triangularPrism();
    glPopMatrix();

    //upper filler shapes
    glPushMatrix();
    glTranslatef(0,4,0);
    glScalef(1,2,1);
    fillerShapes(&matteWhite);
    glTranslatef(2,0,0);
    glScalef(1.5,0.5,1);
    fillerShapes(&shinyBlack);
    glTranslatef(0,1,0);
    fillerShapes(&shinyBlue);
    glPopMatrix();

    //large horizontal textured cube
    glPushMatrix();
    glTranslatef(0,7,-2);
    glRotatef(180,1,1,0);
    texturedCuboids();
    glPopMatrix();

    //top
    glPushMatrix();
    glTranslatef(4,6,0);
    glRotatef(180,1,1,0);
    fillerShapes(&shinyBlack);
    glTranslatef(0,-1,0);
    glScalef(1.5,1,1);
    fillerShapes(&matteWhite);
    glPopMatrix();

    //vertical rotating Prism
    glPushMatrix();
    glTranslatef(4.5,8,-1);
    glScalef(1,0.75,1);
    triangularPrism();
    glPopMatrix();

    shapeBase();

}

void SceneWidget::rotatingSpheres(const materialStruct *material, double radius, double xposition, double yposition, double zposition)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT,    material->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    material->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   material->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   material->shininess);

    //glut object
    GLUquadric *quad;
    quad = gluNewQuadric();
    glTranslatef(xposition,yposition,zposition);
    gluSphere(quad,radius,25,25);
}

void SceneWidget::rotatingSystem()
{

    rotatingSpheres(&shinyBlack,3.5,0,0,0);

    glPushMatrix();
    glRotatef(constantRotationAntiClockwise,0,1,0);
    rotatingSpheres(&shinyPurple,2,0,0,6);
    glPopMatrix();

    glPushMatrix();
    glRotatef(constantRotationClockwise,1,1,0);
    rotatingSpheres(&shinyRed,2,3,-3,-3.75);
    glPopMatrix();

}

void SceneWidget::objectRotation(){

    counter++;

    //ensures the rotations occur continuously
    if(counter>360)
        counter=0;

    //rotation for the triangular prism
    if(counter>=121 && counter<=240)
        angleClockwise -= 1.;

    //rotation for the cuboids
    if(counter<=89)
        angleAntiClockwise += 1.;

    if(counter>=181 && counter<=270)
        angleAntiClockwise += 1.;

    //rotation for the orbiting spheres
    constantRotationAntiClockwise += rotationSpeed;
    constantRotationClockwise -= rotationSpeed;

    this->repaint();
}

//SLOTS
void SceneWidget::updateAngle(int input)
{
    viewingAngle = input;
    this->repaint();
}

void SceneWidget::updateRotation(int input)
{
    rotationAngle=input;
    this->repaint();
}

void SceneWidget::updateSpeed(int input)
{
    rotationSpeed=input;
    this->repaint();
}

//method that calls all the different scene components
void SceneWidget::sceneConstructor()
{
    //centre piece
    glPushMatrix();
    glTranslatef(1,-7,6);
    glRotatef(rotationAngle,0,0,1);
    centrePiece();
    glPopMatrix();

    //rotating system
    glPushMatrix();
    glTranslatef(-9,9,0);
    circle();
    glTranslatef(0,0,8);
    rotatingSystem();
    glPopMatrix();

    //octagons
    glPushMatrix();
    glTranslatef(-9,-8,8);
    glScalef(0.9,0.9,0.9);
    octagon(&matteBlue);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-9,-8,9);
    glScalef(0.7,0.7,0.7);
    octagon(&matteYellow);
    glPopMatrix();


    //background triangles
    glPushMatrix();
    glTranslatef(14,-14,6);
    glScalef(3,3.5,3);
    backgroundTriangles(&matteOffWhite);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-4,5,-1);
    glRotatef(-120,0,0,1);
    glScalef(3,3,2);
    backgroundTriangles(&matteLightBlue);
    glPopMatrix();

    //lines
    //black and red
    glPushMatrix();
    glTranslatef(5,-5,0);
    glRotatef(7,0,0,1);
    glScalef(0.5,0.75,1);
    lines(&shinyBlack);
    glScalef(2,1.25,1);
    glTranslatef(-0.75,-3,0);
    glRotatef(10,0,0,1);
    glScalef(0.5,0.75,1);
    lines(&shinyRed);
    glPopMatrix();

    //slim black line
    glPushMatrix();
    glScalef(1.5,0.25,1);
    glTranslatef(-6,4,0);
    glRotatef(45,0,0,1);
    lines(&shinyBlack);
    glPopMatrix();

    //scrible
    glPushMatrix();
    glTranslatef(-8,1,0);
    glRotatef(10,0,0,1);
    wave();
    glPopMatrix();


    //slim perpinduclar
    glPushMatrix();
    glScalef(0.25,0.5,1);
    glTranslatef(-4,13,0);
    glRotatef(90,0,0,1);
    lines(&shinyBlack);
    glPopMatrix();

    //flick
    glPushMatrix();
    glScalef(0.2,0.8,1);
    glRotatef(180,0,0,1);
    glTranslatef(-11,-13,0);
    curve();
    glPopMatrix();

    //slim, small, lines at the top
    glPushMatrix();
    glTranslatef(-3.5,12.5,1);
    glScalef(0.25,0.15,1);
    lines(&shinyBlack);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.25,11.5,1);
    glScalef(0.25,0.2,1);
    lines(&shinyBlack);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.25,14,1);
    glRotatef(-10,0,0,1);
    glScalef(0.4,0.2,1);
    lines(&shinyBlack);
    glPopMatrix();

}

void SceneWidget::paintGL()
    { // paintGL()
    // clear the widget
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // You must set the matrix mode to model view directly before enabling the depth test
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    gluLookAt(viewingAngle,0.0,17.0, 0.0,0.0,0.0, 0.0,1.0,0.0);

    sceneConstructor();

    // flush to screen
    glFlush();

    }
