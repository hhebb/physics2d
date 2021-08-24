# ifndef MAIN_WINDOW
# define MAIN_WINDOW

# include <QApplication>
# include <QSurfaceFormat>
# include <QPushButton>
# include <QLabel>
# include <QWidget>
# include <QMainWindow>
# include <QVBoxLayout>
# include <QHBoxLayout>
# include <QComboBox>
# include <QSlider>
# include "apps/TestApp.hpp"
# include <sys/stat.h>
# include <dirent.h>
# include <stdio.h>
# include <QString>

using namespace std;

class MainWindow: public QMainWindow
{
    Q_OBJECT

private:
    QComboBox* presetComboBox;
    QPushButton* playButton;
    QPushButton* pauseButton;
    QPushButton* stepButton;
    QPushButton* resetButton;
    QWidget* container;
    QVBoxLayout* mainLayout;
    QWidget* menuBar;

    TestApp* app;
    TestWindow* window;
public:
    MainWindow();
    void SetApp(TestApp* app);
    void SetWindow(TestWindow* window);
    void SetComboBox();
};

# endif