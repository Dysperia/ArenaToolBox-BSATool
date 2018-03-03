#include "../components/MainWindow.h"
#include <QApplication>
#include "../log/Logger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    Logger::getInstance().log(Logger::MessageType::INFO, "Starting application");
    w.show();
    return app.exec();
}
