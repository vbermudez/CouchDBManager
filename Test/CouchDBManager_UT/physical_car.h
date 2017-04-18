#ifndef PHYSICAL_CAR_H
#define PHYSICAL_CAR_H

#include <QList>
#include <QVariant>

#include "versionableentity.h"
#include "physical_ut_car.h"
#include "basereference.h"

//class Physical_ut_car;

class Physical_car : public CouchDBManager::VersionableEntity
{
    Q_OBJECT
    Q_PROPERTY(CouchDBManager::BaseReference* car READ get_car WRITE set_car)
    Q_PROPERTY(QList<Physical_ut_car*> physical_ut_car READ get_physical_ut_car WRITE set_physical_ut_car)

private:
    CouchDBManager::BaseReference* car;
    QList<Physical_ut_car*> physical_ut_car;

public:
    Q_INVOKABLE Physical_car(QObject *parent = 0) : CouchDBManager::VersionableEntity(parent) {


        this->collection = "physical_car";
    }

public slots:

    CouchDBManager::BaseReference *get_car() const;
    void set_car(CouchDBManager::BaseReference *value);

    QList<Physical_ut_car*> get_physical_ut_car() const;
    void set_physical_ut_car(const QList<Physical_ut_car*>& a_physical_ut_car);
    void add_physical_ut_car(CouchDBManager::BaseObject* a_physical_ut_car);

};

Q_DECLARE_METATYPE(Physical_car*)

#endif // PHYSICAL_CAR_H
