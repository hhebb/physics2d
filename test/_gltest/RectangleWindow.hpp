
#ifndef RectangleWindow_H
#define RectangleWindow_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWindow>

/*	This is the window that shows the two triangles to form a rectangle.
	We derive from our QOpenGLWindow base class and implement the
	virtual initializeGL() and paintGL() functions.
*/
class RectangleWindow : public QOpenGLWindow {
public:
	RectangleWindow();
	virtual ~RectangleWindow() Q_DECL_OVERRIDE;

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void animate();
	std::vector<float> vertexBufferData;

private:
	// Wraps an OpenGL VertexArrayObject (VAO)
	QOpenGLVertexArrayObject	m_vao;
	// Vertex buffer (positions and colors, interleaved storage mode).
	QOpenGLBuffer				m_vertexBufferObject;
	// Index buffer to draw two rectangles
	QOpenGLBuffer				m_indexBufferObject;

	// Holds the compiled shader programs.
	QOpenGLShaderProgram		*m_program;

	int count;
};

#endif // RectangleWindow_H