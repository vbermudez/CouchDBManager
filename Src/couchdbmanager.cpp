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
// REVISION: ........ $Revision: 1.23 $ ($Date: 2017/02/26 23:55:25 $)
//
// REFERENCE: .......
//
// CHANGES:
//
// $Log: couchdbmanager.cpp,v $
// Revision 1.23  2017/02/26 23:55:25  jgurrutxaga
// Corregir la gestión del timeout y la terminación de la petición al servidor. Liberar recursos en caso de timeout.
//
// Revision 1.22  2017/02/24 09:37:30  jgurrutxaga
// Disable "read timeout" when readyRead signal is received from QNetworkReply
//
// Revision 1.21  2017/02/22 15:32:36  jgurrutxaga
// Error en la gesti?el timeout provoca que la respuesta del servidor quede entrecortada
//
// Revision 1.20  2017/01/19 20:47:41  jgurrutxaga
// Uso de referencias en foreach
//
// Revision 1.19  2016/12/20 09:01:16  jmonasterio
// - BaseReferenceSimple added
//
// Revision 1.18  2016/12/05 13:15:26  jmonasterio
// - Victor updates: Replication service
//
// Revision 1.17  2016/11/17 14:57:54  jmonasterio
// - Victor fixes
//
// Revision 1.16  2016/11/15 10:52:49  jmonasterio
// - Victor updates
//
// Revision 1.15  2016/10/03 07:11:59  jgurrutxaga
// -A?r la funci?egisterTypes()al API de CouchDBManager para el registro de las entidades.
// -A?r el password al guradar los datos de un usuario y evitar la lectura de este campo.
//
// Revision 1.14  2016/09/27 15:51:36  jgurrutxaga
// - Mantener la sesi?iva.
// - Modificar Ტol de objetos derivados de QObject para facilitar un poco la gesti?e memoria
// - Modificaciones BaseReference
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
#include "couchdbmanager.h"
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QByteArray>
#include <QVariantMap>
#include <QUrl>
#include <QNetworkCookie>
#include <QDate>
#include <QByteArray>

const QString CouchDBManager::DBManager::DATE_FORMAT = "dd/MM/yyyy";
const QString CouchDBManager::DBManager::DATE_TIME_FORMAT = "dd/MM/yyyy HH:mm:ss";

/*
 * CouchDBManager::DBManager
 */
//CouchDBManager::DBManager::DBManager(const QString& address, const QString& db_name)
//{
//    qDebug() << "@" << Q_FUNC_INFO;

//    server_address = address;
//    database_name = db_name;
//}

void CouchDBManager::DBManager::initialize()
{
    qDebug() << ">" << Q_FUNC_INFO;

    this->manager = new QNetworkAccessManager(this);
    this->timeout = 10000;
    this->nw_read = false;
    this->nw_timeout = false;

    this->registerType<CouchDBManager::ActiveTask*>();
    this->registerType<CouchDBManager::DBManagerResponse*>();
    this->registerType<CouchDBManager::UserContext*>();

    this->registerType<CouchDBManager::BaseObject*>();
    this->registerType<CouchDBManager::BaseUser*>();
    this->registerType<CouchDBManager::BaseReference*>();
    this->registerType<CouchDBManager::BaseReferenceSimple*>();
    this->registerType<CouchDBManager::BaseEntity*>();
    this->registerType<CouchDBManager::VersionableEntity*>();
    this->registerType<CouchDBManager::RelationEntity*>();
    this->registerType<CouchDBManager::ServerResource*>();
    this->registerType<CouchDBManager::ReplicationConfig*>();

    this->set_remote_conn();

    qDebug() << "<" << Q_FUNC_INFO;
}

QString CouchDBManager::DBManager::get_url()
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->get_url(QString(), QString());
}

QString CouchDBManager::DBManager::get_url(bool only_root)
{
    qDebug() << "><" << Q_FUNC_INFO;

    if (only_root)
    {
        return server_address + "/";
    }

    return this->get_url(QString(), QString());
}

QString CouchDBManager::DBManager::get_url(const QString& partial, bool use_context)
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->get_url(partial, QString(), use_context);
}

QString CouchDBManager::DBManager::get_url(const QString& partial)
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->get_url(partial, QString());
}

QString CouchDBManager::DBManager::get_url(const QString& partial, const QString& query_string)
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->get_url(partial, query_string, partial != this->database_name);
}

QString CouchDBManager::DBManager::get_url(const QString& partial, const QString& query_string, bool use_context)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QString url = this->server_address + (use_context ? "/" + this->database_name : "");

    if (!partial.isNull())
    {
        url += "/" + partial;
    }

    if (!query_string.isNull())
    {
        url += "?" + query_string;
    }

    qDebug() << "# URL:" << url;

    qDebug() << "<" << Q_FUNC_INFO;

    return url;
}

QString CouchDBManager::DBManager::get_database_name() const
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->database_name;
}

void CouchDBManager::DBManager::set_database_name(const QString& db_name)
{
    qDebug() << "><" << Q_FUNC_INFO;

    this->database_name = db_name;
}

QString CouchDBManager::DBManager::get_server_address() const
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->server_address;
}

void CouchDBManager::DBManager::set_server_address(const QString& address)
{
    qDebug() << "><" << Q_FUNC_INFO;

    this->server_address = address;
    this->set_remote_conn();
}

QString CouchDBManager::DBManager::get_auth_cookie() const
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->auth_cookie;
}

void CouchDBManager::DBManager::set_auth_cookie(const QString& a_auth_cookie)
{
    qDebug() << "><" << Q_FUNC_INFO;

    this->auth_cookie = a_auth_cookie;
}

void CouchDBManager::DBManager::set_remote_conn(bool a_remote_conn)
{
    qDebug() << "><" << Q_FUNC_INFO;

    this->remote_conn = a_remote_conn;
}

