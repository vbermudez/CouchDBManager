#include "replicationconfig.h"

#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

void CouchDBManager::ReplicationConfig::write(QJsonObject &json)
{
    qDebug() << ">" << Q_FUNC_INFO;

    if (this->source == NULL || this->source->get_url().isNull() || this->source->get_url().isEmpty())
    {
        this->set_error_string("Source URL is Null or Empty");

        return;
    }

    if (this->target == NULL || this->target->get_url().isNull() || this->target->get_url().isEmpty())
    {
        this->set_error_string("Target URL is Null or Empty");

        return;
    }

    if (!this->_id.isNull() && !this->_id.isEmpty())
    {
        json.insert("_id", QJsonValue(this->_id));
    }

    if (!this->_rev.isNull() && !this->_rev.isEmpty())
    {
        json.insert("_rev", QJsonValue(this->_rev));
    }

    if (!this->collection.isNull() && !this->collection.isEmpty())
    {
        json.insert("collection", QJsonValue(this->collection));
    }

    if (!this->name.isNull() && !this->name.isEmpty())
    {
        json.insert("name", QJsonValue(this->name));
    }

    if (!this->source->get_authorization().isNull() && !this->source->get_authorization().isEmpty())
    {
        QJsonObject source;
        QJsonObject headers;
        QString auth = "Basic ";

        headers.insert("Authorization", QJsonValue( auth.append( this->source->get_authorization() ) ));
        source.insert("url", this->source->get_url());
        source.insert("headers", QJsonValue(headers));
        json.insert("source", QJsonValue(source));
    }
    else
    {
        json.insert("source", QJsonValue(this->source->get_url()));
    }

    if (!this->target->get_authorization().isNull() && !this->target->get_authorization().isEmpty())
    {
        QJsonObject target;
        QJsonObject headers;
        QString auth = "Basic ";

        headers.insert("Authorization", QJsonValue( auth.append(this->target->get_authorization()) ));
        target.insert("url", this->target->get_url());
        target.insert("headers", QJsonValue(headers));
        json.insert("target", QJsonValue(target));
    }
    else
    {
        json.insert("target", QJsonValue(this->target->get_url()));
    }

    json.insert("continuous", QJsonValue(this->continuous));
    json.insert("cancel", QJsonValue(this->cancel));
    json.insert("create_target", QJsonValue(this->create_target));

    if (!this->doc_ids.isEmpty())
    {
        QJsonArray ids;

        foreach(const QString &id, this->doc_ids)
        {
            ids.append( QJsonValue(id) );
        }

        json.insert("doc_ids", QJsonValue(ids));
    }

    if (!this->filter.isNull() && !this->filter.isEmpty())
    {
        json.insert("filter", QJsonValue(this->filter));
    }

    if (!this->proxy.isNull() && !this->proxy.isEmpty())
    {
        json.insert("proxy", QJsonValue(this->proxy));
    }

    if (!this->query_params.isEmpty())
    {
        QJsonObject query_params;

        foreach(const QString &key, this->query_params.keys())
        {
            query_params.insert(key, QJsonValue::fromVariant( this->query_params.value(key) ));
        }


        json.insert("query_params", QJsonValue(query_params));
    }

    qDebug() << "<" << Q_FUNC_INFO;
}
