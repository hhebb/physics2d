# ifndef TEST_WINDOW
# define TEST_WINDOW

# include "../../../src/Definition.hpp"
# include <QOpenGLWindow>
# include <QOpenGLBuffer>
# include <QOpenGLVertexArrayObject>
# include <QOpenGLShaderProgram>
# include <QVariant>

class TestWindow: public QOpenGLWindow
{
    Q_OBJECT
public slots:
    void Render(QVariant); // param 은 뭐?
private:
    // QOpenGLBuffer vbo;
    // QOpenGLBuffer ibo;
    // QOpenGLVertexArrayObject vao;
    // QOpenGLShaderProgram program;
    POLY_LIST testCoord;
    int width;
    int height;

public:
    bool draw;
    TestWindow();
    ~TestWindow();
    void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void Init();
    void SetVertices(POLY_LIST);
    void Resize();
    void ClearWindow();
    void fff();
};


# endif