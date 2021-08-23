# include "MainWindow.hpp"

MainWindow::MainWindow()
{
    // main GUI
	// QMainWindow* mainWindow = new QMainWindow();
	this->setGeometry(0, 0, 800, 600);
	QWidget* mainWidget = new QWidget();
	this->setCentralWidget(mainWidget);
	mainLayout = new QVBoxLayout();
	mainWidget->setLayout(mainLayout);

	// menu bar
	menuBar = new QWidget();
	QHBoxLayout* menuLayout = new QHBoxLayout();
	menuBar->setLayout(menuLayout);
	QComboBox* presetComboBox = new QComboBox();
	playButton = new QPushButton();
	stepButton = new QPushButton();
	resetButton = new QPushButton();
	QSlider* gravitySlider = new QSlider(Qt::Orientation::Horizontal);
	QSlider* velIterSlider = new QSlider(Qt::Orientation::Horizontal);
	QSlider* posIterSlider = new QSlider(Qt::Orientation::Horizontal);
	velIterSlider->setSingleStep(1);
	menuLayout->addWidget(presetComboBox);
	menuLayout->addWidget(playButton);
	menuLayout->addWidget(stepButton);
	menuLayout->addWidget(resetButton);
	menuLayout->addWidget(gravitySlider);
	menuLayout->addWidget(velIterSlider);
	menuLayout->addWidget(posIterSlider);
	menuBar->setLayout(menuLayout);
	

	// TestWindow* window = new TestWindow();
	// World* world = new World();
	// TestApp* testApp = new TestApp(window, world);


	
	// testApp->InitApp();

}

void MainWindow::SetApp(TestApp* app)
{
	this->app = app;
	connect(playButton, SIGNAL (clicked()), app, SLOT (Run()));
    connect(resetButton, SIGNAL (clicked()), app, SLOT (Pause()));
}

void MainWindow::SetWindow(TestWindow* window)
{
	this->window = window;
	
	QWidget* container = QWidget::createWindowContainer(window);
	mainLayout->addWidget(menuBar);
	mainLayout->addWidget(container);
	mainLayout->setStretchFactor(menuBar, 1);
	mainLayout->setStretchFactor(container, 10);
	
}