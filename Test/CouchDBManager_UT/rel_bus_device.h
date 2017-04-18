#ifndef REL_BUS_DEVICE_H
#define REL_BUS_DEVICE_H

#include <QObject>
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
// AUTHOR: .......... $Author: jmonasterio $
//
// REVISION: ........ $Revision: 1.1 $ ($Date: 2016/11/15 10:52:49 $)
//
// REFERENCE: ....... AD.37.91.3057
//
// CHANGES:
//

#include "bus.h"
#include "device.h"
#include "relationentity.h"


namespace Configuration {

    class Rel_bus_device : public CouchDBManager::RelationEntity
    {
        Q_OBJECT
        Q_PROPERTY(QString address READ get_address WRITE set_address)
        Q_PROPERTY(QString position READ get_position WRITE set_position)
        Q_PROPERTY(Bus *bus READ get_bus WRITE set_bus)
        Q_PROPERTY(Device *device READ get_device WRITE set_device)

    private:

        QString address;
        QString position;
        Bus *bus;
        Device *device;

    public:
        Q_INVOKABLE Rel_bus_device(QObject *parent = 0) : CouchDBManager::RelationEntity(parent),
            bus(0), device(0)
        {

            this->set_collection("bus_device");
        }

    public slots:

        QString get_address() const;
        void set_address(const QString value);

        QString get_position() const;
        void set_position(const QString value);

        Bus* get_bus() const;
        void set_bus(Bus *value);

        Device* get_device() const;
        void set_device(Device *value);

    };

    Q_DECLARE_METATYPE(Configuration::Rel_bus_device*)

}

#endif // REL_BUS_DEVICE_H
