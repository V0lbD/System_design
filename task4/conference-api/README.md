# Conference API — лабораторная работа №4

## Тема

Проектирование и работа с MongoDB.

## Цель работы

Получить практические навыки работы с MongoDB:

- проектирование документной модели данных;
- создание коллекций;
- добавление тестовых данных;
- выполнение CRUD-операций;
- использование операторов MongoDB;
- настройка `$jsonSchema`-валидации;
- подключение MongoDB к API.

## Предметная область

Проект реализует backend для системы "Сайт конференции".

Основные сущности:

- пользователи;
- доклады;
- конференции;
- токены авторизации.

В предыдущей лабораторной работе данные хранились в PostgreSQL. В этой лабораторной добавлена MongoDB и реализован отдельный API endpoint для работы с коллекцией `users`.

## Используемые технологии

- C++20;
- Yandex Userver;
- PostgreSQL 16;
- MongoDB 7;
- Docker;
- Docker Compose;
- VS Code Dev Containers;
- pytest / userver testsuite.

## Структура MongoDB-модели

В MongoDB используется база данных:

```text
conference_mongo
```

Коллекции:

```text
users
talks
conferences
tokens
```

Описание документной модели и обоснование выбора embedded documents / references находится в файле:

```text
schema_design.md
```

## Файлы лабораторной работы №4

```text
schema_design.md   — описание документной модели MongoDB
data.js            — тестовые данные, минимум 10 документов в каждой коллекции
queries.js         — CRUD-запросы и aggregation pipeline
validation.js      — $jsonSchema-валидация коллекции users
README.md          — инструкция по запуску и проверке
docker-compose.yaml — запуск PostgreSQL, MongoDB и API
```

## Запуск инфраструктуры

Из папки проекта:

```powershell
docker compose up -d postgres mongo
```

Проверить контейнеры:

```powershell
docker ps --format "table {{.Names}}\t{{.Image}}\t{{.Ports}}\t{{.Status}}"
```

Ожидаемые контейнеры:

```text
conference-postgres
conference-mongo
```

## Особенность запуска через VS Code Dev Container

API разрабатывается и запускается из VS Code Dev Container.

Если devcontainer не видит контейнеры `conference-postgres` и `conference-mongo`, его нужно подключить к Docker-сети проекта.

Узнать список сетей:

```powershell
docker network ls
```

Подключить devcontainer к сети проекта:

```powershell
docker network connect <network_name> <devcontainer_name>
```

Пример:

```powershell
docker network connect conference-api_default vigorous_brahmagupta
```

После этого из devcontainer должны работать команды:

```bash
getent hosts conference-postgres
getent hosts conference-mongo
```

## Настройки подключения

Для запуска API из devcontainer используется файл:

```text
configs/config_vars.local.yaml
```

Пример значений:

```yaml
postgres_dsn: postgresql://conference_user:conference_pass@conference-postgres:5432/conference_db
mongo_dsn: mongodb://conference-mongo:27017/conference_mongo
```

Для тестов используется:

```text
configs/config_vars.testing.yaml
```

Для запуска всех сервисов внутри `docker-compose` используется:

```text
configs/config_vars.yaml
```

В этом случае имена сервисов:

```yaml
postgres_dsn: postgresql://conference_user:conference_pass@postgres:5432/conference_db
mongo_dsn: mongodb://mongo:27017/conference_mongo
```

## Загрузка тестовых данных MongoDB

Скрипт `data.js` создаёт базу `conference_mongo`, очищает её и добавляет по 10 документов в коллекции:

- `users`;
- `talks`;
- `conferences`;
- `tokens`.

Так как Docker CLI запускается из PowerShell, сначала нужно скопировать файл в контейнер:

```powershell
docker cp .\data.js conference-mongo:/tmp/data.js
```

Затем выполнить:

```powershell
docker exec -it conference-mongo mongosh /tmp/data.js
```

Ожидаемый результат:

```text
Inserted users: 10
Inserted talks: 10
Inserted conferences: 10
Inserted tokens: 10
```

Проверка количества документов:

```powershell
docker exec -it conference-mongo mongosh conference_mongo --eval "print('users: ' + db.users.countDocuments()); print('talks: ' + db.talks.countDocuments()); print('conferences: ' + db.conferences.countDocuments()); print('tokens: ' + db.tokens.countDocuments());"
```

## Выполнение CRUD-запросов

Файл `queries.js` содержит примеры операций:

