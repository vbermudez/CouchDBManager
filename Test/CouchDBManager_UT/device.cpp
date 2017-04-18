#include "device.h"

QString Device::get_type() const
{
    return this->type;
}

void Device::set_type(const QString& a_type)
{
    this->type = a_type;
}

int Device::get_version() const
{
    return this->version;
}

void Device::set_version(int a_version)
{
    this->version = a_version;
}

QString Device::get_description() const
{
    return this->description;
}

void Device::set_description(const QString& a_description)
{
    this->description = a_description;
}

QList<Bus*> Device::get_bus() const
{
    return this->bus;
}

void Device::set_bus(const QList<Bus*>& a_bus)
{
    this->bus = a_bus;
}

void Device::add_bus(CouchDBManager::BaseObject *a_bus)
{
    this->bus.append( qobject_cast<Bus*>( a_bus ) );
}
