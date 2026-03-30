# Задание №2, вариант 3 (Сайт конференции)
**Выполнил**: *Злобин Владимир Олегович* \
**Группа**: *М8О-102СВ-21* \
**Стек:** *C++20 + Yandex Userver* \
**Хранилище:** *in-memory*

---

# Описание проекта

Сервис реализует backend для системы управления конференциями. Были внесены некоторые правки в решение Задания №1.

Поддерживаются следующие сущности:

- **User** — пользователь системы
- **Talk** — доклад
- **Conference** — конференция

---

# Реализованный функционал

## Аутентификация
- регистрация пользователя
- вход пользователя
- генерация bearer token
- защита endpoint'ов через `Authorization: Bearer <token>`

## Пользователи
- поиск пользователя по логину
- поиск пользователей по маске имени/фамилии

## Доклады
- создание доклада
- получение списка всех докладов

## Конференции
- создание конференции
- добавление доклада в конференцию
- получение списка докладов конференции

---

# Реализованные endpoint'ы

| Метод | Endpoint | Auth | Описание |
|---|---|---|---|
| POST | `/auth/register` | Нет | Регистрация пользователя |
| POST | `/auth/login` | Нет | Вход пользователя |
| GET | `/users/by-login?login=...` | Нет | Поиск по логину |
| GET | `/users/search?mask=...` | Нет | Поиск по маске |
| POST | `/talks` | Да | Создание доклада |
| GET | `/talks` | Нет | Список докладов |
| POST | `/conferences` | Да | Создание конференции |
| POST | `/conferences/{conferenceId}/talks/{talkId}` | Да | Добавление доклада в конференцию |
| GET | `/conferences/{conferenceId}/talks` | Нет | Доклады конференции |

---

# Сборка и запуск

## Локальный запуск в Dev Container

```bash
make build-debug
make start-debug
```

---

# Примеры запросов

## 1. Регистрация

```bash
curl -i -X POST "http://127.0.0.1:8080/auth/register" \
  -H "Content-Type: application/json" \
  -d '{
    "login": "ivanov",
    "password": "123456",
    "first_name": "Ivan",
    "last_name": "Ivanov",
    "role": "speaker"
  }'
```

---

## 2. Логин

```bash
curl -i -X POST "http://127.0.0.1:8080/auth/login" \
  -H "Content-Type: application/json" \
  -d '{
    "login": "ivanov",
    "password": "123456"
  }'
```

Пример ответа:

```json
{
  "token": "token_1"
}
```

---

## 3. Создание доклада

```bash
curl -i -X POST "http://127.0.0.1:8080/talks" \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer token_1" \
  -d '{
    "title": "Modern C++ in backend",
    "description": "Talk about userver and modern C++"
  }'
```

---

## 4. Создание конференции

```bash
curl -i -X POST "http://127.0.0.1:8080/conferences" \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer token_1" \
  -d '{
    "title": "CppConf Moscow",
    "description": "Conference about backend and system design",
    "date": "2026-04-10"
  }'
```

---

## 5. Добавление доклада в конференцию

```bash
curl -i -X POST "http://127.0.0.1:8080/conferences/1/talks/1" \
  -H "Authorization: Bearer token_1"
```

---

## 6. Получение докладов конференции

```bash
curl -i "http://127.0.0.1:8080/conferences/1/talks"
```

---

---

## 7. Ошибка: неверный пароль при логине

```bash
curl -i -X POST "http://127.0.0.1:8080/auth/login" \
  -H "Content-Type: application/json" \
  -d '{
    "login": "ivanov",
    "password": "wrong_password"
  }'
```

Ожидаемый ответ:

```json
{
  "message": "invalid credentials"
}
```

HTTP status:

```text
401 Unauthorized
```

---

## 8. Ошибка: пользователь не найден

```bash
curl -i "http://127.0.0.1:8080/users/by-login?login=unknown"
```

Ожидаемый ответ:

```json
{
  "message": "user not found"
}
```

HTTP status:

```text
404 Not Found
```

---

# Обработка ошибок

Во всех endpoint’ах ошибки возвращаются в едином формате:

```json
{
  "message": "error description"
}
```

Примеры кодов ошибок:
- `400 Bad Request`
- `401 Unauthorized`
- `404 Not Found`

---

# Docker

Для контейнерного запуска используются:

```text
Dockerfile
docker-compose.yaml
```

Запуск:

```bash
docker compose up --build
```

---

# Хранилище
Используется in-memory хранилище на основе `std::unordered_map`.

Данные хранятся только в памяти процесса и очищаются после перезапуска сервиса.

# Аутентификация
Используется простая токен-авторизация:

```text
Authorization: Bearer token_1
```

Токены создаются в памяти и валидируются на защищённых endpoint'ах.

---