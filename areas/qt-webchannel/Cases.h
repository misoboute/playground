#pragma once

#include <QJsonObject>

#define SM_DEBUG_FILE_LINENUM // Should it output filename and line number
#include "SMDebug.h"

class Cases : public QObject
{
    Q_OBJECT
public:
    Cases(QObject * parent = nullptr)
        : QObject(parent)
    {
    }

public slots:
    QJsonObject get(const QJsonObject& options)
    {
        SMQDW1(options);
        return { { "Hello", "World" } };
    }

signals:

private:
};
