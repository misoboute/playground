#include <iostream>

#include <QApplication>
#include <QBoxLayout>
#include <QWebEnginePage>
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
    auto webPage = webView->page();
    auto scriptSource = R"(
options =
{
    page: 1, // number indicating page to return
    pageSize: 10,    // number of cases in each page
    sort: ['-lastModified', 'creationDate'],    // array of sort criteria (- means descending)
    startDate:    234234234,    // timestamps of lastModified date. . results should have lastmodified date after this value (inclusive)
    endDate:    32245454,    // timestamp of lastmodified. results should have lastmodified date before this value (exclusive)
    search: '', // string to filter results (deault: undefined)
    owner: ['me', 'others', 'nameOrId'],    // array of owners for filtering results. could be me, others or name or id of owner(s) (defaults: me , others)
    jaw: 'upper',    // jaw of filtering the results. could be upper, lower or both (defaults: all)
    objects: [],    // list of object in case (for filtering the reulsts) (defaults: all)
    status: [1,2,3]    // array of status for filtering the results (defaults: all)    
};
options;
        )";
    webPage->runJavaScript(scriptSource, [](const QVariant &v) { SMQDW1(v); });
    // Yields:
    /*
        v  =
        QVariant(
            QVariantMap, QMap(
                ("endDate", QVariant(int, 32245454))
                ("jaw", QVariant(QString, "upper"))
                ("objects", QVariant(QVariantList, ()))
                ("owner", QVariant(QVariantList, (QVariant(QString, "me"), QVariant(QString, "others"), QVariant(QString, "nameOrId"))))
                ("page", QVariant(int, 1))
                ("pageSize", QVariant(int, 10))
                ("search", QVariant(QString, ""))
                ("sort", QVariant(QVariantList, (QVariant(QString, "-lastModified"), QVariant(QString, "creationDate"))))
                ("startDate", QVariant(int, 234234234))
                ("status", QVariant(QVariantList, (QVariant(int, 1), QVariant(int, 2), QVariant(int, 3))))
            )
        )
    */
    return app.exec();
}
