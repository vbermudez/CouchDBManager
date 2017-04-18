#include "dbmanagerresponse.h"

bool CouchDBManager::DBManagerResponse::get_went_ok() const
{
    return went_ok;
}

void CouchDBManager::DBManagerResponse::set_went_ok(bool value)
{
    went_ok = value;
}

QString CouchDBManager::DBManagerResponse::get_response() const
{
    return response;
}

void CouchDBManager::DBManagerResponse::set_response(const QString &value)
{
    response = value;
}
