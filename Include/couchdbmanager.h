//! @addtogroup CouchDBManager
//! @{
//! @file "couchdbmanager.h"
//! @brief Definición de la clase de acceso a CouchDB.
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
// REVISION: ........ $Revision: 1.24 $ ($Date: 2017/02/26 23:55:25 $)
//
// REFERENCE: .......
//
// CHANGES:
//
// $Log: couchdbmanager.h,v $
// Revision 1.24  2017/02/26 23:55:25  jgurrutxaga
// Corregir la gestión del timeout y la terminación de la petición al servidor. Liberar recursos en caso de timeout.
//
// Revision 1.23  2017/02/24 09:37:30  jgurrutxaga
// Disable "read timeout" when readyRead signal is received from QNetworkReply
//
// Revision 1.22  2017/02/22 15:32:36  jgurrutxaga
// Error en la gestión del timeout provoca que la respuesta del servidor quede entrecortada
//
// Revision 1.21  2017/01/19 20:47:41  jgurrutxaga
// Uso de referencias en foreach
//
// Revision 1.20  2016/12/20 09:01:16  jmonasterio
// - BaseReferenceSimple added
//
// Revision 1.19  2016/12/05 13:15:26  jmonasterio
// - Victor updates: Replication service
//
// Revision 1.18  2016/11/30 10:59:12  jmonasterio
// - Constructor parent modified
//
// Revision 1.17  2016/11/17 14:57:54  jmonasterio
// - Victor fixes
//
// Revision 1.16  2016/11/15 10:52:48  jmonasterio
// - Victor updates
//
// Revision 1.15  2016/11/10 11:03:28  jgurrutxaga
// Añadir función list_users
//
// Revision 1.14  2016/10/03 07:11:59  jgurrutxaga
// -Añadir la función registerTypes()al API de CouchDBManager para el registro de las entidades.
// -Añadir el password al guradar los datos de un usuario y evitar la lectura de este campo.
//
// Revision 1.13  2016/09/27 15:51:36  jgurrutxaga
// - Mantener la sesión viva.
// - Modificar árbol de objetos derivados de QObject para facilitar un poco la gestión de memoria
// - Modificaciones BaseReference
//
// Revision 1.12  2016/08/20 09:00:50  jgurrutxaga
// - Añadir función is_valid a BaseEntity y BaseUser para comprobar si un objeto ha sido inicializado. La comprobación
// se realiza comprobando si el id del objeto es Null o Empty.
// - Añadir sobrecarga de la función is_online que usa el servidor y base de datso actuales
//
// Revision 1.11  2016/08/09 10:53:39  jgurrutxaga
// Cambios en replicación, memory leaks y logout
//
// Revision 1.10  2016/07/27 12:07:12  jmonasterio
// - BUGS: NULL entities
// - GET_INNER_ENTITY function
//
// Revision 1.9  2016/07/19 06:35:00  izalakain
// Modificaciones para el versionado y replicación
//
// Revision 1.8  2016/07/14 14:28:44  izalakain
// Se añade función de generación de fecha para estandarizar formato fecha en BD. Se añaden las propiedades working a
// la entidad VersionableEntity
//
// Revision 1.7  2016/07/13 13:31:33  izalakain
// Corrección de BUGs en BaseReference e inclusión de la clase BaseUser
//
// Revision 1.6  2016/07/13 05:45:01  izalakain
// Corrección de BUGs como la recuperación de elementos relacionados y las funciones relacionadas con la creación de
// usuarios couhch. Se añaden las clases basereference y replicationconfig.
//
// Revision 1.5  2016/07/12 10:05:25  izalakain
// Corrección de varios BUGS, recuperación de entidades, filtros etc.
//
// Revision 1.4  2016/07/01 08:12:29  izalakain
// Corrección de errores y funciones de autentificación de usuarios
//
// Revision 1.3  2016/06/17 12:01:37  jmonasterio
// New architecture
//
// Revision 0.0.4   2016/05/31 12:46:00 vbermudez@xi-tek.com
// Cambios realizados:
// - Añadido el método "list"
// - Modificados los valores de retornos de los métodos "all", "view", "list" a QList<QVariant>
// - Añadido control de errores a través de propiedades de la clase
// - Añadidos los comentarios
//
// Revision 0.0.3   2016/05/24 15:24:50 vbermudez@xi-tek.com
// Cambios realizados:
// - Añadido el método "bulk_docs"
//
// Revision 0.0.2   2016/05/24 14:32:46 vbermudez@xi-tek.com
// Cambios realizados:
// - Añadidos los métodos "all" y "view"
//
#ifndef COUCHDBMANAGER_H
#define COUCHDBMANAGER_H

#include "couchdbmanager_global.h"

#include <QObject>
#include <QString>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QJsonParseError>
#include <QStringList>
#include <QMap>
#include <QDateTime>
#include <QTimer>
#include <QSignalMapper>
#include <QHostInfo>
#include <QHostAddress>

#include <exception>
#include <limits>

#include <type_traits>
#include <assert.h>

#include "baseobject.h"
#include "baseuser.h"
#include "baseentity.h"
#include "relationentity.h"
#include "versionableentity.h"
#include "dbmanagerresponse.h"
#include "usercontext.h"
#include "replicationconfig.h"
#include "basereference.h"
#include "basereferencesimple.h"
#include "activetask.h"


/**
 * @brief El namespace CouchDBManager aisla las clases para evitar duplicidades
 */
namespace CouchDBManager
{
    /**
     * @brief La clase DBManager extiende a QObject para garantizar la completa intregración en el framework QT.
     * Gestiona las distintas operaciones básicas de CouchDB tales como leer, crear, modificar y borrar documentos.
     * Además facilita la integración con el resto de operaciones de CouchDB y es capaz de transformar las respuestas
     * de los servicios web en objetos QVariant.
     * @author vbermudez@xi-tek.com
     * @date 12/05/2016
     */
    class COUCHDBMANAGERSHARED_EXPORT DBManager : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString server_address READ get_server_address WRITE set_server_address)
        Q_PROPERTY(QString database_name READ get_database_name WRITE set_database_name)
        Q_PROPERTY(QString auth_cookie READ get_auth_cookie WRITE set_auth_cookie)
        Q_PROPERTY(QNetworkReply::NetworkError network_error READ get_network_error WRITE set_network_error)
        Q_PROPERTY(QString error_string READ get_error_string WRITE set_error_string)
        Q_PROPERTY(qint64 timeout READ get_timeout WRITE set_timeout)

    private:
        QString server_address;
        QString database_name;
        QString auth_cookie;
        QNetworkReply::NetworkError network_error;
        QString error_string;
        qint64 timeout;
        QNetworkAccessManager* manager;
        bool nw_read;
        bool nw_timeout;
        bool remote_conn;

        static const QString DATE_FORMAT;
        static const QString DATE_TIME_FORMAT;

        enum HTTP_METHOD {
            GET,
            POST,
            PUT,
            DEL
        };

        /**
         * @brief waitForReply Sincroniza la petición HTTP y controla el timeout.
         * @param reply Objeto QNetworkReply.
         * @return false si se ha superado el timeout configurado.
         */
        bool waitForReply(QNetworkReply* reply);
        /**
         * @brief get_url Obtiene la url del servidor CouhDB bien formada.
         * @return QString con la url.
         */
        QString get_url();
        /**
         * @brief get_url Obtiene la url del servidor CouhDB bien formada.
         * @param only_root Especifica si debe responder sólo con la raíz de la url, sin incluir la base de datos.
         * @return QString con la url.
         */
        QString get_url(bool only_root);
        /**
         * @brief get_url Obtiene la url del servidor CouhDB bien formada.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @return QString con la url.
         */
        QString get_url(const QString& partial);
        /**
         * @brief get_url Obtiene la url del servidor CouhDB bien formada.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @param use_context Especifica si debe utilizar la raiz de contexto o no.
         * @return QString con la url.
         */
        QString get_url(const QString& partial, bool use_context);
        /**
         * @brief get_url Obtiene la url del servidor CouhDB bien formada.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @param query_string Especifica una lista de parámetros, con formato QueryString, para añadir en la url.
         * @return QString con la url.
         */
        QString get_url(const QString& partial, const QString& query_string);
        /**
         * @brief get_url Obtiene la url del servidor CouhDB bien formada.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @param query_string Especifica una lista de parámetros, con formato QueryString, para añadir en la url.
         * @param use_context Especifica si debe utilizar la raiz de contexto o no.
         * @return QString con la url.
         */
        QString get_url(const QString& partial, const QString& query_string, bool use_context);

        /**
         * @brief set_remote_conn Comprueba si la conexión es local o remota
         */
        void set_remote_conn();
        /**
         * @brief set_common_headers Establece las cabeceras comunes para todas las peticiones http.
         * @param request Objeto QNetworkRequest en el que configurar las cabeceras.
         */
        void set_common_headers(QNetworkRequest* request);
        /**
         * @brief set_common_headers Establece las cabeceras comunes para todas las peticiones http.
         * @param request Objeto QNetworkRequest en el que configurar las cabeceras.
         * @param use_auth_cookie Fuerza el uso, o no, de la cookie de seguridad
         */
        void set_common_headers(QNetworkRequest* request, bool use_auth_cookie);
        /**
         * @brief request Configura y envía una petición HTTP.
         * @param url Objeto QUrl con la dirección del recurso.
         * @param ba_data QByteArray con los datos a enviar en la petición
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @param method Método de la petición HTTP.
         * @return QString con la respuesta del servicio.
         */
        QString request(const QUrl& url, const QByteArray& ba_data, const QString& partial, CouchDBManager::DBManager::HTTP_METHOD method);
        /**
         * @brief do_get Envía una petición GET a CouchDB.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @param query_string Especifica una lista de parámetros, con formato QueryString, para añadir en la url.
         * @return QString con el resultado de la consulta. Debería ser un texto JSON.
         */
        QString do_get(const QString& partial, const QString& query_string);
        /**
         * @brief do_get Envía una petición GET a CouchDB.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @param query_string Especifica una lista de parámetros, con formato QueryString, para añadir en la url.
         * @param use_context Especifica si debe utilizar la raiz de contexto o no.
         * @return QString con el resultado de la consulta. Debería ser un texto JSON.
         */
        QString do_get(const QString& partial, const QString& query_string, bool use_context);
        /**
         * @brief do_post Envía una petición POST a CouchDB.
         * @param data Son los datos a enviar en la petición. Debería ser un texto JSON o con formato QueryString.
         * @return QString con el resultado de la consulta. Debería ser un texto JSON.
         */
        QString do_post(const QString& data);
        /**
         * @brief do_post Envía una petición POST a CouchDB.
         * @param data Son los datos a enviar en la petición. Debería ser un texto JSON o con formato QueryString.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @return QString con el resultado de la consulta. Debería ser un texto JSON.
         */
        QString do_post(const QString& data, const QString& partial);
        /**
         * @brief do_post Envía una petición POST a CouchDB.
         * @param data Son los datos a enviar en la petición. Debería ser un texto JSON o con formato QueryString.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @param use_context Especifica si debe utilizar la raiz de contexto o no.
         * @return QString con el resultado de la consulta. Debería ser un texto JSON.
         */
        QString do_post(const QString& data, const QString& partial, bool use_context);
        /**
         * @brief do_put Envía una petición PUT a CouchDB.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @param data Son los datos a enviar en la petición. Debería ser un texto JSON o con formato QueryString.
         * @return QString con el resultado de la consulta. Debería ser un texto JSON.
         */
        QString do_put(const QString& partial, const QString& data);
        /**
         * @brief do_delete Envía una petición DELETE a CouchDB.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @param query_string Especifica una lista de parámetros, con formato QueryString, para añadir en la url.
         * @return QString con el resultado de la consulta. Debería ser un texto JSON.
         */
        QString do_delete(const QString& partial, const QString& query_string);
        /**
         * @brief do_delete Envía una petición DELETE a CouchDB.
         * @param partial Especifica una ruta parcial para añadir a la url.
         * @param query_string Especifica una lista de parámetros, con formato QueryString, para añadir en la url.
         * @param use_context Especifica si debe utilizar la raiz de contexto o no.
         * @return QString con el resultado de la consulta. Debería ser un texto JSON.
         */
        QString do_delete(const QString& partial, const QString& query_string, bool use_context);

        /**
         * @brief string_2_json_object Transforma una cadena JSON en un objeto QJsonObject.
         * @param str Cadena con el texto JSON a tranformar.
         * @param jason_parse_error Es igual a QJsonParseError:NoError sin se ha producido ningún error.
         * @return QJsonObject que representa la cadena JSON.
         */
        QJsonObject string_2_json_object(const QString& str, QJsonParseError* json_parse_error);
        /**
         * @brief string_2_json_array Transforma una cadena JSON en un objeto QJsonArray.
         * @param str Cadena con el texto JSON a tranformar.
         * @param jason_parse_error Es igual a QJsonParseError:NoError sin se ha producido ningún error.
         * @return QJsonArray que representa la cadena JSON.
         */
        QJsonArray string_2_json_array(const QString& str, QJsonParseError* json_parse_error);
        /**
         * @brief json_object_2_string Transforma un objeto QJsonObject en una cadena JSON.
         * @param obj Objeto que deriva de QObject.
         * @return QString con la representación JSON del objeto.
         */
        QString json_object_2_string(const QJsonObject& obj);

        /**
         * @brief update_entity_from_bulk_docs Actualiza los _id y _rev de las entidades según el
         * id y rev resultante de la operación _bulk_docs.
         * @param entity Objeto que deriva de BaseEntity.
         * @param id QString con el id del objeto.
         * @param rev QString con la rev del objeto.
         */
        QString get_rev(const QString& id, QJsonArray json_array);

        /**
         * @brief update_related_entities_from_bulk_docs Actualiza los _id y _rev de las relaciones de la entidad.
         * @param entity Objeto que deriva de BaseEntity.
         * @param json_array Objeto QJsonArray con el resultadao de _bulk_docs.
         */
        void update_related_entities_from_bulk_docs(CouchDBManager::BaseEntity* entity, QJsonArray json_array);

        /**
         * @brief update_entity_from_bulk_docs Actualiza los _id y _rev de las entiidades según el
         * resultado de la operación _bulk_docs.
         * @param entity Objeto que deriva de BaseEntity.
         * @param bd_result Objeto QJsonObject con el resultadao de _bulk_docs.
         */
        template <class T>
        void update_entity_from_bulk_docs(T* entity, QJsonArray json_array)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            if (!this->is_base_entity(entity))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
