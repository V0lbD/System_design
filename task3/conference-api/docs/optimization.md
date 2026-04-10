# Оптимизация запросов

## Цель

Целью оптимизации было проанализировать основные SQL-запросы системы управления конференциями, определить часто выполняемые операции, проверить влияние индексов на планы выполнения запросов и сравнить поведение PostgreSQL до и после оптимизации.

Для анализа использовался `EXPLAIN ANALYZE`, который показывает:
- план выполнения запроса;
- выбранные PostgreSQL операции (`Seq Scan`, `Index Scan`, `Bitmap Index Scan`, `Hash Join`, `Nested Loop` и др.);
- `Planning Time` — время построения плана выполнения;
- `Execution Time` — время фактического выполнения запроса.

---

## Используемые индексы

### Автоматически созданные индексы

- `users_pkey` — первичный ключ таблицы `users`
- `talks_pkey` — первичный ключ таблицы `talks`
- `conferences_pkey` — первичный ключ таблицы `conferences`
- `conference_talks_pkey` — составной первичный ключ таблицы `conference_talks`
- уникальный индекс/ограничение по `users(login)`

### Добавленные вручную индексы

- `idx_talks_speaker_id`  
  Индекс по `speaker_id` в таблице `talks`, ускоряет выборки по автору доклада.

- `idx_conference_talks_conference_id`  
  Индекс по `conference_id` в таблице `conference_talks`, предназначен для ускорения выборки докладов конкретной конференции.

- `idx_conference_talks_talk_id`  
  Индекс по `talk_id`, ускоряет операции соединения и проверки связей.

---

## Анализ запросов

### 1. Поиск пользователя по логину

Запрос:

```
EXPLAIN ANALYZE
SELECT id, login, first_name, last_name, role
FROM users
WHERE login = 'speaker1';
```

### До добавления индекса

План выполнения:
```
Seq Scan on users  (cost=0.00..10.75 rows=1 width=720) (actual time=0.016..0.017 rows=1 loops=1)
  Filter: ((login)::text = 'speaker1'::text)
  Rows Removed by Filter: 10
Planning Time: 0.121 ms
Execution Time: 0.026 ms
```
Анализ:

До добавления уникального ограничения по `login` PostgreSQL выполнял последовательное сканирование таблицы `users` (`Seq Scan`).  
Это означает, что для поиска пользователя сервер просматривал все строки таблицы и отбрасывал ненужные по условию `WHERE`.

### После добавления индекса

План выполнения:
```
Seq Scan on users  (cost=0.00..1.14 rows=1 width=720) (actual time=0.007..0.008 rows=1 loops=1)
  Filter: ((login)::text = 'speaker1'::text)
  Rows Removed by Filter: 10
Planning Time: 0.198 ms
Execution Time: 0.020 ms
```
Анализ:

После добавления индекса PostgreSQL всё равно выбрал `Seq Scan`, а не `Index Scan`.  
Это связано с очень маленьким размером таблицы: в таблице всего несколько записей, и оптимизатор считает, что полное сканирование будет дешевле, чем использование индекса.

При этом важно отметить, что `Execution Time` немного уменьшился, а наличие индекса всё равно корректно с точки зрения проектирования схемы.

Вывод:

Для маленьких таблиц PostgreSQL может не использовать индекс даже после его создания. Это нормальное поведение оптимизатора.

---

### 2. Получение списка докладов конференции

Запрос:
```
EXPLAIN ANALYZE
SELECT t.id, t.title, t.description, t.speaker_id
FROM conference_talks ct
JOIN talks t ON t.id = ct.talk_id
WHERE ct.conference_id = 1
ORDER BY t.id;
```
### До добавления отдельного индекса по conference_id

План выполнения:
```
Sort  (cost=27.05..27.07 rows=9 width=466) (actual time=0.058..0.059 rows=2 loops=1)
  Sort Key: t.id
  Sort Method: quicksort  Memory: 25kB
  ->  Hash Join  (cost=14.88..26.91 rows=9 width=466) (actual time=0.041..0.043 rows=2 loops=1)
        Hash Cond: (t.id = ct.talk_id)
        ->  Seq Scan on talks t  (cost=0.00..11.60 rows=160 width=466) (actual time=0.012..0.013 rows=10 loops=1)
        ->  Hash  (cost=14.76..14.76 rows=9 width=8) (actual time=0.023..0.023 rows=2 loops=1)
              Buckets: 1024  Batches: 1  Memory Usage: 9kB
              ->  Bitmap Heap Scan on conference_talks ct  (cost=4.22..14.76 rows=9 width=8) (actual time=0.021..0.021 rows=2 loops=1)
                    Recheck Cond: (conference_id = 1)
                    Heap Blocks: exact=1
                    ->  Bitmap Index Scan on conference_talks_pkey  (cost=0.00..4.22 rows=9 width=0) (actual time=0.012..0.012 rows=2 loops=1)
                          Index Cond: (conference_id = 1)
Planning Time: 0.360 ms
Execution Time: 0.077 ms
```
Анализ:

