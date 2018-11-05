#pragma once

#include <QJsonObject>

#define SM_DEBUG_FILE_LINENUM // Should it output filename and line number
#include "SMDebug.h"

class SmopIface : public QObject
{
    Q_OBJECT
public:
    SmopIface(QObject * parent = nullptr)
        : QObject(parent)
    {
    }

public slots:
    QJsonObject info(const QJsonObject& info)
    {
        SMQDW1(info);
        return { { "Goodbye", "World" } };
    }
};
