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
// REVISION: ........ $Revision: 1.8 $ ($Date: 2016/11/30 10:59:13 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#include "baseobject.h"

#include <QVariant>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>


#include "rapidjson/document.h"

void CouchDBManager::BaseObject::read(const QJsonObject &json)
{
    qDebug() << ">" << Q_FUNC_INFO;

    const QMetaObject* meta_object = this->metaObject();

    qDebug() << "# Read JSON for: " << meta_object->className() << "Inherited from: " << this->staticMetaObject.className();

    for (int i = 0; i < meta_object->propertyCount(); ++i)
    {
        QMetaProperty meta_prop = meta_object->property(i);
        QString name = QString(meta_prop.name());

        if (name == "objectName")
        {
            continue;
        }

        QVariant::Type type = meta_prop.type();

        qDebug() << "# Property: " << name;

        if (json.contains(name))
        {
            QJsonValue json_value = json[name];

            if (json_value.isNull())
            {
                qDebug() << "# Property is NULL. Skipping!";

                continue;
            }

            QVariant value = json_value.toVariant();

            if (value.canConvert( type ))
            {
                value.convert(type);

                qDebug() << "# Converted value: " << value;

                meta_prop.write(this, value);
            }
            else if (meta_prop.typeName() == "QVariant")
            {
                qDebug() << "# Variant value: " << value;
                meta_prop.write(this, value);
            }
            else if ( QString( meta_prop.typeName() ).startsWith("QList") && json_value.isArray() )
            {
                QString prop_type_name = QString( meta_prop.typeName() );
                QString item_type_name = prop_type_name.replace("QList<", "").replace(">", "");
                int item_type_id = QMetaType::type(item_type_name.toUtf8());
                QJsonArray json_array = json_value.toArray();

                qDebug() << "# List value: " << json_array.toVariantList();

                if ( json_array.first().toVariant().canConvert(item_type_id) )
                {
                    meta_prop.write(this, json_array.toVariantList());
                }
                else
                {
                    qDebug() << "# List:" << meta_prop.typeName() << "Type" << item_type_name << "is registered:" << QMetaType::isRegistered(item_type_id);

                    const QMetaObject* item_meta_object = QMetaType::metaObjectForType(item_type_id);

                    qDebug() << "# meta_object: " << item_meta_object->className();

                    for (int i = 0; i < json_array.size(); ++i)
                    {
                        // VBG - 20161130: SE CAMBIA EL PARENT A this->parent() SEGÚN EL CORREO DE Jon Gurrutxaga del 23/11/2016
                        CouchDBManager::BaseObject* entity = qobject_cast<CouchDBManager::BaseObject*>( item_meta_object->newInstance( Q_ARG(QObject*, this->parent()) ) );

                        entity->read( json_array.at(i).toObject() );

                        qDebug() << "# Object: " << QVariant::fromValue<CouchDBManager::BaseObject*>(entity);

                        QMetaObject::invokeMethod(this, QString("add_" + name).toStdString().c_str(), Q_ARG(CouchDBManager::BaseObject*, entity) );
                    }
                }
            }
            else
            {
                qDebug() << "# Single BaseObject. Type" << meta_prop.typeName() << "is registered:" << QMetaType::isRegistered(meta_prop.type());

                int prop_type_id = QMetaType::type(meta_prop.typeName());
                const QMetaObject* prop_meta_object = QMetaType::metaObjectForType(prop_type_id);
                // VBG - 20161130: SE CAMBIA EL PARENT A this->parent() SEGÚN EL CORREO DE Jon Gurrutxaga del 23/11/2016
                CouchDBManager::BaseObject* prop_entity = qobject_cast<CouchDBManager::BaseObject*>( prop_meta_object->newInstance( Q_ARG(QObject*, this->parent()) ) );

                if (json_value.isNull())
                {
//                    qDebug() << "# Valor para" << meta_prop.name() << ": NULL";
                    prop_entity = NULL;
                }
                else
                {
                    prop_entity->read(json_value.toObject());
                }

                QVariant value = QVariant::fromValue<CouchDBManager::BaseObject*>(prop_entity);

                qDebug() << "# Valor para" << meta_prop.name() << ":" << value;

                meta_prop.write(this, value);
            }

        }
    }

    qDebug() << "<" << Q_FUNC_INFO;
}


QVariant CouchDBManager::BaseObject::rjValue2Variant(const rapidjson::Value &m_value) const
{
    if (m_value.IsBool()) return m_value.GetBool();
    if (m_value.IsInt()) return m_value.GetInt();
    if (m_value.IsUint()) return m_value.GetUint();
    if (m_value.IsUint64()) return m_value.GetUint64();
    if (m_value.IsDouble()) return m_value.GetDouble();
    if (m_value.IsString()) return m_value.GetString();

    //Es un mapa de objetos
//    if (m_value.IsObject()) {
//        QVariantMap map;
//        for (rapidjson::Value::ConstValueIterator p = m_value.Begin(); p != m_value.End(); ++p)
//            map.insert(p.key(), rjValue2Variant(p).value());
//    }


    //Es un array
    if (m_value.IsArray()) {
        QVariantList list;
        list.reserve(m_value.Size());
        for (rapidjson::SizeType j = 0; j < m_value.Size(); j++)
        {
            list.append(rjValue2Variant(m_value[j]));
        }

        return list;
    }

    return QVariant();
}

