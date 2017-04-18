//! @addtogroup CouchDBManager
//! @{
//! @file "basereference.h"
//! @brief Definición de la clase de referencia para incrustar en documentos.
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
// REVISION: ........ $Revision: 1.6 $ ($Date: 2016/12/05 13:15:26 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef BASEREFERENCE_H
#define BASEREFERENCE_H

#include "couchdbmanager_global.h"

#include <QObject>
#include <QString>

#include "baseobject.h"

/**
 * @brief El namespace CouchDBManager aisla las clases para evitar duplicidades.
 */
namespace CouchDBManager
{
    /**
     * @brief La clase BaseReference define una referencia para incrustar en un documento.
     * @author vbermudez@xi-tek.com
     * @date 2016/07/12
     */
    class COUCHDBMANAGERSHARED_EXPORT BaseReference : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString id READ get_id WRITE set_id)
        Q_PROPERTY(QString rev READ get_rev WRITE set_rev)
        Q_PROPERTY(int version READ get_version WRITE set_version)
        Q_PROPERTY(QString collection READ get_collection WRITE set_collection)

    private:
        QString id;
        QString rev;
        int version;
        QString collection;

    public:
        /**
          * @brief BaseReference Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE BaseReference(QObject *parent = 0) : CouchDBManager::BaseObject(parent) { }

        template <class T>
        static CouchDBManager::BaseReference* fromEntity(T* a_entity)
        {
            CouchDBManager::BaseReference* br = new CouchDBManager::BaseReference(a_entity->parent());
            const QMetaObject* meta = a_entity->metaObject();
            bool is_base_entity = false;
            bool is_versionable_entity = false;

            for (int i = 0, len = meta->methodCount(); i < len; i++) {
                if (meta->method(i).name() == "get_id" ||
                        meta->method(i).name() == "get_collection" ||
                        meta->method(i).name() == "is_base_entity")
                {
                    is_base_entity = true;
                }

                if (meta->method(i).name() == "get_collection")
                {
                    is_base_entity = true;
                }

                if (meta->method(i).name() == "get_version")
                {
                    is_versionable_entity = true;
                }
            }

            if (is_base_entity)
            {
                QString _id;
                QString _rev;
                QString collection;

                QMetaObject::invokeMethod(a_entity, "get_id", Q_RETURN_ARG(QString, _id) );
                QMetaObject::invokeMethod(a_entity, "get_rev", Q_RETURN_ARG(QString, _rev) );
                QMetaObject::invokeMethod(a_entity, "get_collection", Q_RETURN_ARG(QString, collection) );

                br->set_id( _id );
                br->set_rev( _rev );
                br->set_collection( collection );

                if (is_versionable_entity)
                {
                    int version;

                    QMetaObject::invokeMethod(a_entity, "get_version", Q_RETURN_ARG(int, version) );

                    br->set_version( version );
                }
                else
                {
                    br->set_version(-1);
                }
            }

            return br;
        }

    public slots:
        /**
         * @brief get_id Recupera el identificador único de la entidad.
         * @return QString con el identificador único de la entidad.
         */
        QString get_id() const { return this->id; }
        /**
         * @brief set_id Establece el identificador único de la entidad.
         * @param a_id Identificador único de la entidad.
         */
        void set_id(const QString& a_id) { this->id = a_id; }
        /**
         * @brief get_rev Recupera el identificador de revisión de la entidad.
         * @return QString con el identificador único de la entidad.
         */
        QString get_rev() const { return this->rev; }
        /**
         * @brief set_rev Establece el identificador de revisión de la entidad.
         * @param a_rev Identificador de revisión de la entidad.
         */
        void set_rev(const QString& a_rev) { this->rev = a_rev; }
        /**
         * @brief get_version Recupera la versión de la entidad.
         * @return int con la versión de la entidad.
         */
        int get_version() const { return this->version; }
        /**
         * @brief set_version Establece la versión de la entidad.
         * @param a_version Versión de la entidad.
         */
        void set_version(int a_version) { this->version = a_version; }
        /**
         * @brief get_collection Recupera la colección de la entidad.
         * @return QString con la colección de la entidad.
         */
        QString get_collection() const { return this->collection; }
        /**
         * @brief set_collection Establece la colección de la entidad.
         * @param a_collection La colección de la entidad.
         */
        void set_collection(const QString& a_collection) { this->collection = a_collection; }

        /**
         * @brief is_base_reference Determina si el objeto extiende a BaseReference.
         * @return true si el objeto deriva de BaseReference.
         */
        bool is_base_reference() const { return true; }
    };
}

Q_DECLARE_METATYPE(CouchDBManager::BaseReference*)

#endif // BASEREFERENCE_H
//! @}
