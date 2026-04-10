-- 1. Создание нового пользователя
INSERT INTO users (login, password, first_name, last_name, role)
VALUES ('new_user', '123456', 'Ivan', 'Ivanov', 'speaker');


-- 2. Поиск пользователя по логину
SELECT id, login, first_name, last_name, role
FROM users
WHERE login = 'speaker1';


-- 3. Поиск пользователя по маске имени/фамилии
SELECT id, login, first_name, last_name, role
FROM users
WHERE LOWER(first_name) LIKE LOWER('%ser%')
   OR LOWER(last_name) LIKE LOWER('%ser%')
   OR LOWER(first_name || ' ' || last_name) LIKE LOWER('%ser%');


-- 4. Создание доклада
INSERT INTO talks (title, description, speaker_id)
VALUES ('New Talk', 'Talk about PostgreSQL integration', 3);


-- 5. Получение списка всех докладов
SELECT id, title, description, speaker_id
FROM talks
ORDER BY id;


-- 6. Добавление доклада в конференцию
INSERT INTO conference_talks (conference_id, talk_id)
VALUES (1, 3);


-- 7. Получение списка докладов в конференции
SELECT t.id, t.title, t.description, t.speaker_id
FROM conference_talks ct
JOIN talks t ON t.id = ct.talk_id
WHERE ct.conference_id = 1
ORDER BY t.id;