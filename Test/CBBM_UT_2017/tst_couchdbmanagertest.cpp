#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QDebug>
#include <QList>
#include <QJsonObject>
#include <QJsonValue>
#include <QDate>
#include <QMap>
#include <QMetaType>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "couchdbmanager.h"
#include "dbmanagerresponse.h"
#include "usercontext.h"
#include "car.h"

#define NAME __FUNCTION__

class CouchDBManagerTest : public QObject
{
    Q_OBJECT

private:
    CouchDBManager::DBManager* db;
    QString last_id;
    QString last_rev;
    QMap<QString, bool> exec;

    void sleep(int ms);
    bool skip(const QString& name);

public:
    CouchDBManagerTest();

private Q_SLOTS:
    void initTestCase();

    void createVersionableEntity();
    void lockVersionableEntity();
    void unlockVersionableEntity();
    void lockAndUpdateVersionableEntity();
    void lockVersionableEntityUsingDifferentUser();
    void tryToLockVersionableEntityUsingDifferentUser();
    void tryToLockVersionableEntityUsingAnotherUser();
    void tryToLUnlockVersionableEntityUsingAnotherUser();
    void unlockVersionableEntityUsingDifferentUser();
    void tryToLUnlockVersionableEntityUsingDifferentUser();
    void deleteVersionableEntity();
    void tryToDeleteVersionableEntityUsingDifferentUser();

    void cleanupTestCase();
};

CouchDBManagerTest::CouchDBManagerTest()
{
    exec.insert("CouchDBManagerTest::createVersionableEntity", true);
    exec.insert("CouchDBManagerTest::lockVersionableEntity", true);
    exec.insert("CouchDBManagerTest::unlockVersionableEntity", true);
    exec.insert("CouchDBManagerTest::lockAndUpdateVersionableEntity", true);
    exec.insert("CouchDBManagerTest::lockVersionableEntityUsingDifferentUser", true);
    exec.insert("CouchDBManagerTest::tryToLockVersionableEntityUsingDifferentUser", true);
    exec.insert("CouchDBManagerTest::tryToLockVersionableEntityUsingAnotherUser", true);
    exec.insert("CouchDBManagerTest::tryToLUnlockVersionableEntityUsingAnotherUser", true);
    exec.insert("CouchDBManagerTest::unlockVersionableEntityUsingDifferentUser", true);
    exec.insert("CouchDBManagerTest::tryToLUnlockVersionableEntityUsingDifferentUser", true);
    exec.insert("CouchDBManagerTest::deleteVersionableEntity", true);
    exec.insert("CouchDBManagerTest::tryToDeleteVersionableEntityUsingDifferentUser", true);

    db = new CouchDBManager::DBManager(this);

    db->registerType<Car*>();
}

bool CouchDBManagerTest::skip(const QString &name)
{
    if (exec.contains(name))
    {
        return !exec.value(name);
    }

    return true;
}

void CouchDBManagerTest::sleep(int ms)
{
    QTEST_ASSERT(ms > 0);

#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

void CouchDBManagerTest::initTestCase()
{
    db->set_server_address("http://localhost:5984");
    db->set_database_name("aunia");
    db->set_timeout(5000);
    db->set_remote_conn(true);

    QStringList roles;
    bool logged_in = db->login("root", "root", roles);

    QVERIFY2(logged_in, "Not logged in");
    QVERIFY2(roles.size() > 0, "Root has no roles");
}

void CouchDBManagerTest::createVersionableEntity()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    car.set_name("My Car");
    car.set_description("My Own Car");
    car.set_version(12);

    CouchDBManager::DBManagerResponse* response = db->create<Car>(&car);

    QVERIFY2(response->get_went_ok(), "Operation failed");
    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");

    last_id = car.get_id();
    last_rev = car.get_rev();
}

void CouchDBManagerTest::lockVersionableEntity()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "My Car", "Incorrect name");

    bool locked = db->lock<Car>(&car);

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() == usr, "Incorrect user");
}

void CouchDBManagerTest::unlockVersionableEntity()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "My Car", "Incorrect name");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked(), "Entity not locked");
    QVERIFY2(car.get_locked_by() == usr, "Incorrect user");

    bool unlocked = db->unlock<Car>(&car);

    QVERIFY2(unlocked, "Entity not unlocked");
    QVERIFY2(car.get_locked_by().isEmpty(), "User not empty");
    QVERIFY2(!car.get_locked(), "Entity not unlocked");
}

