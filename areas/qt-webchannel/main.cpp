#include <iostream>

#include <QApplication>
#include <QBoxLayout>
#include <QWebEngineView>
#include <QWidget>

#include "SMDebug.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    QWidget mainWidget;
    auto mainWidgetLayout = new QVBoxLayout(&mainWidget);
    mainWidget.setLayout(mainWidgetLayout);
    auto webView = new QWebEngineView(&mainWidget);
    webView->setUrl(QUrl("https://www.google.com"));
    mainWidgetLayout->addWidget(webView, 1);
    mainWidget.showMaximized();
    SMDBG;
    return app.exec();
}