//                qFatal(err.toStdString().c_str());
            }
            else
            {
                QString rev = this->get_rev(entity->get_id(), json_array);

                if (!rev.isNull() && !rev.isEmpty())
                    {
                        entity->set_rev(rev);
                    }

                this->update_related_entities_from_bulk_docs(qobject_cast<CouchDBManager::BaseEntity*>(entity), json_array);
            }

            qDebug() << "<" << Q_FUNC_INFO;
        }

        /**
         * @brief object_2_json Transforma un objeto derivador de BaseObject en un objeto QJsonObject.
         * @param object Objeto que dervica de BaseObject.
         * @param json Objeto QJsonObject.
         */
        template <class T>
        bool object_2_json(T* object, QJsonObject &json)
        {
            qDebug() << "><" << Q_FUNC_INFO;

            return this->object_2_json<T>(object, json, false);
        }
        /**
         * @brief object_2_json Transforma un objeto derivador de BaseObject en un objeto QJsonObject.
         * @param object Objeto que dervica de BaseObject.
         * @param relation_as_entity Fuerza a tratar un RelationEntity como si fuera un BaseEntity.
         * @param json Objeto QJsonObject.
         */
        template <class T>
        bool object_2_json(T* object, QJsonObject &json, bool relation_as_entity)
        {
            qDebug() << "><" << Q_FUNC_INFO;

            return this->object_2_json<T>(object, json, relation_as_entity, false);
        }
        /**
         * @brief object_2_json Transforma un objeto derivador de BaseObject en un objeto QJsonObject.
         * @param object Objeto que dervica de BaseObject.
         * @param relation_as_entity Fuerza a tratar un RelationEntity como si fuera un BaseEntity.
         * @param gen_uuid Fuerza la creación de UUID para la entidad antes de transformarla a JSON.
         * @param json Objeto QJsonObject.
         */
        template <class T>
        bool object_2_json(T* object, QJsonObject &json, bool relation_as_entity, bool gen_uuid)
        {
            qDebug() << "><" << Q_FUNC_INFO;

            return this->object_2_json<T>(object, json, relation_as_entity, gen_uuid, true);
        }

        /**
         * @brief object_2_json Transforma un objeto derivador de BaseObject en un objeto QJsonObject.
         * @param object Objeto que dervica de BaseObject.
         * @param relation_as_entity Fuerza a tratar un RelationEntity como si fuera un BaseEntity.
         * @param gen_uuid Fuerza la creación de UUID para la entidad antes de transformarla a JSON.
         * @param load_relations Fuerza el proceso recursivo de relaciones.
         * @param json Objeto QJsonObject.
         */
        template <class T>
        bool object_2_json(T* object, QJsonObject &json, bool relation_as_entity, bool gen_uuid, bool load_relations)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            bool is_bulk_docs = false;
            const QMetaObject* meta_object = object->metaObject();
            QString class_name = meta_object->className();
            QString static_class_name = object->staticMetaObject.className();
            QJsonArray other_docs;
            QJsonObject json_tmp;
            bool is_a_rel_ent = this->is_relation_entity(object);
            QString uuid;

            qDebug() << "# Write JSON for: " << class_name << "Inherited from: " << static_class_name;
            qDebug() << "# Generate UUIDs:" << gen_uuid;

            if (gen_uuid)
            {
                QStringList uuids = this->uuids(1);

                if (!uuids.isEmpty() && this->is_base_entity(object))
                {
                    uuid = uuids.at(0);
                }
            }

            for (int i = 0; i < meta_object->propertyCount(); ++i)
            {
                QMetaProperty meta_prop = meta_object->property(i);
                QString name = QString(meta_prop.name());

                qDebug() << "# Procesando propiedad" << name;

                if (name == "objectName")
                {
                    continue;
                }

                if (gen_uuid && name == "_id")
                {
                    QVariant _id_value = object->property(meta_prop.name());

                    if (_id_value.isNull() || !_id_value.isValid())
                    {
                        qDebug() << "# Escribiendo UUID" << uuid;

                        meta_prop.write(object, QVariant(uuid));
                    }
                    else
                    {
                        qDebug() << "# La propiedad _id ya tiene un valor:" << _id_value;
                    }
                }

                QVariant value = object->property(meta_prop.name());

                if ( QString( meta_prop.typeName() ).startsWith("QList") )
                {
                    qDebug() << "# Es un array:" << value.typeName();

                    QList<QVariant> value_list = object->toList(value);

                    if (value_list.isEmpty())
                    {
                        qDebug() << "# El array original está vacio.";

                        continue;
                    }

                    QJsonArray array_value;

                    foreach (const QVariant &item, value_list)
                    {
                        if (this->is_relation_entity( qvariant_cast<QObject*>(item) ))
                        {
                            qDebug() << "# Item es RelationEntity.";

                            if (!load_relations)
                            {
                                qDebug() << "# No se procesa por ser un objeto ya relacionado.";

                                continue;
                            }

                            QJsonObject relation_json;
                            const QMetaObject* relation_meta = qvariant_cast<CouchDBManager::RelationEntity*>(item)->metaObject();

                            this->object_2_json<CouchDBManager::RelationEntity>( qvariant_cast<CouchDBManager::RelationEntity*>(item), relation_json, true, gen_uuid);

                            for (int i = 0, len = relation_meta->propertyCount(); i < len; i++)
                            {
                                QMetaProperty relation_prop = relation_meta->property(i);
                                QVariant relation_prop_value = relation_prop.read(item.value<QObject*>());

                                if (this->is_base_entity(relation_prop_value.value<QObject*>()))
                                {
                                    CouchDBManager::BaseEntity* related_entity = qvariant_cast<CouchDBManager::BaseEntity*>(relation_prop_value);
                                    const QMetaObject* rel_ent_meta_object = related_entity->metaObject();
                                    QString rel_ent_class_name = rel_ent_meta_object->className();

                                    if (rel_ent_class_name != class_name)
                                    {
                                    QJsonObject relation_prop_json;
                                    QJsonObject related_json;

                                        this->object_2_json( related_entity, relation_prop_json, false, gen_uuid, false);
                                    related_json.remove("objectName");
                                    related_json.insert("id", relation_prop_json["_id"]);

                                    if (relation_prop_json.contains("version"))
                                    {
                                        related_json.insert("version", relation_prop_json["version"]);
                                    }

                                    relation_json.insert(relation_prop.name(), QJsonValue(related_json));
                                    other_docs.append(QJsonValue(relation_prop_json));
                                }
                                else
                                {
                                        qDebug() << "# Referencia circular detectada. No se procesará la relación por tratarse del mismo documento:" << rel_ent_class_name << related_entity->get_collection() << related_entity->get_id();
                                    }
                                }
                                else
                                {
                                    relation_json.insert(relation_prop.name(), QJsonValue::fromVariant(relation_prop_value));
                                }
                            }

                            if (relation_as_entity)
                            {
                                json_tmp = relation_json;
                            }

                            other_docs.append(QJsonValue(relation_json));
                        }
                        else if (this->is_base_entity( qvariant_cast<QObject*>(item) ))
                        {
                            qDebug() << "# Item es BaseEntity.";

                            if (!load_relations)
                            {
                                qDebug() << "# No se procesa por ser un objeto ya relacionado.";

                                continue;
                            }

                            QJsonObject item_json;

                            this->object_2_json<CouchDBManager::BaseEntity>( qvariant_cast<CouchDBManager::BaseEntity*>(item), item_json, false, gen_uuid, false);
                            other_docs.append(QJsonValue(item_json));
                        }
                        else if (this->is_base_reference( qvariant_cast<QObject*>(value) ))
                        {
                            qDebug() << "# Item es BaseReference.";

                            QJsonObject item_json;

                            this->object_2_json<CouchDBManager::BaseReference>( qvariant_cast<CouchDBManager::BaseReference*>(value), item_json, false);
                            array_value.append(QJsonValue(item_json));
                        }
                        else if (this->is_base_reference_simple( qvariant_cast<QObject*>(value) ))
                        {
                            qDebug() << "# Item es BaseReference simple.";

                            QJsonObject item_json;

                            this->object_2_json<CouchDBManager::BaseReferenceSimple>( qvariant_cast<CouchDBManager::BaseReferenceSimple*>(value), item_json, false);
                            array_value.append(QJsonValue(item_json));
                        }
                        else if (this->is_base_object( qvariant_cast<QObject*>(item) ))
                        {
                            qDebug() << "# Item es BaseObject.";

                            QJsonObject item_json;

                            this->object_2_json<CouchDBManager::BaseObject>( qvariant_cast<CouchDBManager::BaseObject*>(item), item_json, false);
                            array_value.append(QJsonValue(item_json));
                        }
                        else
                        {
                            qDebug() << "# Item no es una entidad.";

                            array_value.append(QJsonValue::fromVariant(item));
                        }
                    }

                    if (!array_value.isEmpty())
                    {
                        qDebug() << "# El array no esta vacio.";

                        json_tmp.insert(name, QJsonValue(array_value));
                    }
                }
                else
                {
                    qDebug() << "# Es un valor simple";

                    if (value.isNull() || !value.isValid())
                    {
                        qDebug() << "# Es nulo o inválido.";

                        continue;
                    }

//                    if ( QJsonValue::fromVariant(value).isNull() )
//                    {
//                        qDebug() << "# Es nulo o inválido.";

//                        continue;
//                    }

                    if (this->is_relation_entity( qvariant_cast<QObject*>(value) ))
                    {
                        qDebug() << "# La propiedad es RelationEntity.";

                        if (!load_relations)
                        {
                            qDebug() << "# No se procesa por ser un objeto ya relacionado.";

                            continue;
                        }

                        QJsonObject relation_json;
                        const QMetaObject* relation_meta = qvariant_cast<CouchDBManager::RelationEntity*>(value)->metaObject();

                        this->object_2_json<CouchDBManager::RelationEntity>( qvariant_cast<CouchDBManager::RelationEntity*>(value), relation_json, true, gen_uuid);
                        other_docs.append(QJsonValue(relation_json));

                        for (int i = 0, len = relation_meta->propertyCount(); i < len; i++)
                        {
                            QMetaProperty relation_prop = relation_meta->property(i);
                            QVariant relation_prop_value = relation_prop.read(value.value<QObject*>());

                            if (this->is_base_entity(relation_prop_value.value<QObject*>()))
                            {
                                QJsonObject item_json;
                                QJsonObject related_json;

                                this->object_2_json( qvariant_cast<CouchDBManager::BaseEntity*>(relation_prop_value), item_json, false, gen_uuid, false);
                                related_json.remove("objectName");
                                related_json.insert("id", item_json["_id"]);

                                if (item_json.contains("version"))
                                {
                                    related_json.insert("version", item_json["version"]);
                                }

                                relation_json.insert(relation_prop.name(), QJsonValue(related_json));
                                other_docs.append(QJsonValue(item_json));
                            }
                            else
                            {
                                relation_json.insert(relation_prop.name(), QJsonValue::fromVariant(relation_prop_value));
                            }
                        }

                        if (relation_as_entity)
                        {
                            json_tmp = relation_json;
                        }

                        other_docs.append(QJsonValue(relation_json));
                    }
                    else if (this->is_base_entity( qvariant_cast<QObject*>(value) ))
                    {
                        qDebug() << "# La propiedad es BaseEntity.";

                        if (!load_relations)
                        {
                            qDebug() << "# No se procesa por ser un objeto ya relacionado.";

                            continue;
                        }

                        QJsonObject item_json;

                        this->object_2_json<CouchDBManager::BaseEntity>( qvariant_cast<CouchDBManager::BaseEntity*>(value), item_json, false, gen_uuid, false);

                        if (item_json.contains("objectName"))
                        {
                            item_json.remove("objectName");
                        }

                        if (is_a_rel_ent)
                        {
                            qDebug() << "# Se añade id y version de la entidad relacionada por ser un RelationEntity.";

                            QJsonObject relation_property;
                            QString _id = item_json["_id"].toString();
                            QString _rev = item_json["_rev"].toString();

                            relation_property.insert("id", item_json["_id"]);

                            if (item_json.contains("version"))
                            {
                                relation_property.insert("version", item_json["version"]);
                            }

                            json_tmp.insert(name, QJsonValue(relation_property));

                            QJsonArray relation_list_prop;

                            if (item_json.contains(class_name.toLower()))
                            {
                                relation_list_prop = item_json[class_name.toLower()].toArray();
                            }

                            if ( (_id.isNull() || _id.isEmpty()) && (_rev.isNull() || _rev.isEmpty()))
                            {
                                other_docs.append(QJsonValue(item_json));
                            }
                        }
                        else
                        {
                            other_docs.append(QJsonValue(item_json));
                        }
                    }
                    else if (this->is_base_reference( qvariant_cast<QObject*>(value) ))
                    {
                        qDebug() << "# La propiedad es BaseReference.";

                        QJsonObject item_json;

                        this->object_2_json<CouchDBManager::BaseReference>( qvariant_cast<CouchDBManager::BaseReference*>(value), item_json, false);
                        json_tmp.insert(name, QJsonValue(item_json));
                    }
                    else if (this->is_base_reference_simple( qvariant_cast<QObject*>(value) ))
                    {
                        qDebug() << "# La propiedad es BaseReferenceSimple.";

                        QJsonObject item_json;

                        this->object_2_json<CouchDBManager::BaseReferenceSimple>( qvariant_cast<CouchDBManager::BaseReferenceSimple*>(value), item_json, false);
                        json_tmp.insert(name, QJsonValue(item_json));
                    }
                    else if (this->is_base_object( qvariant_cast<QObject*>(value) ))
                    {
                        qDebug() << "# La propiedad es BaseObject.";

                        QJsonObject item_json;

                        this->object_2_json<CouchDBManager::BaseObject>( qvariant_cast<CouchDBManager::BaseObject*>(value), item_json, false);
                        json_tmp.insert(name, QJsonValue(item_json));
                    }
                    else
                    {
                        qDebug() << "# La propiedad no es una entidad.";

//                        QJsonValue value_json = QJsonValue::fromVariant(value);

//                        if ( !value_json.isNull() || (value_json.isString() && !value_json.toString().isEmpty()) )
//                        {
//                            json_tmp.insert(name, value_json);
//                        }

                        json_tmp.insert(name, QJsonValue::fromVariant(value));
                    }
                }
            }

            if (other_docs.isEmpty() || relation_as_entity)
            {
                qDebug() << "# Utilizando formato documento";

                json = json_tmp;
            }
            else
            {
                qDebug() << "# Utilizando formato _bulk_docs";

                is_bulk_docs = true;
                other_docs.append( QJsonValue(json_tmp) );
                json.insert("docs", QJsonValue(other_docs) );
            }

            qDebug() << "<" << Q_FUNC_INFO;

            return is_bulk_docs;
        }

        /**
         * @brief reset_errors Restablece el control de errores.
         */
        void reset_errors();

        /**
         * @brief is_relation_entity Determina si un QObject deriva de RelationEntity.
         * @param object QObject.
         * @return
         */
        bool is_relation_entity(QObject* object);
        /**
         * @brief is_base_entity Determina si un QObject deriva de BaseEntity.
         * @param object QObject.
         * @return
         */
        bool is_base_entity(QObject* object);
        /**
         * @brief is_versionable_entity Determina si un QObject deriva de VersionableEntity.
         * @param object QObject.
         * @return
         */
        bool is_versionable_entity(QObject* object);
        /**
         * @brief is_base_user Determina si un QObject deriva de BaseUser.
         * @param object QObject.
         * @return
         */
        bool is_base_user(QObject* object);
        /**
         * @brief is_base_object Determina si un QObject deriva de BaseObject.
         * @param object QObject.
         * @return
         */
        bool is_base_object(QObject* object);
        /**
         * @brief is_base_object Determina si un QObject deriva de BaseReference.
         * @param object QObject.
         * @return
         */
        bool is_base_reference(QObject *object);
        /**
         * @brief is_base_object Determina si un QObject deriva de BaseReferenceSimple.
         * @param object QObject.
         * @return
         */
        bool is_base_reference_simple(QObject *object);
        /**
         * @brief is_relation_entity_list Determina si el tipo de una lista deriva de RelationEntity.
         * @param type Meta-tipo de la lista.
         * @return
         */
        template <typename T>
        bool is_relation_entity_type() {
            qDebug() << ">" << Q_FUNC_INFO;

            bool is_rel = true;
            QString typeName = QString( typeid(T).name() ).replace("class", "").replace(" ", "");

            qDebug() << "Comprobando el tipo" << typeName;

            int type_id = qMetaTypeId<T>();

            if (type_id == QMetaType::UnknownType)
            {
                qDebug() << "El tipo" << typeName << "no esta registrado";

                is_rel = false;
            }
            else
            {
                const QMetaObject* meta_object = QMetaType::metaObjectForType(type_id);
                // VBG - 20161130: SE CAMBIA EL PARENT A this->parent() SEGÚN EL CORREO DE Jon Gurrutxaga del 23/11/2016
                QObject* instance = meta_object->newInstance( Q_ARG(QObject*, this->parent()) );
                is_rel = this->is_relation_entity(instance);

                delete instance;
            }

            qDebug() << "<" << Q_FUNC_INFO;

            return is_rel;
        }

        /**
         * @brief initialize Inicializa la API, registrando todos lo tipos.
         */
        void initialize();

    public:
        /**
         * @brief DBManager Contructor por defecto.
         * @param parent QObject padre.
         */
        DBManager(QObject *parent = 0) : QObject(parent) {
            this->initialize();
        }
        /**
         * @brief DBManager Contructor
         * @param parent QObject padre.
         * @param address Url raíz del servidor CouchDB. Ejplo: http://localhost:5984.
         * @param db_name Nombre de la base de datos sobre la que se actúa.
         */
        DBManager(const QString& address, const QString& db_name, QObject *parent = 0) : server_address(address), database_name(db_name), QObject(parent) {
            this->initialize();
        }
        /**
         * @brief ~DBManager Destructor
         */
        virtual ~DBManager() { }

        /**
         * @brief registerType Registra un tipo.
         * @return int Identificador del tipo.
         */
        template <typename T>
        static int registerType()
        {
            qDebug() << ">" << Q_FUNC_INFO;

            QString typeName = QString( typeid(T).name() ).replace("class", "").replace(" ", "");

            qDebug() << "Comprobando si el tipo" << typeName << "esta registrado";

            int type_id = qMetaTypeId<T>(); //QMetaType::type( typeName.toUtf8() );

            if (type_id == QMetaType::UnknownType)
            {
                qDebug() << "El tipo" << typeName << "no esta registrado";

                type_id = qRegisterMetaType<T>();
            }

            qDebug() << "El tipo" << typeName << "esta registrado con id" << type_id;

            return type_id;
        }

        /**
         * @brief set_server_address Establece la url  raíz del servidor CouchDB.
         * @param address Url raíz del servidor CouchDB. Ejplo: http://localhost:5984.
         */
        void set_server_address(const QString& address);
        /**
         * @brief get_server_address Recupera la url  raíz del servidor CouchDB.
         * @return QString con la url  raíz del servidor CouchDB.
         */
        QString get_server_address() const;
        /**
         * @brief set_database_name Establece el nombre de la base de datos.
         * @param name Nombre de la base de datos.
         */
        void set_database_name(const QString& name);
        /**
         * @brief get_database_name Recupera el nombre de la base de datos.
         * @return QString con el nombre de la base de datos.
         */
        QString get_database_name() const;
        /**
         * @brief set_auth_cookie Establece la cookie de sesión.
         * @param a_auth_cookie Cookie de sesión.
         */
        void set_auth_cookie(const QString& a_auth_cookie);
        /**
         * @brief get_auth_cookie Recupera el nombre de la base de datos.
         * @return QString con el nombre de la base de datos.
         */
        QString get_auth_cookie() const;
        /**
         * @brief set_remote_conn Fuerza el tipo de conexión a remota o local.
         * @param a_remote_conn Si es true, la conexión se tratará como remota.
         */
        void set_remote_conn(bool a_remote_conn);
        /**
         * @brief get_remote_conn Recupera el tipo de conexión (remota o local).
         * @return true si la conexión es remota.
         */
        bool get_remote_conn() const;
        /**
         * @brief get_network_error Indica si en la última operación ha existido algún error de comunicaciones.
         * @return QNetworkReply::NoError si no ha existido ningún error.
         */
        QNetworkReply::NetworkError get_network_error() const;
        /**
         * @brief set_network_error Establece un error de comunicaciones.
         * @param a_error El error, como constante de QNetworkReply::NetworkError.
         */
        void set_network_error(const QNetworkReply::NetworkError a_error);
        /**
         * @brief get_error_string Indica si ha existido algún error en alguna transformción de objetos o JSON.
         * @return QString con la descripción del error.
         */
        QString get_error_string() const;
        /**
         * @brief set_error_string Establece un error de transformación.
         * @param a_error_string La descripción del error.
         */
        void set_error_string(const QString& a_error_string);
        /**
         * @brief get_timeout Recupera el timeout actual, en milisegundos.
         * @return El valor en milisegundos del timeout actual.
         */
        qint64 get_timeout() const;
        /**
         * @brief set_timeout Establece el timeout, en milisegundos.
         * @param a_timeout Un valor de timeout, en milisegundos.
         */
        void set_timeout(qint64 a_timeout);

        /**
         * @brief is_online Comprueba si una serivdor y base de datos existen y est?n online.
         * @return true si existe y est? online.
         */
        bool is_online();

        /**
         * @brief is_online Comprueba si una serivdor y base de datos existen y están online.
         * @param a_server Dirección del servidor.
         * @param a_database Nombre de la base de datos.
         * @return true si existe y está online.
         */
        bool is_online(const QString& a_server, const QString& a_database);

        /**
         * @brief create Crea un nuevo documento en CouchDB.
         * @param entity Objeto que extiende BaseEntity.
         * @return QJsonObject con la respuesta del servidor. Contiene el nuevo _id.
         */
        template <class T>
        CouchDBManager::DBManagerResponse* create(T* entity)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            CouchDBManager::DBManagerResponse* response = new CouchDBManager::DBManagerResponse(this);
            const char* entityClass = entity->metaObject()->className();
            const char* className = entity->staticMetaObject.className();
            bool is_bulk_docs = false;

            qDebug() << "# Entity:" << entityClass << "derived from:" << className;

            if (!this->is_base_entity(entity))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
