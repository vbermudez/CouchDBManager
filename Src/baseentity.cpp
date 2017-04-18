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
// REVISION: ........ $Revision: 1.3 $ ($Date: 2016/08/09 10:53:39 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#include "baseentity.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QVariant>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

QString CouchDBManager::BaseEntity::get_id() const
{
    return this->_id;
}

void CouchDBManager::BaseEntity::set_id(const QString& a_id)
{
    this->_id = a_id;
}

QString CouchDBManager::BaseEntity::get_rev() const
{
    return this->_rev;
}

void CouchDBManager::BaseEntity::set_rev(const QString& a_rev)
{
    this->_rev = a_rev;
}

QString CouchDBManager::BaseEntity::get_collection() const
{
    return this->collection;
}

void CouchDBManager::BaseEntity::set_collection(const QString& a_collection)
{
    this->collection = a_collection;
}

QString CouchDBManager::BaseEntity::get_name() const
{
    return this->name;
}

void CouchDBManager::BaseEntity::set_name(const QString& a_name)
{
    this->name = a_name;
}

QString CouchDBManager::BaseEntity::get_description() const
{
    return this->description;
}

void CouchDBManager::BaseEntity::set_description(const QString& a_description)
{
    this->description = a_description;
}

bool CouchDBManager::BaseEntity::is_base_entity(QObject *object) const
{
    qDebug() << ">" << Q_FUNC_INFO;

    CouchDBManager::BaseEntity* e = qobject_cast<CouchDBManager::BaseEntity*>(object);
    bool is_be = false;

    if (e != NULL)
    {
        qDebug() << "# Se puede convertir en BaseEntity";

        const QMetaObject* meta = e->metaObject();

        for (int i = 0; i < meta->propertyCount(); i++)
        {
            if (meta->property(i).name() == "_id" || meta->property(i).name() == "_rev")
            {
                is_be = true;
                qDebug() << "# Es un objeto BaseEntity.";

                break;
            }
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return is_be;
}

