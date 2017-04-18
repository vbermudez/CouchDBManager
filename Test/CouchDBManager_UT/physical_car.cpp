#include "physical_car.h"

CouchDBManager::BaseReference* Physical_car::get_car() const
{
    return this->car;
}

void Physical_car::set_car(CouchDBManager::BaseReference *value)
{
    this->car = value;
}


QList<Physical_ut_car*> Physical_car::get_physical_ut_car() const
{
    return this->physical_ut_car;
}

void Physical_car::set_physical_ut_car(const QList<Physical_ut_car*>& a_physical_ut_car)
{
    this->physical_ut_car = a_physical_ut_car;
}

void Physical_car::add_physical_ut_car(CouchDBManager::BaseObject* a_physical_ut_car)
{
    this->physical_ut_car.append( qobject_cast<Physical_ut_car*>(a_physical_ut_car) );
}