//                qFatal(err.toStdString().c_str());
                response->set_went_ok(false);
                response->set_response(err);
            }
            else
            {
                QJsonObject json_object;
                is_bulk_docs = this->object_2_json<T>(entity, json_object, false, true);
                QString json_string = this->json_object_2_string(json_object);

                qDebug() << "# JSON String (_bulk_docs:" << is_bulk_docs << "):" << json_string;

                if (is_bulk_docs)
                {
                    qDebug() << "# Procesando para _bulk_docs";

                    QJsonArray json_array = json_object["docs"].toArray();
                    QJsonArray json_array_copy;
                    QStringList last_ids;

                    for (int i = 0, len = json_array.size(); i < len; i++)
                    {
                        QJsonObject json_item = json_array.at(i).toObject();

                        if ( json_item.contains("_id") && last_ids.contains( json_item["_id"].toString() ))
                        {
                            continue;
                        }

                        if (json_item.contains("_rev") && (json_item["_rev"].toString().isNull() || json_item["_rev"].toString().isEmpty()) )
                        {
                            json_item.remove("_rev");
                        }

//                        if (json_item.contains("_id"))
//                        {
//                            json_item.remove("_id");
//                        }

                        if (json_item.contains("objectName"))
                        {
                            json_item.remove("objectName");
                        }

                        json_array_copy.append(QJsonValue(json_item));
                        last_ids.append( json_item["_id"].toString() );
                    }

                    json_object.insert("docs", QJsonValue(json_array_copy));
                    json_string = this->json_object_2_string(json_object);

                    qDebug() << "# BULK JSON String corrected: " << json_string;
                }
                else if (json_string.contains("_rev"))
                {
                    qDebug() << "# Procesando documento";

                    json_object.remove("_rev");

                    json_string = this->json_object_2_string(json_object);

                    qDebug() << "# JSON String corrected: " << json_string;
                }

                QString result;

                if (is_bulk_docs)
                {
                    result = this->do_post( json_string, "_bulk_docs" );
                }
                else
                {
                    result = this->do_post( json_string );
                }

                qDebug() << "# do_post: " << result;

                if (result.isNull() || result.isEmpty())
                {
                    response->set_went_ok(false);
                    response->set_response(this->error_string);
                    qDebug() << "# Errors?: " << this->error_string;
                }
                else
                {
                    QJsonParseError json_parse_error;

                    if (is_bulk_docs)
                    {
                        QJsonArray json = this->string_2_json_array(result, &json_parse_error);

                        if (json_parse_error.error == QJsonParseError::NoError)
                        {
                            this->update_entity_from_bulk_docs<T>(entity, json);
                        }
                    }
                    else
                    {
                    QJsonObject json = this->string_2_json_object(result, &json_parse_error);

                        if (json_parse_error.error == QJsonParseError::NoError)
                        {
                            entity->set_id(json["id"].toString());
                            entity->set_rev(json["rev"].toString());
                        }
                    }

                    if (json_parse_error.error != QJsonParseError::NoError)
                    {
                        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

                        this->set_error_string(err);
                        qCritical() << err;
//                        qFatal(err.toStdString().c_str());
                        response->set_went_ok(false);
                        response->set_response( err );
                    }
                    else
                    {
                        response->set_went_ok(true);
                        response->set_response( "Entity created" );
                    }
                }
            }

            qDebug() << "<" << Q_FUNC_INFO;

            return response;
        }
        /**
         * @brief read Recupera la última revisión de un documento de CouchDB.
         * @param id Identificador único del documento.
         * @param entity Instancia de un objeto derivado de BaseEntity.
         * @return Entidad CouchDBManager::BaseEntity con la información recuperada de CouchDB.
         */
        template <class  T>
        void read(const QString& id, T* entity)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            this->read<T>(id, QString(), entity);

            qDebug() << "<" << Q_FUNC_INFO;
        }
        /**
         * @brief read Recupera la última revisión de una versión de un documento de CouchDB.
         * @param id Identificador único del documento.
         * @param version Número de versión del documento;
         * @param entity Instancia de un objeto derivado de BaseEntity.
         * @return Entidad CouchDBManager::BaseEntity con la información recuperada de CouchDB.
         */
        template <class  T>
        void read(const QString& id, int version, T* entity)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            this->read<T>(QString(id + "::%1").arg(version), QString(), entity);

            qDebug() << "<" << Q_FUNC_INFO;
        }
        /**
         * @brief read Recupera un documento de CouchDB, según los parámetro proporcionados.
         * @param id Identificador único del documento.
         * @param query_string Especifica una lista de parámetros, con formato QueryString.
         * @param entity Instancia de un objeto derivado de BaseEntity.
         * @return Entidad CouchDBManager::BaseEntity con la información recuperada de CouchDB.
         */
        template <class  T>
        void read(const QString& id, const QString& query_string, T* entity)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            const char* entityClass = entity->metaObject()->className();
            const char* className = entity->staticMetaObject.className();

            qDebug() << "# Entity:" << entityClass << "derived from:" << className;

            if (!this->is_base_entity(entity))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
