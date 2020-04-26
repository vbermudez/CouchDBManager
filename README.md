CouchDBManager
================
> _A C++/Qt API that manages entities stored in CouchDB._

[Documentación anterior](https://github.com/vbermudez/CouchDBManager/wiki)

[CouchDBManager Design Documents](https://github.com/vbermudez/CouchDBManager-DesignDocuments)

## Tabla de contenidos
- **[Gestión de bloqueos](#gestión-de-bloqueos)**
    - [Creación de CouchDBManager](#creación-de-couchdbmanager)
    - [Forzar el tipo de conexión](#forzar-el-tipo-de-conexión)
    - [Iniciar sesión en CouchDB](#iniciar-sesión-en-couchdb)
    - [Recuperar una entidad](#recuperar-una-entidad)
    - [Bloquear y desbloquear de una entidad](#bloquear-y-desbloquear-de-una-entidad)
    - [Actualizar una entidad](#actualizar-una-entidad)
    - [Eliminar una entidad](#eliminar-una-entidad)
    - [Errores](#errores)
- **[Mejoras en sincronización](#mejoras-en-sincronización)**
    - [Realizar una sincronización a demanda](#realizar-una-sincronización-a-demanda)
    - [Configurar una sincronización continua](#configurar-una-sincronización-continua)
    - [Configurar una sincronización filtrada](#configurar-una-sincronización-filtrada)
    - [Listar las tareas activas](#listar-las-tareas-activas)
    - [Listar las replicaciones activas](#listar-las-replicaciones-activas)
- **[Resolución de incidencias y mejoras](#resolución-de-incidencias-y-mejoras)**
    - [Mejora en la gestión de versiones](#mejora-en-la-gestión-de-versiones)
    - [Nuevas funciones](#nuevas-funciones)

## Novedades 2017

### Gestión de bloqueos

Ahora es posible gestionar los bloqueos de las entidades versionables a nivel de usuario. Además, al actualizar una entidad (cualquier actualizaión), se comprueba el estado del bloqueo. Si la entidad no está bloqueada, o está bloqueada por otro usuario, la actualización no se realiza y se genera un error.

Este mecanismo entra en funcionamiento únicamente si la conexión es remota o si se fuerza dicho comportamiento.

Los siguientes ejemplos pueden servir como referencia.

_[Inicio](#tabla-de-contenidos)_

#### Creación de CouchDBManager
> _Con detección automática del tipo de conexión (remota o local)_

- Contructor sin configuración
```c++
CouchDBManager::DBManager* db = new CouchDBManager::DBManager(this);

db->set_server_address("http://nombre.o.ip:5984");
db->set_database_name("base_de_datos");
qDebug() << db->get_remote_conn(); // prints true or false
```

- Constructor con configuración
```c++
CouchDBManager::DBManager* db = new CouchDBManager::DBManager("http://nombre.o.ip:5984", "base_de_datos", this);
qDebug() << db->get_remote_conn(); // prints true or false
```

_[Inicio](#tabla-de-contenidos)_

#### Forzar el tipo de conexión
> _Activando la gestión del bloqueo_

- Contructor sin configuración
```c++
CouchDBManager::DBManager* db = new CouchDBManager::DBManager(this);

db->set_server_address("http://nombre.o.ip:5984");
db->set_database_name("base_de_datos");
db->set_remote_conn(true);
qDebug() << db->get_remote_conn(); // prints true
```

- Constructor con configuración
```c++
CouchDBManager::DBManager* db = new CouchDBManager::DBManager("http://nombre.o.ip:5984", "base_de_datos", this);
db->set_remote_conn(true);
qDebug() << db->get_remote_conn(); // prints true
```

_[Inicio](#tabla-de-contenidos)_

#### Iniciar sesión en CouchDB
```c++
QStringList roles;
bool logged_in = db->login("usuario", "clave", roles);
qDebug() << logged_in; // prints true or false
qDebug() << roles; // prints list of roles or nothing
```

_[Inicio](#tabla-de-contenidos)_

#### Recuperar una entidad

```c++
VersionableEntity entity;
db->read<VersionableEntity>("identificador", &entity);
qDebug() << entity.get_locked(); // prints true or false
qDebug() << entity.get_locked_by(); // prints "" or user name
```

_[Inicio](#tabla-de-contenidos)_

#### Bloquear y desbloquear de una entidad
> _Actualiza la BBDD_

- Bloquear con el usuario que ha iniciado sesión
```c++
bool locked = db->lock<VersionableEntity>(&entity);
qDebug() << locked; // prints true or false
```

- Desbloquear con el usuario que ha iniciado sesión
```c++
bool unlocked = db->unlock<VersionableEntity>(&entity);
qDebug() << unlocked; // prints true or false
```

- Bloquear con un usuario distinto al que ha iniciado sesión
```c++
bool locked = db->lock<VersionableEntity>(&entity, "otrousuario");
qDebug() << locked; // prints true or false
```

- Desbloquear on un usuario distinto al que ha iniciado sesión
```c++
bool unlocked = db->unlock<VersionableEntity>(&entity, "otrousuario");
qDebug() << unlocked; // prints true or false
```

_[Inicio](#tabla-de-contenidos)_

#### Actualizar una entidad

- Con desbloqueo en una petición
```c++
bool updated = db->update<VersionableEntity>(&entity, true);
qDebug() << updated; // prints true or false
```

- Con desbloqueo en otra petición
```c++
CouchDBManager::DBManagerResponse* resp = db->update<VersionableEntity>(&entity);
qDebug() << resp->get_went_ok(); // prints true or false
bool unlocked = db->unlock<VersionableEntity>(&entity);
qDebug() << unlocked; // prints true or false
```

_[Inicio](#tabla-de-contenidos)_

#### Eliminar una entidad

```c++
CouchDBManager::DBManagerResponse* resp = db->delete_doc<VersionableEntity>(&entity);
qDebug() << resp->get_went_ok(); // prints true or false
```

_[Inicio](#tabla-de-contenidos)_

#### Errores

- Intenta bloquear una entidad ya bloqueada por el usuario actual
```c++
bool locked = db->lock<VersionableEntity>(&entity);
qDebug() << locked; // prints false
// Escribe en el stream qWarning() un aviso
qDebug() << db->get_error_string() // prints nothing
```

- Intenta desbloquear una entidad ya desbloqueada
```c++
bool unlocked = db->unlock<VersionableEntity>(&entity);
qDebug() << unlocked; // prints false
// Escribe en el stream qWarning() un aviso
qDebug() << db->get_error_string() // prints nothing
```

- Intenta bloquear una entidad ya bloqueada por otro usuario
```c++
bool locked = db->lock<VersionableEntity>(&entity);
qDebug() << locked; // prints false
// Escribe en el stream qCritical() un error
qDebug() << db->get_error_string() // prints error string
```

- Intenta desbloquear una entidad ya bloqueada por otro usuario
```c++
bool unlocked = db->unlock<VersionableEntity>(&entity);
qDebug() << unlocked; // prints false
// Escribe en el stream qCritical() un error
qDebug() << db->get_error_string() // prints error string
```

- Intenta actualizar una entidad no bloqueada o bloqueada por otro usuario
```c++
CouchDBManager::DBManagerResponse* resp = db->update<VersionableEntity>(&entity); // Igual con la sobrecarga
qDebug() << resp->get_went_ok(); // prints false
// Escribe en el stream qCritical() un error
qDebug() << db->get_error_string() // prints error string
```

- Intenta eliminar una entidad no bloqueada o bloqueada por otro usuario
```c++
CouchDBManager::DBManagerResponse* resp = db->delete_doc<VersionableEntity>(&entity);
qDebug() << resp->get_went_ok(); // prints false
// Escribe en el stream qCritical() un error
qDebug() << db->get_error_string() // prints error string
```

_[Inicio](#tabla-de-contenidos)_

### Mejoras en sincronización

Ahora, al realizar una `sincronización a demanda`, la función espera a la finalización de la trasacción.
También se pueden configurar sincronizaciones filtradas. Los filtros están integrados con PQL.
Por último, se pueden listar de forma detallada tanto las tareas en segundo plano, como las sincronizaciones (continuas o a demanda), actualmente activas en la base de datos.

_[Inicio](#tabla-de-contenidos)_

#### Realizar una sincronización a demanda

```c++
CouchDBManager::ReplicationConfig cfg;
CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(this);
CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(this);

source->set_url("BBDD_LOCAL_ORIGEN");
// También puede ser otra BBDD local, no requiriendo la URL ni la información de usuario
target->set_url("http://nombre.o.ip:5984/BBDD_DESTINO");
target->set_authorization( CouchDBManager::DBManager::base64_encode("usuario:clave") );
cfg.set_source(source);
cfg.set_target(target);
cfg.set_create_target(true);

bool replicated = db->replicate(&cfg);
```

_[Inicio](#tabla-de-contenidos)_

#### Configurar una sincronización continua

```c++
CouchDBManager::ReplicationConfig cfg;
CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(this);
CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(this);

source->set_url("http://nombre.o.ip:5984/BBDD_ORIGEN");
source->set_authorization( CouchDBManager::DBManager::base64_encode("usuario:clave") );
target->set_url("http://nombre.o.ip.dest:5984/BBDD_DESTINO");
target->set_authorization( CouchDBManager::DBManager::base64_encode("usuario:clave") );
cfg.set_source(source);
cfg.set_target(target);
cfg.set_create_target(true);
cfg.set_continuous(true);

bool added = db->add_replication_service(&cfg);
```

_[Inicio](#tabla-de-contenidos)_

#### Configurar una sincronización filtrada

```c++
CouchDBManager::ReplicationConfig cfg;
CouchDBManager::ServerResource* source = new CouchDBManager::ServerResource(this);
CouchDBManager::ServerResource* target = new CouchDBManager::ServerResource(this);
QMap<QString, QVariant> params;

source->set_url("http://nombre.o.ip:5984/BBDD_ORIGEN");
source->set_authorization( CouchDBManager::DBManager::base64_encode("usuario:clave") );
target->set_url("http://nombre.o.ip.dest:5984/BBDD_DESTINO");
target->set_authorization( CouchDBManager::DBManager::base64_encode("usuario:clave") );
cfg.set_source(source);
cfg.set_target(target);
cfg.set_create_target(true);
cfg.set_continuous(true);
// Filtro personalizado existente. Filtra usando PQL.
cfg.set_filter("commons/by_query");
// Parámetro QueryString "where" con la consulta PQL.
params.insert("where", "collection='car'");
cfg.set_query_params(params);

bool added = db->add_replication_service(&cfg);
```

_[Inicio](#tabla-de-contenidos)_

#### Listar las tareas activas

```c++
QList<CouchDBManager::ActiveTask*> tasks = db->list_active_tasks();

// ...
```

_[Inicio](#tabla-de-contenidos)_

#### Listar las replicaciones activas

```c++
QList<CouchDBManager::ReplicationConfig*> repls = db->list_active_replications();

// ...
```

_[Inicio](#tabla-de-contenidos)_

### Resolución de incidencias y mejoras

Resolución de incidencias y mejoras realizadas durante esta fase de desarrollo.

_[Inicio](#tabla-de-contenidos)_

#### Mejora en la gestión de versiones

Ahora no es posible disminiur la versión de un documento `VersionableEntity`. Si se intenta actualiza la propiedad `version` de un documento versionable con un valor inferior al de la misma propiedad del documento existente en la BBDD, `CouchDBManager` no permitirá la acutalización y generará un error.

_[Inicio](#tabla-de-contenidos)_

#### Nuevas funciones

Ahora se puede listar las `BBDD` existentes en el servidor `CouchDB`.

```c++
QStringList db_list = db->list_databases();

// Escribe los nombres de las BBDD por consola
qDebug() << db_list; // ["_replicator","_users",...]
```

_[Inicio](#tabla-de-contenidos)_
