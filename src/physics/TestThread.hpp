# ifndef TEST_THREAD
# define TEST_THREAD

# include <QThread>
# include <QObject>
# include <iostream>
# include "Body.hpp"

class TestThread : public QThread
{
    Q_OBJECT
signals:
    void counter(int);
private:
    void run();
    vector<Body> bodies;
public:
    TestThread();
    void Init(vector<Body>);
};

# endif