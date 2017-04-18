#include "usercontext.h"

void CouchDBManager::UserContext::set_info(const Info &a_info)
{
    this->info = a_info;
}

CouchDBManager::UserContext::Info CouchDBManager::UserContext::get_info() const
{
    return this->info;
}

void CouchDBManager::UserContext::set_ok(bool a_ok)
{
    this->ok = a_ok;
}

bool CouchDBManager::UserContext::get_ok() const
{
    return this->ok;
}

void CouchDBManager::UserContext::set_userCtx(const CouchDBManager::UserContext::UserCtx a_userCtx)
{
    this->userCtx = a_userCtx;
}

CouchDBManager::UserContext::UserCtx CouchDBManager::UserContext::get_userCtx() const
{
    return this->userCtx;
}
