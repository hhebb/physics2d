# include "App.hpp"
# include <iostream>
# include <QKeyEvent>
# include <QThread>
# include <QEventLoop>
# include <QCoreApplication>

using namespace std;

TestApp::TestApp(TestWindow* window, World* world)//: TestWIndow*(), World*()
{
    // 생성자에서 멤버 변수, 시그널 전부 셋팅.
    this->window = window;
    this->world = world;
    // 물리엔진-렌러창 signal -slot 연결
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

    window->SetVertices(world->GetVertices());
    connect(world, SIGNAL (physicsUpdate(QVariant)), window, SLOT (Render(QVariant)));

}

void TestApp::SetWorld(World* world)
{
    this->world = world;
}

void TestApp::SetWindow(TestWindow* window)
{
    this->window = window;
}

TestWindow* TestApp::GetWindow()
{
    return window;
}


void TestApp::Run()
{
    // 루프를 돌면서 물리 연산과 디버깅 창 렌더링 동시에 진행.
    world->SetState(world->PLAY);
    window->draw = true;
    world->start();
}

void TestApp::SingleStep()
{
    world->SetState(world->SINGLE_STEP);
    window->draw = true;
    world->start();

}

void TestApp::Pause()
{
    world->SetState(world->PAUSE);
}

void TestApp::Reset()
{
    SetPreset(presetName);
}

void TestApp::setReady()
{
    world->ready = true;
}

void TestApp::SetPreset(QString name)
{
    // comboBox 에서 preset 선택할 때마다 world Init 해줌.
    presetName = name;
    // cout << "set preset" << name.toStdString() << endl;
    world->Init(name.toStdString());
    world->SetState(world->SINGLE_STEP);
    window->ClearWindow();
}

void TestApp::SetCollisionVelIter(int value)
{
    world->SetVelCollisionIter(value);
}

void TestApp::SetCollisionPosIter(int value)
{
    world->SetPosCollisionIter(value);
}

void TestApp::SetJointVelIter(int value)
{
    world->SetVelJointIter(value);
}

void TestApp::SetJointPosIter(int value)
{
    world->SetPosJointIter(value);
}

// not using
bool TestApp::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        // qDebug() << "Ate key press" << keyEvent->key();
        // this->Run();

        return true;
    } else {
        return false;
    }

}
