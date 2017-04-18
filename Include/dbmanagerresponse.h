//! @addtogroup CouchDBManager
//! @{
//! @file "dbmanagerresponse.h"
//! @brief Definición de la clase básica que define una entidad.
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
// REVISION: ........ $Revision: 1.3 $ ($Date: 2016/10/03 07:11:59 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef DBMANAGERRESPONSE_H
#define DBMANAGERRESPONSE_H

#include "couchdbmanager_global.h"

#include <QObject>

/**
 * @brief El namespace CouchDBManager aisla las clases para evitar duplicidades.
 */
namespace CouchDBManager
{
    /**
     * @brief La clase DBManagerResponse define la respuesta básica de las operaciones del DBManager.
     * @author vbermudez@xi-tek.com
     * @date 2016/05/31
     */
    class COUCHDBMANAGERSHARED_EXPORT DBManagerResponse : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(bool went_ok READ get_went_ok WRITE set_went_ok)
        Q_PROPERTY(QString response READ get_response WRITE set_response)

    private:
        bool went_ok;
        QString response;

    public:
        /**
          * @brief DBManagerResponse Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE DBManagerResponse(QObject *parent = 0) : QObject(parent) { }
        /**
         * @brief DBManagerResponse Constructor de copia. No utilizar.
         * @param a_entity DBManagerResponse original.
         */
        DBManagerResponse(const DBManagerResponse& a_response, QObject *parent = 0) : QObject(parent) {
            this->went_ok = a_response.went_ok;
            this->response = a_response.response;
        }
        /**
         * @brief ~DBManagerResponse Destructor
         */
        virtual ~DBManagerResponse() {}

    public slots:
        bool get_went_ok() const;
        void set_went_ok(bool value);
        QString get_response() const;
        void set_response(const QString &value);

    };
}

Q_DECLARE_METATYPE(CouchDBManager::DBManagerResponse*)

#endif // DBMANAGERRESPONSE_H
//! @}
