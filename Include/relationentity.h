//! @addtogroup CouchDBManager
//! @{
//! @file "relationentity.h"
//! @brief Definición de la clase básica que define una relación.
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
// REVISION: ........ $Revision: 1.2 $ ($Date: 2016/10/03 07:11:59 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef RELATIONENTITY_H
#define RELATIONENTITY_H

#include "couchdbmanager_global.h"

#include <QObject>
#include <QJsonObject>

#include "lockableentity.h"

/**
 * @brief El namespace CouchDBManager aisla las clases para evitar duplicidades.
 */
namespace CouchDBManager
{
    /**
     * @brief La clase RelationEntity define la relación entre entidades para AUNIA.
     * @author vbermudez@xi-tek.com
     * @date 2016/06/15
     */
    class COUCHDBMANAGERSHARED_EXPORT RelationEntity : public CouchDBManager::LockableEntity
    {
        Q_OBJECT
        Q_PROPERTY(bool is_virtual READ get_is_virtual WRITE set_is_virtual)

    protected:
        bool is_virtual;
        
    public:
        /**
          * @brief RelationEntity Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE RelationEntity(QObject *parent = 0) : CouchDBManager::LockableEntity(parent)
        {
            is_virtual = false;
        }

    public slots:
        /**
         * @brief write Escribe la entidad como objeto JSON.
         * @param json Objeto JSON.
         */
//        void write(QJsonObject &json) const;

        bool is_relation_entity(QObject* object) const;

        bool get_is_virtual();
        void set_is_virtual(bool _virtual);
    };
}

Q_DECLARE_METATYPE(CouchDBManager::RelationEntity*)

#endif // RELATIONENTITY_H
//! @}
