#include "cube_widget.h"
#include <QApplication>
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

CubeWidget::CubeWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_timer(new QTimer(this))
    , m_currentImage(0)
    , m_program(nullptr)
    , m_rotationX(0.0f)
    , m_rotationY(0.0f)
    , m_rotationZ(0.0f)
{
    for (int i = 0; i < 6; ++i) {
        m_textTextures[i] = nullptr;
    }
    
    loadImages();
    
    connect(m_timer, &QTimer::timeout, this, &CubeWidget::animate);
    m_timer->start(16); // 1s/16m * 1000 is roughly 60FPS
}

CubeWidget::~CubeWidget()
{
    makeCurrent();
    
    for (int i = 0; i < 6; ++i) {
        delete m_textTextures[i];
    }
    
    delete m_program;
    
    doneCurrent();
}

void CubeWidget::loadImages()
{
    m_tiImage.load("ti_logo.jpg");
    if (m_tiImage.isNull()) {
        qDebug() << "Failed to load ti_logo.jpg";
    } else {
        qDebug() << "Successfully loaded ti_logo.jpg";
    }
    
    m_toradexImage.load("toradex_logo.png");
    if (m_toradexImage.isNull()) {
        qDebug() << "Failed to load toradex_logo.png";
    } else {
        qDebug() << "Successfully loaded toradex_logo.png";
    }
}

void CubeWidget::initializeGL()
{
    initializeOpenGLFunctions();
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    m_program = new QOpenGLShaderProgram;
    
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;
        
        out vec3 vertexColor;
        out vec2 texCoord;
        
        uniform mat4 mvp;
        
        void main()
        {
            gl_Position = mvp * vec4(aPos, 1.0);
            vertexColor = aColor;
            texCoord = aTexCoord;
        }
    )";
    
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec3 vertexColor;
        in vec2 texCoord;
        
        uniform sampler2D ourTexture;
        
        void main()
        {
            FragColor = texture(ourTexture, texCoord) * vec4(vertexColor, 1.0);
        }
    )";
    
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    
    setupVertexData();
    createImageTextures();
}

void CubeWidget::setupVertexData()
{
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };
    
    m_vao.create();
    m_vao.bind();
    
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices));
    
    m_program->bind();
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 8 * sizeof(float));
    
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 8 * sizeof(float));
    
    m_program->enableAttributeArray(2);
    m_program->setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(float), 2, 8 * sizeof(float));
    m_program->release();
    
    m_vao.release();
}

void CubeWidget::createImageTextures()
{
    QImage tiImage("ti_logo.jpg");
    if (tiImage.isNull()) {
        tiImage = QImage(512, 512, QImage::Format_RGBA8888);
        tiImage.fill(Qt::red);
        qDebug() << "Failed to load ti_logo.jpg, using red fallback";
    } else {
        qDebug() << "Successfully loaded ti_logo.jpg";
    }
    
    QImage toradexImage("toradex_logo.png");
    if (toradexImage.isNull()) {
        toradexImage = QImage(512, 512, QImage::Format_RGBA8888);
        toradexImage.fill(Qt::blue);
        qDebug() << "Failed to load toradex_logo.png, using blue fallback";
    } else {
        qDebug() << "Successfully loaded toradex_logo.png";
    }
    
    // Create textures for TI (index 0) and Toradex (index 1)
    m_textTextures[0] = createTextureFromImage(tiImage);
    m_textTextures[1] = createTextureFromImage(toradexImage);
}

QOpenGLTexture* CubeWidget::createTextureFromImage(const QImage& image)
{
    QOpenGLTexture* texture = new QOpenGLTexture(image.mirrored(false, true));
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    return texture;
}

void CubeWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_program->bind();
    m_vao.bind();
    
    m_model.setToIdentity();
    m_model.rotate(m_rotationX, 1.0f, 0.0f, 0.0f);
    m_model.rotate(m_rotationY, 0.0f, 1.0f, 0.0f);
    m_model.rotate(m_rotationZ, 0.0f, 0.0f, 1.0f);
    
    QMatrix4x4 mvp = m_projection * m_view * m_model;
    m_program->setUniformValue("mvp", mvp);
    
    // Bind depending on the current image
    int textureIndex = m_currentImage;
    if (m_textTextures[textureIndex]) {
        m_textTextures[textureIndex]->bind();
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices for 12 triangles
    }
    
    m_vao.release();
    m_program->release();
}

void CubeWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    
    m_projection.setToIdentity();
    float aspect = float(width) / float(height ? height : 1);
    m_projection.perspective(45.0f, aspect, 0.1f, 100.0f);
    
    m_view.setToIdentity();
    m_view.translate(0.0f, 0.0f, -3.0f);
}

void CubeWidget::animate()
{
    m_rotationX += 1.0f;
    m_rotationY += 2.0f;
    m_rotationZ += 0.5f;
    
    if (m_rotationX >= 360.0f) m_rotationX -= 360.0f;
    if (m_rotationY >= 360.0f) m_rotationY -= 360.0f;
    if (m_rotationZ >= 360.0f) m_rotationZ -= 360.0f;

    m_currentImage = 1;
    
    // This code makes the images switch every now and then
    // Switch texture every 120 frames (at 60FPS, roughly 2 seconds each)
    // static int frameCount = 0;
    // frameCount++;
    // if (frameCount >= 120) {
    //     m_currentImage = (m_currentImage + 1) % 2;
    //     frameCount = 0;
    // }
    
    update();
} 