void CouchDBManagerTest::lockAndUpdateVersionableEntity()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "My Car", "Incorrect name");

    bool locked = db->lock<Car>(&car);

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() == usr, "Incorrect user");

    car.set_name("My unlocked car");

    CouchDBManager::DBManagerResponse* resp = db->update<Car>(&car, true);

    QVERIFY2(resp->get_went_ok(), "Entity not updated");
    QVERIFY2(car.get_locked_by().isEmpty(), "User not empty");
    QVERIFY2(!car.get_locked(), "Entity not unlocked");
}

void CouchDBManagerTest::lockVersionableEntityUsingDifferentUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    bool locked = db->lock<Car>(&car, "usuario");

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() != usr, "Incorrect user");
}

void CouchDBManagerTest::tryToLockVersionableEntityUsingDifferentUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    bool locked = db->lock<Car>(&car, "usuario");

    QVERIFY2(!locked, "Entity locked");
    QVERIFY2(db->get_error_string().isEmpty(), "Error generated");
}

void CouchDBManagerTest::tryToLockVersionableEntityUsingAnotherUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    bool locked = db->lock<Car>(&car, "distinto");

    QVERIFY2(!locked, "Entity locked");
    QVERIFY2(!db->get_error_string().isEmpty(), "No error generated");
}

void CouchDBManagerTest::tryToLUnlockVersionableEntityUsingAnotherUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked(), "Entity not locked");
    QVERIFY2(car.get_locked_by() != usr, "Incorrect user");

    bool unlocked = db->unlock<Car>(&car, "distinto");

    QVERIFY2(!unlocked, "Entity unlocked");
    QVERIFY2(!car.get_locked_by().isEmpty(), "User empty");
    QVERIFY2(car.get_locked(), "Entity unlocked");
    QVERIFY2(car.get_locked_by() != "distinto", "Incorrect user");
    QVERIFY2(!db->get_error_string().isEmpty(), "Error not generated");
}

void CouchDBManagerTest::unlockVersionableEntityUsingDifferentUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked(), "Entity not locked");
    QVERIFY2(car.get_locked_by() != usr, "Incorrect user");

    bool unlocked = db->unlock<Car>(&car, "usuario");

    QVERIFY2(unlocked, "Entity not unlocked");
    QVERIFY2(car.get_locked_by().isEmpty(), "User not empty");
    QVERIFY2(!car.get_locked(), "Entity not unlocked");
}

void CouchDBManagerTest::tryToLUnlockVersionableEntityUsingDifferentUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(!car.get_locked(), "Entity locked");
    QVERIFY2(car.get_locked_by().isEmpty(), "Incorrect user");

    bool unlocked = db->unlock<Car>(&car, "usuario");

    QVERIFY2(!unlocked, "Entity unlocked");
    QVERIFY2(car.get_locked_by().isEmpty(), "User not empty");
    QVERIFY2(db->get_error_string().isEmpty(), "Error generated");
}

void CouchDBManagerTest::deleteVersionableEntity()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    bool locked = db->lock<Car>(&car);

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() == usr, "Incorrect user");

    CouchDBManager::DBManagerResponse* resp = db->delete_doc<Car>(&car);

    QVERIFY2(resp->get_went_ok(), "Entity not deleted");
}

void CouchDBManagerTest::tryToDeleteVersionableEntityUsingDifferentUser()
{
    if (skip(NAME)) QSKIP("Prueba no necesaria");

    Car car;

    car.set_name("My Car");
    car.set_description("My Own Car");
    car.set_version(12);

    CouchDBManager::DBManagerResponse* response = db->create<Car>(&car);

    QVERIFY2(response->get_went_ok(), "Operation failed");
    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");

    bool locked = db->lock<Car>(&car, "usuario");

    QVERIFY2(locked, "Entity not locked");

    CouchDBManager::UserContext* uc = db->user_context();
    QString usr = uc->get_userCtx().name;

    QVERIFY2(car.get_locked_by() != usr, "Incorrect user");

    CouchDBManager::DBManagerResponse* resp = db->delete_doc<Car>(&car);

    QVERIFY2(!resp->get_went_ok(), "Entity deleted");
}

void CouchDBManagerTest::cleanupTestCase()
{
    bool logged_out = db->logout();

    QVERIFY2(logged_out, "Not logged oun");

    delete db;
}

QTEST_MAIN(CouchDBManagerTest)

#include "tst_couchdbmanagertest.moc"
