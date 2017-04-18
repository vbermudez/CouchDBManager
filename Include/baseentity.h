//! @addtogroup CouchDBManager
//! @{
//! @file "baseentity.h"
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
// REVISION: ........ $Revision: 1.7 $ ($Date: 2017/01/19 20:47:41 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "couchdbmanager_global.h"

#include <QObject>
#include <QString>
#include <QJsonObject>

#include "baseobject.h"

/**
 * @brief El namespace CouchDBManager aisla las clases para evitar duplicidades.
 */
namespace CouchDBManager
{
    /**
     * @brief La clase BaseEntity define la entidad básica para AUNIA.
     * @author vbermudez@xi-tek.com
     * @date 2016/05/31
     */
    class COUCHDBMANAGERSHARED_EXPORT BaseEntity : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString _id READ get_id WRITE set_id)
        Q_PROPERTY(QString _rev READ get_rev WRITE set_rev)
        Q_PROPERTY(QString collection READ get_collection WRITE set_collection)
        Q_PROPERTY(QString name READ get_name WRITE set_name)
        Q_PROPERTY(QString description READ get_description WRITE set_description)

    protected:
        QString _id;
        QString _rev;
        QString collection;
        QString name;
        QString description;

        /**
         * @brief test_inner_object Comprueba que un objeto tiene una propiedad con un valor específico.
         * @param name Nombre de la propiedad.
         * @param value Valor de la propiedad.
         * @return true si el objeto tiene la propiedad y, ésta, el valor especificado.
         */
        template <class T>
        bool test_inner_object(T* object, const QString& name, QVariant value)
        {
            const QMetaObject* meta = object->metaObject();

            for (int i = 0, len = meta->propertyCount(); i < len; i++)
            {
                QMetaProperty prop = meta->property(i);

                if (prop.name() == name && prop.read(object) == value )
                {
                    return true;
                }
            }

            return false;
        }

    public:
        /**
          * @brief BaseEntity Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE BaseEntity(QObject *parent = 0) : CouchDBManager::BaseObject(parent) { }

        /**
         * @brief get_inner_object Recupera un relación interna.
         * @param id Identificador por el que buscar.
         * @return NULL o un objeto que deriva de BaseObject.
         */
        template <class T>
        T* get_inner_object(int id)
        {
            return this->get_inner_object("id", QVariant(id));
        }

        /**
         * @brief get_inner_object Recupera un relación interna.
         * @param name Nombre de la propiedad por la que buscar.
         * @param value Valor a buscar.
         * @return NULL o un objeto que deriva de BaseObject.
         */
        template <class T>
        T* get_inner_object(const QString& name, const QVariant& value)
        {
            const QMetaObject* meta = this->metaObject();
            T t;
            QString type_name = QString(t.metaObject()->className());

            for (int i = 0, len = meta->propertyCount(); i < len; i++)
            {
                QMetaProperty prop = meta->property(i);
                QString prop_name = QString(prop.name());
                QString prop_type_name = QString(prop.typeName());

                if ( prop_type_name.startsWith("QList") && prop_type_name.contains(type_name) )
                {
                    qDebug() << "# Property" << prop_name << "is a QList";

                    QVariant prop_value = this->property(prop.name());

                    foreach(const QVariant &item, this->toList(prop_value))
                    {
                        T* object = qvariant_cast<T*>(item);

                        if ( this->test_inner_object(object, name, value) )
                        {
                            return object;
                        }
                    }
                }
                else if (prop_type_name == type_name)
                {
                    qDebug() << "# Property" << prop_name << "is an object";

                    T* object = qvariant_cast<T*>( prop.read(this) );

                    if ( this->test_inner_object(object, name, value) )
                    {
                        return object;
                    }
                }
            }

            return NULL;
        }

    public slots:
        /**
         * @brief get_id Recupera el identificador único de la entidad.
         * @return QString con el identificador único de la entidad.
         */
        QString get_id() const;
        /**
         * @brief set_id Establece el identificador único de la entidad.
         * @param a_id Identificador único de la entidad.
         */
        void set_id(const QString& a_id);
        QString get_rev() const;
        void set_rev(const QString& a_rev);
        QString get_collection() const;
        void set_collection(const QString& a_collection);
        QString get_name() const;
        void set_name(const QString& a_name);
        QString get_description() const;
        void set_description(const QString& a_description);

        bool is_base_entity(QObject* object) const;

        /**
         * @brief is_valid Determina si el objeto es válido.
         * @return true si el objeto ha sido inicializado.
         */
        bool is_valid() const { return (!_id.isNull() && !_id.isEmpty()); }

    };
}

Q_DECLARE_METATYPE(CouchDBManager::BaseEntity*)

#endif // BASEENTITY_H
//! @}
