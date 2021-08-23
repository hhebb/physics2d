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

	// // main GUI
	// QMainWindow* mainWindow = new QMainWindow();
	// mainWindow->setGeometry(0, 0, 800, 600);
	// QWidget* mainWidget = new QWidget();
	// mainWindow->setCentralWidget(mainWidget);
	// QVBoxLayout* mainLayout = new QVBoxLayout();
	// mainWidget->setLayout(mainLayout);

	// // menu bar
	// QWidget* menuBar = new QWidget();
	// QHBoxLayout* menuLayout = new QHBoxLayout();
	// menuBar->setLayout(menuLayout);
	// QComboBox* presetComboBox = new QComboBox();
	// QPushButton* playButton = new QPushButton();
	// QPushButton* stepButton = new QPushButton();
	// QPushButton* resetButton = new QPushButton();
	// QSlider* gravitySlider = new QSlider(Qt::Orientation::Horizontal);
	// QSlider* velIterSlider = new QSlider(Qt::Orientation::Horizontal);
	// QSlider* posIterSlider = new QSlider(Qt::Orientation::Horizontal);
	// velIterSlider->setSingleStep(1);
	// menuLayout->addWidget(presetComboBox);
	// menuLayout->addWidget(playButton);
	// menuLayout->addWidget(stepButton);
	// menuLayout->addWidget(resetButton);
	// menuLayout->addWidget(gravitySlider);
	// menuLayout->addWidget(velIterSlider);
	// menuLayout->addWidget(posIterSlider);
	// menuBar->setLayout(menuLayout);
	

	TestWindow* window = new TestWindow();
	World* world = new World();
	TestApp* testApp = new TestApp(window, world);

	mainWindow->SetApp(testApp);
	mainWindow->SetWindow(window);
	testApp->InitApp();

	// QWidget* container = QWidget::createWindowContainer(window);
	// mainLayout->addWidget(menuBar);
	// mainLayout->addWidget(container);
	// mainLayout->setStretchFactor(menuBar, 1);
	// mainLayout->setStretchFactor(container, 10);

	// 
	
	mainWindow->show();

	return app.exec();
}