void CouchDBManager::DBManager::set_remote_conn()
{
    qDebug() << ">" << Q_FUNC_INFO;

    QHostInfo hostInfo;
    QString hostName = QHostInfo::localHostName();
    QString serverAddr = this->get_server_address();
    QStringList localNames;

    this->remote_conn = false;

    if (serverAddr.isEmpty() || serverAddr.isNull())
    {
        return;
    }

    QString serverHost = QUrl(this->get_server_address()).host();

    localNames.append(hostName);
    localNames.append("localhost");
    localNames.append("127.0.0.1");

    if (!hostInfo.addresses().isEmpty())
    {
        foreach (QHostAddress addr, hostInfo.addresses()) {
            localNames.append(addr.toString());
        }
    }

    if (!localNames.contains(serverHost, Qt::CaseInsensitive))
    {
        this->remote_conn = true;
    }

    qDebug() << "<" << Q_FUNC_INFO;
}

bool CouchDBManager::DBManager::get_remote_conn() const
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->remote_conn;
}

QNetworkReply::NetworkError CouchDBManager::DBManager::get_network_error() const
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->network_error;
}

void CouchDBManager::DBManager::set_network_error(const QNetworkReply::NetworkError a_error)
{
    qDebug() << "><" << Q_FUNC_INFO;

    this->network_error = a_error;
}

QString CouchDBManager::DBManager::get_error_string() const
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->error_string;
}

void CouchDBManager::DBManager::set_error_string(const QString& a_error_string)
{
    qDebug() << "><" << Q_FUNC_INFO;

    this->error_string = a_error_string;
}

qint64 CouchDBManager::DBManager::get_timeout() const
{
    return this->timeout;
}

void CouchDBManager::DBManager::set_timeout(qint64 a_timeout)
{
    this->timeout = a_timeout;
}

bool CouchDBManager::DBManager::is_online()
{
    qDebug() << ">" << Q_FUNC_INFO;

    bool is_online = true;

    QString result = this->do_get(QString(), QString(), true);

    if (this->network_error != QNetworkReply::NoError || !this->error_string.isNull() || !this->error_string.isEmpty())
    {
        is_online = false;
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return is_online;
}

bool CouchDBManager::DBManager::is_online(const QString &a_server, const QString &a_database)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QString current_address = this->server_address;
    QString current_db_name = this->database_name;
    bool is_online = true;

    this->server_address = a_server;
    this->database_name = a_database;

    QString result = this->do_get(QString(), QString(), true);

    if (this->network_error != QNetworkReply::NoError || !this->error_string.isNull() || !this->error_string.isEmpty())
    {
        is_online = false;
    }

    this->server_address = current_address;
    this->database_name = current_db_name;

    qDebug() << "<" << Q_FUNC_INFO;

    return is_online;
}

void CouchDBManager::DBManager::set_common_headers(QNetworkRequest *request)
{
    qDebug() << "><" << Q_FUNC_INFO;

    this->set_common_headers(request, true);
}

void CouchDBManager::DBManager::set_common_headers(QNetworkRequest *request, bool use_auth_cookie)
{
    qDebug() << ">" << Q_FUNC_INFO;

    request->setRawHeader("Accept", "application/json,text/html,text/plain");

    if (use_auth_cookie && !this->get_auth_cookie().isNull() && !this->get_auth_cookie().isEmpty())
    {
        qDebug() << "# Setting Auth Cookie" << this->get_auth_cookie();
        QNetworkCookie cookie;

        cookie.setName("AuthSession");
        cookie.setPath("/");
        cookie.setValue(this->get_auth_cookie().toUtf8());
        cookie.setHttpOnly(true);
        cookie.setDomain(this->get_server_address());
        request->setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue<QNetworkCookie>(cookie));
    }

    qDebug() << "<" << Q_FUNC_INFO;
}

QString CouchDBManager::DBManager::request(const QUrl& url, const QByteArray& ba_data, const QString& partial, CouchDBManager::DBManager::HTTP_METHOD method)
{
    qDebug() << ">" << Q_FUNC_INFO;

    this->reset_errors();

    QNetworkRequest request(url);
    QNetworkReply *reply = NULL;

    bool use_auth_cookie = (partial != "_session");
    this->set_common_headers(&request, use_auth_cookie);

    switch (method)
    {
        case GET:
            reply = this->manager->get(request);

            break;

        case POST:
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
            request.setHeader(QNetworkRequest::ContentLengthHeader, ba_data.length());

            reply = this->manager->post(request, ba_data);

            break;

        case PUT:
            if (!ba_data.isNull() && !ba_data.isEmpty())
            {
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
                request.setHeader(QNetworkRequest::ContentLengthHeader, ba_data.length());
            }

            reply = this->manager->put(request, ba_data);

            break;

        case DEL:
            reply = this->manager->deleteResource(request);

            break;
    }

    Q_ASSERT(reply);

    QString result;

    if (!this->waitForReply(reply))
    {
        this->set_network_error( QNetworkReply::TimeoutError );
        this->set_error_string( "Network timeout" );
        qCritical() << "CouchDBManager::DBManager::request FATAL " << this->error_string;
    }
    else if (reply->error() != QNetworkReply::NoError && reply->error() != QNetworkRequest::RedirectionTargetAttribute)
    {
        this->set_network_error( reply->error() );
        this->set_error_string( reply->errorString() );
        qCritical() << "CouchDBManager::DBManager::request FATAL " << this->error_string;
    }
    else
    {
        result = reply->readAll();

        if(use_auth_cookie)
        {
            QVariant set_cookie_header = reply->header(QNetworkRequest::SetCookieHeader);
            QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(set_cookie_header);

            for(int i=0; i < cookies.size(); ++i)
            {
                qDebug() << "# Set Cookie:" << cookies[i].name() << cookies[i].value();

                if (cookies[i].name() == "AuthSession")
                {
                    this->set_auth_cookie( QString(cookies[i].value()) );
                    break;
                }
            }
        }
    }

    delete reply;

    qDebug() << "<" << Q_FUNC_INFO;

    return result;
}