//                qFatal(err.toStdString().c_str());

                return;
            }

            QString result = this->do_get(id, query_string);

            qDebug() << "# do_get: " << result;

            if (result.isNull() || result.isEmpty())
            {
                qDebug() << "# Errors?: " << this->error_string;

                return;
            }

            QJsonParseError json_parse_error;
            QJsonObject json = this->string_2_json_object(result, &json_parse_error);

            if (json_parse_error.error != QJsonParseError::NoError)
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

                this->set_error_string(err);
                qCritical() << err;
//                qFatal(err.toStdString().c_str());

                return;
            }

            entity->read(json);

            qDebug() << "<" << Q_FUNC_INFO;
        }
        /**
         * @brief read_with_relations Recupera la última revisión de una versión de un documento de CouchDB, cargando todas sus relaciones.
         * @param design_document Nombre del Design Document donde está la vista.
         * @param id Identificador único del documento.
         * @param entity Instancia de un objeto derivado de BaseEntity.
         * @return Entidad CouchDBManager::BaseEntity con la información recuperada de CouchDB.
         */
        template <class  T>
        void read_with_relations(const QString& design_document, const QString& id, T* entity)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            this->read_with_relations(design_document, id, -1, entity);

            qDebug() << "<" << Q_FUNC_INFO;
        }
        /**
         * @brief read_with_relations Recupera la última revisión de un documento de CouchDB, cargando todas sus relaciones.
         * @param design_document Nombre del Design Document donde está la vista.
         * @param id Identificador único del documento.
         * @param version Número de versión del documento;
         * @param entity Instancia de un objeto derivado de BaseEntity.
         * @return Entidad CouchDBManager::BaseEntity con la información recuperada de CouchDB.
         */
        template <class  T>
        void read_with_relations(const QString& design_document, const QString& id, int version, T* entity)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            QString query_string = "include_docs=true&format=json&startkey=[\"";
            QString list = "relations";
            QString view = entity->get_collection();
            QString ver_str = QString("%1").arg(version);
            QString id_str = QString(id);
            if (version != -1)
            {
                id_str.append("::");
                id_str.append(ver_str);
            }
            QString where = QString("_id='%1'").arg(id_str);

            qDebug() << "# Design Document: " << design_document << "List: " << list << "View: " << view << "Where: " << where;

            query_string.append(id_str);
            query_string.append("\",");
            query_string.append( version == -1 ? "0" : ver_str);
            query_string.append(",0]&endkey=[\"");
            query_string.append(id_str);
            query_string.append("\",");
            query_string.append( version == -1 ? QString("%1").arg( (std::numeric_limits<int>::max)() ) : ver_str );
            query_string.append(",999]");
            query_string.append("&where=");
            query_string.append( QUrl::toPercentEncoding(where) );

            qDebug() << "# Filter:" << where;
            qDebug() << "# QueryString:" << query_string;

            QString partial = "_design/commons/_list/" + list + "/" + design_document + "/" + view;
            QString result = this->do_get(partial, query_string);

            qDebug() << "# do_get: " << result;

            if (result.isNull() || result.isEmpty())
            {
                qDebug() << "# Errors?: " << this->error_string;
                qDebug() << "<" << Q_FUNC_INFO;

                return;
            }

            QJsonParseError json_parse_error;
            QJsonObject result_json = this->string_2_json_object(result, &json_parse_error);

            if (json_parse_error.error != QJsonParseError::NoError)
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

                this->set_error_string(err);
                qCritical() << err;

                return;
            }

            QJsonArray docs_array = result_json["rows"].toArray();

            if (docs_array.size() > 0)
            {
                entity->read( docs_array.at(0).toObject() );
            }

            qDebug() << "<" << Q_FUNC_INFO;
        }
        /**
         * @brief update Actualiza un documento en CouchDB.
         * @param entity Objeto que extiende BaseEntity.
         * @return QJsonObject con la respuesta del servidor. Contiene la nueva _rev.
         */
        template <class T>
        CouchDBManager::DBManagerResponse* update(T* entity)
        {
            qDebug() << "><" << Q_FUNC_INFO;

            return this->update<T>(entity, false);
        }
        /**
         * @brief update Actualiza un documento en CouchDB.
         * @param entity Objeto que extiende BaseEntity.
         * @param unlock Si debe desbloquear la entidad o no. Sólo aplica a derivados de VersionableEntity.
         * @return QJsonObject con la respuesta del servidor. Contiene la nueva _rev.
         */
        template <class T>
        CouchDBManager::DBManagerResponse* update(T* entity, bool unlock)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            CouchDBManager::DBManagerResponse* response = new CouchDBManager::DBManagerResponse(this);
            const QMetaObject* meta_entity = entity->metaObject();
            const char* entityClass = meta_entity->className();
            const char* className = entity->staticMetaObject.className();
            bool is_bulk_docs = false;
            bool delete_locked_by = false;

            qDebug() << "# Entity:" << entityClass << "derived from:" << className;

            if (!this->is_base_entity(entity))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
                response->set_went_ok(false);
                response->set_response(err);
            }
            else
            {
                if (this->is_versionable_entity(entity))
                {
                    T persisted;
                    int persisted_version = 0;
                    int entity_version = 0;
                    bool locked_by_user = true;

                    this->read<T>(entity->get_id(), QString(), &persisted);

                    // Comprueba si la conexión es remota
                    if (this->get_remote_conn())
                    {
                        bool local_locked = false;
                        bool persisted_locked = false;
                        QString local_locked_by;

                        QMetaObject::invokeMethod(entity, "get_locked", Q_RETURN_ARG(bool, local_locked));
                        QMetaObject::invokeMethod(&persisted, "get_locked", Q_RETURN_ARG(bool, persisted_locked));
                        QMetaObject::invokeMethod(entity, "get_locked_by", Q_RETURN_ARG(QString, local_locked_by));

                        // Comprueba que la entidad de BBDD está bloqueada por el usuario establecido en la entidad local
                        locked_by_user = this->is_locked_by_user<T>(&persisted, local_locked_by);

                        // Comprueba que el documento de BBDD no está bloqueado y el local sí lo está
                        if (!persisted_locked && local_locked)
                        {
                            // Se está bloqueando el documento, debe permitir la actualización
                            locked_by_user = true;
                        }

                        // Comprueba que el documento está bloqueado en la BBDD por el usuario y el local no
                        if (locked_by_user && !local_locked)
                        {
                            delete_locked_by = true;
                        }

                        if (!locked_by_user)
                        {
                            QString err = QString(Q_FUNC_INFO) + " FATAL The entity is not locked by the current user.";

                            this->set_error_string(err);

                            qCritical() << err;
                            qDebug() << "<" << Q_FUNC_INFO;
                            response->set_went_ok(false);
                            response->set_response(err);

                            return response;
                        }
                    }

                    QMetaObject::invokeMethod(&persisted, "get_version", Q_RETURN_ARG(int, persisted_version));
                    QMetaObject::invokeMethod(entity, "get_version", Q_RETURN_ARG(int, entity_version));

                    if (locked_by_user && persisted_version != entity_version)
                    {
                        persisted.set_id( QString(persisted.get_id() + "::%1").arg(persisted_version) );
                        persisted.set_rev("");
                        QMetaObject::invokeMethod(&persisted, "set_locked", Q_ARG(bool, false));
                        QMetaObject::invokeMethod(&persisted, "set_locked_by", Q_ARG(QString, ""));
                        QMetaObject::invokeMethod(&persisted, "set_working", Q_ARG(bool, false));

                        DBManagerResponse* resp = this->create<T>(&persisted);

                        if (!resp->get_went_ok()) {
                            return resp;
                        }
                    }

                    if (locked_by_user && unlock)
                    {
                        QMetaObject::invokeMethod(entity, "set_locked", Q_ARG(bool, false));
//                        QMetaObject::invokeMethod(entity, "set_locked_by", Q_ARG(QString, ""));
                        delete_locked_by = true;
                    }
                }

                QJsonObject json_object;
                is_bulk_docs = this->object_2_json<T>(entity, json_object);
                QString json_string = QString(QJsonDocument(json_object).toJson(QJsonDocument::Compact));

                qDebug() << "# JSON String (_bulk_docs:" << is_bulk_docs << "):" << json_string;

                if (is_bulk_docs)
                {
                    qDebug() << "# Procesando para _bulk_docs";

                    QJsonArray json_array = json_object["docs"].toArray();
                    QJsonArray json_array_copy ;

                    for (int i = 0, len = json_array.size(); i < len; i++)
                    {
                        QJsonObject json_item = json_array.at(i).toObject();

                        if (json_item.contains("objectName"))
                        {
                            json_item.remove("objectName");
                        }

                        json_array_copy.append(QJsonValue(json_item));
                    }

                    json_object.insert("docs", QJsonValue(json_array_copy));
                    json_string = this->json_object_2_string(json_object);

                    qDebug() << "# BULK JSON String corrected: " << json_string;
                }

                QString result;

                if (is_bulk_docs)
                {
                    result = this->do_post( json_string, "_bulk_docs" );
                }
                else
                {
                    result = this->do_post( json_string );
                }

                qDebug() << "# do_post:" << result;

                if (result.isNull() || result.isEmpty())
                {
                    qDebug() << "# Errors?: " << this->error_string;

                    response->set_went_ok(false);
                    response->set_response(this->error_string);
                }
                else
                {
                    QJsonParseError json_parse_error;

                    if (is_bulk_docs)
                    {
                        QJsonArray json_result = this->string_2_json_array(result, &json_parse_error);

                        if (json_parse_error.error == QJsonParseError::NoError)
                        {
                            this->update_entity_from_bulk_docs<T>(entity, json_result);
                        }
                    }
                    else
                    {
                    QJsonObject json_result = this->string_2_json_object(result, &json_parse_error);

                        if (json_parse_error.error == QJsonParseError::NoError)
                        {
                            entity->set_rev(json_result["rev"].toString());
                        }
                    }

                    if (json_parse_error.error != QJsonParseError::NoError)
                    {
                        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

                        this->set_error_string(err);
                        qCritical() << err;
//                        qFatal(err.toStdString().c_str());
                        response->set_went_ok(false);
                        response->set_response(err);
                    }
                    else
                    {
                        response->set_went_ok(true);
                        response->set_response( "Entity updated" );

                        if (delete_locked_by)
                        {
                            QMetaObject::invokeMethod(entity, "set_locked_by", Q_ARG(QString, ""));
                        }
                    }
                }
            }

            qDebug() << "<" << Q_FUNC_INFO;

            return response;
        }
        /**
         * @brief delete_doc Borra un documento de CouhDB.
         * @param document Objeto que extiende QObject.
         * @return QJsonObject con la respuesta del servidor.
         */
        template <class T>
        CouchDBManager::DBManagerResponse* delete_doc(T* entity)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            CouchDBManager::DBManagerResponse* response = new CouchDBManager::DBManagerResponse(this);
            const char* entityClass = entity->metaObject()->className();
            const char* className = entity->staticMetaObject.className();

            qDebug() << "# Entity:" << entityClass << "derived from:" << className;

            if (!this->is_base_entity(entity))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
                response->set_went_ok(false);
                response->set_response(err);
            }
            else
            {
                if (this->is_versionable_entity(entity))
                {
                    // Comprueba si la conexión es remota
                    if (this->get_remote_conn())
                    {
                        // Comprueba que la entidad está bloqueada por el usuario actual
                        if (!this->is_locked_by_user<T>(entity))
                        {
                            QString err = QString(Q_FUNC_INFO) + " FATAL The entity is not locked by the current user.";

                            this->set_error_string(err);

                            qCritical() << err;
                            qDebug() << "<" << Q_FUNC_INFO;
                            response->set_went_ok(false);
                            response->set_response(err);

                            return response;
                        }
                    }
                }

                QString id = entity->get_id(); //json_object["_id"].toString();
                QString rev = entity->get_rev(); //json_object["_rev"].toString();

                qDebug() << "# Entity _id: " << id << "_rev: " << rev;

                QString result = this->do_delete( id, "rev=" + rev );

                qDebug() << "# do_delete: " << result;

                if (result.isNull() || result.isEmpty())
                {
                    qDebug() << "#Errors?: " << this->error_string;

                    response->set_went_ok(false);
                    response->set_response(this->error_string);
                }
                else {
                    response->set_went_ok(true);
                    response->set_response("Deleted");
                }
            }

            return response;
        }

        /**
         * @brief view Ejecuta una vista de CouchDB.
         * @param design_document Nombre del Design Document donde está la vista.
         * @param view Nombre de la vista.
         * @return Lista de Qvariant con cada fila de doscolumnas: "key" y "value".
         */
        template <class T>
        void view(const QString& design_document, const QString& view, QList<T*>& result_list)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            this->view(design_document, view, QString(), result_list);

            qDebug() << "<" << Q_FUNC_INFO;
        }
        /**
         * @brief view Ejecuta una vista de CouchDB. Se pueden enviar parámetros.
         * @param design_document Nombre del Design Document donde está la vista.
         * @param view Nombre de la vista.
         * @param query_string Especifica una lista de parámetros, con formato QueryString.
         * @return Lista de Qvariant con cada fila de dos o tres columnas: "key", "value" y, opcionalmente, "doc".
         */
        template <class T>
        void view(const QString& design_document, const QString& view, const QString& query_string, QList<T*>& result_list)
        {
            qDebug() << ">" << Q_FUNC_INFO;
            qDebug() << "# Design Document: " << design_document << "View: " << view << "Query String: " << query_string;

            QString partial = "_design/" + design_document + "/_view/" + view;
            QString result = this->do_get(partial, query_string);

            qDebug() << "# do_get: " << result;

            if (result.isNull() || result.isEmpty())
            {
                qDebug() << "# Errors?: " << this->error_string;
                qDebug() << "<" << Q_FUNC_INFO;

                return;
            }

            QJsonParseError json_parse_error;
            QJsonObject result_json = this->string_2_json_object(result, &json_parse_error);

            if (json_parse_error.error != QJsonParseError::NoError)
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

                this->set_error_string(err);
                qCritical() << err;
//                qFatal(err.toStdString().c_str());

                return;
            }

            QJsonArray docs_array = result_json["rows"].toArray();

            for (int i = 0; i < docs_array.size(); ++i)
            {
                // VBG - 20161130: SE CAMBIA EL PARENT A this->parent() SEGÚN EL CORREO DE Jon Gurrutxaga del 23/11/2016
                T* entity = new T(this->parent());

                entity->read(docs_array[i]["doc"].toObject());
                result_list.append(entity);
            }

            qDebug() << "<" << Q_FUNC_INFO;
        }

        /**
         * @brief list Ejecuta una función de listado de CouchDB.
         * @param design_document Nombre del Design Document donde está la función de listado.
         * @param list Nombre de la función de listado.
         * @param view Nombre de la vista sobre la que se ejecuta la función de listado.
         * @return Lista de Qvariant con la representación de cada objeto JSON encontrado.
         */
        template <class T>
        void list(const QString& design_document, const QString& list, const QString& view, QList<T*>& result_list)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            this->list<T>(design_document, list, view, QString(), result_list);

            qDebug() << "<" << Q_FUNC_INFO;
        }
        /**
         * @brief list Ejecuta una función de listado de CouchDB. Se pueden establecer filtros.
         * @param design_document Nombre del Design Document donde está la función de listado.
         * @param list Nombre de la función de listado.
         * @param view Nombre de la vista sobre la que se ejecuta la función de listado.
         * @param where QSring con una consulta PQL.
         * PQL: Es una mezcla de SQL básico y JavaScript, dónde se pueden realizar consultas
         * con los operadores de ambos lenguajes. Los valores son case-insentive.
         * Ejplo: name like 'nombre' and (descripcion != 'descripcion' or descripcion like '\s+[d|e].*')
         * El último valor es una expresión regular JavaScript, que siempre debe combinarse con el operador like.
         * @return Lista de Qvariant con la representación de cada objeto JSON encontrado.
         */
        template <class T>
        void list(const QString& design_document, const QString& list, const QString& view, const QString& where, QList<T*>& result_list)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            this->list<T>(design_document, list, view, where, QString(), result_list);

            qDebug() << "<" << Q_FUNC_INFO;
        }
        /**
         * @brief list Ejecuta una función de listado de CouchDB. Se pueden establecer filtros.
         * @param design_document Nombre del Design Document donde está la función de listado.
         * @param list Nombre de la función de listado.
         * @param view Nombre de la vista sobre la que se ejecuta la función de listado.
         * @param where QSring con una consulta PQL.
         * PQL: Es una mezcla de SQL básico y JavaScript, dónde se pueden realizar consultas
         * con los operadores de ambos lenguajes. Los valores son case-insentive.
         * Ejplo: name like 'nombre' and (descripcion != 'descripcion' or descripcion like '\s+[d|e].*')
         * El último valor es una expresión regular JavaScript, que siempre debe combinarse con el operador like.
         * @param query_str Especifica una lista de parámetros, con formato QueryString.
         * @return Lista de Qvariant con la representación de cada objeto JSON encontrado.
         */
        template <class T>
        void list(const QString& design_document, const QString& list, const QString& view, const QString& where, const QString& query_str, QList<T*>& result_list)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            QString query_string = "include_docs=true&format=json";

            if (this->is_relation_entity_type<T*>())
            {
                query_string.append("&simple_relations=true");
            }

            if (!query_str.isNull() && !query_str.isEmpty())
            {
                query_string.append("&");
                query_string.append(query_str);
            }

            qDebug() << "# Design Document: " << design_document << "List: " << list << "View: " << view << "Where: " << where;

            if (!where.isNull() && !where.isEmpty())
            {
                query_string += "&where=" + QUrl::toPercentEncoding(where);
            }

            QString partial = "_design/commons/_list/" + list + "/" + design_document + "/" + view;
            QString result = this->do_get(partial, query_string);

            qDebug() << "# do_get: " << result;

            if (result.isNull() || result.isEmpty())
            {
                qDebug() << "# Errors?: " << this->error_string;
                qDebug() << "<" << Q_FUNC_INFO;

                return;
            }

            QJsonParseError json_parse_error;
            QJsonObject result_json = this->string_2_json_object(result, &json_parse_error);

            if (json_parse_error.error != QJsonParseError::NoError)
            {
                Q_ASSERT(0);
                QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

                this->set_error_string(err);
                qCritical() << err;
//                qFatal(err.toStdString().c_str());

                return;
            }

            QJsonArray docs_array = result_json["rows"].toArray();

            for (int i = 0; i < docs_array.size(); ++i)
            {
                // VBG - 20161130: SE CAMBIA EL PARENT A this->parent() SEGÚN EL CORREO DE Jon Gurrutxaga del 23/11/2016
                T* entity = new T(this->parent());

                entity->read(docs_array[i].toObject());
                result_list.append(entity);
            }

            qDebug() << "<" << Q_FUNC_INFO;
        }

        /**
         * @brief lock Bloquea una entidad.
         * @param entity Clase que deriva de VersionableEntity.
         * @return true si la entidad se ha bloqueado.
         */
        template <class T>
        bool lock(T* entity)
        {
            assert((CouchDBManager::VersionableEntity const*)&entity);
            qDebug() << ">" << Q_FUNC_INFO;

            CouchDBManager::UserContext* uc = this->user_context();
            bool went_ok = this->lock<T>(entity, uc->get_userCtx().name);

            delete uc;

            qDebug() << "<" << Q_FUNC_INFO;

            return went_ok;
        }/**
         * @brief lock Bloquea una entidad.
         * @param entity Clase que deriva de VersionableEntity.
         * @param locked_by Nombre de usuario que realiza el bloqueo de la entidad.
         * @return true si la entidad se ha bloqueado.
         */
        template <class T>
        bool lock(T* entity, const QString& locked_by)
        {
            assert((CouchDBManager::VersionableEntity const*)&entity);
            qDebug() << ">" << Q_FUNC_INFO;

            if (!this->is_versionable_entity(entity))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
                qDebug() << ">" << Q_FUNC_INFO;

                return false;
            }

            bool went_ok = false;
            bool ent_locked;
            QString ent_locked_by;

            QMetaObject::invokeMethod(entity, "get_locked", Q_RETURN_ARG(bool, ent_locked));
            QMetaObject::invokeMethod(entity, "get_locked_by", Q_RETURN_ARG(QString, ent_locked_by));

            if (!ent_locked)
            {
                QMetaObject::invokeMethod(entity, "set_locked", Q_ARG(bool, true));
                QMetaObject::invokeMethod(entity, "set_locked_by", Q_ARG(QString, locked_by));

                CouchDBManager::DBManagerResponse* resp = this->update<T>(entity);
                went_ok = resp->get_went_ok();

                if (!went_ok)
                {
                    QMetaObject::invokeMethod(entity, "set_locked", Q_ARG(bool, ent_locked));
                    QMetaObject::invokeMethod(entity, "set_locked_by", Q_ARG(QString, ent_locked_by));
                }

                delete resp;
            }
            else if (this->is_locked_by_user<T>(entity, locked_by))
            {
                QString err = QString(Q_FUNC_INFO) + " WARN Entity is already locked.";

//                this->set_error_string(err);

                qWarning() << err;
            }
            else
            {
                QString err = QString(Q_FUNC_INFO) + " ERROR Entity is already locked by another user.";

                this->set_error_string(err);

                qCritical() << err;
            }

            qDebug() << "<" << Q_FUNC_INFO;

            return went_ok;
        }
        /**
         * @brief unlock Desbloquea una entidad.
         * @param entity Clase que deriva de VersionableEntity.
         * @return true si la entidad se ha desbloqueado.
         */
        template <class T>
        bool unlock(T* entity)
        {
            assert((CouchDBManager::VersionableEntity const*)&entity);
            qDebug() << ">" << Q_FUNC_INFO;

            CouchDBManager::UserContext* uc = this->user_context();
            bool went_ok = this->unlock<T>(entity, uc->get_userCtx().name);

            delete uc;

            qDebug() << "<" << Q_FUNC_INFO;

            return went_ok;
        }
        /**
         * @brief unlock Desbloquea una entidad.
         * @param entity Clase que deriva de VersionableEntity.
         * @param locked_by Nombre de usuario que realiza el desbloqueo de la entidad.
         * @return true si la entidad se ha desbloqueado.
         */
        template <class T>
        bool unlock(T* entity, const QString& locked_by)
        {
            assert((CouchDBManager::VersionableEntity const*)&entity);
            qDebug() << ">" << Q_FUNC_INFO;

            if (!this->is_versionable_entity(entity))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
                qDebug() << ">" << Q_FUNC_INFO;

                return false;
            }

            bool went_ok = false;
            bool ent_locked;
            QString ent_locked_by;

            QMetaObject::invokeMethod(entity, "get_locked", Q_RETURN_ARG(bool, ent_locked));
            QMetaObject::invokeMethod(entity, "get_locked_by", Q_RETURN_ARG(QString, ent_locked_by));

            if (this->is_locked_by_user<T>(entity, locked_by))
            {
                QMetaObject::invokeMethod(entity, "set_locked", Q_ARG(bool, false));
//                QMetaObject::invokeMethod(entity, "set_locked_by", Q_ARG(QString, ""));

                CouchDBManager::DBManagerResponse* resp = this->update<T>(entity);
                went_ok = resp->get_went_ok();

                if (!went_ok)
                {
                    QMetaObject::invokeMethod(entity, "set_locked", Q_ARG(bool, ent_locked));
                    QMetaObject::invokeMethod(entity, "set_locked_by", Q_ARG(QString, ent_locked_by));
                }
                else
                {
                    QMetaObject::invokeMethod(entity, "set_locked_by", Q_ARG(QString, ""));
                }

                delete resp;
            }
            else if (ent_locked)
            {
                QString err = QString(Q_FUNC_INFO) + " ERROR Entity locked by another user.";

                this->set_error_string(err);

                qCritical() << err;
            }
            else
            {
                QString err = QString(Q_FUNC_INFO) + " WARN Entity is already unlocked.";

//                this->set_error_string(err);

                qWarning() << err;
            }

            qDebug() << "<" << Q_FUNC_INFO;

            return went_ok;
        }
        /**
         * @brief is_locked Verifica si la entidad está bloqueada por el usuario actual.
         * @param entity Clase que deriva de VersionableEntity.
         * @return true si la entidad está bloqueada por el usuario actual.
         */
        template <class T>
        bool is_locked_by_user(T* entity)
        {
            assert((CouchDBManager::VersionableEntity const*)&entity);
            qDebug() << ">" << Q_FUNC_INFO;

            CouchDBManager::UserContext* uc = this->user_context();
            bool is_locked = this->is_locked_by_user<T>(entity, uc->get_userCtx().name);

            delete uc;

            qDebug() << "<" << Q_FUNC_INFO;

            return is_locked;
        }
        /**
         * @brief is_locked Verifica si la entidad está bloqueada por el usuario actual.
         * @param entity Clase que deriva de VersionableEntity.
         * @param locked_by Nombre de usuario para comprobar el bloqueo de la entidad.
         * @return true si la entidad está bloqueada por el usuario actual.
         */
        template <class T>
        bool is_locked_by_user(T* entity, const QString& locked_by)
        {
            assert((CouchDBManager::VersionableEntity const*)&entity);
            qDebug() << ">" << Q_FUNC_INFO;

            if (!this->is_versionable_entity(entity))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
                qDebug() << ">" << Q_FUNC_INFO;

                return false;
            }

            bool ent_locked;
            QString ent_locked_by;
            QMetaObject::invokeMethod(entity, "get_locked", Q_RETURN_ARG(bool, ent_locked));
            QMetaObject::invokeMethod(entity, "get_locked_by", Q_RETURN_ARG(QString, ent_locked_by));
            bool is_locked = ent_locked && ent_locked_by == locked_by;

            qDebug() << "<" << Q_FUNC_INFO;

            return is_locked;
        }

        /**
         * @brief bulk_docs Ejecuta un envío masivo de documentos en una sóla transacción.
         * @param documents Lista de objetos que extienden QObject.
         * @return QJsonObject con la respuesta del servidor.
         */
        QJsonObject bulk_docs(QList<QObject*> documents);

        /**
         * @brief create_database Crea una base de datos.
         * @param name Nombre de la base de datos.
         * @return true si se ha creado correctamente.
         */
        bool create_database(const QString& name);
        /**
         * @brief delete_database Borra una base de datos.
         * @param name Nombre de la base de datos.
         * @return true si se ha borrado correctamente.
         */
        bool delete_database(const QString& name);
        /**
         * @brief read_database_security Recupera la seguridad de la base de datos actual.
         * @param admins Recupera los roles administradores de la base de datos.
         * @param users Recupera los roles usuarios de la base de datos.
         */
        void read_database_security(QStringList& admins, QStringList& users);
        /**
         * @brief update_database_security Actualiza la seguridad de una base de datos.
         * @param admins Lista de roles que accederán como administradores.
         * @param users Lista de roles que accederán como usuarios.
         * @return  true si se ha actualizado la base de datos.
         */
        bool update_database_security(const QStringList admins, const QStringList users);

        /**
         * @brief uuids Genera UUIDS desde CouchDB.
         * @param num Número de UUIDS a generar.
         * @return QJsonObject con la respuesta del servidor.
         */
        QStringList uuids(int num);

        /**
         * @brief login Inicia sesión con un usuario de CouchDB.
         * @param username Nombre de usuario.
         * @param password Clave de acceso del usuario.
         * @return true si ha iniciado sesión correctamente.
         */
        bool login(const QString& username, const QString& password, QStringList& roles);
        /**
         * @brief user_context Recupera el contexto del usuario de CouchDB.
         * @return UserContext con el contexto del usuario.
         */
        CouchDBManager::UserContext* user_context();
        /**
         * @brief logout Cierra la sesión del usuario de CouchDB.
         * @return true si ha cerrado sesión correctamente.
         */
        bool logout();
        /**
         * @brief read_user Recupera un objeto usuario de CouchDB.
         * @param id Identificador único de usuario.
         * @param user Instancia de un objeto derivado de BaseEntity.
         */
        template <class T>
        void read_user(const QString& id, T* user)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            const char* userClass = user->metaObject()->className();
            const char* className = user->staticMetaObject.className();

            qDebug() << "# User:" << userClass << "derived from:" << className;

            if (!this->is_base_user(user))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
