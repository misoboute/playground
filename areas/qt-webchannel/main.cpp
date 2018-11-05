#include <iostream>

#include <QApplication>
#include <QBoxLayout>
#include <QFile>
#include <QJsonObject>
#include <QWebChannel>
#include <QWebEnginePage>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QWebEngineView>
#include <QWidget>

#include "Cases.h"
#include "SmopIface.h"
#define SM_DEBUG_FILE_LINENUM // Should it output filename and line number
#include "SMDebug.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    QWidget mainWidget;
    auto mainWidgetLayout = new QVBoxLayout(&mainWidget);
    mainWidget.setLayout(mainWidgetLayout);
    auto webView = new QWebEngineView(&mainWidget);
    mainWidgetLayout->addWidget(webView, 1);
    mainWidget.showMaximized();
    auto webPage = webView->page();
    QFile webChannelScriptSourceFile(":/qtwebchannel/qwebchannel.js");
    if (!webChannelScriptSourceFile.open(QIODevice::ReadOnly))
        SMQDBG << "Couldn't load Qt's QWebChannel API!";
    auto webChannelScriptSource =
        QString::fromUtf8(webChannelScriptSourceFile.readAll());
    QWebEngineScript webChannelScript;
    webChannelScript.setSourceCode(webChannelScriptSource);
    webChannelScript.setInjectionPoint(QWebEngineScript::DocumentCreation);
    webChannelScript.setWorldId(QWebEngineScript::MainWorld);
    webPage->scripts().insert(webChannelScript);
    webPage->runJavaScript(
        webChannelScriptSource, [](const QVariant& v) { SMQDW1(v); });
    auto channel = new QWebChannel(webPage);
    webPage->setWebChannel(channel);
    Cases casesObj;
    SmopIface smopIfaceObj;

    channel->registerObject(QStringLiteral("Cases"), &casesObj);
    channel->registerObject(QStringLiteral("SmopIface"), &smopIfaceObj);

    webView->setUrl(QUrl::fromLocalFile(INDEX_HTML_PATH));
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
    // Expects as result:
    /*
    {
        meta: {
            count: 155    // total number of cases (after applying the filters)
        },
        data: [
            {caseId: 3232, ...},
            {caseId: 5345, ...},
            ...
        ]
    }
    */
    // Setting security token:
    // SmopInterface.Info({token: string, userId: 4234, ...})
    return app.exec();
}
