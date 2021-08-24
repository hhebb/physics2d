# include "MainWindow.hpp"

MainWindow::MainWindow()
{
    // main GUI
	this->setGeometry(0, 0, 800, 600);
	QWidget* mainWidget = new QWidget();
	this->setCentralWidget(mainWidget);
	mainLayout = new QVBoxLayout();
	mainWidget->setLayout(mainLayout);

	// menu bar
	menuBar = new QWidget();
	QHBoxLayout* menuLayout = new QHBoxLayout();
	menuBar->setLayout(menuLayout);
	presetComboBox = new QComboBox();
	playButton = new QPushButton();
	pauseButton = new QPushButton();
	stepButton = new QPushButton();
	resetButton = new QPushButton();
	QSlider* gravitySlider = new QSlider(Qt::Orientation::Horizontal);
	QSlider* velIterSlider = new QSlider(Qt::Orientation::Horizontal);
	QSlider* posIterSlider = new QSlider(Qt::Orientation::Horizontal);
	velIterSlider->setSingleStep(1);
	menuLayout->addWidget(presetComboBox);
	menuLayout->addWidget(playButton);
	menuLayout->addWidget(pauseButton);
	menuLayout->addWidget(stepButton);
	menuLayout->addWidget(resetButton);
	menuLayout->addWidget(gravitySlider);
	menuLayout->addWidget(velIterSlider);
	menuLayout->addWidget(posIterSlider);
	menuBar->setLayout(menuLayout);


}

void MainWindow::SetApp(TestApp* app)
{
	this->app = app;
	connect(playButton, SIGNAL (clicked()), app, SLOT (Run()));
	connect(pauseButton, SIGNAL (clicked()), app, SLOT (Pause()));
	connect(stepButton, SIGNAL (clicked()), app, SLOT (SingleStep()));
    connect(resetButton, SIGNAL (clicked()), app, SLOT (Pause()));
	SetComboBox();

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

void MainWindow::SetComboBox()
{
	// search preset json files.
	const char* path = "/home/hebb/project/pe2d/physics2d/test/peTest/presets";
	vector<string> presetLists;
	DIR *dir = opendir(path);
    if(dir == NULL)
    {
        printf("failed open\n");
    }
	
    struct dirent *de=NULL;
    while((de = readdir(dir))!=NULL)
    {
		string s(de->d_name);
		int pos = s.find(".json");
		if (pos != string::npos)
		{
			// add preset item.
			QString qs = s.substr(0, pos).c_str();
			presetComboBox->addItem(qs);
		}
    }
    closedir(dir);
	connect(presetComboBox, SIGNAL (currentTextChanged(QString)), app, SLOT (SetPreset(QString)));
}