//                qFatal(err.toStdString().c_str());

                return;
            }

            QString partial = "_users/";

            if (!id.startsWith("org.couchdb.user:"))
            {
                partial.append("org.couchdb.user:");
            }

            QString result = this->do_get(partial + id, QString(), false);

            qDebug() << "# do_get: " << result;

            if (result.isNull() || result.isEmpty())
            {
                qDebug() << "# Errors?: " << this->error_string;

                return;
            }

            QJsonParseError json_parse_error;
            QJsonObject json = this->string_2_json_object(result, &json_parse_error);

            if (json_parse_error.error != QJsonParseError::NoError)
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

                this->set_error_string(err);
                qCritical() << err;

                return;
            }

            if (json.contains("password"))
            {
                json.remove("password");
            }

            user->read(json);

            qDebug() << "<" << Q_FUNC_INFO;
        }

        /**
         * @brief save_user Crea o modifica un objeto usuario en CouchDB.
         * @param user Objeto que deriva de BaseUsery.
         * @return true si se ha guardado el usuario.
         */
        template <class T>
        bool save_user(T* user)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            if (!this->is_base_user(user))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
//                qFatal(err.toStdString().c_str());

                return false;
            }

            QJsonObject user_json;
            this->object_2_json<T>(user, user_json);

            if (!user_json.contains("name") || user_json["name"].toString().isEmpty())
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL BaseUser requires a name";

                this->set_error_string(err);

                qCritical() << err;
