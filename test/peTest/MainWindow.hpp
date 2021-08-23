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

class MainWindow: public QMainWindow
{
    Q_OBJECT

private:
    QPushButton* playButton;
    QPushButton* stepButton;
    QPushButton* stopButton;
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
};

# endif