# include "RenderWindow.hpp"
# include <iostream>

using namespace std;

TestWindow::TestWindow()
    // : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate)
{
    draw = false;
}

TestWindow::~TestWindow()
{
}

void TestWindow::initializeGL()
{
    Init();
}

void TestWindow::resizeGL(int w, int h)
{
    width = w;
    height = h;
}

void TestWindow::paintGL()
{
    // clear
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f((246/255.0f), (210/255.0f), (88/255.0f));
    
    Resize();

    for (int o = 0; o < testCoord.size(); o ++)
    {
        glBegin(GL_POLYGON);
        for (int v = 0; v < testCoord[o].size(); v ++)
        {
            glVertex2d(this->testCoord[o][v].x, this->testCoord[o][v].y);
            // cout << this->testCoord[o][v].x << ", " << this->testCoord[o][v].y<< endl;
        }
        glEnd();
    }

    glFlush();
    // cout << "painting" << endl; //<< testCoord.x << ", " << testCoord.y << endl;
}

void TestWindow::Init()
{
    // init window
    glClearColor((12/255.0f), (76/255.0f), (138/255.0f), 1);
}

void TestWindow::SetVertices(POLY_LIST vertices)
{
    testCoord = vertices;
}

void TestWindow::Resize()
{
    if (width > height)
        glViewport((width - height) / 2, 0, height, height);
    else
        glViewport(0, (height - width) / 2, width, width);

}

void TestWindow::ClearWindow()
{
    // 환경 바뀌었을 때 윈도우만 지워줌.
    glClear(GL_COLOR_BUFFER_BIT);
}

void TestWindow::Render(QVariant vertices)
{    
    this->testCoord = vertices.value<POLY_LIST>();
    // 버퍼 데이터들을 받아와서 셋팅해주기.
    update();
}