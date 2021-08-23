// #include "_gltest/RectangleWindow.hpp"
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
# include "MainWindow.hpp"

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	MainWindow* mainWindow = new MainWindow();

	TestWindow* window = new TestWindow();
	World* world = new World();
	TestApp* testApp = new TestApp(window, world);

	mainWindow->SetApp(testApp);
	mainWindow->SetWindow(window);
	testApp->InitApp();

	
	mainWindow->show();

	return app.exec();
}