QByteArray CouchDBManager::DBManager::request_rapidjson(const QUrl& url, const QByteArray& ba_data, const QString& partial, CouchDBManager::DBManager::HTTP_METHOD method)
{
    qDebug() << ">" << Q_FUNC_INFO;

    this->reset_errors();

    QNetworkRequest request(url);
    QNetworkReply *reply = NULL;

    bool use_auth_cookie = (partial != "_session");
    this->set_common_headers(&request, use_auth_cookie);

    switch (method)
    {
        case GET:
            reply = this->manager->get(request);

            break;

        case POST:
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
            request.setHeader(QNetworkRequest::ContentLengthHeader, ba_data.length());

            reply = this->manager->post(request, ba_data);

            break;

        case PUT:
            if (!ba_data.isNull() && !ba_data.isEmpty())
            {
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
                request.setHeader(QNetworkRequest::ContentLengthHeader, ba_data.length());
            }

            reply = this->manager->put(request, ba_data);

            break;

        case DEL:
            reply = this->manager->deleteResource(request);

            break;
    }

    Q_ASSERT(reply);

    QByteArray result;

    if (!this->waitForReply(reply))
    {
        this->set_network_error( QNetworkReply::TimeoutError );
        this->set_error_string( "Network timeout" );
        qCritical() << "CouchDBManager::DBManager::request FATAL " << this->error_string;
    }
    else if (reply->error() != QNetworkReply::NoError && reply->error() != QNetworkRequest::RedirectionTargetAttribute)
    {
        this->set_network_error( reply->error() );
        this->set_error_string( reply->errorString() );
        qCritical() << "CouchDBManager::DBManager::request FATAL " << this->error_string;
    }
    else
    {
        result = reply->readAll();

        if(use_auth_cookie)
        {
            QVariant set_cookie_header = reply->header(QNetworkRequest::SetCookieHeader);
            QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(set_cookie_header);

            for(int i=0; i < cookies.size(); ++i)
            {
                qDebug() << "# Set Cookie:" << cookies[i].name() << cookies[i].value();

                if (cookies[i].name() == "AuthSession")
                {
                    this->set_auth_cookie( QString(cookies[i].value()) );
                    break;
                }
            }
        }
    }

    delete reply;

    qDebug() << "<" << Q_FUNC_INFO;

    return result;
}


QString CouchDBManager::DBManager::do_get(const QString& partial, const QString& query_string)
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->do_get(partial, query_string, true);
}

QByteArray CouchDBManager::DBManager::do_get_rapidjson(const QString& partial, const QString& query_string)
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->do_get_rapidjson(partial, query_string, true);
}

QString CouchDBManager::DBManager::do_get(const QString& partial, const QString& query_string, bool use_context)
{
    qDebug() << ">" << Q_FUNC_INFO;
    qDebug() << "# Partial:" << partial;
    qDebug() << "# Query String:" << query_string;
    qDebug() << "# Use context:" << use_context;

    QUrl url( get_url(partial, query_string, use_context) );
    QString result = this->request(url, QString("").toUtf8(), partial, GET);

    qDebug() << "<" << Q_FUNC_INFO;

    return result;
}

QByteArray CouchDBManager::DBManager::do_get_rapidjson(const QString& partial, const QString& query_string, bool use_context)
{
    qDebug() << ">" << Q_FUNC_INFO;
    qDebug() << "# Partial:" << partial;
    qDebug() << "# Query String:" << query_string;
    qDebug() << "# Use context:" << use_context;

    QUrl url( get_url(partial, query_string, use_context) );
    QByteArray result = this->request_rapidjson(url, QString("").toUtf8(), partial, GET);

    qDebug() << "<" << Q_FUNC_INFO;

    return result;
}

QString CouchDBManager::DBManager::do_post(const QString &data)
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->do_post(data, QString());
}

QString CouchDBManager::DBManager::do_post(const QString& data, const QString& partial)
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->do_post(data, partial, true);
}

QString CouchDBManager::DBManager::do_post(const QString& data, const QString& partial, bool use_context)
{
    qDebug() << ">" << Q_FUNC_INFO;
    qDebug() << "# Partial:" << partial;

    QUrl url( partial.isNull() ? get_url() : get_url(partial, use_context) );
    QByteArray ba_data = data.toUtf8();
    QString result = this->request(url, ba_data, partial, POST);

    qDebug() << "<" << Q_FUNC_INFO;

    return result;
}

QString CouchDBManager::DBManager::do_put(const QString& partial, const QString& data)
{
    qDebug() << ">" << Q_FUNC_INFO;
    qDebug() << "# Partial:" << partial;

    QString url_str = "";

    if (!data.isNull())
    {
        url_str = get_url(partial);
    }
    else
    {
        url_str = get_url(true) + partial;
    }

    qDebug() << "# URL:" << url_str;

    QUrl url( url_str );
    QByteArray ba_data;

    if (!data.isNull())
    {
        ba_data = data.toUtf8();
    }
    else
    {
        ba_data = QString("").toUtf8();
    }

    QString result = this->request(url, ba_data, partial, PUT);

    qDebug() << "<" << Q_FUNC_INFO;

    return result;
}

QString CouchDBManager::DBManager::do_delete(const QString& partial, const QString& query_string)
{
    qDebug() << "><" << Q_FUNC_INFO;

    return this->do_delete(partial, query_string, true);
}

QString CouchDBManager::DBManager::do_delete(const QString& partial, const QString& query_string, bool use_context)
{
    qDebug() << ">" << Q_FUNC_INFO;
    qDebug() << "# Partial:" << partial;
    qDebug() << "# Query String:" << query_string;

    QUrl url( get_url(partial, query_string, use_context) );
    QString result = this->request(url, QString("").toUtf8(), partial, DEL);

    qDebug() << "<" << Q_FUNC_INFO;

    return result;
}


rapidjson::Document CouchDBManager::DBManager::string_2_rapidjson_object(const QString &str)
{
    qDebug() << "><" << Q_FUNC_INFO;

    rapidjson::Document json_result;
    json_result.Parse(str.toStdString().c_str());

    return json_result;
}

QJsonObject CouchDBManager::DBManager::string_2_json_object(const QString &str, QJsonParseError* json_parse_error)
{
    qDebug() << "><" << Q_FUNC_INFO;

    QJsonDocument json_result = QJsonDocument::fromJson(str.toUtf8(), json_parse_error);

    return json_result.object();
}

