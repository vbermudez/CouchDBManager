//! @addtogroup CouchDBManager
//! @{
//! @file "replicationconfig.h"
//! @brief Definición de la clase de configuración de una replicación.
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
// REVISION: ........ $Revision: 1.4 $ ($Date: 2016/12/05 13:15:26 $)
//
// REFERENCE: .......
//
// CHANGES:
//
#ifndef REPLICATIONCONFIG_H
#define REPLICATIONCONFIG_H

#include "couchdbmanager_global.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QJsonObject>
#include <QVariant>

#include "baseentity.h"

/**
 * @brief El namespace CouchDBManager aisla las clases para evitar duplicidades.
 */
namespace CouchDBManager
{
    /**
     * @brief La clase ServerResource representa una conexión a un servidor CouchDB.
     * @author vbermudez@xi-tek.com
     * @date 2016/11/30
     */
    class COUCHDBMANAGERSHARED_EXPORT ServerResource : public CouchDBManager::BaseObject
    {
        Q_OBJECT
        Q_PROPERTY(QString url READ get_url WRITE set_url)
        Q_PROPERTY(QString authorization READ get_authorization WRITE set_authorization)

    private:
        QString url;
        QString authorization;

    public:
        /**
          * @brief ServerResource Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE ServerResource(QObject *parent = 0) : CouchDBManager::BaseObject(parent) { }
        /**
         * @brief ReplicationConfig Constructor de copia. No utilizar.
         * @param a_entity DBManagerResponse original.
         */
        ServerResource(const ServerResource& a_server_resource) {
            this->url = a_server_resource.url;
            this->authorization = a_server_resource.authorization;
        }
        /**
         * @brief ~ReplicationConfig Destructor
         */
        virtual ~ServerResource() {}

    public slots:
        QString get_url() const { return this->url; }
        void set_url(const QString& value) { this->url = value; }

        QString get_authorization() const { return this->authorization; }
        void set_authorization(const QString& value) { this->authorization = value; }
    };

    /**
     * @brief La clase ReplicationConfig define la configuración de una replicación.
     * @author vbermudez@xi-tek.com
     * @date 2016/07/12
     */
    class COUCHDBMANAGERSHARED_EXPORT ReplicationConfig : public CouchDBManager::BaseEntity
    {
        Q_OBJECT
        Q_PROPERTY(CouchDBManager::ServerResource* source READ get_source WRITE set_source)
        Q_PROPERTY(CouchDBManager::ServerResource* target READ get_target WRITE set_target)
        Q_PROPERTY(bool continuous READ get_continuous WRITE set_continuous)
        Q_PROPERTY(bool cancel READ get_cancel WRITE set_cancel)
        Q_PROPERTY(bool create_target READ get_create_target WRITE set_create_target)
        Q_PROPERTY(QStringList doc_ids READ get_doc_ids WRITE set_doc_ids)
        Q_PROPERTY(QString filter READ get_filter WRITE set_filter)
        Q_PROPERTY(QString proxy READ get_proxy WRITE set_proxy)
        Q_PROPERTY(QMap query_params READ get_query_params WRITE set_query_params)
        Q_PROPERTY(QString error_string READ get_error_string WRITE set_error_string)

    public:
        /**
          * @brief ReplicationConfig Constructor por defecto.
          * @param parent QObject padre.
          */
        Q_INVOKABLE ReplicationConfig(QObject *parent = 0) : CouchDBManager::BaseEntity(parent) {
            this->continuous = false;
            this->cancel = false;
            this->create_target = false;
            this->collection = "custom-replication";
        }
        /**
         * @brief ReplicationConfig Constructor de copia. No utilizar.
         * @param a_entity DBManagerResponse original.
         */
        ReplicationConfig(const ReplicationConfig& a_replication_cfg) {
            this->source = a_replication_cfg.source;
            this->target = a_replication_cfg.target;
            this->continuous = a_replication_cfg.continuous;
            this->cancel = a_replication_cfg.cancel;
            this->create_target = a_replication_cfg.create_target;
            this->doc_ids = a_replication_cfg.doc_ids;
            this->filter = a_replication_cfg.filter;
            this->proxy = a_replication_cfg.proxy;
            this->query_params = a_replication_cfg.query_params;
            this->error_string = a_replication_cfg.error_string;
        }
        /**
         * @brief ~ReplicationConfig Destructor
         */
        virtual ~ReplicationConfig() {}

        /**
         * @brief write Escribe la configuración como objeto JSON.
         * @param json Objeto JSON.
         */
        void write(QJsonObject &json);

    private:
        CouchDBManager::ServerResource* source;
        CouchDBManager::ServerResource* target;
        bool continuous;
        bool cancel;
        bool create_target;
        QStringList doc_ids;
        QString filter;
        QString proxy;
        QMap<QString, QVariant> query_params;
        QString error_string;

