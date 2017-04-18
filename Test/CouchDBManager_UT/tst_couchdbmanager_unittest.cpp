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
#include "replicationconfig.h"
#include "basereference.h"
#include "baseuser.h"
#include "car.h"
#include "ut.h"
#include "physical_car.h"
#include "physical_ut.h"
#include "physical_ut_car.h"
#include "physical_device.h"
#include "bus.h"
#include "rs485cosmos.h"
#include "monitconfig.h"
#include "auxiliary.h"
#include "device.h"
#include "types.h"
//#include "action.h"
//#include "connection.h"
//#include "entityfactory.h"
//#include "rel_bus_device.h"

class CouchDBManager_UnitTest : public QObject
{
    Q_OBJECT

private:
    CouchDBManager::DBManager* db;
    QString last_id;
    QString last_rev;
    QString user_name;
    QString last_id_aux;
    QString last_rev_aux;

    void sleep(int ms);

public:
    CouchDBManager_UnitTest();

private Q_SLOTS:
    void configureObject();
    void userLogin();
    void testUpdateConflicts();
    void listNoDocs();
    void listRelationEntities();
    void getUserContext();
    void createUser();
    void readUser();
    void deleteUser();
    void createDatabase();
    void deleteDatabase();
    void readTypes();
    void createBaseEntity();
    void updateBaseEntity();
    void createEntity();
    void readEntity();
    void lockVersionableEntity();
    void unlockVersionableEntity();
    void lockAndUpdateVersionableEntity();
    void readEntityWithRelations();
    void updateEntity();
    void versionEntity();
    void createAndListDevice();
    void createMulitLevelComplexEntity();
    void readMulitLevelComplexEntity();
    void listMultiLevelComplexEntity();
    void createComplexEntity();
    void readComplexEntity();
    void listComplexEntity();
    void updateComplexEntity();
    void deleteComplexEntity();
    void updateFromList();
    void createEntityWithBaseReference();
    void readEntityWithBaseReference();
    void updateEntityWithBaseReference();
    void testBaseReference();
    void testBaseReferenceUsingFilter();
    void createEntityWithRelations();
    void listEntityWithRelations();
    void updateEntityWithRelations();
    void deleteEntity();
    void replicate_local_2_local();
    void replicate_local_2_remote();
    void replicate_remote_2_local();
    void replicate_service_local_2_remote();
    void update_replication_service();
    void listActiveTasks();
    void remove_reaplication_service();
    void controlPQLError();
    void setInnerData();
    void getInnerData();
    void userLogout();
    void userIsLoggedOut();
    void playingWithDates();
    void destroyManager();
};

