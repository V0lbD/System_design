# Задание №3, вариант 3 (Сайт конференции)
**Выполнил**: *Злобин Владимир Олегович* \
**Группа**: *М8О-102СВ-21* \
**Стек:** *C++20 + Yandex Userver* \
**Хранилище:** *PostgreSQL*

---

## Описание проекта

В рамках лабораторной работы №3 выполнен переход от InMemoryStorage к PostgreSQL с полноценной схемой БД, SQL-запросами, индексацией и оптимизацией.

---

## Реализованный функционал

## Пользователи
- регистрация пользователя
- авторизация
- поиск пользователя по логину
- поиск пользователей по маске имени и фамилии

### Доклады
- создание доклада
- получение списка всех докладов

### Конференции
- создание конференции
- добавление доклада в конференцию
- получение списка докладов конференции

---

## Структура базы данных

Используются таблицы:

- users
- tokens
- talks
- conferences
- conference_talks

### SQL-файлы
- `sql/schema.sql` - создание схемы БД
- `sql/data.sql` - тестовые данные
- `sql/queries.sql` - примеры SQL-запросов
- `optimization.md` - анализ и оптимизация запросов

---

## Запуск проекта через Docker Compose

Из корня проекта:

```
docker compose down -v
docker compose up --build
```

После запуска сервис будет доступен по адресу:

http://127.0.0.1:8080

---

## Проверка работоспособности

### Ping
```bash
curl http://127.0.0.1:8080/ping
```

### Регистрация пользователя
``` bash
curl -X POST "http://127.0.0.1:8080/auth/register" \
  -H "Content-Type: application/json" \
  -d '{
    "login": "test_user",
    "password": "123456",
    "first_name": "Test",
    "last_name": "User",
    "role": "organizer"
  }'
```

### Авторизация
```bash
curl -X POST "http://127.0.0.1:8080/auth/login" \
  -H "Content-Type: application/json" \
  -d '{
    "login": "test_user",
    "password": "123456"
  }'
```

### Получение всех докладов
```bash
curl http://127.0.0.1:8080/talks
```

### Создание доклада
```bash
curl -X POST "http://127.0.0.1:8080/talks" \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer token_424242" \
  -d '{
    "title": "PostgreSQL Integration",
    "description": "Migration from in-memory storage to PostgreSQL"
  }'
```

### Создание конференции
```bash
curl -X POST "http://127.0.0.1:8080/conferences" \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer token_424242" \
  -d '{
    "title": "System Design Conference",
    "description": "Final database integration test",
    "date": "2026-05-01"
  }'
```

### Получение докладов конференции
```bash
curl "http://127.0.0.1:8080/conferences/1/talks"
```

---

## Запуск в Dev Container

### Поднять PostgreSQL
На хостовой машине:

```
docker compose up -d postgres
```

### Запуск API внутри dev-container
```
make build-debug

./build-debug/conference_api \
  --config ./configs/static_config.yaml \
  --config_vars ./configs/config_vars.yaml
```
---

## Особенности реализации

### PostgreSQL storage layer
Создан отдельный компонент:

- `src/storage/postgres_storage.hpp`
- `src/storage/postgres_storage.cpp`

В нём реализованы SQL-операции для:
- users
- tokens
- talks
- conferences
- conference_talks

### Индексация
Добавлены индексы для ускорения поиска пользователя по логину, выборки докладов конференции и join-операций.

### Анализ производительности
Для основных запросов использован `EXPLAIN ANALYZE`.

Результаты анализа и выводы находятся в файле:
`optimization.md`

---

## Итоги лабораторной работы

В рамках лабораторной работы:
- спроектирована реляционная схема БД;
- реализована PostgreSQL persistence layer;
- перенесён REST API с in-memory на PostgreSQL;
- проведена индексация и оптимизация запросов.