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
	playButton = new QPushButton("PLAY");
	pauseButton = new QPushButton("PAUSE");
	stepButton = new QPushButton("STEP");
	resetButton = new QPushButton("RESET");

	velColSlider = new QSlider(Qt::Orientation::Horizontal);
	posColSlider = new QSlider(Qt::Orientation::Horizontal);
	velJointSlider = new QSlider(Qt::Orientation::Horizontal);
	posJointSlider = new QSlider(Qt::Orientation::Horizontal);
	velColSlider->setMaximum(16);
	velColSlider->setSingleStep(1);
	velColSlider->setValue(4);
	posColSlider->setMaximum(16);
	posColSlider->setSingleStep(1);
	posColSlider->setValue(4);
	velJointSlider->setMaximum(16);
	velJointSlider->setSingleStep(1);
	velJointSlider->setValue(4);
	posJointSlider->setMaximum(16);
	posJointSlider->setSingleStep(1);
	posJointSlider->setValue(1);

	menuLayout->addWidget(presetComboBox);
	menuLayout->addWidget(playButton);
	menuLayout->addWidget(pauseButton);
	menuLayout->addWidget(stepButton);
	menuLayout->addWidget(resetButton);
	
	// slider
	QVBoxLayout* v1 = new QVBoxLayout();
	QLabel* l1 = new QLabel("vel iterator");
	lt1 = new QLabel(QString::fromStdString(to_string(velColSlider->value())));
	v1->addWidget(l1);
	v1->addWidget(velColSlider);
	v1->addWidget(lt1);

	QVBoxLayout* v2 = new QVBoxLayout();
	QLabel* l2 = new QLabel("pos iterator");
	lt2 = new QLabel(QString::fromStdString(to_string(posColSlider->value())));
	v2->addWidget(l2);
	v2->addWidget(posColSlider);
	v2->addWidget(lt2);

	QVBoxLayout* v3 = new QVBoxLayout();
	QLabel* l3 = new QLabel("joint vel iterator");
	lt3 = new QLabel(QString::fromStdString(to_string(velJointSlider->value())));
	v3->addWidget(l3);
	v3->addWidget(velJointSlider);
	v3->addWidget(lt3);

	QVBoxLayout* v4 = new QVBoxLayout();
	QLabel* l4 = new QLabel("joint pos iterator");
	lt4 = new QLabel(QString::fromStdString(to_string(posJointSlider->value())));
	v4->addWidget(l4);
	v4->addWidget(posJointSlider);
	v4->addWidget(lt4);

	menuLayout->addLayout(v1);
	menuLayout->addLayout(v2);
	menuLayout->addLayout(v3);
	menuLayout->addLayout(v4);


	menuBar->setLayout(menuLayout);

}

void MainWindow::SetApp(TestApp* app)
{
	this->app = app;
	connect(playButton, SIGNAL (clicked()), app, SLOT (Run()));
	connect(pauseButton, SIGNAL (clicked()), app, SLOT (Pause()));
	connect(stepButton, SIGNAL (clicked()), app, SLOT (SingleStep()));
    connect(resetButton, SIGNAL (clicked()), app, SLOT (Reset()));

	connect(velColSlider, SIGNAL (valueChanged(int)), app, SLOT (SetCollisionVelIter(int)));
	connect(posColSlider, SIGNAL (valueChanged(int)), app, SLOT (SetCollisionPosIter(int)));
	connect(velJointSlider, SIGNAL (valueChanged(int)), app, SLOT (SetJointVelIter(int)));
	connect(posJointSlider, SIGNAL (valueChanged(int)), app, SLOT (SetJointPosIter(int)));

	connect(velColSlider, SIGNAL (valueChanged(int)), this, SLOT (SetLabelVelColIter(int)));
	connect(posColSlider, SIGNAL (valueChanged(int)), this, SLOT (SetLabelPosColIter(int)));
	connect(velJointSlider, SIGNAL (valueChanged(int)), this, SLOT (SetLabelVelJointIter(int)));
	connect(posJointSlider, SIGNAL (valueChanged(int)), this, SLOT (SetLabelPosJointIter(int)));

	app->SetCollisionVelIter(velColSlider->value());
	app->SetCollisionPosIter(posColSlider->value());
	app->SetJointVelIter(velJointSlider->value());
	app->SetJointPosIter(posJointSlider->value());
	
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

	// set combobox items
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

	// 초기값 preset 설정.
	app->SetPreset(presetComboBox->currentText());

	// signal slot 연결.
	connect(presetComboBox, SIGNAL (currentTextChanged(QString)), app, SLOT (SetPreset(QString)));
}

// slots
void MainWindow::SetLabelVelColIter(int iter)
{
	string s(to_string(iter));
	QString qs = QString::fromStdString(s);
	lt1->setText(qs);
}

void MainWindow::SetLabelPosColIter(int iter)
{
	string s(to_string(iter));
	QString qs = QString::fromStdString(s);
	lt2->setText(qs);
}

void MainWindow::SetLabelVelJointIter(int iter)
{
	string s(to_string(iter));
	QString qs = QString::fromStdString(s);
	lt3->setText(qs);
}

void MainWindow::SetLabelPosJointIter(int iter)
{
	string s(to_string(iter));
	QString qs = QString::fromStdString(s);
	lt4->setText(qs);
}