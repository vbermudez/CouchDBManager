//! @addtogroup CouchDBManager
//! @{
//! @file "versionableentity.h"
//! @brief Definición de la clase básica que define una entidad versionable.
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
// PROJECT: ......... AUNIA/DBMANAGER
//
// NAME: ............ $RCSFile: $
//
// AUTHOR: .......... $Author: jgurrutxaga $
//
// REVISION: ........ $Revision: 1.6 $ ($Date: 2016/10/03 07:11:59 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef VERSIONABLEENTITY_H
#define VERSIONABLEENTITY_H

#include "couchdbmanager_global.h"
#include "lockableentity.h"

#include <QObject>

namespace CouchDBManager
{
    class COUCHDBMANAGERSHARED_EXPORT VersionableEntity : public CouchDBManager::LockableEntity
    {
        Q_OBJECT
        Q_PROPERTY(int version READ get_version WRITE set_version)
        Q_PROPERTY(bool working READ get_working WRITE set_working)

    protected:
        int version;
        bool working;

    public:
        Q_INVOKABLE VersionableEntity(QObject *parent = 0) : CouchDBManager::LockableEntity(parent) {
            this->version = 0;
            this->working = true;
        }

    public slots:
        int get_version() const;
        void set_version(int a_version);
        bool get_working() const;
        void set_working(bool a_working);

        bool is_versionable_entity() const { return true; }
    };
}

Q_DECLARE_METATYPE(CouchDBManager::VersionableEntity*)

#endif // VERSIONABLEENTITY_H
//! @}
