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
#include "rel_bus_device.h"


namespace Configuration
{
    QString Rel_bus_device::get_address() const
    {
        return address;
    }

    void Rel_bus_device::set_address(const QString value)
    {
        address = value;
    }
    QString Rel_bus_device::get_position() const
    {
        return position;
    }

    void Rel_bus_device::set_position(const QString value)
    {
        position = value;
    }
    Bus* Rel_bus_device::get_bus() const
    {
        return bus;
    }

    void Rel_bus_device::set_bus(Bus *value)
    {
        bus = value;
    }


    void Rel_bus_device::set_device(Device *value)
    {
        device = value;
    }

    Device* Rel_bus_device::get_device() const
    {
        return device;
    }
}
