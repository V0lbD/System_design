# Задание №5, вариант 3 (Сайт конференции)
**Выполнил**: *Злобин Владимир Олегович* \
**Группа**: *М8О-102СВ-21* \
**Стек:** *C++20 + Yandex Userver* \

## Тема

Оптимизация производительности через кеширование и rate limiting.

## Цель работы

Получить практические навыки проектирования REST API с учётом производительности:

- анализ hot paths;
- проектирование стратегии кеширования;
- реализация кеширования для часто читаемых endpoints;
- инвалидация кеша при изменении данных;
- проектирование rate limiting;
- реализация ограничения частоты запросов;
- документирование влияния оптимизаций на производительность.

## Что реализовано

### 1. Анализ производительности

Описание hot paths, медленных операций, требований к производительности и метрик находится в файле:

```text
performance_design.md
```

### 2. Кеширование

Реализовано in-memory Cache-Aside кеширование для двух endpoints:

```text
GET /talks
GET /conferences/{conferenceId}/talks
```

Ключи кеша:

```text
talks:all
conference:{conferenceId}:talks
```

TTL:

```text
60 секунд
```

Инвалидация кеша:

| Операция | Инвалидируемый ключ |
|---|---|
| `POST /talks` | `talks:all` |
| `POST /conferences/{conferenceId}/talks/{talkId}` | `conference:{conferenceId}:talks` |

Компонент кеша:

```text
src/cache/response_cache.hpp
src/cache/response_cache.cpp
```

### 3. Rate limiting

Реализован in-memory Fixed Window Counter rate limiting для endpoint:

```text
POST /auth/login
```

Параметры:

```text
лимит: 5 запросов
окно: 60 секунд
ключ клиента: IP-адрес или X-RateLimit-Key
```

При превышении лимита API возвращает:

```text
429 Too Many Requests
```

Также добавляются заголовки:

```text
X-RateLimit-Limit
X-RateLimit-Remaining
X-RateLimit-Reset
```

Компонент rate limiting:

```text
src/rate_limit/rate_limiter.hpp
src/rate_limit/rate_limiter.cpp
```

### 4. Redis

В `docker-compose.yaml` добавлен сервис Redis:

```text
conference-redis
```

Было решено реализовать упрощённый вариант лабораторной работы. В текущей реализации кеширование и rate limiting реализованы in-memory внутри API. Redis добавлен как инфраструктурный компонент, который можно использовать для дальнейшего перехода к распределённому кешированию и rate limiting.

## Запуск инфраструктуры

Из папки проекта:

```powershell
docker compose up -d postgres mongo redis
```

Проверка Redis:

```powershell
docker exec -it conference-redis redis-cli ping
```

Ожидаемый ответ:

```text
PONG
```

## Особенность запуска через VS Code Dev Container

API запускается из VS Code Dev Container. Devcontainer должен быть подключён к Docker-сети проекта.

Проверка доступности контейнеров из devcontainer:

```bash
getent hosts conference-postgres
getent hosts conference-mongo
getent hosts conference-redis
```

Если контейнеры не видны, подключить devcontainer к сети проекта:

```powershell
docker network connect <network_name> <devcontainer_name>
```

## Сборка API

Внутри devcontainer:

```bash
make build-debug
```

## Запуск API

Внутри devcontainer:

```bash
./build-debug/conference_api \
  --config ./configs/static_config.yaml \
  --config_vars ./configs/config_vars.local.yaml
```

Проверка:

```bash
curl http://127.0.0.1:8080/ping
```

## Проверка кеширования вручную

### GET /talks

Первый запрос заполняет кеш:

```bash
curl -s http://127.0.0.1:8080/talks
```

После создания нового доклада кеш `talks:all` инвалидируется:

```bash
curl -s -X POST http://127.0.0.1:8080/talks \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer <token>" \
  -d '{
    "title": "New cached talk",
    "description": "This talk invalidates talks cache"
  }'
```

### GET /conferences/{conferenceId}/talks

Первый запрос заполняет кеш конференции:

```bash
curl -s http://127.0.0.1:8080/conferences/1/talks
```

После добавления доклада в конференцию кеш `conference:1:talks` инвалидируется:

```bash
curl -s -X POST http://127.0.0.1:8080/conferences/1/talks/1 \
  -H "Authorization: Bearer <token>"
```

## Проверка rate limiting вручную

```bash
for i in 1 2 3 4 5 6; do
  echo "---- request $i ----"
  curl -i -s -X POST http://127.0.0.1:8080/auth/login \
    -H "Content-Type: application/json" \
    -H "X-RateLimit-Key: manual-check" \
    -d '{"login":"wrong","password":"wrong"}' | grep -E "HTTP/|X-RateLimit|error"
done
```

Ожидаемое поведение:

- первые 5 запросов проходят rate limit и возвращают `401 Unauthorized`, так как логин и пароль неверные;
- шестой запрос возвращает `429 Too Many Requests`;
- в ответах есть заголовки `X-RateLimit-Limit`, `X-RateLimit-Remaining`, `X-RateLimit-Reset`.

## Запуск тестов

Внутри devcontainer:

```bash
make test-debug
```

Ожидаемый результат:

```text
11 passed
```

Покрываются сценарии:

- базовая проверка `/ping`;
- регистрация и логин пользователя;
- создание доклада;
- добавление доклада в конференцию;
- конфликт повторного добавления доклада;
- MongoDB endpoint из лабораторной №4;
- инвалидация кеша после `POST /talks`;
- инвалидация кеша после добавления доклада в конференцию;
- rate limiting для `POST /auth/login`;
- проверка `429 Too Many Requests`;
- проверка заголовков rate limiting.

## Итог

В лабораторной работе №5 реализовано:

- описание стратегии кеширования и rate limiting;
- in-memory Cache-Aside кеширование для двух endpoints;
- инвалидация кеша при изменении данных;
- Fixed Window Counter rate limiting для login endpoint;
- корректный ответ `429 Too Many Requests`;
- заголовки rate limiting;
- Redis как инфраструктурный сервис;
- автоматические тесты.