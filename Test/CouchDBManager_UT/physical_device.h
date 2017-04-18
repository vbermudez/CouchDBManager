#ifndef PHYSICAL_DEVICE_H
#define PHYSICAL_DEVICE_H

#include <QList>
#include <QVariant>

#include "device.h"

class Physical_device : public CouchDBManager::VersionableEntity
{
    Q_OBJECT
    Q_PROPERTY(Device* device READ get_device WRITE set_device)

private:
    Device* device;

public:
    Q_INVOKABLE Physical_device(QObject *parent = 0) : CouchDBManager::VersionableEntity(parent) {


        this->collection = "physical_device";
    }

public slots:

    Device *get_device() const;
    void set_device(Device *value);

};

Q_DECLARE_METATYPE(Physical_device*)

#endif // PHYSICAL_DEVICE_H