    public slots:
        /**
         * @brief get_source Recupera de la propiedad source.
         * @return El valor de la propiedad.
         */
        CouchDBManager::ServerResource* get_source() const { return this->source; }
        /**
         * @brief set_source Establece el valor a la propiedad source.
         * @param a_source El valor para la propiedad source.
         */
        void set_source(CouchDBManager::ServerResource* a_source) { this->source = a_source; }
        /**
         * @brief get_target Recupera de la propiedad target.
         * @return El valor de la propiedad.
         */
        CouchDBManager::ServerResource* get_target() const { return this->target; }
        /**
         * @brief set_target Establece el valor a la propiedad target.
         * @param a_target El valor para la propiedad target.
         */
        void set_target(CouchDBManager::ServerResource* a_target) { this->target = a_target; }
        /**
         * @brief get_continuous Recupera de la propiedad continuous.
         * @return El valor de la propiedad.
         */
        bool get_continuous() const { return this->continuous; }
        /**
         * @brief set_continuous Establece el valor a la propiedad continuous.
         * @param is_continuous El valor para la propiedad continuous.
         */
        void set_continuous(bool is_continuous) { this->continuous = is_continuous; }
        /**
         * @brief get_cancel Recupera de la propiedad cancel.
         * @return El valor de la propiedad.
         */
        bool get_cancel() const { return this->cancel; }
        /**
         * @brief set_cancel Establece el valor a la propiedad is_cancel.
         * @param is_cancel El valor para la propiedad is_cancel.
         */
        void set_cancel(bool is_cancel) { this->cancel = is_cancel; }
        /**
         * @brief get_create_target Recupera de la propiedad create_target.
         * @return El valor de la propiedad.
         */
        bool get_create_target() const { return this->create_target; }
        /**
         * @brief set_create_target Establece el valor a la propiedad create_target.
         * @param is_create_target El valor para la propiedad create_target.
         */
        void set_create_target(bool is_create_target) { this->create_target = is_create_target; }
        /**
         * @brief get_doc_ids Recupera de la propiedad doc_ids.
         * @return El valor de la propiedad.
         */
        QStringList get_doc_ids() { return this->doc_ids; }
        /**
         * @brief set_doc_ids Establece el valor a la propiedad doc_ids.
         * @param a_doc_ids El valor para la propiedad doc_ids.
         */
        void set_doc_ids(QStringList a_doc_ids) { this->doc_ids = a_doc_ids; }
        /**
         * @brief add_doc_id Añade un valor a la propiedad doc_ids.
         * @param a_doc_id Un valor para la propiedad doc_id.
         */
        void add_doc_id(QString a_doc_id) { this->doc_ids.append(a_doc_id); }
        /**
         * @brief get_filter Recupera de la propiedad filter.
         * @return El valor de la propiedad.
         */
        QString get_filter() const { return this->filter; }
        /**
         * @brief set_filter Establece el valor a la propiedad filter.
         * @param a_filter El valor para la propiedad filter.
         */
        void set_filter(const QString& a_filter) { this->filter = a_filter; }
        /**
         * @brief get_proxy Recupera de la propiedad proxy.
         * @return El valor de la propiedad.
         */
        QString get_proxy() const { return this->proxy; }
        /**
         * @brief set_proxy Establece el valor a la propiedad proxy.
         * @param a_proxy El valor para la propiedad proxy.
         */
        void set_proxy(const QString& a_proxy) { this->proxy = a_proxy; }
        /**
         * @brief get_query_params Recupera de la propiedad query_params.
         * @return El valor de la propiedad.
         */
        QMap<QString, QVariant> get_query_params() { return this->query_params; }
        /**
         * @brief set_query_params Establece el valor a la propiedad query_params.
         * @param a_query_params El valor para la propiedad query_params.
         */
        void set_query_params(QMap<QString, QVariant> a_query_params) { this->query_params = a_query_params; }
        /**
         * @brief add_query_param Añadir un valor a la propiedad query_params.
         * @param key Una clave.
         * @param value Un valor.
         */
        void add_query_param(const QString& key, const QVariant& value) { this->query_params.insert(key, value); }
        /**
         * @brief get_error Recupera de la propiedad error.
         * @return El valor de la propiedad.
         */
        QString get_error_string() const { return this->error_string; }
        /**
         * @brief set_error Establece el valor a la propiedad error.
         * @param a_error El valor para la propiedad error.
         */
        void set_error_string(const QString& a_error_string) { this->error_string = a_error_string; }
        /**
         * @brief has_error Comprueba si se ha producido algún error.
         * @return true si se ha producido un error.
         */
        bool has_error() const { return !(this->error_string.isNull() || this->error_string.isEmpty()); }
    };
}

Q_DECLARE_METATYPE(CouchDBManager::ServerResource*)
Q_DECLARE_METATYPE(CouchDBManager::ReplicationConfig*)

#endif // REPLICATIONCONFIG_H
//! @}
