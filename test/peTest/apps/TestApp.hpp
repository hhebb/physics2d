# ifndef TEST_APP
# define TEST_APP

# include <QObject>
# include "../../../src/physics/World.hpp"
# include "../window/TestWindow.hpp"
# include "../../../src/Definition.hpp"

class TestApp: public QObject
{
    Q_OBJECT

signals:
    void requestUpdate(Vector2); // vertices 를 넘겨줘야함.
public slots:
    void Run();
    void Pause();
    void setReady();

private:
    World* world;
    TestWindow* window;
    int count;
public:
    bool ready;
    TestApp(TestWindow*, World*);
    void InitApp();
    void SetWorld(World*);
    void SetWindow(TestWindow*);
    TestWindow* GetWindow();
    bool eventFilter(QObject*, QEvent*);
};

# endif