//                qFatal(err.toStdString().c_str());

                return false;
            }

            if (!user_json.contains("_id") || user_json["_rev"].toString().isEmpty() || !user_json["_id"].toString().startsWith("org.couchdb.user:"))
            {
                QString _id = QString("org.couchdb.user:") + user_json["name"].toString();

                user_json.insert("_id", QJsonValue(_id) );
                user->set_id(_id);

                if (user_json.contains("_rev") && user_json["_rev"].toString().isEmpty())
                {
                    user_json.remove("_rev");
                }
            }

            if (user_json.contains("password") && user_json["password"].toString().isEmpty())
            {
                user_json.remove("password");
            }

            QString json_string = this->json_object_2_string(user_json);
            QString result = this->do_post(json_string, "_users", false);

            if (result.isNull() || result.isEmpty())
            {
                qDebug() << "# Errors?: " << this->error_string;

                return false;
            }

            QJsonParseError json_parse_error;
            QJsonObject result_json = this->string_2_json_object(result, &json_parse_error);

            if (json_parse_error.error != QJsonParseError::NoError)
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

                this->set_error_string(err);
                qCritical() << err;
//                qFatal(err.toStdString().c_str());
            }

            qDebug() << "<" << Q_FUNC_INFO;

            return result_json.contains("ok") && result_json["ok"].toBool();
        }
        /**
         * @brief delete_user Elimina un objeto usuario de CouchDB.
         * @param user Objeto que deriva de BaseEntity.
         * @return true si se ha borrado el usuario.
         */
        template <class T>
        bool delete_user(T* user)
        {
            qDebug() << ">" << Q_FUNC_INFO;

            if (!this->is_base_user(user))
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL Incorrect Document Type";

                this->set_error_string(err);

                qCritical() << err;
//                qFatal(err.toStdString().c_str());

                return false;
            }


            QString result = this->do_delete("_users/" + user->get_id(), "rev=" + user->get_rev(), false);

            if (result.isNull() || result.isEmpty())
            {
                qDebug() << "# Errors?: " << this->error_string;

                return false;
            }

            QJsonParseError json_parse_error;
            QJsonObject result_json = this->string_2_json_object(result, &json_parse_error);

            if (json_parse_error.error != QJsonParseError::NoError)
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

                this->set_error_string(err);
                qCritical() << err;