- `insertOne`;
- `find`;
- `updateOne`;
- `deleteOne`;
- `deleteMany`;
- `$eq`;
- `$ne`;
- `$gt`;
- `$lt`;
- `$in`;
- `$and`;
- `$or`;
- `$push`;
- `$pull`;
- `$addToSet`;
- aggregation pipeline со стадиями `$match`, `$group`, `$project`, `$sort`.

Запуск:

```powershell
docker cp .\queries.js conference-mongo:/tmp/queries.js
docker exec -it conference-mongo mongosh /tmp/queries.js
```

В конце должен быть вывод:

```text
CRUD queries completed successfully.
```

Проверка, что временные документы удалены:

```powershell
docker exec -it conference-mongo mongosh conference_mongo --eval "print('temporary users: ' + db.users.countDocuments({login: 'temporary_user'})); print('temporary talks: ' + db.talks.countDocuments({title: 'Временный доклад по MongoDB CRUD'})); print('temporary tokens: ' + db.tokens.countDocuments({token: 'token_temporary_user'}));"
```

Ожидаемый результат:

```text
temporary users: 0
temporary talks: 0
temporary tokens: 0
```

## Валидация схемы MongoDB

Файл `validation.js` задаёт `$jsonSchema`-валидацию для коллекции `users`.

Проверяются:

- обязательные поля;
- типы данных;
- формат `login`;
- формат `email`;
- допустимые значения `role`;
- диапазон `rating`;
- типы дат.

Запуск:

```powershell
docker cp .\validation.js conference-mongo:/tmp/validation.js
docker exec -it conference-mongo mongosh /tmp/validation.js
```

Ожидаемый результат:

```text
Invalid user rejected as expected.
Invalid update rejected as expected.
Schema validation script completed successfully.
```

Проверка установленного валидатора:

```powershell
docker exec -it conference-mongo mongosh conference_mongo --eval "printjson(db.getCollectionInfos({name: 'users'})[0].options.validator)"
```

## MongoDB API

В API добавлен endpoint:

```text
GET /mongo/users
POST /mongo/users
```

### GET /mongo/users

Возвращает статистику по коллекции `users` в MongoDB.

Пример:

```bash
curl -s http://127.0.0.1:8080/mongo/users
```

Пример ответа:

```json
{
  "storage": "mongodb",
  "collection": "users",
  "total_count": 10,
  "speakers_count": 5,
  "organizers_count": 2,
  "participants_count": 3
}
```

### POST /mongo/users

Создаёт пользователя в коллекции `users`.

Пример:

```bash
curl -s -X POST http://127.0.0.1:8080/mongo/users \
  -H "Content-Type: application/json" \
  -d '{
    "login": "api_mongo_user",
    "password": "123456",
    "first_name": "Api",
    "last_name": "Mongo",
    "role": "participant",
    "email": "api_mongo_user@example.com"
  }'
```

Пример успешного ответа:

```json
{
  "storage": "mongodb",
  "login": "api_mongo_user",
  "first_name": "Api",
  "last_name": "Mongo",
  "role": "participant",
  "email": "api_mongo_user@example.com"
}
```

При повторной вставке пользователя с тем же `login` MongoDB возвращает ошибку unique index.

## Сборка API

Внутри VS Code Dev Container:

```bash
make build-debug
```

## Запуск API из devcontainer

```bash
./build-debug/conference_api \
  --config ./configs/static_config.yaml \
  --config_vars ./configs/config_vars.local.yaml
```

Проверка:

```bash
curl http://127.0.0.1:8080/ping
```

## Запуск тестов

Внутри devcontainer:

```bash
make test-debug
```

В проекте проверяются:

- `/ping`;
- регистрация и логин через PostgreSQL;
- создание доклада;
- добавление доклада в конференцию;
- конфликт повторного добавления доклада;
- чтение статистики пользователей MongoDB;
- создание пользователя MongoDB;
- конфликт при повторном создании пользователя MongoDB;
- валидация ошибок входных данных MongoDB API.

Ожидаемый результат:

```text
8 passed
```

## Итог

В лабораторной работе выполнено:

- спроектирована документная модель MongoDB;
- созданы коллекции `users`, `talks`, `conferences`, `tokens`;
- добавлены тестовые данные;
- реализованы CRUD-запросы;
- использованы основные операторы MongoDB;
- добавлена `$jsonSchema`-валидация;
- реализовано подключение MongoDB к C++ API;
- добавлены тесты для MongoDB endpoint.
