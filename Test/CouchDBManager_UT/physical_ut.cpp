#include "physical_ut.h"

CouchDBManager::BaseReference *Physical_ut::get_ut() const
{
    return this->ut;
}

void Physical_ut::set_ut(CouchDBManager::BaseReference *value)
{
    this->ut = value;
}


QList<Physical_ut_car*> Physical_ut::get_physical_ut_car() const
{
    return this->physical_ut_car;
}

void Physical_ut::set_physical_ut_car(const QList<Physical_ut_car*>& a_physical_ut_car)
{
    this->physical_ut_car = a_physical_ut_car;
}

void Physical_ut::add_physical_ut_car(CouchDBManager::BaseObject* a_physical_ut_car)
{
    this->physical_ut_car.append( qobject_cast<Physical_ut_car*>(a_physical_ut_car) );
}

