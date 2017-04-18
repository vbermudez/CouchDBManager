﻿#include "versionableentity.h"

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

void CouchDBManager::VersionableEntity::set_locked(bool a_locked)
{
    this->locked = a_locked;
}

bool CouchDBManager::VersionableEntity::get_locked() const
{
    return this->locked;
}

void CouchDBManager::VersionableEntity::set_locked_by(const QString& a_locked_by)
{
    this->locked_by = a_locked_by;
}

QString CouchDBManager::VersionableEntity::get_locked_by() const
{
    return this->locked_by;
}
