//
// COMPANY: ......... Construcciones y Auxiliar de Ferrocarriles S.A.
//
// COPYRIGHT:
//   This document is the property of CAF. Under the copyright laws, the
//   documentation may not be copied, photocopied, reproduced, translated, or
//   reduced to any electronic medium or machine-readable form, in whole or
//   in part, without the prior written consent of CAF.
//   Copyright 2014 (c) CAF, S.A
//
// PROJECT: ......... AD37
//
// NAME: ............ $RCSFile: $
//
// AUTHOR: .......... $Author: jgurrutxaga $
//
// REVISION: ........ $Revision: 1.1 $ ($Date: 2016/08/09 10:53:40 $)
//
// REFERENCE: ....... AD.37.91.3057
//
// CHANGES:
//
#include "types.h"

namespace Configuration {

QList<Variable_types*> Types::get_variable_types() const
{
    return variable_types;
}

void Types::set_variable_types(const QList<Variable_types*>& value)
{
    variable_types = value;
}

void Types::add_variable_types(CouchDBManager::BaseObject* value)
{
    this->variable_types.append( qobject_cast<Variable_types*>(value));
}

QList<CouchDBManager::BaseObject*> Types::get_distributed_container_types() const
{
    return distributed_container_types;
}

void Types::set_distributed_container_types(const QList<CouchDBManager::BaseObject*>& value)
{
    distributed_container_types = value;
}

void Types::add_distributed_container_types(CouchDBManager::BaseObject* value)
{
    this->distributed_container_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}


QList<CouchDBManager::BaseObject*> Types::get_distributed_container_rs485_sub_types() const
{
    return distributed_container_rs485_sub_types;
}

void Types::set_distributed_container_rs485_sub_types(const QList<CouchDBManager::BaseObject*>& value)
{
    distributed_container_rs485_sub_types = value;
}

void Types::add_distributed_container_rs485_sub_types(CouchDBManager::BaseObject* value)
{
    this->distributed_container_rs485_sub_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}

QList<CouchDBManager::BaseObject*> Types::get_distributed_container_can_sub_types() const
{
    return distributed_container_can_sub_types;
}

void Types::set_distributed_container_can_sub_types(const QList<CouchDBManager::BaseObject*>& value)
{
    distributed_container_can_sub_types = value;
}

void Types::add_distributed_container_can_sub_types(CouchDBManager::BaseObject* value)
{
    this->distributed_container_can_sub_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}

QList<CouchDBManager::BaseObject*> Types::get_local_container_types() const
{
    return local_container_types;
}

void Types::set_local_container_types(const QList<CouchDBManager::BaseObject *> &value)
{
    local_container_types = value;
}

void Types::add_local_container_types(CouchDBManager::BaseObject* value)
{
    this->local_container_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}

QList<Io_card_types*> Types::get_io_card_types() const
{
    return io_card_types;
}

void Types::set_io_card_types(const QList<Io_card_types*>& value)
{
    io_card_types = value;
}

void Types::add_io_card_types(CouchDBManager::BaseObject* value)
{
    this->io_card_types.append( qobject_cast<Io_card_types*>(value));
}

QList<CouchDBManager::BaseObject*> Types::get_digital_channel_inversion_mode_types() const
{
    return digital_channel_inversion_mode_types;
}

void Types::set_digital_channel_inversion_mode_types(const QList<CouchDBManager::BaseObject *> &value)
{
    digital_channel_inversion_mode_types = value;
}

void Types::add_digital_channel_inversion_mode_types(CouchDBManager::BaseObject* value)
{
    this->digital_channel_inversion_mode_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}

QList<CouchDBManager::BaseObject*> Types::get_task_types() const
{
    return task_types;
}

void Types::set_task_types(const QList<CouchDBManager::BaseObject*>& value)
{
    task_types = value;
}

void Types::add_task_types(CouchDBManager::BaseObject* value)
{
    this->task_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}

QList<CouchDBManager::BaseObject*> Types::get_publishing_version_types() const
{
    return publishing_version_types;
}

void Types::set_publishing_version_types(const QList<CouchDBManager::BaseObject*>& value)
{
    publishing_version_types = value;
}

void Types::add_publishing_version_types(CouchDBManager::BaseObject* value)
{
    this->publishing_version_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}

QList<Bus_types*> Types::get_bus_types() const
{
    return bus_types;
}

void Types::set_bus_types(const QList<Bus_types*>& value)
{
    bus_types = value;
}

void Types::add_bus_types(CouchDBManager::BaseObject* value)
{
    this->bus_types.append( qobject_cast<Bus_types*>(value));
}

QList<System_types*> Types::get_system_types() const
{
    return system_types;
}

void Types::set_system_types(const QList<System_types*>& value)
{
    system_types = value;
}

void Types::add_system_types(CouchDBManager::BaseObject* value)
{
    this->system_types.append( qobject_cast<System_types*>(value));
}

QList<CouchDBManager::BaseObject*> Types::get_device_types() const
{
    return device_types;
}

void Types::set_device_types(const QList<CouchDBManager::BaseObject*>& value)
{
    device_types = value;
}

void Types::add_device_types(CouchDBManager::BaseObject* value)
{
    this->device_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}

QList<Device_hw_types*> Types::get_device_hw_types() const
{
    return device_hw_types;
}

void Types::set_device_hw_types(const QList<Device_hw_types*>& value)
{
    device_hw_types = value;
}

void Types::add_device_hw_types(CouchDBManager::BaseObject* value)
{
    this->device_hw_types.append( qobject_cast<Device_hw_types*>(value));
}

QList<Device_hw_version*> Types::get_device_hw_version() const
{
    return device_hw_version;
}

void Types::set_device_hw_version(const QList<Device_hw_version*>& value)
{
    device_hw_version = value;
}

void Types::add_device_hw_version(CouchDBManager::BaseObject* value)
{
    this->device_hw_version.append( qobject_cast<Device_hw_version*>(value));
}

QList<Tools*> Types::get_tools() const
{
    return tools;
}

void Types::set_tools(const QList<Tools*>& value)
{
    tools = value;
}

void Types::add_tools(CouchDBManager::BaseObject* value)
{
    this->tools.append( qobject_cast<Tools*>(value));
}

QList<CouchDBManager::BaseObject*> Types::get_tools_types() const
{
    return tools_types;
}

void Types::set_tools_types(const QList<CouchDBManager::BaseObject *> &value)
{
    tools_types = value;
}

void Types::add_tools_types(CouchDBManager::BaseObject* value)
{
    this->tools_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}

QList<CouchDBManager::BaseObject*> Types::get_unit_types() const
{
    return unit_types;
}

void Types::set_unit_types(const QList<CouchDBManager::BaseObject*>& value)
{
    unit_types = value;
}

void Types::add_unit_types(CouchDBManager::BaseObject* value)
{
    this->unit_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}

QList<CouchDBManager::BaseObject*> Types::get_geometric_config_types() const
{
    return geometric_config_types;
}

void Types::set_geometric_config_types(const QList<CouchDBManager::BaseObject *> &value)
{
    geometric_config_types = value;
}

void Types::add_geometric_config_types(CouchDBManager::BaseObject* value)
{
    this->geometric_config_types.append( qobject_cast<CouchDBManager::BaseObject*>(value));
}

//////////////////////////////////////////////////////////////////////////

QString Variable_types::get_name() const
{
    return name;
}

void Variable_types::set_name(const QString &value)
{
    name = value;
}

QString Variable_types::get_description() const
{
    return description;
}

void Variable_types::set_description(const QString &value)
{
    description = value;
}

int Variable_types::get_size() const
{
    return size;
}

void Variable_types::set_size(int value)
{
    size = value;
}

QString Variable_types::get_c_type() const
{
    return c_type;
}

void Variable_types::set_c_type(const QString &value)
{
    c_type = value;
}

///////////////////////////////////////////////////////////////////////////


QString Io_card_types::get_name() const
{
    return name;
}

void Io_card_types::set_name(const QString &value)
{
    name = value;
}

QString Io_card_types::get_description() const
{
    return description;
}

void Io_card_types::set_description(const QString &value)
{
    description = value;
}

QString Io_card_types::get_digital_channel_inversion_mode_type_id() const
{
    return digital_channel_inversion_mode_type_id;
}

void Io_card_types::set_digital_channel_inversion_mode_type_id(const QString &value)
{
    digital_channel_inversion_mode_type_id = value;
}

QStringList Io_card_types::get_variable_types_names() const
{
    return variable_types_names;
}

void Io_card_types::set_variable_types_names(const QStringList &value)
{
    variable_types_names = value;
}

void Io_card_types::add_variable_types_names(const QString &value)
{
    variable_types_names.append(value);
}

////////////////////////////////////////////////////////////////////////////

QString Bus_types::get_name() const
{
    return name;
}

void Bus_types::set_name(const QString &value)
{
    name = value;
}

QString Bus_types::get_description() const
{
    return description;
}

void Bus_types::set_description(const QString &value)
{
    description = value;
}

int Bus_types::get_show_order() const
{
    return show_order;
}

void Bus_types::set_show_order(int value)
{
    show_order = value;
}
QList<System_types*> Bus_types::get_system_types() const
{
    return system_types;
}

void Bus_types::set_system_types(const QList<System_types*>& value)
{
    system_types = value;
}

void Bus_types::add_system_types(CouchDBManager::BaseObject* value)
{
    this->system_types.append( qobject_cast<System_types*>(value));
}

QList<Tools *> Bus_types::get_tools() const
{
    return tools;
}

void Bus_types::set_tools(const QList<Tools *> &value)
{
    tools = value;
}

void Bus_types::add_tools(CouchDBManager::BaseObject* value)
{
    this->tools.append( qobject_cast<Tools*>(value));
}

////////////////////////////////////////////////////////////////////////////

QString System_types::get_name() const
{
    return name;
}

void System_types::set_name(const QString &value)
{
    name = value;
}

QString System_types::get_description() const
{
    return description;
}

void System_types::set_description(const QString &value)
{
    description = value;
}

QList<CouchDBManager::BaseObject *> System_types::get_sub_system_types() const
{
    return sub_system_types;
}

void System_types::set_sub_system_types(const QList<CouchDBManager::BaseObject *> &value)
{
    sub_system_types = value;
}

////////////////////////////////////////////////////////////////////////////

QString Device_hw_types::get_name() const
{
    return name;
}

void Device_hw_types::set_name(const QString &value)
{
    name = value;
}

QString Device_hw_types::get_description() const
{
    return description;
}

void Device_hw_types::set_description(const QString &value)
{
    description = value;
}

int Device_hw_types::get_device_type_id() const
{
    return device_type_id;
}

void Device_hw_types::set_device_type_id(int value)
{
    device_type_id = value;
}

////////////////////////////////////////////////////////////////////////////

QString Device_hw_version::get_name() const
{
    return name;
}

void Device_hw_version::set_name(const QString &value)
{
    name = value;
}

QString Device_hw_version::get_description() const
{
    return description;
}

void Device_hw_version::set_description(const QString &value)
{
    description = value;
}

int Device_hw_version::get_device_hw_type_id() const
{
    return device_hw_type_id;
}

void Device_hw_version::set_device_hw_type_id(int value)
{
    device_hw_type_id = value;
}

QList<Tools *> Device_hw_version::get_tools() const
{
    return tools;
}

void Device_hw_version::set_tools(const QList<Tools *> &value)
{
    tools = value;
}

void Device_hw_version::add_tools(CouchDBManager::BaseObject* value)
{
    this->tools.append( qobject_cast<Tools*>(value));
}


QList<QString> Device_hw_version::get_io_card_type_ids() const
{
    return io_card_type_ids;
}

void Device_hw_version::set_io_card_type_ids(const QList<QString> &value)
{
    io_card_type_ids = value;
}

QList<QString> Device_hw_version::get_allowed_bus_type_ids() const
{
    return allowed_bus_type_ids;
}

void Device_hw_version::set_allowed_bus_type_ids(const QList<QString> &value)
{
    allowed_bus_type_ids = value;
}
QList<QString> Device_hw_version::get_allowed_local_container_ids() const
{
    return allowed_local_container_ids;
}

void Device_hw_version::set_allowed_local_container_ids(const QList<QString> &value)
{
    allowed_local_container_ids = value;
}

////////////////////////////////////////////////////////////////////////////

QString Tools::get_name() const
{
    return name;
}

void Tools::set_name(const QString &value)
{
    name = value;
}

QString Tools::get_description() const
{
    return description;
}

void Tools::set_description(const QString &value)
{
    description = value;
}

int Tools::get_tool_type_id() const
{
    return tool_type_id;
}

void Tools::set_tool_type_id(int value)
{
    tool_type_id = value;
}

////////////////////////////////////////////////////////////////////////////

}
