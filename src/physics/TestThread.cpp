# include "TestThread.hpp"

TestThread::TestThread()
{

}

void TestThread::Init(vector<Body> bodies)
{
    this->bodies = bodies;
}

void TestThread::run()
{
    int i = 0;
    while(true)
    {
        emit counter(i++);
        QThread::sleep(1);
    }
}