Даже до добавления отдельного индекса PostgreSQL уже использовал индекс, потому что составной первичный ключ `conference_talks_pkey (conference_id, talk_id)` позволяет выполнять поиск по первому столбцу `conference_id`.

В данном случае отдельный индекс по `conference_id` частично дублирует возможности составного PK.

### После добавления отдельного индекса по conference_id

План выполнения:
```
Sort  (cost=9.52..9.53 rows=1 width=466) (actual time=0.039..0.039 rows=2 loops=1)
  Sort Key: t.id
  Sort Method: quicksort  Memory: 25kB
  ->  Nested Loop  (cost=0.14..9.51 rows=1 width=466) (actual time=0.031..0.034 rows=2 loops=1)
        ->  Seq Scan on conference_talks ct  (cost=0.00..1.25 rows=1 width=8) (actual time=0.007..0.008 rows=2 loops=1)
              Filter: (conference_id = 1)
              Rows Removed by Filter: 18
        ->  Index Scan using talks_pkey on talks t  (cost=0.14..8.16 rows=1 width=466) (actual time=0.011..0.011 rows=1 loops=2)
              Index Cond: (id = ct.talk_id)
Planning Time: 0.295 ms
Execution Time: 0.057 ms
```
Анализ:

После добавления отдельного индекса по `conference_id` PostgreSQL выбрал другой план выполнения:
- вместо `Hash Join` был выбран `Nested Loop`;
- для таблицы `talks` используется `Index Scan` по `talks_pkey`;
- для таблицы `conference_talks` снова выбран `Seq Scan`, потому что таблица очень маленькая.

Несмотря на то, что индекс `idx_conference_talks_conference_id` был создан, PostgreSQL не обязан использовать его всегда. Оптимизатор выбирает план, исходя из стоимости. На маленьких таблицах последовательное сканирование часто оказывается дешевле.

При этом итоговое `Execution Time` уменьшилось с `0.077 ms` до `0.057 ms`, то есть запрос стал выполняться немного быстрее.

Вывод:

Оптимизация запроса действительно повлияла на план выполнения, но эффект оказался не таким очевидным, как в учебных примерах с большими таблицами.

---

### 3. Поиск пользователя по маске имени и фамилии

Запрос:
```
EXPLAIN ANALYZE
SELECT id, login, first_name, last_name, role
FROM users
WHERE LOWER(first_name) LIKE LOWER('%an%')
   OR LOWER(last_name) LIKE LOWER('%an%')
   OR LOWER(first_name || ' ' || last_name) LIKE LOWER('%an%');
```
План выполнения:
```
Seq Scan on users  (cost=0.00..1.33 rows=5 width=720) (actual time=0.009..0.020 rows=3 loops=1)
  Filter: ((lower((first_name)::text) ~~ '%an%'::text) OR (lower((last_name)::text) ~~ '%an%'::text) OR (lower((((first_name)::text || ' '::text) || (last_name)::text)) ~~ '%an%'::text))
  Rows Removed by Filter: 8
Planning Time: 0.101 ms
Execution Time: 0.029 ms
```
Анализ:

Для данного запроса PostgreSQL использует `Seq Scan`, то есть последовательное сканирование всей таблицы `users`.

Это ожидаемо, потому что поиск по шаблону вида `%...%` с использованием `LOWER(...)` плохо оптимизируется обычными B-tree индексами. Даже если создать обычный индекс на `first_name` или `last_name`, он не даст заметного ускорения для такого типа поиска.

Вывод:

Этот запрос является наименее эффективным из рассмотренных.  

---

## Общий вывод

В ходе работы были проанализированы основные запросы системы и выполнено сравнение планов до и после оптимизации.

Полученные результаты показывают, что:

- PostgreSQL выбирает план выполнения на основе стоимости, а не только на основе наличия индекса;
- для маленьких таблиц оптимизатор часто предпочитает `Seq Scan`, даже если индекс существует;
- составные первичные ключи также могут участвовать в оптимизации запросов;
- эффект индексов наиболее заметен на больших объёмах данных.