//                qFatal(err.toStdString().c_str());
            }

            qDebug() << "<" << Q_FUNC_INFO;

            return result_json.contains("ok") && result_json["ok"].toBool();
        }

        /** Ejecuta una función de listado de usuarios de CouchDB. Se pueden establecer filtros.
         * @param design_document Nombre del Design Document donde est? la funci?n de listado.
         * @param list Nombre de la funci?n de listado.
         * @param view Nombre de la vista sobre la que se ejecuta la funci?n de listado.
         * @param where QSring con una consulta PQL.
         * PQL: Es una mezcla de SQL b?sico y JavaScript, d?nde se pueden realizar consultas
         * con los operadores de ambos lenguajes. Los valores son case-insentive.
         * Ejplo: name like 'nombre' and (descripcion != 'descripcion' or descripcion like '\s+[d|e].*')
         * El ?ltimo valor es una expresi?n regular JavaScript, que siempre debe combinarse con el operador like.
         * @return Lista de Qvariant con la representaci?n de cada objeto JSON encontrado.
         */
        template <class T>
        void list_users(const QString& where, QList<T*>& result_list)
        {
            qDebug() << ">" << Q_FUNC_INFO;
            QString partial = "_all_docs";

            QString query_string = "startkey=\"org.couchdb.user:\"&include_docs=true&format=json";

            qDebug() << "# Where: " << where;

            if (!where.isNull() && !where.isEmpty())
            {
                query_string += "&where=" + QUrl::toPercentEncoding(where);
            }


            QString result = this->do_get(partial, query_string);

            qDebug() << "# do_get: " << result;

            if (result.isNull() || result.isEmpty())
            {
                qDebug() << "# Errors?: " << this->error_string;
                qDebug() << "<" << Q_FUNC_INFO;

                return;
            }

            QJsonParseError json_parse_error;
            QJsonObject result_json = this->string_2_json_object(result, &json_parse_error);

            if (json_parse_error.error != QJsonParseError::NoError)
            {
                QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

                this->set_error_string(err);
                qCritical() << err;

                return;
            }

            QJsonArray docs_array = result_json["rows"].toArray();

            for (int i = 0; i < docs_array.size(); ++i)
            {
                QJsonObject json = docs_array[i].toObject();
                //TODO: ¿? descartar usuarios de tipo _admin
                if (json.contains("password"))
                {
                    json.remove("password");
                }

                // VBG - 20161130: SE CAMBIA EL PARENT A this->parent() SEGÚN EL CORREO DE Jon Gurrutxaga del 23/11/2016
                T* entity = new T(this->parent());
                entity->read(json["doc"].toObject());
                result_list.append(entity);
            }

            qDebug() << "<" << Q_FUNC_INFO;
        }

        /**
         * @brief replicate Inicia una replicación según la configuración proporcionada.
         * @param repl_config Configuración de la replicación.
         * @return true si la replicación ha funcionado correctamente.
         */
        bool replicate(CouchDBManager::ReplicationConfig* repl_config);
        /**
         * @brief add_replication_service Añade un servicio de replicación, según la configuración proporcionada.
         * @param repl_config Configuración de la replicación.
         * @return true si se ha añadido el servicio correctamente.
         */
        bool add_replication_service(CouchDBManager::ReplicationConfig* repl_config);
        /**
         * @brief read_replication_service lee un servicio de replicación.
         * @param repl_config Configuración de la replicación.
         * @return Configuración del servicio de replicación.
         */
        CouchDBManager::ReplicationConfig* read_replication_service(const QString& id);
        /**
         * @brief update_replication_service actualiza un servicio de replicación, según la configuración proporcionada.
         * @param repl_config Configuración de la replicación.
         * @return true si se ha actualizado el servicio correctamente.
         */
        bool update_replication_service(CouchDBManager::ReplicationConfig* repl_config);
        /**
         * @brief remove_replication_service Elimina un servicio de replicación.
         * @param id Identificación del servicio de replicación.
         * @return true si se ha eliminado el servicio correctamente.
         */
        bool remove_replication_service(const QString& id);
        /**
         * @brief list_active_tasks Lista las tareas activas en CouchDB.
         * @return QList de ActiveTask.
         */
        QList<CouchDBManager::ActiveTask*> list_active_tasks();

        /**
         * @brief toUnixTimeStamp Obtiene un timestamp a partir de un objeto QDateTime.
         * @param datetime Objeto QDateTime.
         * @return qint64 con el timestamp.
         */
        static qint64 toUnixTimeStamp(const QDateTime& datetime);
        /**
         * @brief toUnixTimeStamp Obtiene un timestamp a partir de un QString que contiene una fecha/hora.
         * @param date_string QString que contiene una fecha/hora.
         * @return qint64 con el timestamp.
         */
        static qint64 toUnixTimeStamp(const QString& date_string);
        /**
         * @brief toUnixTimeStamp Obtiene un timestamp a partir de un QString que contiene una fecha/hora en el formato especificado.
         * @param date_string QString que contiene una fecha/hora.
         * @param format Formato de la fecha.
         * @return qint64 con el timestamp.
         */
        static qint64 toUnixTimeStamp(const QString& date_string, const QString& format);
        /**
         * @brief toDateTime Obtiene un objeto QDateTime a partir de un timestamp.
         * @param timestamp Timestamp con formato unix.
         * @return QDateTime con la fecha del timestamp.
         */
        static QDateTime toDateTime(qint64 timestamp);
        /**
         * @brief toDateTimeObtiene un objeto QDateTime a partir de un QString que contiene una fecha/hora.
         * @param date_string QString que contiene una fecha/hora.
         * @return QDateTime con la fecha del QString.
         */
        static QDateTime toDateTime(const QString& date_string);
        /**
         * @brief toDateTimeObtiene un objeto QDateTime a partir de un QString que contiene una fecha/hora en el formato especificado.
         * @param date_string QString que contiene una fecha/hora.
         * @param format Formato de la fecha.
         * @return QDateTime con la fecha del QString.
         */
        static QDateTime toDateTime(const QString& date_string, const QString& format);
        /**
         * @brief toDateString Obtiene un QString con la representación de la fecha de un timestamp.
         * @param timestamp Timestamp con formato unix.
         * @return QString con la representación de la fecha.
         */
        static QString toDateString(qint64 timestamp);
        /**
         * @brief toDateString Obtiene un QString con la representación de la fecha de un objeto QDateTime.
         * @param datetime Objeto QDateTime.
         * @return QString con la representación de la fecha.
         */
        static QString toDateString(const QDateTime& datetime);
        /**
         * @brief today Obtiene la fecha/hora actual como un timestamp.
         * @return qint64 con el timestamp.
         */
        static qint64 todayTimestamp();
        /**
         * @brief today Obtiene la fecha/hora actual como un objeto QDateTime.
         * @return QDateTime con la fecha/hora actual.
         */
        static QDateTime todayDateTime();
        /**
         * @brief today Obtiene un QString con la representación de la fecha/hora actual.
         * @return QString con la representación de la fecha/hora actual.
         */
        static QString todayDateString();
        /**
         * @brief formatDate Obtiene un QString con la representación de la fecha/hora del QDateTime,
         * en el formato especificado en el QString.
         * @param datetime Objeto QDateTime.
         * @param format QString con el formato.
         * @return QString con la fecha formateada según el esquema proporcionado.
         */
        static QString formatDate(const QDateTime& datetime, const QString& format);
        /**
         * @brief formatDate Obtiene un QString con la representación de la fecha/hora del timestamp,
         * en el formato especificado en el QString.
         * @param timestamp qin64 con el timestamp.
         * @param format QString con el formato.
         * @return QString con la fecha formateada según el esquema proporcionado.
         */
        static QString formatDate(qint64 timestamp, const QString& format);

        /**
         * @brief base64_encode Codifica una cadena en Base64.
         * @param str Cadena a codificar.
         * @return QString con la cadena codificada.
         */
        static QString base64_encode(const QString& str);
        /**
         * @brief base64_decode Decodifica una cadena en Base64.
         * @param str Cadena a decodificar.
         * @return QString con la cadena decodificada.
         */
        static QString base64_decode(const QString& str);

    private slots:
        void network_read();
        void network_timeout();

    signals:
        void network_read_timeout();
    };
}

#endif // COUCHDBMANAGER_H
//! @}
