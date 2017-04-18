#ifndef PHYSICAL_UT_H
#define PHYSICAL_UT_H

#include <QList>
#include <QVariant>

#include "versionableentity.h"
#include "physical_ut_car.h"
#include "basereference.h"

#include "ut.h"

class Physical_ut : public CouchDBManager::VersionableEntity
{
    Q_OBJECT
    Q_PROPERTY(CouchDBManager::BaseReference* ut READ get_ut WRITE set_ut)
    Q_PROPERTY(QList<Physical_ut_car*> physical_ut_car READ get_physical_ut_car WRITE set_physical_ut_car)

private:
    CouchDBManager::BaseReference* ut;
    QList<Physical_ut_car*> physical_ut_car;

public:
    Q_INVOKABLE Physical_ut(QObject *parent = 0) : CouchDBManager::VersionableEntity(parent) {


        this->collection = "physical_ut";
    }

public slots:

    CouchDBManager::BaseReference *get_ut() const;
    void set_ut(CouchDBManager::BaseReference *value);

    QList<Physical_ut_car*> get_physical_ut_car() const;
    void set_physical_ut_car(const QList<Physical_ut_car*>& a_physical_ut_car);
    void add_physical_ut_car(CouchDBManager::BaseObject* a_physical_ut_car);

};

Q_DECLARE_METATYPE(Physical_ut*)

#endif // PHYSICAL_UT_H
