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
// REVISION: ........ $Revision: 1.2 $ ($Date: 2016/10/03 07:12:00 $)
//
// REFERENCE: ....... AD.37.91.3057
//
// CHANGES:
//


#ifndef TYPES_H
#define TYPES_H

#include <QList>
#include <QVariant>

//#include "libconfiguration_global.h"

//#include "rel_physical_ut_car.h"
#include "versionableentity.h"
#include "BaseObject.h"

namespace Configuration {

    class Variable_types;
    class Io_card_types;
    class Bus_types;
    class System_types;
    class Device_hw_types;
    class Device_hw_version;
    class Tools;

    class Types : public CouchDBManager::BaseEntity
    {
        Q_OBJECT
        Q_PROPERTY(QList<Configuration::Variable_types*> variable_types READ get_variable_types WRITE set_variable_types)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> distributed_container_types READ get_distributed_container_types WRITE set_distributed_container_types)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> distributed_container_rs485_sub_types READ get_distributed_container_rs485_sub_types WRITE set_distributed_container_rs485_sub_types)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> distributed_container_can_sub_types READ get_distributed_container_can_sub_types WRITE set_distributed_container_can_sub_types)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> local_container_types READ get_local_container_types WRITE set_local_container_types)
        Q_PROPERTY(QList<Configuration::Io_card_types*> io_card_types READ get_io_card_types WRITE set_io_card_types)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> digital_channel_inversion_mode_types READ get_digital_channel_inversion_mode_types WRITE set_digital_channel_inversion_mode_types)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> task_types READ get_task_types WRITE set_task_types)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> publishing_version_types READ get_publishing_version_types WRITE set_publishing_version_types)
        Q_PROPERTY(QList<Configuration::Bus_types*> bus_types READ get_bus_types WRITE set_bus_types)
        Q_PROPERTY(QList<Configuration::System_types*> system_types READ get_system_types WRITE set_system_types)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> device_types READ get_device_types WRITE set_device_types)
        Q_PROPERTY(QList<Configuration::Device_hw_types*> device_hw_types READ get_device_hw_types WRITE set_device_hw_types)
        Q_PROPERTY(QList<Configuration::Device_hw_version*> device_hw_version READ get_device_hw_version WRITE set_device_hw_version)
        Q_PROPERTY(QList<Configuration::Tools*> tools READ get_tools WRITE set_tools)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> tools_types READ get_tools_types WRITE set_tools_types)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> unit_types READ get_unit_types WRITE set_unit_types)
        //
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> geometric_config_types READ get_geometric_config_types WRITE set_geometric_config_types)


    private:
        QList<Configuration::Variable_types*> variable_types;
        QList<CouchDBManager::BaseObject*> distributed_container_types;
        QList<CouchDBManager::BaseObject*> distributed_container_rs485_sub_types;
        QList<CouchDBManager::BaseObject*> distributed_container_can_sub_types;
        QList<CouchDBManager::BaseObject*> local_container_types;
        QList<Configuration::Io_card_types*> io_card_types;
        QList<CouchDBManager::BaseObject*> digital_channel_inversion_mode_types;
        QList<CouchDBManager::BaseObject*> task_types;
        QList<CouchDBManager::BaseObject*> publishing_version_types;
        QList<Configuration::Bus_types*> bus_types;
        QList<Configuration::System_types*> system_types;
        QList<CouchDBManager::BaseObject*> device_types;
        QList<Configuration::Device_hw_types*> device_hw_types;
        QList<Configuration::Device_hw_version*> device_hw_version;
        QList<Configuration::Tools*> tools;
        QList<CouchDBManager::BaseObject*> tools_types;
        QList<CouchDBManager::BaseObject*> unit_types;
        QList<CouchDBManager::BaseObject*> geometric_config_types;

    public:
        Q_INVOKABLE Types(QObject *parent = 0) : CouchDBManager::BaseEntity(parent) {


            this->set_collection("conf_types");
        }

    public slots:

        QList<Configuration::Variable_types*> get_variable_types() const;
        void set_variable_types(const QList<Configuration::Variable_types *> &value);
        void add_variable_types(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_distributed_container_types() const;
        void set_distributed_container_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_distributed_container_types(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_distributed_container_rs485_sub_types() const;
        void set_distributed_container_rs485_sub_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_distributed_container_rs485_sub_types(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_distributed_container_can_sub_types() const;
        void set_distributed_container_can_sub_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_distributed_container_can_sub_types(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_local_container_types() const;
        void set_local_container_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_local_container_types(CouchDBManager::BaseObject* value);

        QList<Configuration::Io_card_types*> get_io_card_types() const;
        void set_io_card_types(const QList<Configuration::Io_card_types *> &value);
        void add_io_card_types(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_digital_channel_inversion_mode_types() const;
        void set_digital_channel_inversion_mode_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_digital_channel_inversion_mode_types(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_task_types() const;
        void set_task_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_task_types(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_publishing_version_types() const;
        void set_publishing_version_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_publishing_version_types(CouchDBManager::BaseObject* value);

        QList<Configuration::Bus_types*> get_bus_types() const;
        void set_bus_types(const QList<Configuration::Bus_types *> &value);
        void add_bus_types(CouchDBManager::BaseObject* value);

        QList<Configuration::System_types*> get_system_types() const;
        void set_system_types(const QList<Configuration::System_types*>& value);
        void add_system_types(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_device_types() const;
        void set_device_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_device_types(CouchDBManager::BaseObject* value);

        QList<Configuration::Device_hw_types*> get_device_hw_types() const;
        void set_device_hw_types(const QList<Configuration::Device_hw_types *> &value);
        void add_device_hw_types(CouchDBManager::BaseObject* value);

        QList<Configuration::Device_hw_version*> get_device_hw_version() const;
        void set_device_hw_version(const QList<Configuration::Device_hw_version*>& value);
        void add_device_hw_version(CouchDBManager::BaseObject* value);

        QList<Configuration::Tools*> get_tools() const;
        void set_tools(const QList<Tools *> &value);
        void add_tools(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_tools_types() const;
        void set_tools_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_tools_types(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_unit_types() const;
        void set_unit_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_unit_types(CouchDBManager::BaseObject* value);

        QList<CouchDBManager::BaseObject*> get_geometric_config_types() const;
        void set_geometric_config_types(const QList<CouchDBManager::BaseObject*>& value);
        void add_geometric_config_types(CouchDBManager::BaseObject* value);


    };

    Q_DECLARE_METATYPE(Configuration::Types*)


    class  Variable_types : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ get_name WRITE set_name)
        Q_PROPERTY(QString description READ get_description WRITE set_description)
        Q_PROPERTY(int size READ get_size WRITE set_size)
        Q_PROPERTY(QString c_type READ get_c_type WRITE set_c_type)

    private:

        QString name;
        QString description;
        int size;
        QString c_type;

    public:
        Q_INVOKABLE Variable_types(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {

        }

    public slots:

        QString get_name() const;
        void set_name(const QString &value);

        QString get_description() const;
        void set_description(const QString &value);

        int get_size() const;
        void set_size(int value);

        QString get_c_type() const;
        void set_c_type(const QString &value);

    };

    Q_DECLARE_METATYPE(Configuration::Variable_types*)


    class  Io_card_types : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ get_name WRITE set_name)
        Q_PROPERTY(QString description READ get_description WRITE set_description)
        Q_PROPERTY(QString digital_channel_inversion_mode_type_id  READ get_digital_channel_inversion_mode_type_id WRITE set_digital_channel_inversion_mode_type_id)
        Q_PROPERTY(QStringList variable_types_names READ get_variable_types_names WRITE set_variable_types_names)


    private:

        QString name;
        QString description;
        QString digital_channel_inversion_mode_type_id;
        QStringList variable_types_names;

    public:
        Q_INVOKABLE Io_card_types(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {

        }

    public slots:

        QString get_name() const;
        void set_name(const QString &value);

        QString get_description() const;
        void set_description(const QString &value);

        QString get_digital_channel_inversion_mode_type_id() const;
        void set_digital_channel_inversion_mode_type_id(const QString &value);

        QStringList get_variable_types_names() const;
        void set_variable_types_names(const QStringList &value);
        void add_variable_types_names(const QString &value);

    };

    Q_DECLARE_METATYPE(Configuration::Io_card_types*)

    class  System_types : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ get_name WRITE set_name)
        Q_PROPERTY(QString description READ get_description WRITE set_description)
        Q_PROPERTY(QList<CouchDBManager::BaseObject*> sub_system_types READ get_sub_system_types WRITE set_sub_system_types)

    private:

        QString name;
        QString description;
        int show_order;
        QList<CouchDBManager::BaseObject*> sub_system_types;

    public:
        Q_INVOKABLE System_types(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {

        }

    public slots:

        QString get_name() const;
        void set_name(const QString &value);

        QString get_description() const;
        void set_description(const QString &value);

        QList<CouchDBManager::BaseObject *> get_sub_system_types() const;
        void set_sub_system_types(const QList<CouchDBManager::BaseObject *> &value);

    };

    Q_DECLARE_METATYPE(Configuration::System_types*)

    class  Bus_types : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ get_name WRITE set_name)
        Q_PROPERTY(QString description READ get_description WRITE set_description)
        Q_PROPERTY(int show_order READ get_show_order WRITE set_show_order)
        Q_PROPERTY(QList<Configuration::System_types*> system_types READ get_system_types WRITE set_system_types)
        Q_PROPERTY(QList<Configuration::Tools*> tools READ get_tools WRITE set_tools)

    private:

        QString name;
        QString description;
        int show_order;
        QList<Configuration::System_types*> system_types;
        QList<Configuration::Tools*> tools;

    public:
        Q_INVOKABLE Bus_types(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {

        }


    public slots:

        QString get_name() const;
        void set_name(const QString &value);

        QString get_description() const;
        void set_description(const QString &value);

        int get_show_order() const;
        void set_show_order(int value);

        QList<Configuration::System_types*> get_system_types() const;
        void set_system_types(const QList<Configuration::System_types*>& value);
        void add_system_types(CouchDBManager::BaseObject* value);

        QList<Configuration::Tools *> get_tools() const;
        void set_tools(const QList<Configuration::Tools *> &value);
        void add_tools(CouchDBManager::BaseObject* value);

    };

    Q_DECLARE_METATYPE(Configuration::Bus_types*)


    class  Device_hw_types : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ get_name WRITE set_name)
        Q_PROPERTY(QString description READ get_description WRITE set_description)
        Q_PROPERTY(int device_type_id READ get_device_type_id WRITE set_device_type_id)

    private:

        QString name;
        QString description;
        int device_type_id;

    public:

        Q_INVOKABLE Device_hw_types(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {

        }

    public slots:

        QString get_name() const;
        void set_name(const QString &value);

        QString get_description() const;
        void set_description(const QString &value);

        int get_device_type_id() const;
        void set_device_type_id(int value);

    };

    Q_DECLARE_METATYPE(Configuration::Device_hw_types*)

    class  Device_hw_version : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ get_name WRITE set_name)
        Q_PROPERTY(QString description READ get_description WRITE set_description)
        Q_PROPERTY(int device_hw_type_id READ get_device_hw_type_id WRITE set_device_hw_type_id)
        Q_PROPERTY(QList<Configuration::Tools*> tools READ get_tools WRITE set_tools)
        Q_PROPERTY(QList<QString> allowed_local_container_ids READ get_allowed_local_container_ids WRITE set_allowed_local_container_ids)
        Q_PROPERTY(QList<QString> io_card_type_ids READ get_io_card_type_ids WRITE set_io_card_type_ids)
        Q_PROPERTY(QList<QString> allowed_bus_type_ids READ get_allowed_bus_type_ids WRITE set_allowed_bus_type_ids)

    private:

        QString name;
        QString description;
        int device_hw_type_id;
        QList<Configuration::Tools*> tools;
        QList<QString> allowed_local_container_ids;
        QList<QString> io_card_type_ids;
        QList<QString> allowed_bus_type_ids;

    public:

        Q_INVOKABLE Device_hw_version(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {

        }

    public slots:

        QString get_name() const;
        void set_name(const QString &value);

        QString get_description() const;
        void set_description(const QString &value);

        int get_device_hw_type_id() const;
        void set_device_hw_type_id(int value);

        QList<Configuration::Tools *> get_tools() const;
        void set_tools(const QList<Configuration::Tools *> &value);
        void add_tools(CouchDBManager::BaseObject* value);

        QList<QString> get_allowed_local_container_ids() const;
        void set_allowed_local_container_ids(const QList<QString> &value);

        QList<QString> get_io_card_type_ids() const;
        void set_io_card_type_ids(const QList<QString> &value);

        QList<QString> get_allowed_bus_type_ids() const;
        void set_allowed_bus_type_ids(const QList<QString> &value);

    };

    Q_DECLARE_METATYPE(Configuration::Device_hw_version*)

    class  Tools : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ get_name WRITE set_name)
        Q_PROPERTY(QString description READ get_description WRITE set_description)
        Q_PROPERTY(int tool_type_id READ get_tool_type_id WRITE set_tool_type_id)

    private:

        QString name;
        QString description;
        int tool_type_id;

    public:
        Q_INVOKABLE Tools(QObject *parent = 0) : CouchDBManager::BaseObject(parent) {

        }


    public slots:

        QString get_name() const;
        void set_name(const QString &value);

        QString get_description() const;
        void set_description(const QString &value);

        int get_tool_type_id() const;
        void set_tool_type_id(int value);

    };

    Q_DECLARE_METATYPE(Configuration::Tools*)

}

#endif // TYPES_H
