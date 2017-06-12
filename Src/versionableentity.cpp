#include "versionableentity.h"

int CouchDBManager::VersionableEntity::get_version() const
{
    return this->version;
}

void CouchDBManager::VersionableEntity::set_version(int a_version)
{
    this->version = a_version;
}

bool CouchDBManager::VersionableEntity::get_working() const
{
    return this->working;
}

void CouchDBManager::VersionableEntity::set_working(bool a_working)
{
    this->working = a_working;
}
