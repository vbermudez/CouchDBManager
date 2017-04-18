#include "variables.h"

QString Variables::get_full_name() const
{
    return this->full_name;
}

void Variables::set_full_name(const QString & a_full_name)
{
    this->full_name = a_full_name;
}
