#ifndef MONITCONFIG_H
#define MONITCONFIG_H

#include <QList>
#include <QVariant>

#include "baseentity.h"
#include <QDebug>

class MonitConfig : public CouchDBManager::BaseEntity
{
    Q_OBJECT

private:


public:

    Q_INVOKABLE MonitConfig(QObject *parent = 0) : CouchDBManager::BaseEntity(parent) {


        this->collection = "monit_config";
    }

public slots:

};

Q_DECLARE_METATYPE(MonitConfig*)

#endif // MONITCONFIG_H
