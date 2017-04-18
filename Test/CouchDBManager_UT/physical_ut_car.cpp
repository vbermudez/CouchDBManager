#include "physical_ut_car.h"


Physical_ut *Physical_ut_car::get_physical_ut() const
{
    return this->physical_ut;
}

void Physical_ut_car::set_physical_ut(Physical_ut *value)
{
    this->physical_ut=value;
}

Physical_car *Physical_ut_car::get_physical_car() const
{
    return this->physical_car;
}

void Physical_ut_car::set_physical_car(Physical_car *value)
{
    this->physical_car = value;
}

