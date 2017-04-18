#include "physical_device.h"

Device *Physical_device::get_device() const
{
    return this->device;
}

void Physical_device::set_device(Device *value)
{
    this->device = value;
}


