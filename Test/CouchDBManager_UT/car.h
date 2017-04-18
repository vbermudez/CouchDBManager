#ifndef CAR_H
#define CAR_H

#include <QList>
#include <QVariant>

#include "versionableentity.h"
#include "auxiliary.h"
#include "rs485cosmos.h"
#include <QDebug>

class Car : public CouchDBManager::VersionableEntity
{
    Q_OBJECT
    Q_PROPERTY(Linked* linked READ get_linked WRITE set_linked)
    Q_PROPERTY(QList<Rs485Cosmos*> rs485_cosmos READ get_rs485_cosmos WRITE set_rs485_cosmos)
    Q_PROPERTY(CosmosRef* cosmos READ get_cosmos WRITE set_cosmos)

private:
    Linked *linked;
    QList<Rs485Cosmos*> rs485_cosmos;
    CosmosRef* cosmos;

public:

    Q_INVOKABLE Car(QObject *parent = 0) : CouchDBManager::VersionableEntity(parent) {


        this->collection = "car";
        this->linked = NULL;
        this->cosmos = NULL;
    }

public slots:
    Linked *get_linked() const { return this->linked; }
    void set_linked(Linked *value) { this->linked = value; }

    QList<Rs485Cosmos*> get_rs485_cosmos() { return this->rs485_cosmos; }
    void set_rs485_cosmos(QList<Rs485Cosmos*> a_rs485_cosmos) { this->rs485_cosmos = a_rs485_cosmos; }
    void add_rs485_cosmos(CouchDBManager::BaseObject* a_rs485_cosmos)
    {
        this->rs485_cosmos.append( qobject_cast<Rs485Cosmos*>(a_rs485_cosmos) );
    }

    CosmosRef *get_cosmos() const { return this->cosmos; }
    void set_cosmos(CosmosRef *value) { this->cosmos = value; }
};

Q_DECLARE_METATYPE(Car*)

#endif // CAR_H
