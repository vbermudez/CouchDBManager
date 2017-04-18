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
// REVISION: ........ $Revision: 1.1 $ ($Date: 2016/12/20 09:01:16 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef BASEREFERENCESIMPLE_H
#define BASEREFERENCESIMPLE_H

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
     * @brief La clase BaseReferenceSimple define una referencia para incrustar en un documento.
     * @author josetxo.monasterio@altran.com
     * @date 2016/12/20
     */
    class COUCHDBMANAGERSHARED_EXPORT BaseReferenceSimple : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString id READ get_id WRITE set_id)

    private:
        QString id;

    public:
        /**
          * @brief BaseReferenceSimple Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE BaseReferenceSimple(QObject *parent = 0) : CouchDBManager::BaseObject(parent) { }

        template <class T>
        static CouchDBManager::BaseReferenceSimple* fromEntity(T* a_entity)
        {
            CouchDBManager::BaseReferenceSimple* br = new CouchDBManager::BaseReferenceSimple(a_entity->parent());
            const QMetaObject* meta = a_entity->metaObject();
            bool is_base_entity = false;
            bool is_versionable_entity = false;
            bool is_relation_entity = false;

            for (int i = 0, len = meta->methodCount(); i < len; i++) {
                if (meta->method(i).name() == "get_id")
                {
                    is_base_entity = true;
                }

            }

            if (is_base_entity)
            {
                QString _id;

                QMetaObject::invokeMethod(a_entity, "get_id", Q_RETURN_ARG(QString, _id) );

                br->set_id( _id );

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
         * @brief is_base_reference Determina si el objeto extiende a BaseReferenceSimple.
         * @return true si el objeto deriva de BaseReferenceSimple.
         */
        bool is_base_reference_simple() const { return true; }

    };
}

Q_DECLARE_METATYPE(CouchDBManager::BaseReferenceSimple*)

#endif // BASEREFERENCESIMPLE_H
//! @}
