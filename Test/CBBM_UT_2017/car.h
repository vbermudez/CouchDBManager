#ifndef CAR_H
#define CAR_H

#include <QList>
#include <QVariant>

#include "versionableentity.h"
#include <QDebug>

class Car : public CouchDBManager::VersionableEntity
{
    Q_OBJECT

private:

public:

    Q_INVOKABLE Car(QObject *parent = 0) : CouchDBManager::VersionableEntity(parent) {
        this->collection = "car";
    }

public slots:

};

Q_DECLARE_METATYPE(Car*)

#endif // CAR_H
