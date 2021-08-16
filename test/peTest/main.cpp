// #include "_gltest/RectangleWindow.hpp"
# include <QApplication>
# include <QSurfaceFormat>
# include <QPushButton>
# include "apps/TestApp.hpp"
# include "../../src/physics/WorldParser.hpp"

int main(int argc, char **argv) {
	WorldParser parser;
	parser.Parse("/home/hebb/project/physics2d/test/test.json");
	QApplication app(argc, argv);
	TestWindow* window = new TestWindow();
	World* world = new World();
	TestApp testApp(window, world);
	testApp.InitApp();

	// Note: The format must be set before show() is called.
	// RectangleWindow window;
	// window.setFormat(format);
	// window.resize(640, 480);
	// window.show();

	return app.exec();
}