QJsonArray CouchDBManager::DBManager::string_2_json_array(const QString &str, QJsonParseError* json_parse_error)
{
    qDebug() << "><" << Q_FUNC_INFO;

    QJsonDocument json_result = QJsonDocument::fromJson(str.toUtf8(), json_parse_error);

    return json_result.array();
}

QString CouchDBManager::DBManager::json_object_2_string(const QJsonObject &obj)
{
    qDebug() << "><" << Q_FUNC_INFO;

    QJsonDocument doc(obj);

    return QString(doc.toJson(QJsonDocument::Compact));
}

QString CouchDBManager::DBManager::get_rev(const QString &id, QJsonArray json_array)
{
    for (int i = 0, len = json_array.size(); i < len; i++)
    {
        QJsonObject json_item = json_array.at(i).toObject();

        if (id == json_item["id"].toString())
        {
            return json_item["rev"].toString();
        }
    }

    return QString();
}

void CouchDBManager::DBManager::update_related_entities_from_bulk_docs(CouchDBManager::BaseEntity *entity, QJsonArray json_array)
{
    qDebug() << ">" << Q_FUNC_INFO;

    const QMetaObject* meta_obj = entity->metaObject();

    for (int i = 0, len = meta_obj->propertyCount(); i < len; i++)
    {
        QMetaProperty meta_prop = meta_obj->property(i);
        QVariant prop_value = meta_prop.read(entity);

        if (prop_value.isNull() || !prop_value.isValid())
        {
            continue;
        }

        if (QString(meta_prop.typeName()).startsWith("QList"))
        {
            qDebug() << "# Es un array:" << prop_value.typeName();

            QList<QVariant> prop_value_list = entity->toList(prop_value);

            foreach (const QVariant &list_item, prop_value_list)
            {
                if (list_item.isNull() || !list_item.isValid())
                {
                    continue;
                }

                if (this->is_base_entity( qvariant_cast<QObject*>(list_item) ))
                {
                    CouchDBManager::BaseEntity* item_entity = list_item.value<CouchDBManager::BaseEntity*>();
                    QString rev = this->get_rev(item_entity->get_id(), json_array);

                    if (!rev.isNull() && !rev.isEmpty())
                    {
                        item_entity->set_rev(rev);
                    }

                    this->update_related_entities_from_bulk_docs(item_entity, json_array);
                }
            }
        }
        else
        {
            qDebug() << "# Es valor sencillo:" << prop_value.typeName();

            if (this->is_base_entity( qvariant_cast<QObject*>(prop_value) ))
            {
                CouchDBManager::BaseEntity* prop_entity = prop_value.value<CouchDBManager::BaseEntity*>();
                QString rev = this->get_rev(prop_entity->get_id(), json_array);

                if (!rev.isNull() && !rev.isEmpty())
                {
                    prop_entity->set_rev(rev);
                }

                this->update_related_entities_from_bulk_docs(prop_entity, json_array);
            }
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;
}

void CouchDBManager::DBManager::reset_errors()
{
    qDebug() << "><" << Q_FUNC_INFO;

    this->set_network_error(QNetworkReply::NoError);
    this->set_error_string(QString());
}

bool CouchDBManager::DBManager::create_database(const QString &name)
{
    qDebug() << ">" << Q_FUNC_INFO;
    qDebug() << "# Create_database: " << name;

    QString result = this->do_put(name, QString());

    qDebug() << "# Create_database_response: " << result;

    if (result.isNull() || result.isEmpty())
    {
        qDebug() << "# Errors?: " << this->error_string;
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    QJsonParseError json_parse_error;
    QJsonObject json_object = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;
//        qFatal(err.toStdString().c_str());
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return json_object["ok"].toBool();
}

bool CouchDBManager::DBManager::delete_database(const QString &name)
{
    qDebug() << ">" << Q_FUNC_INFO;
    qDebug() << "# Delete_database: " << name;

    QString result = this->do_delete(name, QString(), false);

    qDebug() << "# Delete_database_response: " << result;

    if (result.isNull() || result.isEmpty())
    {
        qDebug() << "# Errors?: " << this->error_string;
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    QJsonParseError json_parse_error;
    QJsonObject json_object = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;
//        qFatal(err.toStdString().c_str());
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return json_object["ok"].toBool();
}

void CouchDBManager::DBManager::read_database_security(QStringList &admins, QStringList &users)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QString result = this->do_get("_security", QString());

    qDebug() << "# read_database_security response: " << result;

    if (result.isNull() || result.isEmpty())
    {
        qDebug() << "# Errors?: " << this->error_string;
        qDebug() << "<" << Q_FUNC_INFO;

        return;
    }

    QJsonParseError json_parse_error;
    QJsonObject json_object = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;
//        qFatal(err.toStdString().c_str());
        qDebug() << "<" << Q_FUNC_INFO;

        return;
    }

    QJsonArray admins_json = json_object["admins"].toObject()["roles"].toArray();
    QJsonArray users_json = json_object["members"].toObject()["roles"].toArray();

    foreach (const QJsonValue &rol, admins_json)
    {
        admins.append( rol.toString() );
    }

    foreach (const QJsonValue &rol, users_json)
    {
        users.append( rol.toString() );
    }

    qDebug() << "<" << Q_FUNC_INFO;
}

bool CouchDBManager::DBManager::update_database_security(const QStringList admins, const QStringList users)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QJsonObject security_json;
    QJsonObject admins_json;
    QJsonObject users_json;
    QJsonArray admin_user;

    admin_user.append( QJsonValue( QString("root") ) );
    admins_json.insert("names", QJsonValue(admin_user));
    admins_json.insert("roles", QJsonValue::fromVariant( QVariant::fromValue<QStringList>(admins) ));
    users_json.insert("names", QJsonValue( QJsonArray() ));
    users_json.insert("roles", QJsonValue::fromVariant( QVariant::fromValue<QStringList>(users) ));
    security_json.insert("admins", QJsonValue(admins_json));
    security_json.insert("members", QJsonValue(users_json));

    QString result = this->do_put("_security", this->json_object_2_string(security_json));

    qDebug() << "# update_database_security response: " << result;

    if (result.isNull() || result.isEmpty())
    {
        qDebug() << "# Errors?: " << this->error_string;
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    QJsonParseError json_parse_error;
    QJsonObject json_object = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;
//        qFatal(err.toStdString().c_str());
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return json_object["ok"].toBool();
}

QJsonObject CouchDBManager::DBManager::bulk_docs(QList<QObject*> entities)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QList<CouchDBManager::DBManagerResponse*> responses;
    QJsonObject docs;
    QJsonArray values;

    foreach (QObject *obj, entities)
    {
        if (!this->is_base_entity(obj))
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL One o more object does not inherit from BaseEntity.";

            this->set_error_string(err);
            qCritical() << err;
//            qFatal(err.toStdString().c_str());

            qDebug() << "<" << Q_FUNC_INFO;

            return QJsonObject();
        }

        CouchDBManager::BaseEntity* entity = qobject_cast<CouchDBManager::BaseEntity*>(obj);
        bool is_relation_entity = this->is_relation_entity(entity);
        QJsonObject json_doc;

        this->object_2_json<CouchDBManager::BaseEntity>(entity, json_doc, is_relation_entity);
//        entity->write(json_doc);
        values.append(QJsonValue(json_doc));
    }

    docs.insert("docs", QJsonValue(values));

    QString data = this->json_object_2_string(docs);

    qDebug() << "# JSON Document: " << data;

    QString result = this->do_post(data, "_bulk_docs");

    qDebug() << "# do_post: " << result;

    if (result.isNull() || result.isEmpty())
    {
        qDebug() << "# Errors?: " << this->error_string;
        qDebug() << "<" << Q_FUNC_INFO;

        return QJsonObject();
    }

    QJsonParseError json_parse_error;
    QJsonObject json_object = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;
//        qFatal(err.toStdString().c_str());

        qDebug() << "<" << Q_FUNC_INFO;

        return QJsonObject();
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return json_object;
}

