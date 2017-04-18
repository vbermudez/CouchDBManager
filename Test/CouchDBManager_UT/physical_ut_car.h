#ifndef PHYSICAL_UT_CAR_H
#define PHYSICAL_UT_CAR_H

#include <QList>
#include <QVariant>

#include "relationentity.h"

//#include "physical_ut.h"
//#include "physical_car.h"

class Physical_ut;
class Physical_car;

class Physical_ut_car : public CouchDBManager::RelationEntity
{
    Q_OBJECT
    Q_PROPERTY(Physical_ut* physical_ut READ get_physical_ut WRITE set_physical_ut)
    Q_PROPERTY(Physical_car* physical_car READ get_physical_car WRITE set_physical_car)

private:
    Physical_ut* physical_ut;
    Physical_car* physical_car;

public:
    Q_INVOKABLE Physical_ut_car(QObject *parent = 0) : CouchDBManager::RelationEntity(parent) {


        this->physical_ut = NULL;
        this->physical_car = NULL;
        this->collection = "physical_ut_car";
    }

public slots:

    Physical_ut *get_physical_ut() const;
    void set_physical_ut(Physical_ut *value);

    Physical_car *get_physical_car() const;
    void set_physical_car(Physical_car *value);

};

Q_DECLARE_METATYPE(Physical_ut_car*)

#endif // PHYSICAL_UT_CAR_H
