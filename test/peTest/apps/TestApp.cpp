# include "TestApp.hpp"
# include <iostream>
# include <QKeyEvent>
# include <QThread>
# include <QEventLoop>
# include <QCoreApplication>
# include "../../../src/physics/TestThread.hpp"

using namespace std;

TestApp::TestApp(TestWindow* window, World* world)//: TestWIndow*(), World*()
{
    // 생성자에서 멤버 변수, 시그널 전부 셋팅해준다.
    
    this->window = window;
    this->world = world;
    // 물리엔진-렌러창 signal -slot 연결
    // connect(this, SIGNAL (requestUpdate(Vector2)), this->window, SLOT (Render(Vector2)));
    connect(window, SIGNAL (frameSwapped()), this, SLOT (setReady()));
    window->installEventFilter(this);
}

void TestApp::InitApp()
{
    // Set OpenGL Version information
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3,3);
    format.setSwapInterval(1);
    // format.setDepthBufferSize(24);
    // format.setStencilBufferSize(8);
    // window->setFormat(format);
    window->resize(640, 480);

    world->Init();
    window->SetVertices(world->GetVertices());
    window->show();

}
// void TestApp::SetWorld(World* world)
// {
//     this->world = world;
// }

// void TestApp::SetWindow(TestWindow* window)
// {
//     this->window = window;
    
// }

TestWindow* TestApp::GetWindow()
{
    return window;
}


void TestApp::Run()
{
    // 루프를 돌면서 물리 연산과 디버깅 창 렌더링 동시에 진행.

    // window create 함수 따로 만들기.
    // QSurfaceFormat format;
	// format.setRenderableType(QSurfaceFormat::OpenGL);
	// format.setProfile(QSurfaceFormat::CoreProfile);
	// format.setVersion(3, 3);
    // window->setFormat(format);
	// window->resize(640, 600);
	// window->show();
    
    window->draw = true;
    connect(world, SIGNAL (physicsUpdate(QVariant)), window, SLOT (Render(QVariant)));
    world->start();
}

void TestApp::setReady()
{
    world->ready = true;
}

bool TestApp::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        // qDebug() << "Ate key press" << keyEvent->key();
        this->Run();

        return true;
    } else {
        return false;
    }

}