rapidjson::Document CouchDBManager::DBManager::bulk_rapidjson_docs(QList<QObject*> entities)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QList<CouchDBManager::DBManagerResponse*> responses;
    rapidjson::Document  docs;
    rapidjson::Document values;
    values.SetArray();
    rapidjson::Document _documents;
    rapidjson::Document json_doc;

    foreach (QObject *obj, entities)
    {
        if (!this->is_base_entity(obj))
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL One o more object does not inherit from BaseEntity.";

            this->set_error_string(err);
            qCritical() << err;
            qDebug() << "<" << Q_FUNC_INFO;

            return _documents;
        }

        CouchDBManager::BaseEntity* entity = qobject_cast<CouchDBManager::BaseEntity*>(obj);
        bool is_relation_entity = this->is_relation_entity(entity);

        this->object_2_rapidjson<CouchDBManager::BaseEntity>(entity, json_doc, is_relation_entity);
        values.PushBack(json_doc, values.GetAllocator());
    }

    docs.SetObject();
    docs.AddMember("docs", values, docs.GetAllocator());
    std::string data = this->get_rjson_string(docs);

    qDebug() << "# JSON Document: " << data.c_str();

    QString result = this->do_post(QString::fromUtf8(data.c_str()), "_bulk_docs");

    qDebug() << "# do_post: " << result;

    if (result.isNull() || result.isEmpty())
    {
        qDebug() << "# Errors?: " << this->error_string;
        qDebug() << "<" << Q_FUNC_INFO;

        return rapidjson::Document();
    }

    rapidjson::Document json = this->string_2_rapidjson_object(result);

    if (json.HasParseError())
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + rapidjson::GetParseError_En(json.GetParseError());

        this->set_error_string(err);
        qCritical() << err;
        qDebug() << "<" << Q_FUNC_INFO;

        return rapidjson::Document();
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return json;
}


QStringList CouchDBManager::DBManager::uuids(int num)
{
    qDebug() << ">" << Q_FUNC_INFO;

    qDebug() << "# Num uuids" << num;

    QStringList result_list;
    QString result = this->do_get("_uuids", "count=" + QString::number(num), false);

    qDebug() << "# do_get: " << result;

    if (result.isNull() || result.isEmpty())
    {
        qDebug() << "#Errors?: " << this->error_string;

        return QStringList();
    }

    QJsonParseError json_parse_error;
    QJsonObject json_object = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;
//        qFatal(err.toStdString().c_str());

        qDebug() << "<" << Q_FUNC_INFO;

        return QStringList();
    }

    QJsonArray json_array = json_object["uuids"].toArray();

    foreach(const QJsonValue &value, json_array)
    {
        result_list.append(value.toString());
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return result_list;
}

