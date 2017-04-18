//! @addtogroup CouchDBManager
//! @{
//! @file "baseobject.h"
//! @brief Definición de la clase básica que define un objeto.
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
// REVISION: ........ $Revision: 1.5 $ ($Date: 2016/11/15 10:52:48 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include "couchdbmanager_global.h"

#include <QObject>
#include <QJsonObject>
#include <QMetaObject>
#include <QMetaProperty>
#include <QDebug>

/**
 * @brief El namespace CouchDBManager aisla las clases para evitar duplicidades.
 */
namespace CouchDBManager
{
    /**
     * @brief La clase BaseObject define el objeto básico para AUNIA.
     * @author vbermudez@xi-tek.com
     * @date 2016/05/31
     */
    class COUCHDBMANAGERSHARED_EXPORT BaseObject : public QObject
    {
        Q_OBJECT

    public:
        /**
          * @brief BaseObject Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE BaseObject(QObject *parent = 0) : QObject(parent) { }
        /**
         * @brief BaseObject Constructor de copia.
         * @param a_entity BaseObject Original.
         */
        BaseObject(const BaseObject& a_entity) {
            const QMetaObject* meta = a_entity.metaObject();

            for (int i = 0, len = meta->propertyCount(); i < len; i++)
            {
                QMetaProperty prop = meta->property(i);

                if (prop.name() == "objectName")
                {
                    continue;
                }

                this->property(prop.name()) = a_entity.property(prop.name());
            }
        }
        /**
         * @brief BaseObject Asginacion por copia.
         * @param a_entity BaseObject Original.
         */
        BaseObject& operator=(const BaseObject& a_entity) {
            const QMetaObject* meta = a_entity.metaObject();

            for (int i = 0, len = meta->propertyCount(); i < len; i++)
            {
                QMetaProperty prop = meta->property(i);

                if (prop.name() == "objectName")
                {
                    continue;
                }

                this->property(prop.name()) = a_entity.property(prop.name());
            }

            return *this;
        }
        /**
         * @brief ~BaseObject Destructor
         */
        virtual ~BaseObject() { }

        /**
         * @brief copy_from Copia los valores coicidentes de otro objeto.
         * @param other El objeto del caul copiar.
         */
        template <class T>
        void copy_from(T* other)
        {
            const QMetaObject* meta = other->metaObject();

            for (int i = 0, len = meta->propertyCount(); i < len; i++)
            {
                QMetaProperty prop = meta->property(i);

                if (prop.name() == "objectName")
                {
                    continue;
                }

                QVariant local_prop = this->property(prop.name());

                if (local_prop.isValid())
                {
                    this->property(prop.name()) = other->property(prop.name());
                }
            }
        }

    public slots:
        /**
         * @brief read Lee un objeto JSON y carga la entidad.
         * @param json Objeto JSON.
         */
        void read(const QJsonObject &json);
        /**
         * @brief write Escribe la entidad como objeto JSON.
         * @param json Objeto JSON.
         */
//        void write(QJsonObject &json) const;

        bool is_base_object(QObject* object) const;

        QList<QVariant> toList(QVariant list) const
        {
            return list.value<QList<QVariant>>();
        }
    };
}

Q_DECLARE_METATYPE(CouchDBManager::BaseObject*)

#endif // BASEOBJECT_H
//! @}