void CouchDBManager_UnitTest::sleep(int ms)
{
    QTEST_ASSERT(ms > 0);

#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

CouchDBManager_UnitTest::CouchDBManager_UnitTest()
{
    db = new CouchDBManager::DBManager(this);

//    db->registerType<Actions::Action*>();
    db->registerType<CosmosRef*>();
    db->registerType<Rule*>();
    db->registerType<Linked*>();
    db->registerType<Bus*>();
    db->registerType<Car*>();
//    db->registerType<Global::Types*>();
//    db->registerType<Actions::Connection*>();
    db->registerType<Device*>();
//    db->registerType<Global::DistributedContainerTypes*>();
    db->registerType<MonitConfig*>();
    db->registerType<Physical_car*>();
    db->registerType<Physical_device*>();
    db->registerType<Physical_ut*>();
    db->registerType<Physical_ut_car*>();
//    db->registerType<Actions::Rel_physical_device_connection*>();
    db->registerType<Rs485Cosmos*>();
    db->registerType<Configuration::Types*>();
    db->registerType<Configuration::Variable_types*>();
    db->registerType<Configuration::Io_card_types*>();
    db->registerType<Configuration::Bus_types*>();
    db->registerType<Configuration::Bus_types*>();
    db->registerType<Configuration::Device_hw_types*>();
    db->registerType<Configuration::Device_hw_version*>();
    db->registerType<Configuration::Tools*>();
    db->registerType<Ut*>();
    db->registerType<Variables*>();
//    db->registerType<Configuration::Rel_bus_device*>();
}

void CouchDBManager_UnitTest::configureObject()
{
    db->set_server_address("http://localhost:5984");
    db->set_database_name("aunia");
    db->set_timeout(5000);

    QVERIFY2(db->get_server_address() == "http://localhost:5984", "Incorrect server address");
    QVERIFY2(db->get_database_name() == "aunia", "Incorrect database name");
    QVERIFY2(QString(db->metaObject()->className()) == "CouchDBManager::DBManager", "Object is not DBManager");
    QVERIFY2(db->get_timeout() == 5000, "Incorrect timeout");
}

void CouchDBManager_UnitTest::userLogin()
{
//    return;
    QStringList roles;
    bool logged_in = db->login("root", "root", roles);

    QVERIFY2(logged_in, "Not logged in");
    QVERIFY2(roles.size() > 0, "Root has no roles");
    qDebug() << "# AuthCookie" << db->get_auth_cookie();
}

void CouchDBManager_UnitTest::testUpdateConflicts()
{

}

void CouchDBManager_UnitTest::listNoDocs()
{
    return;
//    QList<Actions::Connection*> list;

//    db->list<Actions::Connection>("actions", "relations", "connection", QString(), list);

//    QVERIFY2(list.isEmpty(), "List should be empty");
}

void CouchDBManager_UnitTest::listRelationEntities()
{
    return;
//    QList<Configuration::Rel_bus_device*> list;
//    QString old_db = db->get_database_name();

//    db->set_database_name("device_problem");
//    db->list<Configuration::Rel_bus_device>("configuration", "relations", "rel_bus_device", QString(), list);
//    db->set_database_name(old_db);

//    QVERIFY2(!list.isEmpty(), "List shouldn't be empty");
//    QVERIFY2(list.at(0)->get_bus() != NULL, "No relations loaded!");
}

void CouchDBManager_UnitTest::getUserContext()
{
    return;
    CouchDBManager::UserContext* ctx = db->user_context();

    QVERIFY2(ctx != NULL, "UserContext is NULL");
    QVERIFY2(QString(ctx->metaObject()->className()) == "CouchDBManager::UserContext", "Object is not UserContext");
    QVERIFY2(ctx->get_ok(), "Incorrect UserContext");
    QVERIFY2(ctx->get_userCtx().name.isNull(), "Incorrect UserContext Name");
    QVERIFY2(ctx->get_info().authentication_db == "_users", "Incorrect authentication database");

    qDebug() << "# AuthCookie" << db->get_auth_cookie();
    qDebug() << "# Sleep for 1.5 mins!";
    sleep(90000);
    ctx = db->user_context();

    qDebug() << "# AuthCookie" << db->get_auth_cookie();
}

void CouchDBManager_UnitTest::createUser()
{
    return;
    CouchDBManager::BaseUser user;

    user_name = "testuser" + QString::number( qrand() % 100 );
    user.set_name(user_name);
    user.set_password("password");
    user.add_roles("test_role");

    bool user_created = db->save_user<CouchDBManager::BaseUser>(&user);

    QVERIFY2(user_created, "User not created");
    QVERIFY2(user.get_id() == "org.couchdb.user:" + user.get_name(), "Incorrect name");
}

void CouchDBManager_UnitTest::readUser()
{
    return;
    CouchDBManager::BaseUser user;

    db->read_user<CouchDBManager::BaseUser>(user_name, &user);

    QVERIFY2(user.get_id() == "org.couchdb.user:" + user_name, "Incorrect _id");
    QVERIFY2(user.get_name() == user_name, "Incorrect name");
}

void CouchDBManager_UnitTest::deleteUser()
{
    return;
    CouchDBManager::BaseUser user;

    db->read_user<CouchDBManager::BaseUser>("org.couchdb.user:" + user_name, &user);

    bool user_deleted = db->delete_user<CouchDBManager::BaseUser>(&user);

    QVERIFY2(user_deleted, "User not deleted");
}

void CouchDBManager_UnitTest::createDatabase()
{
    return;
    bool db_created = db->create_database("db_unittest");

    QVERIFY2(db_created, "Database not created");
}

void CouchDBManager_UnitTest::deleteDatabase()
{
    return;
    bool db_deleted = db->delete_database("db_unittest");

    QVERIFY2(db_deleted, "Database not deleted");
}

void CouchDBManager_UnitTest::readTypes()
{
    return;
//    CouchDBManager::EntityFactory* ef = new CouchDBManager::EntityFactory(db, this);
//    Global::Types* types = ef->newEntity<Global::Types>();

//    db->read<Global::Types>("ad08e069be0f824c3a2153bd9d2b6eb5", QString(), types);

//    QVERIFY2(!types->get_id().isNull(), "_id is null");
//    QVERIFY2(!types->get_id().isEmpty(), "_id is empty");
//    QVERIFY2(!types->get_rev().isNull(), "_rev is null");
//    QVERIFY2(!types->get_rev().isEmpty(), "_rev is empty");
//    QVERIFY2(types->get_collection() == "conf_types", "Incorrect collection");
//    QVERIFY2(types->get_name() == "Configuration types", "Incorrect name");

//    QList<Global::DistributedContainerTypes*> dcts = types->get_distributed_container_types();

//    foreach (Global::DistributedContainerTypes* dct, dcts)
//    {
//        qDebug() << "Name:" << dct->get_name() << ", Description:" << dct->get_description();
//    }
}

void CouchDBManager_UnitTest::createBaseEntity()
{
    return;
    MonitConfig mcfg;

    mcfg.set_name("My Config 1");
    mcfg.set_description("My Own Config 1");

    CouchDBManager::DBManagerResponse* response = db->create<MonitConfig>(&mcfg);

    QVERIFY2(response->get_went_ok(), "Operation failed");
    QVERIFY2(!mcfg.get_id().isNull(), "_id is null");
    QVERIFY2(!mcfg.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!mcfg.get_rev().isNull(), "_rev is null");
    QVERIFY2(!mcfg.get_rev().isEmpty(), "_rev is empty");

    last_id = mcfg.get_id();
    last_rev = mcfg.get_rev();
}

void CouchDBManager_UnitTest::updateBaseEntity()
{
    return;
    MonitConfig mcfg;

    db->read<MonitConfig>(last_id, QString(), &mcfg);

    QVERIFY2(!mcfg.get_id().isNull(), "_id is null");
    QVERIFY2(!mcfg.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!mcfg.get_rev().isNull(), "_rev is null");
    QVERIFY2(!mcfg.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(mcfg.get_collection() == "monit_config", "Incorrect collection");
    QVERIFY2(mcfg.get_name() == "My Config 1", "Incorrect name");

    mcfg.set_name("My Other Config 1");

    CouchDBManager::DBManagerResponse* response = db->update<MonitConfig>(&mcfg);

    QVERIFY2(response->get_went_ok(), "Operation failed");
    QVERIFY2(mcfg.get_id() == last_id, "Incorrect _id");
    QVERIFY2(mcfg.get_rev() != last_rev, "Incorrect _rev");
    QVERIFY2(mcfg.get_name() == "My Other Config 1", "Incorrect name");
}

void CouchDBManager_UnitTest::createEntity()
{
//    return;
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

void CouchDBManager_UnitTest::readEntity()
{
    return;
    Car car;

    db->read<Car>(last_id, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "My Car", "Incorrect name");
}

void CouchDBManager_UnitTest::lockVersionableEntity()
{
//    return;
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

void CouchDBManager_UnitTest::unlockVersionableEntity()
{
//    return;

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

void CouchDBManager_UnitTest::lockAndUpdateVersionableEntity()
{
//    return;

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


void CouchDBManager_UnitTest::updateEntity()
{
    return;
    Car car;

    db->read<Car>(last_id, QString(), &car);

    car.set_name("My Other Car");
    car.set_version(34);

    CouchDBManager::DBManagerResponse* resp = db->update<Car>(&car);

    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(car.get_id() == last_id, "_id has cahnged");
    QVERIFY2(car.get_rev() != last_rev, "_rev has not changed");

    last_id = car.get_id();
    last_rev = car.get_rev();
}

void CouchDBManager_UnitTest::versionEntity()
{
    return;
    Car car;

    db->read<Car>(last_id, QString(), &car);

    car.set_version(13);

    CouchDBManager::DBManagerResponse* resp = db->update<Car>(&car);

    QVERIFY2(resp->get_went_ok(), "Operation failed");

    Car rcar;
    QString version_id = car.get_id() + "::12";

    db->read<Car>(version_id, QString(), &rcar);

    QVERIFY2(!rcar.get_id().isNull(), "_id is null");
    QVERIFY2(!rcar.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!rcar.get_rev().isNull(), "_rev is null");
    QVERIFY2(!rcar.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(rcar.get_collection() == "car", "Incorrect collection");
    QVERIFY2(rcar.get_name() == "My Car", "Incorrect name");
    QVERIFY2(rcar.get_version() == 12, "Incorrect version");
    QVERIFY2(!rcar.get_working(), "Incorrect working");
}

void CouchDBManager_UnitTest::readEntityWithRelations()
{
    return;
    Physical_car car;

    db->set_database_name("josetxo_problem");
    db->read_with_relations<Physical_car>("configuration", "3bba055faa9708254850b4a189284071", &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "physical_car", "Incorrect collection");
    QVERIFY2(car.get_name() == "UT2_S1", "Incorrect name");
    QVERIFY2(car.get_physical_ut_car().size() > 0, "No relations loaded");
    db->set_database_name("aunia");
}

void CouchDBManager_UnitTest::createAndListDevice()
{
    return;
    Device dev1;

    dev1.set_name("device 1");
    dev1.set_description("mi device");
    dev1.set_version(567);

    CouchDBManager::DBManagerResponse* resp = db->create<Device>(&dev1);
    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(!dev1.get_id().isNull(), "_id is null");
    QVERIFY2(!dev1.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!dev1.get_rev().isNull(), "_rev is null");
    QVERIFY2(!dev1.get_rev().isEmpty(), "_rev is empty");

    QList<Device*> devList;
    db->list<Device>("configuration", "relations", "device", QString(), devList);

}

void CouchDBManager_UnitTest::createMulitLevelComplexEntity()
{
    return;
    Car car;
//    Linked linked;
    Rs485Cosmos rs1;
    Rs485Cosmos rs2;
    Rs485Cosmos rs3;
    CosmosRef cosmos;

    car.set_name("Car 1");
    car.set_version(567);
    car.set_collection("car");
//    car.set_linked(&linked);
    rs1.set_name("COSMOS1");
    rs1.set_baudrate(10);
    rs2.set_name("COSMOS2");
    rs2.set_baudrate(20);
    rs3.set_name("COSMOS3");
    rs3.set_baudrate(30);
    cosmos.set_name("COSMOS2");
    car.add_rs485_cosmos(&rs1);
    car.add_rs485_cosmos(&rs2);
    car.add_rs485_cosmos(&rs3);
    car.set_cosmos(&cosmos);

    CouchDBManager::DBManagerResponse* resp = db->create<Car>(&car);

    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");

    last_id_aux = car.get_id();
    last_rev_aux = car.get_rev();
}

void CouchDBManager_UnitTest::readMulitLevelComplexEntity()
{
    return;
    Car car;

    db->read<Car>(last_id_aux, QString(), &car);

    QVERIFY2(!car.get_id().isNull(), "_id is null");
    QVERIFY2(!car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!car.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(car.get_collection() == "car", "Incorrect collection");
    QVERIFY2(car.get_name() == "Car 1", "Incorrect name");

//    Linked* linked = car.get_linked();

//    QVERIFY2(linked != NULL, "Linked is null");

//    CouchDBManager::BaseReference* parent = linked->get_parent();

//    QVERIFY2(parent == NULL, "Parent is not null");

//    Rule* rule = linked->get_rule();

//    QVERIFY2(rule == NULL, "Rule is not null");

    QList<Rs485Cosmos*> cosmosList = car.get_rs485_cosmos();
    Rs485Cosmos* rs485 = cosmosList.at(1);

    QVERIFY2(cosmosList.size() == 3, "Bad Cosmos size");
    QVERIFY2(rs485->get_name() == "COSMOS2", "Bad Rs485Cosmos name");
    QVERIFY2(rs485->get_baudrate() == 20, "Bad Rs485Cosmos baud rate");

    CosmosRef* cosmos = car.get_cosmos();

    QVERIFY2(cosmos != NULL, "CosmosRef is NULL");
    QVERIFY2(cosmos->get_name() == "COSMOS2", "Bad CosmosRef name");

    Rs485Cosmos* rs485_1 = car.get_inner_object<Rs485Cosmos>("name", QVariant(cosmos->get_name()));

    QVERIFY2(rs485_1->get_name() == rs485->get_name(), "Not found!");
    QVERIFY2(rs485_1->get_baudrate() == rs485->get_baudrate(), "Not found!");

    Rs485Cosmos* rs485_2 = car.get_inner_object<Rs485Cosmos>("name", QVariant("COSMOS3"));

    QVERIFY2(rs485_2->get_name() == "COSMOS3", "Not found!");
    QVERIFY2(rs485_2->get_baudrate() == 30, "Not found!");
}

void CouchDBManager_UnitTest::listMultiLevelComplexEntity()
{
    return;
    QList<Car*> carList;

    db->list<Car>("configuration", "relations", "car", QString(), carList);

    QVERIFY2(carList.size() > 0, "No document recovered");
    qDebug() << "# ====================> size" << carList.size();

    foreach(Car* car, carList)
    {
        QVERIFY2(!car->get_id().isNull(), "_id is null");
        QVERIFY2(!car->get_id().isEmpty(), "_id is empty");
        QVERIFY2(!car->get_rev().isNull(), "_rev is null");
        QVERIFY2(!car->get_rev().isEmpty(), "_rev is empty");
        QVERIFY2(car->get_collection() == "car", "Incorrect collection");

        Linked* linked = car->get_linked();

        if (linked != NULL)
        {
            QVERIFY2(linked != NULL, "Linked is null");

            CouchDBManager::BaseReference* parent = linked->get_parent();

            QVERIFY2(parent == NULL, "Parent is not null");

            Rule* rule = linked->get_rule();

            QVERIFY2(rule == NULL, "Rule is not null");
        }
        else
        {
            QVERIFY2(linked == NULL, "Linked is not null");
        }

        QList<Rs485Cosmos*> cosmosList = car->get_rs485_cosmos();

        if (cosmosList.size() > 0)
        {
            Rs485Cosmos* rs485 = cosmosList.at(1);

            QVERIFY2(cosmosList.size() == 3, "Bad Cosmos size");
            QVERIFY2(rs485->get_name() == "COSMOS2", "Bad Rs485Cosmos name");
            QVERIFY2(rs485->get_baudrate() == 20, "Bad Rs485Cosmos baud rate");

            CosmosRef* cosmos = car->get_cosmos();

            QVERIFY2(cosmos != NULL, "CosmosRef is NULL");
            QVERIFY2(cosmos->get_name() == "COSMOS2", "Bad CosmosRef name");

            Rs485Cosmos* rs485_1 = car->get_inner_object<Rs485Cosmos>("name", QVariant(cosmos->get_name()));

            QVERIFY2(rs485_1->get_name() == rs485->get_name(), "Not found!");
            QVERIFY2(rs485_1->get_baudrate() == rs485->get_baudrate(), "Not found!");

            Rs485Cosmos* rs485_2 = car->get_inner_object<Rs485Cosmos>("name", QVariant("COSMOS3"));

            QVERIFY2(rs485_2->get_name() == "COSMOS3", "Not found!");
            QVERIFY2(rs485_2->get_baudrate() == 30, "Not found!");
        }
    }
}

void CouchDBManager_UnitTest::createComplexEntity()
{
    return;
    Rs485Cosmos cosmos;
    Bus bus;
    Variables var1;
    Variables var2;

    cosmos.set_baudrate(10);
    cosmos.set_bit_stop(6);
    cosmos.set_parity("B");
    cosmos.set_error_detection("FAIL");
    var1.set_full_name("Variable 1");
    var2.set_full_name("Variable 2");
    bus.set_name("My Bus 1");
    bus.set_description("My Own Bus 1");
    bus.set_version(10);
    bus.set_working(false);
    bus.set_rs485_cosmos(&cosmos);
//    bus.add_variables( &var1 );
//    bus.add_variables( &var2 );

    CouchDBManager::DBManagerResponse* resp = db->create<Bus>(&bus);

    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(!bus.get_id().isNull(), "_id is null");
    QVERIFY2(!bus.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!bus.get_rev().isNull(), "_rev is null");
    QVERIFY2(!bus.get_rev().isEmpty(), "_rev is empty");

    last_id_aux = bus.get_id();
    last_rev_aux = bus.get_rev();
}

void CouchDBManager_UnitTest::readComplexEntity()
{
    return;
    Bus bus;

    db->read<Bus>(last_id_aux, QString(), &bus);

    QVERIFY2(!bus.get_id().isNull(), "_id is null");
    QVERIFY2(!bus.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!bus.get_rev().isNull(), "_rev is null");
    QVERIFY2(!bus.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(bus.get_collection() == "bus", "Incorrect collection");
    QVERIFY2(bus.get_name() == "My Bus 1", "Incorrect name");

    Rs485Cosmos* cosmos = bus.get_rs485_cosmos();

    QVERIFY2(cosmos->get_baudrate() == 10, "Incorrect baudrate");
    QVERIFY2(cosmos->get_bit_stop() == 6, "Incorrect baudrate");
    QVERIFY2(cosmos->get_parity() == "B", "Incorrect baudrate");
    QVERIFY2(cosmos->get_error_detection() == "FAIL", "Incorrect baudrate");

//    QList<Variables*> variables = bus.get_variables();

//    foreach(Variables* var, variables)
//    {
//        QVERIFY2(!var->get_full_name().isNull(), "full_name is Null");
//        QVERIFY2(!var->get_full_name().isEmpty(), "full_name is Empty");
//    }
}

void CouchDBManager_UnitTest::listComplexEntity()
{
    return;
    QList<Bus*> busList;

    db->list<Bus>("configuration", "relations", "bus", QString(), busList);

    QVERIFY2(busList.size() > 0, "No document recovered");
    qDebug() << "# ====================> size" << busList.size();

    foreach(Bus* bus, busList)
    {
        QVERIFY2(!bus->get_id().isNull(), "_id is null");
        QVERIFY2(!bus->get_id().isEmpty(), "_id is empty");
        QVERIFY2(!bus->get_rev().isNull(), "_rev is null");
        QVERIFY2(!bus->get_rev().isEmpty(), "_rev is empty");
        QVERIFY2(bus->get_collection() == "bus", "Incorrect collection");

        Rs485Cosmos* cosmos = bus->get_rs485_cosmos();

        QVERIFY2(cosmos->get_baudrate() == 10, "Incorrect baudrate");
        QVERIFY2(cosmos->get_bit_stop() == 6, "Incorrect baudrate");
        QVERIFY2(cosmos->get_parity() == "B", "Incorrect baudrate");
        QVERIFY2(cosmos->get_error_detection() == "FAIL", "Incorrect baudrate");

//        QList<Variables*> variables = bus->get_variables();

//        foreach(Variables* var, variables)
//        {
//            QVERIFY2(!var->get_full_name().isNull(), "full_name is Null");
//            QVERIFY2(!var->get_full_name().isEmpty(), "full_name is Empty");
//        }
    }
}

void CouchDBManager_UnitTest::updateComplexEntity()
{
    return;
    Bus bus;

    db->read<Bus>(last_id_aux, QString(), &bus);

    bus.set_name("My Other Bus 1");

    CouchDBManager::DBManagerResponse* resp = db->update<Bus>(&bus);

    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(!bus.get_id().isNull(), "_id is null");
    QVERIFY2(!bus.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!bus.get_rev().isNull(), "_rev is null");
    QVERIFY2(!bus.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(bus.get_rev() != last_rev_aux, "_rev is empty");

    Bus rbus;

    db->read<Bus>(last_id_aux, QString(), &rbus);

    QVERIFY2(rbus.get_id() == bus.get_id(), "Incorrect _id");
    QVERIFY2(rbus.get_rev() == bus.get_rev(), "Incorrect _rev");
    QVERIFY2(rbus.get_name() == bus.get_name(), "Inocrrect name");

    Rs485Cosmos* cosmos = bus.get_rs485_cosmos();
    Rs485Cosmos* rcosmos = rbus.get_rs485_cosmos();

    QVERIFY2(rcosmos->get_baudrate() == cosmos->get_baudrate(), "Incorrect baudrate");
    QVERIFY2(rcosmos->get_bit_stop() == cosmos->get_bit_stop(), "Incorrect baudrate");
    QVERIFY2(rcosmos->get_parity() == cosmos->get_parity(), "Incorrect baudrate");
    QVERIFY2(rcosmos->get_error_detection() == cosmos->get_error_detection(), "Incorrect baudrate");

//    QList<Variables*> variables = bus.get_variables();
//    QList<Variables*> rvariables = rbus.get_variables();

//    for (int i = 0, len = variables.size(); i < len; i++)
//    {
//        Variables* var = variables.at(i);
//        Variables* rvar = rvariables.at(i);

//        QVERIFY2(var->get_full_name() == rvar->get_full_name(), "Incorrect full_name");
//    }

    last_rev_aux = bus.get_rev();
}

void CouchDBManager_UnitTest::deleteComplexEntity()
{
    return;
    Bus bus;

    db->read<Bus>(last_id_aux, QString(), &bus);

    CouchDBManager::DBManagerResponse* resp = db->delete_doc<Bus>(&bus);

    QVERIFY2(resp->get_went_ok(), "Operation failed");
}

void CouchDBManager_UnitTest::updateFromList()
{
    return;
    QString old_db = db->get_database_name();
    QList<Physical_car*> list;

    db->set_database_name("device_problem");
    db->list<Physical_car>("configuration", "relations", "physical_car", QString(), list);

    QVERIFY2(!list.isEmpty(), "List shouldn't be empty");

    Physical_car* car = list.first();
    QString orig_id = car->get_id();
    QString orig_rev = car->get_rev();
    int version = car->get_version() + 1;

    car->set_version(version);

    foreach(Physical_ut_car* ut_car, car->get_physical_ut_car())
    {
        ut_car->get_physical_ut()->set_version(version);
        ut_car->get_physical_car()->set_version(version);
    }

    CouchDBManager::DBManagerResponse* resp = db->update<Physical_car>(car);

    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(car->get_id() == orig_id, "Id shouldn't be updated");
    QVERIFY2(car->get_rev() != orig_rev, "Rev should be updated");
    db->set_database_name(old_db);
}

void CouchDBManager_UnitTest::createEntityWithBaseReference()
{
    return;
    Physical_car phyCar;
    Car car;

    db->read<Car>(last_id, QString(), &car);

    CouchDBManager::BaseReference* car_ref = CouchDBManager::BaseReference::fromEntity<Car>(&car);

    QVERIFY2(car_ref->get_id() == car.get_id(), "Incorrect BaseReference._id");
    QVERIFY2(car_ref->get_version() == car.get_version(), "Incorrect BaseReference.version");
    QVERIFY2(car_ref->get_collection() == car.get_collection(), "Incorrect BaseReference.collection");

    phyCar.set_collection("physical_car");
    phyCar.set_name("My Physical Car 1");
    phyCar.set_description("My Own Physical Car 1");
    phyCar.set_version(223);
    phyCar.set_car(car_ref);

    CouchDBManager::DBManagerResponse* response = db->create<Physical_car>(&phyCar);

    QVERIFY2(response->get_went_ok(), "Operation failed");
    QVERIFY2(!phyCar.get_id().isNull(), "_id is null");
    QVERIFY2(!phyCar.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!phyCar.get_rev().isNull(), "_rev is null");
    QVERIFY2(!phyCar.get_rev().isEmpty(), "_rev is empty");

    last_id = phyCar.get_id();
}

void CouchDBManager_UnitTest::readEntityWithBaseReference()
{
    return;
    Physical_car phyCar;

    db->read<Physical_car>(last_id, QString(), &phyCar);

    QVERIFY2(!phyCar.get_id().isNull(), "_id is null");
    QVERIFY2(!phyCar.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!phyCar.get_rev().isNull(), "_rev is null");
    QVERIFY2(!phyCar.get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(phyCar.get_collection() == "physical_car", "Incorrect collection");
    QVERIFY2(phyCar.get_name() == "My Physical Car 1", "Incorrect name");

    CouchDBManager::BaseReference* car_ref = phyCar.get_car();

    QVERIFY2(car_ref->get_collection() == "car", "Incorrect BaseReference.collection");
    QVERIFY2(car_ref->get_version() == 34, "Incorrect BaseReference.version");

    last_id = phyCar.get_id();
    last_rev = phyCar.get_rev();
}

void CouchDBManager_UnitTest::updateEntityWithBaseReference()
{
    return;
    Physical_car phyCar;

    db->read<Physical_car>(last_id, QString(), &phyCar);

    phyCar.set_name("My other physical car");
    phyCar.set_version(76);

    CouchDBManager::DBManagerResponse* resp = db->update<Physical_car>(&phyCar);

    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(phyCar.get_id() == last_id, "_id has cahnged");
    QVERIFY2(phyCar.get_rev() != last_rev, "_rev has not changed");

    CouchDBManager::BaseReference* car_ref = phyCar.get_car();

    last_id = car_ref->get_id();
}

void CouchDBManager_UnitTest::testBaseReference()
{
    return;
    ////////////////////////////////////////////////
    //Creamos un car para la relacion
    int version = 11;
    Car rel_car;

    rel_car.set_name("C1");
    rel_car.set_version(version);

    CouchDBManager::DBManagerResponse* resp = db->create<Car>(&rel_car);
    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(!rel_car.get_id().isNull(), "_id is null");
    QVERIFY2(!rel_car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!rel_car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!rel_car.get_rev().isEmpty(), "_rev is empty");

    ////////////////////////////////////////////////
    //Creamos un physicalCar
    Physical_car phCar;

    phCar.set_name("Physical car 1");
    phCar.set_description("Mi physical car");
    phCar.set_version(version);

    // OPCION 1: FUNCIONA
    CouchDBManager::BaseReference* _carReference= CouchDBManager::BaseReference::fromEntity<Car>(&rel_car);

    QVERIFY2(_carReference->get_id() == rel_car.get_id(), "Entity's and BaseRefence's id are distinct");
    QVERIFY2(_carReference->get_rev() == rel_car.get_rev(), "Entity's and BaseRefence's rev are distinct");
    QVERIFY2(_carReference->get_collection() == rel_car.get_collection(), "Entity's and BaseRefence's collection are distinct");
    QVERIFY2(_carReference->get_version() == rel_car.get_version(), "Entity's and BaseRefence's version are distinct");

    phCar.set_car(_carReference);

    resp = db->create<Physical_car>(&phCar);
    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(!phCar.get_id().isNull(), "_id is null");
    QVERIFY2(!phCar.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!phCar.get_rev().isNull(), "_rev is null");
    QVERIFY2(!phCar.get_rev().isEmpty(), "_rev is empty");
    ////////////////////////////////////////////////
}

void CouchDBManager_UnitTest::testBaseReferenceUsingFilter()
{
    return;
    ////////////////////////////////////////////////
    //Creamos un car para la relacion
    int version = 12;
    Car rel_car;

    rel_car.set_name("C1");
    rel_car.set_version(version);

    CouchDBManager::DBManagerResponse* resp = db->create<Car>(&rel_car);
    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(!rel_car.get_id().isNull(), "_id is null");
    QVERIFY2(!rel_car.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!rel_car.get_rev().isNull(), "_rev is null");
    QVERIFY2(!rel_car.get_rev().isEmpty(), "_rev is empty");

    ////////////////////////////////////////////////
    //Creamos un physicalCar
    Physical_car phCar;

    phCar.set_name("Physical car 1");
    phCar.set_description("Mi physical car");
    phCar.set_version(version);

    //OPCION 2: NO FUNCIONA
    QList<Car*> _car_list;
    QString filterSql;
    filterSql = "(name = 'C1')";
    db->list<Car>("configuration", "relations", "car", filterSql, _car_list);
    if (!_car_list.isEmpty())
    {
         CouchDBManager::BaseReference* _carReference= CouchDBManager::BaseReference::fromEntity<Car>(_car_list.first());

         qDebug() << "Creating Physical Car name..." << _carReference->get_id();
         phCar.set_car(_carReference);
    }


    resp = db->create<Physical_car>(&phCar);
    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(!phCar.get_id().isNull(), "_id is null");
    QVERIFY2(!phCar.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!phCar.get_rev().isNull(), "_rev is null");
    QVERIFY2(!phCar.get_rev().isEmpty(), "_rev is empty");
    ////////////////////////////////////////////////

}

void CouchDBManager_UnitTest::createEntityWithRelations()
{
    return;
    Car car;
    Ut ut;
    CouchDBManager::BaseReference ut_ref;
    CouchDBManager::BaseReference car_ref;
    Physical_ut phyUt;
    Physical_car phyCar;
    Physical_ut_car phyUtCar;

    db->read<Car>(last_id, QString(), &car);
    ut.set_name("My UT 1");
    ut.set_description("Muy Own UT");
    ut.set_version(3);
    ut.set_collection("ut");

    CouchDBManager::DBManagerResponse* resp_ut = db->create<Ut>(&ut);

    QVERIFY2(resp_ut->get_went_ok(), "Operation failed");

    ut_ref.set_id(ut.get_id());
    ut_ref.set_version(ut.get_version());
    ut_ref.set_collection(ut.get_collection());
    car_ref.set_id(car.get_id());
    car_ref.set_version(car.get_version());
    car_ref.set_collection(car.get_collection());
    phyUt.set_name("My Physical UT 1");
    phyUt.set_description("My Own Physical UT");
    phyUt.set_version(4);
    phyUt.set_collection("physical_ut");
    phyUt.set_ut(&ut_ref);
    phyCar.set_name("My Physical Car 1");
    phyCar.set_description("My Own Physical Car");
    phyCar.set_version(8);
    phyCar.set_collection("physical_car");
    phyCar.set_car(&car_ref);
    phyUtCar.set_name("My physical UT->Car");
    phyUtCar.set_physical_car(&phyCar);
    phyUtCar.set_physical_ut(&phyUt);
    phyUtCar.set_collection("physical_ut_car");
    phyUt.add_physical_ut_car( qobject_cast<CouchDBManager::BaseObject*>(&phyUtCar) );

    CouchDBManager::DBManagerResponse* resp = db->create<Physical_ut>(&phyUt);

    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(!phyUt.get_id().isNull(), "_id is null");
    QVERIFY2(!phyUt.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!phyUt.get_id().isNull(), "_id is null");
    QVERIFY2(!phyUt.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!phyUtCar.get_id().isNull(), "_id is null");
    QVERIFY2(!phyUtCar.get_id().isEmpty(), "_id is empty");

    last_id = phyUt.get_id();
}

void CouchDBManager_UnitTest::listEntityWithRelations()
{
    return;
    QList<Physical_ut*> phyUtList;

    db->list("configuration", "relations", "physical_ut", phyUtList);

    QVERIFY2(phyUtList.size() > 0, "Sin registros");

    Physical_ut* phyUt = phyUtList.last();

    QVERIFY2(!phyUt->get_id().isNull(), "_id is null");
    QVERIFY2(!phyUt->get_id().isEmpty(), "_id is empty");
    QVERIFY2(!phyUt->get_rev().isNull(), "_rev is null");
    QVERIFY2(!phyUt->get_rev().isEmpty(), "_rev is empty");
    QVERIFY2(phyUt->get_collection() == "physical_ut", "Incorrect collection");
    QVERIFY2(phyUt->get_name() == "My Physical UT 1", "Incorrect name");
    QVERIFY2(phyUt->get_physical_ut_car().size() > 0, "No relations loaded");
    QVERIFY2(!phyUt->get_ut()->get_id().isNull(), "BaseReference._id is Null");
    QVERIFY2(!phyUt->get_ut()->get_id().isEmpty(), "BaseReference._id is Empty");
    QVERIFY2(phyUt->get_ut()->get_collection() == "ut", "Incorrect BaseReference.collection");
    QVERIFY2(phyUt->get_ut()->get_version() == 3, "Incorrect BaseReference.version");

    if (phyUt->get_physical_ut_car().size() > 0)
    {
        Physical_ut_car* phyUtCar = phyUt->get_physical_ut_car().at(0);

        QVERIFY2(!phyUtCar->get_id().isNull(), "_id is null");
        QVERIFY2(!phyUtCar->get_id().isEmpty(), "_id is empty");
        QVERIFY2(!phyUtCar->get_rev().isNull(), "_rev is null");
        QVERIFY2(!phyUtCar->get_rev().isEmpty(), "_rev is empty");
        QVERIFY2(phyUtCar->get_collection() == "physical_ut_car", "Incorrect collection");
        QVERIFY2(phyUtCar->get_name() == "My physical UT->Car", "Incorrect name");
        QVERIFY2(phyUtCar->get_physical_car() != NULL, "No relation loaded");

        if (phyUtCar->get_physical_car() != NULL)
        {
            Physical_car* phyCar = phyUtCar->get_physical_car();

            QVERIFY2(!phyCar->get_id().isNull(), "_id is null");
            QVERIFY2(!phyCar->get_id().isEmpty(), "_id is empty");
            QVERIFY2(!phyCar->get_rev().isNull(), "_rev is null");
            QVERIFY2(!phyCar->get_rev().isEmpty(), "_rev is empty");
            QVERIFY2(phyCar->get_collection() == "physical_car", "Incorrect collection");
            QVERIFY2(phyCar->get_name() == "My Physical Car 1", "Incorrect name");
            QVERIFY2(!phyCar->get_car()->get_id().isNull(), "BaseReference._id is Null");
            QVERIFY2(!phyCar->get_car()->get_id().isEmpty(), "BaseReference._id is Empty");
            QVERIFY2(phyCar->get_car()->get_collection() == "car", "Incorrect BaseReference.collection");
            QVERIFY2(phyCar->get_car()->get_version() == 12, "Incorrect BaseReference.version");
        }
    }

    last_id = phyUt->get_id();
}

void CouchDBManager_UnitTest::updateEntityWithRelations()
{
    return;
    QString old_db = db->get_database_name();
    QList<Device*> list;

    db->list<Device>("configuration", "relations", "device", "name='sMON11'", list);

    if (list.size() > 0)
    {
        Device* dev = list.first();
        QString orig_id = dev->get_id();
//        int orig_size = dev->get_bus_device().size();

//        if (dev->get_bus_device().size() > 0)
//        {
//            Configuration::Rel_bus_device* rel = dev->get_bus_device().first();

//            dev->get_bus_device().removeOne(rel);

//            CouchDBManager::DBManagerResponse* resp = db->update<Device>(dev);

//            QVERIFY2(resp->get_went_ok(), "Operation failed");
//        }
    }

    db->set_database_name(old_db);
}

void CouchDBManager_UnitTest::deleteEntity()
{
    return;
    Car car;

    db->read<Car>(last_id, QString(), &car);

    CouchDBManager::DBManagerResponse response = db->delete_doc<Car>(&car);

    QVERIFY2(response.get_went_ok(), "Operation failed");
}

void CouchDBManager_UnitTest::replicate_local_2_local()
{
    return;
    CouchDBManager::ReplicationConfig cfg;
    CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(this);
    CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(this);

    source->set_url("aunia");
    target->set_url("aunia_replicate_unit_test");
    cfg.set_source(source);
    cfg.set_target(target);
    cfg.set_create_target(true);

    bool replicated = db->replicate(&cfg);

    QVERIFY2(replicated, "Replication failed");
}

void CouchDBManager_UnitTest::replicate_local_2_remote()
{
    return;
    CouchDBManager::ReplicationConfig cfg;
    CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(this);
    CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(this);

    source->set_url("aunia_replicate_unit_test");
    target->set_url("http://172.24.2.69:5984/aunia_replicate_unit_test");
    target->set_authorization( CouchDBManager::DBManager::base64_encode("root:root") );
    cfg.set_source(source);
    cfg.set_target(target);
    cfg.set_create_target(true);

    bool replicated = db->replicate(&cfg);

    QVERIFY2(replicated, "Replication failed");
}

void CouchDBManager_UnitTest::replicate_remote_2_local()
{
    return;
    CouchDBManager::ReplicationConfig cfg;
    CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(this);
    CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(this);

    source->set_url("http://172.24.2.69:5984/aunia_replicate_unit_test");
    source->set_authorization( CouchDBManager::DBManager::base64_encode("root:root") );
    target->set_url("aunia_replicate_unit_test_2");
    cfg.set_source(source);
    cfg.set_target(target);
    cfg.set_create_target(true);

    bool replicated = db->replicate(&cfg);

    QVERIFY2(replicated, "Replication failed");
}

void CouchDBManager_UnitTest::replicate_service_local_2_remote()
{
    return;
    CouchDBManager::ReplicationConfig cfg;
    QMap<QString, QVariant> params;
    CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(this);
    CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(this);

    source->set_url("http://localhost:5984/aunia");
//    source->set_url("http://localhost:5984/josetxo_problem");
    source->set_authorization( CouchDBManager::DBManager::base64_encode("root:root") );
    target->set_url("http://172.24.2.69:5984/aunia_repl_serv_test");
//    target->set_url("http://172.24.2.69:5984/josetxo_problem_repl_test");
    target->set_authorization( CouchDBManager::DBManager::base64_encode("root:root") );

    params.insert("where", "_id like '_design'");

    cfg.set_id("replication-service-central-test");
    cfg.set_name("Central replication service test");
    cfg.set_source(source);
    cfg.set_target(target);
    cfg.set_create_target(true);
    cfg.set_continuous(true);
    cfg.set_filter("commons/by_query");
    cfg.set_query_params(params);

    bool added = db->add_replication_service(&cfg);

    QVERIFY2(added, "Add replication service failed");
}

void CouchDBManager_UnitTest::update_replication_service()
{
    return;
//    CouchDBManager::ReplicationConfig* cfg = db->read_replication_service("replication-service-central-test");
    CouchDBManager::ReplicationConfig* cfg = db->read_replication_service("replication-service-josetxo_problem");

    QVERIFY2(!cfg->get_name().isNull(), "El nombre de la configuración es nulo");
    QVERIFY2(!cfg->get_name().isEmpty(), "El nombre de la configuración es vacio");

    cfg->set_name("Nombre modificado");
    cfg->set_rev(QString());

    bool deleted = db->remove_replication_service(cfg->get_id());

    QVERIFY2(deleted, "Delete replication service failed");

    bool added = db->add_replication_service(cfg);

    QVERIFY2(added, "Add replication service failed");
}

void CouchDBManager_UnitTest::listActiveTasks()
{
    return;
    QList<CouchDBManager::ActiveTask*> list = db->list_active_tasks();

    foreach (CouchDBManager::ActiveTask* task, list)
    {
        qDebug() << "TASK PID:" << task->get_pid();
    }

    QVERIFY2(list.size() > 0, "List should contains data");
}

void CouchDBManager_UnitTest::remove_reaplication_service()
{
    return;
    QThread::sleep(10);
    bool removed = db->remove_replication_service("replication-service-central-test");

    QVERIFY2(removed, "Replication service not removed");
}

void CouchDBManager_UnitTest::controlPQLError()
{
    return;
    QList<Bus*> busList;

    db->list("configuration", "relations", "bus", "name is 'Bus 1'", busList);

    QVERIFY2(busList.size() == 0, "QList contains items");
    QVERIFY2(db->get_network_error() != QNetworkReply::NoError, "Does not exists network error");
    QVERIFY2(!db->get_error_string().isNull(), "Error string is NULL");
    QVERIFY2(!db->get_error_string().isEmpty(), "Error string is EMPTY");
}

void CouchDBManager_UnitTest::setInnerData()
{
    return;
    Configuration::Types _types;

    _types.set_name("Configuration types");
    _types.set_description("Tipos Configuracion Aunia");


    Configuration::Variable_types _variable_types_1;

    _variable_types_1.set_name("Variable 1");
    _variable_types_1.set_description("Desc Variable 1");
    _variable_types_1.set_size(20);
    _variable_types_1.set_c_type("Type 1");

    Configuration::Variable_types _variable_types_2;

    _variable_types_2.set_name("Variable 2");
    _variable_types_2.set_description("Desc Variable 2");
    _variable_types_2.set_size(25);
    _variable_types_2.set_c_type("Type 2");

    _types.add_variable_types(&_variable_types_1);
    _types.add_variable_types(&_variable_types_2);

    Configuration::Io_card_types _io_card_type;

    _io_card_type.set_name("Io_card_type_1");
    _io_card_type.set_description("Description Io_card_type_1");
    _io_card_type.set_digital_channel_inversion_mode_type_id("DIGI_CHANNEL_ID");
    _io_card_type.add_variable_types_names("Variable 1");
    _io_card_type.add_variable_types_names("Variable 2");

    _types.add_io_card_types(&_io_card_type);

    CouchDBManager::DBManagerResponse* resp = db->create<Configuration::Types>(&_types);

    QVERIFY2(resp->get_went_ok(), "Operation failed");
    QVERIFY2(!_types.get_id().isNull(), "_id is null");
    QVERIFY2(!_types.get_id().isEmpty(), "_id is empty");
    QVERIFY2(!_types.get_rev().isNull(), "_rev is null");
    QVERIFY2(!_types.get_rev().isEmpty(), "_rev is empty");

    last_id_aux = _types.get_id();

}

void CouchDBManager_UnitTest::getInnerData()
{
    return;
    Configuration::Types _types;
    db->read<Configuration::Types>(last_id_aux, QString(), &_types);

    Configuration::Io_card_types *_listIoCards = _types.get_io_card_types().first();
    QList<QString> _variable_types_names = _listIoCards->get_variable_types_names();

    Configuration::Variable_types *_var;
    foreach(QString _var_type, _variable_types_names)
    {
        _var = _types.get_inner_object<Configuration::Variable_types>("name", QVariant("Variable 1"));
        QString descr= _var->get_description();
        QVERIFY2(!_var->get_description().isNull(), "_id is null");
    }
}


void CouchDBManager_UnitTest::userLogout()
{
    bool logged_out = db->logout();

    QVERIFY2(logged_out, "Not logged oun");
}

void CouchDBManager_UnitTest::userIsLoggedOut()
{
    QVERIFY2(db->get_auth_cookie().isNull(), "Auth is not NULL");
    QVERIFY2(db->get_auth_cookie().isEmpty(), "Auth is not EMPTY");

    CouchDBManager::UserContext* uc = db->user_context();

    QVERIFY2(uc->get_userCtx().name.isNull(), "User name is not NULL");
    QVERIFY2(uc->get_userCtx().name.isEmpty(), "User name is not EMPTY");
}

void CouchDBManager_UnitTest::playingWithDates()
{
    return;
    QString a_day = "14/07/2016";
    QDateTime datetime = CouchDBManager::DBManager::toDateTime(a_day);
    qint64 timestamp = CouchDBManager::DBManager::toUnixTimeStamp(a_day);
    QString datestring = CouchDBManager::DBManager::formatDate(datetime, "dd/MM/yyyy");
    QString tsstring = CouchDBManager::DBManager::formatDate(timestamp, "dd/MM/yyyy");

    QVERIFY2(a_day == datestring, "Incorrect str > datetime > str parse");
    QVERIFY2(a_day == tsstring, "Incorrect str > timestamp > str parse");
}

void CouchDBManager_UnitTest::destroyManager()
{
    delete db;
}

QTEST_MAIN(CouchDBManager_UnitTest)

#include "tst_couchdbmanager_unittest.moc"
