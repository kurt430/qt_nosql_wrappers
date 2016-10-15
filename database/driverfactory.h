#ifndef DRIVERFACTORY_H
#define DRIVERFACTORY_H

#include <QList>
#include <QString>

class QObject;

namespace krdb {

class AbstractDriver;

class DriverFactory
{
public:
    static QList<QString> avaiable();
    static AbstractDriver *create(const QString& text, QObject *parent = nullptr);
};

}

#endif // DRIVERFACTORY_H