bool CouchDBManager::DBManager::is_relation_entity(QObject *object)
{
    qDebug() << ">" << Q_FUNC_INFO;

    CouchDBManager::RelationEntity* e = qobject_cast<CouchDBManager::RelationEntity*>(object);
    bool is_re = false;

    if (e != NULL)
    {
        qDebug() << "# Se puede convertir en RelationEntity";

        const QMetaObject* meta = e->metaObject();

        for (int i = 0; i < meta->methodCount(); i++)
        {
            if (QString(meta->method(i).name()) == "is_relation_entity")
            {
                is_re = true;
                qDebug() << "# Es un objeto RelationEntity.";

                break;
            }
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return is_re;
}

bool CouchDBManager::DBManager::is_base_entity(QObject *object)
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
            if (QString(meta->property(i).name()) == "_id" || QString(meta->property(i).name()) == "_rev")
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

bool CouchDBManager::DBManager::is_versionable_entity(QObject *object)
{
    qDebug() << ">" << Q_FUNC_INFO;

    CouchDBManager::VersionableEntity* e = qobject_cast<CouchDBManager::VersionableEntity*>(object);
    bool is_ve = false;

    if (e != NULL)
    {
        qDebug() << "# Se puede convertir en VersionableEntity";

        const QMetaObject* meta = e->metaObject();

        for (int i = 0; i < meta->methodCount(); i++)
        {
            if (QString(meta->method(i).name()) == "is_versionable_entity")
            {
                is_ve = true;
                qDebug() << "# Es un objeto VersionableEntity.";

                break;
            }
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return is_ve;
}

bool CouchDBManager::DBManager::is_base_user(QObject *object)
{
    qDebug() << ">" << Q_FUNC_INFO;

    CouchDBManager::BaseUser* e = qobject_cast<CouchDBManager::BaseUser*>(object);
    bool is_bu = false;

    if (e != NULL)
    {
        qDebug() << "# Se puede convertir en BaseUser";

        const QMetaObject* meta = e->metaObject();

        for (int i = 0; i < meta->methodCount(); i++)
        {
            if (QString(meta->method(i).name()) == "is_base_user")
            {
                is_bu = true;

                qDebug() << "# Es un BaseUser.";

                break;
            }
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return is_bu;
}

bool CouchDBManager::DBManager::is_base_object(QObject *object)
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
            if (QString(meta->property(i).name()) == "_id" || QString(meta->property(i).name()) == "_rev")
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

bool CouchDBManager::DBManager::is_base_reference(QObject *object)
{
    qDebug() << ">" << Q_FUNC_INFO;

    CouchDBManager::BaseReference* e = qobject_cast<CouchDBManager::BaseReference*>(object);
    bool is_br = false;

    if (e != NULL)
    {
        qDebug() << "# Se puede convertir en BaseReference";

        const QMetaObject* meta = e->metaObject();

        for (int i = 0; i < meta->methodCount(); i++)
        {
            if (QString(meta->method(i).name()) == "is_base_reference")
            {
                is_br = true;

                qDebug() << "# Es un BaseReference.";

                break;
            }
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return is_br;
}

bool CouchDBManager::DBManager::is_base_reference_simple(QObject *object)
{
    qDebug() << ">" << Q_FUNC_INFO;

    CouchDBManager::BaseReferenceSimple* e = qobject_cast<CouchDBManager::BaseReferenceSimple*>(object);
    bool is_br = false;

    if (e != NULL)
    {
        qDebug() << "# Se puede convertir en BaseReferenceSimple";

        const QMetaObject* meta = e->metaObject();

        for (int i = 0; i < meta->methodCount(); i++)
        {
            if (QString(meta->method(i).name()) == "is_base_reference_simple")
            {
                is_br = true;

                qDebug() << "# Es un BaseReferenceSimple.";

                break;
            }
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return is_br;
}

bool CouchDBManager::DBManager::login(const QString &username, const QString &password, QStringList& roles)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QJsonObject user;

    user.insert("name", QJsonValue(username));
    user.insert("password", QJsonValue(password));
    qDebug() << "Iniciando sesión con el el usuario:" << user["name"].toString();

    QString result = this->do_post( this->json_object_2_string(user) , "_session", false);

    if (result.isEmpty() || result.isNull())
    {
        if (this->get_error_string().isEmpty() || this->get_error_string().isNull())
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL Unknown error: Response is empty";

            this->set_error_string(err);
            qCritical() << err;
        }

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    QJsonParseError json_parse_error;
    QJsonObject result_json = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    if (result_json.contains("error"))
    {
        QString err = QString(Q_FUNC_INFO) + " WARN " + result_json["error"].toString() + ": " + result_json["reason"].toString();

        this->set_error_string( result_json["reason"].toString() );
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    if (result_json.contains("roles") && !result_json["roles"].toArray().isEmpty())
    {
        foreach (const QJsonValue &value, result_json["roles"].toArray())
        {
            roles.append( value.toString() );
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return result_json.contains("ok") && result_json["ok"].toBool();
}

CouchDBManager::UserContext* CouchDBManager::DBManager::user_context()
{
    qDebug() << ">" << Q_FUNC_INFO;

    QString result = this->do_get("_session", QString(), false);

    if (result.isEmpty() || result.isNull())
    {
        if (this->get_error_string().isEmpty() || this->get_error_string().isNull())
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL Unknown error: Response is empty";

            this->set_error_string(err);
            qCritical() << err;
        }

        qDebug() << "<" << Q_FUNC_INFO;

        return NULL;
    }

    QJsonParseError json_parse_error;
    QJsonObject result_json = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;

        qDebug() << "<" << Q_FUNC_INFO;

        return NULL;
    }

    CouchDBManager::UserContext* uc = new CouchDBManager::UserContext(this);
    CouchDBManager::UserContext::UserCtx ctx;
    CouchDBManager::UserContext::Info info;
    QJsonObject ctx_json = result_json["userCtx"].toObject();
    QJsonObject info_json = result_json["info"].toObject();

    ctx.name = ctx_json["name"].toString();

    foreach(const QJsonValue &value, ctx_json["roles"].toArray())
    {
        ctx.roles.append( value.toString() );
    }

    uc->set_userCtx(ctx);
    uc->set_ok( result_json["ok"].toBool() );
    info.authenticated = info_json["authenticated"].toString();
    info.authentication_db = info_json["authentication_db"].toString();

    foreach(const QJsonValue &value, info_json["authentication_handlers"].toArray())
    {
        info.authentication_handlers.append( value.toString() );
    }

    uc->set_info(info);

    qDebug() << "<" << Q_FUNC_INFO;

    return uc;
}

bool CouchDBManager::DBManager::logout()
{
    qDebug() << ">" << Q_FUNC_INFO;

    QString result = this->do_delete("_session", QString(), false);

    if (result.isEmpty() || result.isNull())
    {
        if (this->get_error_string().isEmpty() || this->get_error_string().isNull())
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL Unknown error: Response is empty";

            this->set_error_string(err);
            qCritical() << err;
        }

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    QJsonParseError json_parse_error;
    QJsonObject result_json = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    if (result_json.contains("error"))
    {
        QString err = QString(Q_FUNC_INFO) + " WARN " + result_json["error"].toString() + ": " + result_json["reason"].toString();

        this->set_error_string( result_json["reason"].toString() );
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    this->set_auth_cookie(QString());

    qDebug() << "<" << Q_FUNC_INFO;

    return result_json.contains("ok") && result_json["ok"].toBool();
}

bool CouchDBManager::DBManager::replicate(CouchDBManager::ReplicationConfig *repl_config)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QJsonObject repl_cfg_json;

    repl_config->write(repl_cfg_json);

    if (repl_config->has_error())
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + repl_config->get_error_string();

        this->set_error_string(err);
        qCritical() << err;

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    qint64 orig_timeout = this->get_timeout();
    qint64 max_timeout = std::numeric_limits<qint64>::max();

    this->set_timeout(max_timeout);

    QString repl_cfg_str = this->json_object_2_string(repl_cfg_json);
    QString result = this->do_post(repl_cfg_str, "_replicate", false);

    if (result.isEmpty() || result.isNull())
    {
        if (this->get_error_string().isEmpty() || this->get_error_string().isNull())
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL Unknown error: Response is empty";

            this->set_error_string(err);
            qCritical() << err;
        }

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    qDebug() << "# do_post response:" << result;

    QJsonParseError json_parse_error;
    QJsonObject json_object = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    this->set_timeout(orig_timeout);

    qDebug() << "<" << Q_FUNC_INFO;

    return json_object.contains("ok") && json_object["ok"].toBool();
}

bool CouchDBManager::DBManager::add_replication_service(CouchDBManager::ReplicationConfig *repl_config)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QJsonObject repl_cfg_json;

    repl_config->set_continuous(true);
    repl_config->write(repl_cfg_json);

    if (repl_config->has_error())
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + repl_config->get_error_string();

        this->set_error_string(err);
        qCritical() << err;
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    QString repl_cfg_str = this->json_object_2_string(repl_cfg_json);
    QString result = this->do_post(repl_cfg_str, "_replicator", false);

    if (result.isEmpty() || result.isNull())
    {
        if (this->get_error_string().isEmpty() || this->get_error_string().isNull())
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL Unknown error: Response is empty";

            this->set_error_string(err);
            qCritical() << err;
        }

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    qDebug() << "# do_post response:" << result;

    QJsonParseError json_parse_error;
    QJsonObject json_object = this->string_2_json_object(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    bool went_ok = json_object.contains("ok") && json_object["ok"].toBool();

    if (went_ok)
    {
        repl_config->set_id(json_object["id"].toString());
        repl_config->set_rev(json_object["rev"].toString());
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return went_ok;
}

CouchDBManager::ReplicationConfig* CouchDBManager::DBManager::read_replication_service(const QString &id)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QString read_result = this->do_get("_replicator/" + id, QString(), false);

    if (read_result.isEmpty() || read_result.isNull())
    {
        if (this->get_error_string().isEmpty() || this->get_error_string().isNull())
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL Unknown error: Response is empty";

            this->set_error_string(err);
            qCritical() << err;
        }

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    QJsonParseError json_parse_error;
    QJsonObject json_read = this->string_2_json_object(read_result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    CouchDBManager::ReplicationConfig* config = new CouchDBManager::ReplicationConfig(this->parent());

    config->read(json_read);

    if (json_read["source"].isObject() && !json_read["source"].isNull())
    {
        QJsonObject source = json_read["source"].toObject();

        if (source["headers"].isObject() && !source["headers"].isNull())
        {
            QJsonObject headers = source["headers"].toObject();

            if (headers.contains("Authorization") && !headers["Authorization"].isNull())
            {
                config->get_source()->set_authorization( headers["Authorization"].toString() );
            }
        }
    }

    if (json_read["target"].isObject() && !json_read["target"].isNull())
    {
        QJsonObject target = json_read["source"].toObject();

        if (target["headers"].isObject() && !target["headers"].isNull())
        {
            QJsonObject headers = target["headers"].toObject();

            if (headers.contains("Authorization") && !headers["Authorization"].isNull())
            {
                config->get_target()->set_authorization( headers["Authorization"].toString() );
            }
        }
    }

    return config;
}

QList<CouchDBManager::ActiveTask*> CouchDBManager::DBManager::list_active_tasks()
{
    qDebug() << ">" << Q_FUNC_INFO;

    QList<CouchDBManager::ActiveTask*> list;
    QString result = this->do_get("_active_tasks", QString(), false);

    QJsonParseError json_parse_error;
    QJsonArray json_array = this->string_2_json_array(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;

        qDebug() << "<" << Q_FUNC_INFO;

        return list;
    }

    foreach (QJsonValue value, json_array)
    {
        QJsonObject item = value.toObject();
        CouchDBManager::ActiveTask* task = new CouchDBManager::ActiveTask(this);

        task->read(item);
        list.append(task);
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return list;
}

QList<CouchDBManager::ReplicationConfig*> CouchDBManager::DBManager::list_active_replications()
{
    qDebug() << ">" << Q_FUNC_INFO;

    QList<CouchDBManager::ReplicationConfig*> list;
    QString result = this->do_get("_active_tasks", QString(), false);

    QJsonParseError json_parse_error;
    QJsonArray json_array = this->string_2_json_array(result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;

        qDebug() << "<" << Q_FUNC_INFO;

        return list;
    }

    foreach (QJsonValue value, json_array)
    {
        QJsonObject item = value.toObject();

        if (item["type"] == "replication")
        {
            CouchDBManager::ReplicationConfig* cfg = new CouchDBManager::ReplicationConfig(this);
            CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(cfg);
            CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(cfg);

            source->set_url(item["source"].toString());
            target->set_url(item["target"].toString());
            cfg->set_source(source);
            cfg->set_target(target);
            cfg->set_continuous(item["continuous"].toBool());
            cfg->set_checkpoint_seq(item["checkpointed_source_seq"].toInt());
            cfg->set_source_seq(item["source_seq"].toInt());
            cfg->set_progress(item["progress"].toInt());

            if (item.contains("doc_id") && !item["doc_id"].isNull() &&
                    !item["doc_id"].isUndefined() && !item["doc_id"].toString().isEmpty())
            {
                cfg->set_id(item["doc_id"].toString());
            }

            list.append(cfg);
        }
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return list;
}

QStringList CouchDBManager::DBManager::list_databases()
{
    qDebug() << ">" << Q_FUNC_INFO;

    QString read_result = this->do_get("_all_dbs", QString(), false);

    if (read_result.isEmpty() || read_result.isNull())
    {
        if (this->get_error_string().isEmpty() || this->get_error_string().isNull())
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL Unknown error: Response is empty";

            this->set_error_string(err);
            qCritical() << err;
        }

        qDebug() << "<" << Q_FUNC_INFO;

        return QStringList();
    }

    QJsonParseError json_parse_error;
    QJsonArray json_array = this->string_2_json_array(read_result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;
        qDebug() << "<" << Q_FUNC_INFO;

        return QStringList();
    }

    QStringList result;

    foreach (QJsonValue value, json_array)
    {
        result.append(value.toString());
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return result;
}

bool CouchDBManager::DBManager::remove_replication_service(const QString &id)
{
    qDebug() << ">" << Q_FUNC_INFO;

    QString read_result = this->do_get("_replicator/" + id, QString(), false);

    if (read_result.isEmpty() || read_result.isNull())
    {
        if (this->get_error_string().isEmpty() || this->get_error_string().isNull())
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL Unknown error: Response is empty";

            this->set_error_string(err);
            qCritical() << err;
        }

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    QJsonParseError json_parse_error;
    QJsonObject json_read = this->string_2_json_object(read_result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    QString delete_result = this->do_delete("_replicator/" + json_read["_id"].toString(), "rev=" + json_read["_rev"].toString(), false);

    if (delete_result.isEmpty() || delete_result.isNull())
    {
        if (this->get_error_string().isEmpty() || this->get_error_string().isNull())
        {
            QString err = QString(Q_FUNC_INFO) + " FATAL Unknown error: Response is empty";

            this->set_error_string(err);
            qCritical() << err;
        }

        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    QJsonObject json_delete = this->string_2_json_object(delete_result, &json_parse_error);

    if (json_parse_error.error != QJsonParseError::NoError)
    {
        QString err = QString(Q_FUNC_INFO) + " FATAL " + json_parse_error.errorString();

        this->set_error_string(err);
        qCritical() << err;
        qDebug() << "<" << Q_FUNC_INFO;

        return false;
    }

    qDebug() << "<" << Q_FUNC_INFO;

    return json_delete.contains("ok") && json_delete["ok"].toBool();
}

qint64 CouchDBManager::DBManager::toUnixTimeStamp(const QDateTime &datetime)
{
    return datetime.toMSecsSinceEpoch();
}

qint64 CouchDBManager::DBManager::toUnixTimeStamp(const QString &date_string)
{
    return toDateTime(date_string).toMSecsSinceEpoch();
}

qint64 CouchDBManager::DBManager::toUnixTimeStamp(const QString &date_string, const QString& format)
{
    return toDateTime(date_string, format).toMSecsSinceEpoch();
}

QDateTime CouchDBManager::DBManager::toDateTime(const QString &date_string)
{
    if (!date_string.contains(" "))
    {
        return QDateTime( QDate::fromString(date_string, DATE_FORMAT) );
    }

    return QDateTime::fromString(date_string, DATE_TIME_FORMAT);
}

QDateTime CouchDBManager::DBManager::toDateTime(const QString &date_string, const QString& format)
{
    return QDateTime::fromString(date_string, format);
}

QDateTime CouchDBManager::DBManager::toDateTime(qint64 timestamp)
{
    return QDateTime::fromMSecsSinceEpoch(timestamp);
}

QString CouchDBManager::DBManager::toDateString(const QDateTime &datetime)
{
    return formatDate(datetime, DATE_TIME_FORMAT);
}

QString CouchDBManager::DBManager::toDateString(qint64 timestamp)
{
    return formatDate(timestamp, DATE_TIME_FORMAT);
}

qint64 CouchDBManager::DBManager::todayTimestamp()
{
    return QDateTime::currentMSecsSinceEpoch();
}

QDateTime CouchDBManager::DBManager::todayDateTime()
{
    return QDateTime::currentDateTime();
}

QString CouchDBManager::DBManager::todayDateString()
{
    return formatDate(QDateTime::currentDateTime(), DATE_TIME_FORMAT);
}

QString CouchDBManager::DBManager::formatDate(const QDateTime &datetime, const QString &format)
{
    return datetime.toString(format);
}

QString CouchDBManager::DBManager::formatDate(qint64 timestamp, const QString &format)
{
    return toDateTime(timestamp).toString(format);
}

QString CouchDBManager::DBManager::base64_encode(const QString &str)
{
    QByteArray ba;

    ba.append(str);

    return ba.toBase64();
}

QString CouchDBManager::DBManager::base64_decode(const QString &str)
{
    QByteArray ba;

    ba.append(str);

    return QByteArray::fromBase64(ba);
}

bool CouchDBManager::DBManager::waitForReply(QNetworkReply* reply)
{
    QTimer timer;
    QEventLoop event_loop;

    timer.setSingleShot(true);
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(network_timeout()));
    QObject::connect(this, SIGNAL(network_read_timeout()), reply, SLOT(abort()));
    QObject::connect(this->manager, SIGNAL(finished(QNetworkReply*)), &event_loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(network_read()));

    timer.start(this->timeout);

    event_loop.exec(QEventLoop::ExcludeUserInputEvents);

    timer.stop();

    //Q_ASSERT(reply->isFinished());

    bool reply_received = !this->nw_timeout;
    this->nw_read = false;
    this->nw_timeout = false;
    return reply_received;
}

void CouchDBManager::DBManager::network_read()
{
    if(!this->nw_read)
        this->nw_read = true;
}

void CouchDBManager::DBManager::network_timeout()
{
    if(!this->nw_read)
    {
        this->nw_timeout = true;
        emit this->network_read_timeout();
    }
}