void CouchDBManager::BaseObject::read_rapidjson(const rapidjson::Value &json)
{
    qDebug() << ">" << Q_FUNC_INFO;

    const QMetaObject* meta_object = this->metaObject();

    qDebug() << "# Read JSON for: " << meta_object->className() << "Inherited from: " << this->staticMetaObject.className();

    for (int i = 0; i < meta_object->propertyCount(); ++i)
    {
        QMetaProperty meta_prop = meta_object->property(i);
        QString name = QString(meta_prop.name());
        std::string name_string = name.toStdString();

        if (name == "objectName")
        {
            continue;
        }

        QVariant::Type type = meta_prop.type();

        qDebug() << "# Property: " << name;


        if (json.HasMember(name_string.c_str()))
        {

            if (json[name_string.c_str()].IsNull())
            {
                qDebug() << "# Property is NULL. Skipping!";

                continue;
            }


            QVariant value = rjValue2Variant(json[name_string.c_str()]);

            if (value.canConvert( type ))
            {
                value.convert(type);

                qDebug() << "# Converted value: " << value;

                meta_prop.write(this, value);
            }
            else if (meta_prop.typeName() == "QVariant")
            {
                qDebug() << "# Variant value: " << value;
                meta_prop.write(this, value);
            }
            else if ( QString( meta_prop.typeName() ).startsWith("QList") && json[name_string.c_str()].IsArray() )
            {

                QString prop_type_name = QString( meta_prop.typeName() );
                QString item_type_name = prop_type_name.replace("QList<", "").replace(">", "");
                int item_type_id = QMetaType::type(item_type_name.toUtf8());
                QJsonArray json_array;// = json_value.toArray();

                QVariantList list;
                list.reserve(json[name_string.c_str()].Size());

                for (rapidjson::SizeType j = 0; j < json[name_string.c_str()].Size(); j++)
                {
                    list.append(rjValue2Variant(json[name_string.c_str()][j]));
                }


                //qDebug() << "# List value: " << json_array.toVariantList();

                if ( list.first().canConvert(item_type_id) )
                {
                    meta_prop.write(this, list);
                }
                else
                {
                    qDebug() << "# List:" << meta_prop.typeName() << "Type" << item_type_name << "is registered:" << QMetaType::isRegistered(item_type_id);

                    const QMetaObject* item_meta_object = QMetaType::metaObjectForType(item_type_id);

                    qDebug() << "# meta_object: " << item_meta_object->className();

                    for (int i = 0; i < json[name_string.c_str()].Size(); ++i)
                    {
                        // VBG - 20161130: SE CAMBIA EL PARENT A this->parent() SEGÚN EL CORREO DE Jon Gurrutxaga del 23/11/2016
                        CouchDBManager::BaseObject* entity = qobject_cast<CouchDBManager::BaseObject*>( item_meta_object->newInstance( Q_ARG(QObject*, this->parent()) ) );

                        //TODO
                        entity->read_rapidjson(json[name_string.c_str()][i]);

                        qDebug() << "# Object: " << QVariant::fromValue<CouchDBManager::BaseObject*>(entity);

                        QMetaObject::invokeMethod(this, QString("add_" + name).toStdString().c_str(), Q_ARG(CouchDBManager::BaseObject*, entity) );
                    }
                }
            }
            else
            {
                qDebug() << "# Single BaseObject. Type" << meta_prop.typeName() << "is registered:" << QMetaType::isRegistered(meta_prop.type());

                int prop_type_id = QMetaType::type(meta_prop.typeName());
                const QMetaObject* prop_meta_object = QMetaType::metaObjectForType(prop_type_id);
                // VBG - 20161130: SE CAMBIA EL PARENT A this->parent() SEGÚN EL CORREO DE Jon Gurrutxaga del 23/11/2016
                CouchDBManager::BaseObject* prop_entity = qobject_cast<CouchDBManager::BaseObject*>( prop_meta_object->newInstance( Q_ARG(QObject*, this->parent()) ) );

                if (json[name_string.c_str()].IsNull())
                {
//                    qDebug() << "# Valor para" << meta_prop.name() << ": NULL";
                    prop_entity = NULL;
                }
                else
                {
                    prop_entity->read_rapidjson(json[name_string.c_str()]);
                }

                QVariant value = QVariant::fromValue<CouchDBManager::BaseObject*>(prop_entity);

                qDebug() << "# Valor para" << meta_prop.name() << ":" << value;

                meta_prop.write(this, value);
            }

        }
    }

    qDebug() << "<" << Q_FUNC_INFO;
}

bool CouchDBManager::BaseObject::is_base_object(QObject *object) const
{
    qDebug() << ">" << Q_FUNC_INFO;

    CouchDBManager::BaseObject* e = qobject_cast<CouchDBManager::BaseObject*>(object);
    bool is_bo = false;

    if (e != NULL)
    {
        is_bo = true;
        qDebug() << "# Se puede convertir en BaseObject";

        const QMetaObject* meta = e->metaObject();

        for (int i = 0; i < meta->propertyCount(); i++)
        {
            if (meta->property(i).name() == "_id" || meta->property(i).name() == "_rev")
            {
                is_bo = false;
                qDebug() << "# No es un BaseObject, parece un BaseEntity";

                break;
            }
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return is_bo;
}


