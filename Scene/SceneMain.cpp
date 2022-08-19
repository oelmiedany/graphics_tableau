#include <QApplication>
#include <QVBoxLayout>
#include "SceneWindow.h"

int main(int argc, char *argv[])
    {
        QApplication app(argc, argv);

        SceneWindow *window = new SceneWindow(NULL);

        window -> resize(750, 850);
        window -> show();
        QPalette colour = QPalette();
        colour.setColor(QPalette::Window, Qt::black);
        window->setAutoFillBackground(true);
        window->setPalette(colour);

        app.exec();

        delete window;

        return 0;
    }
