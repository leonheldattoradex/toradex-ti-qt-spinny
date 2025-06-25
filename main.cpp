#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QSurfaceFormat>
#include "cube_widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);
    
    QMainWindow window;
    window.resize(800, 600);
    
    QWidget* centralWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    
    CubeWidget* cubeWidget = new CubeWidget();
    
    layout->addWidget(cubeWidget, 1);
    
    window.setCentralWidget(centralWidget);
    
    window.setStyleSheet(R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #2c3e50, stop:1 #34495e);
        }
        QWidget {
            background: transparent;
        }
    )");
    
    window.show();
    
    return app.exec();
}