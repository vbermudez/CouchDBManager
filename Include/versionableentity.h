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
#include "baseentity.h"

#include <QObject>

namespace CouchDBManager
{
    class COUCHDBMANAGERSHARED_EXPORT VersionableEntity : public CouchDBManager::BaseEntity
    {
        Q_OBJECT
        Q_PROPERTY(int version READ get_version WRITE set_version)
        Q_PROPERTY(bool working READ get_working WRITE set_working)
        Q_PROPERTY(QString virtual_type READ get_virtual_type WRITE set_virtual_type)
        Q_PROPERTY(bool locked READ get_locked WRITE set_locked)
        Q_PROPERTY(QString locked_by READ get_locked_by WRITE set_locked_by)

    protected:
        int version;
        bool working;
        bool locked;
        QString locked_by;
        QString virtual_type;

    public:
        Q_INVOKABLE VersionableEntity(QObject *parent = 0) : CouchDBManager::BaseEntity(parent) {
            this->version = 0;
            this->working = true;
            this->locked = false;
        }

    public slots:
        int get_version() const;
        void set_version(int a_version);
        bool get_working() const;
        void set_working(bool a_working);

        void set_locked(bool a_locked);
        bool get_locked() const;

        void set_locked_by(const QString& a_locked);
        QString get_locked_by() const;

        QString get_virtual_type() const;
        void set_virtual_type(const QString &value);

        bool is_versionable_entity() const { return true; }
    };
}

Q_DECLARE_METATYPE(CouchDBManager::VersionableEntity*)

#endif // VERSIONABLEENTITY_H
//! @}
