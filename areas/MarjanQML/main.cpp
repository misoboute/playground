#include <iostream>

#include <QApplication>
#include <QQmlApplicationEngine>


int main(int argc, char** argv)
{
    // try
    // {
        QApplication app(argc, argv);
        QQmlApplicationEngine engine;
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        return app.exec();
    // }
    // catch(const std::exception& e)
    // {
    //     std::cout << "What the hell: " << e.what() << std::endl;
    // }
}