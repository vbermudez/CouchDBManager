//! @addtogroup CouchDBManager
//! @{
//! @file "usercontext.h"
//! @brief Definición de la clase que define el contexto del usuario.
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
// AUTHOR: .......... $Author: jmonasterio $
//
// REVISION: ........ $Revision: 1.4 $ ($Date: 2016/11/15 10:52:48 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef USERCONTEXT_H
#define USERCONTEXT_H

#include "couchdbmanager_global.h"

#include <QObject>
#include <QString>
#include <QStringList>

/**
 * @brief El namespace CouchDBManager aisla las clases para evitar duplicidades.
 */
namespace CouchDBManager
{
    /**
     * @brief La clase UserContext define el contexto del usuario.
     * @author vbermudez@xi-tek.com
     * @date 2016/06/28
     */
    class COUCHDBMANAGERSHARED_EXPORT UserContext : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(Info info READ get_info WRITE set_info)
        Q_PROPERTY(bool ok READ get_ok WRITE set_ok)
        Q_PROPERTY(UserCtx userCtx READ get_userCtx WRITE set_userCtx)

    public:
        struct Info
        {
            QString authenticated;
            QString authentication_db;
            QStringList authentication_handlers;
        };

        struct UserCtx
        {
            QString name;
            QStringList roles;
        };

        /**
          * @brief UserContext Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE UserContext(QObject *parent = 0) : QObject(parent) { }

        /**
         * @brief ~UserContext Destructor
         */
        virtual ~UserContext() {}

    private:
        Info info;
        bool ok;
        UserCtx userCtx;

    public slots:
        void set_info(const CouchDBManager::UserContext::Info& a_info);
        CouchDBManager::UserContext::Info get_info() const;
        void set_ok(bool a_ok);
        bool get_ok() const;
        void set_userCtx(const CouchDBManager::UserContext::UserCtx a_userCtx);
        CouchDBManager::UserContext::UserCtx get_userCtx() const;
    };
}

Q_DECLARE_METATYPE(CouchDBManager::UserContext*)

#endif // USERCONTEXT_H
//! @}
