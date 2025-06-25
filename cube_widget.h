#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <QImage>

class CubeWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CubeWidget(QWidget *parent = nullptr);
    ~CubeWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private slots:
    void animate();

private:
    void loadImages();
    void setupVertexData();
    void createImageTextures();
    QOpenGLTexture* createTextureFromImage(const QImage& image);

    QTimer *m_timer;
    QImage m_tiImage;
    QImage m_toradexImage;
    int m_currentImage;
    
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program;
    QOpenGLTexture *m_textTextures[6];
    
    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;
    QMatrix4x4 m_model;
    
    float m_rotationX;
    float m_rotationY;
    float m_rotationZ;
};