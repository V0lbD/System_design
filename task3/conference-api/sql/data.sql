INSERT INTO users (login, password, first_name, last_name, role) VALUES
('organizer1', '123456', 'Olga', 'Smirnova', 'organizer'),
('organizer2', '123456', 'Ivan', 'Petrov', 'organizer'),
('speaker1', '123456', 'Sergey', 'Ivanov', 'speaker'),
('speaker2', '123456', 'Anna', 'Kuznetsova', 'speaker'),
('speaker3', '123456', 'Dmitry', 'Sokolov', 'speaker'),
('speaker4', '123456', 'Maria', 'Popova', 'speaker'),
('speaker5', '123456', 'Pavel', 'Volkov', 'speaker'),
('attendee1', '123456', 'Elena', 'Morozova', 'attendee'),
('attendee2', '123456', 'Alexey', 'Lebedev', 'attendee'),
('attendee3', '123456', 'Natalia', 'Fedorova', 'attendee');

INSERT INTO talks (title, description, speaker_id) VALUES
('Modern C++', 'Modern C++ features in backend development', 3),
('Userver Basics', 'Introduction to Yandex userver framework', 4),
('PostgreSQL Indexing', 'How indexing improves SQL performance', 5),
('System Design 101', 'Basics of designing scalable systems', 6),
('Microservices', 'Practical microservice architecture patterns', 7),
('REST API Design', 'Best practices for RESTful services', 3),
('Concurrency in C++', 'Threads, coroutines and async programming', 4),
('Database Constraints', 'Using constraints in relational databases', 5),
('Docker for Developers', 'Containerization for local development', 6),
('Logging and Monitoring', 'Observability in backend systems', 7);

INSERT INTO conferences (title, description, conference_date) VALUES
('CppConf Moscow', 'Conference about C++ and backend development', '2026-05-10'),
('Backend Days', 'Conference for backend engineers', '2026-06-15'),
('DevOps Russia', 'Conference about DevOps and infrastructure', '2026-07-01'),
('System Design Conf', 'Conference on architecture and system design', '2026-08-20'),
('Database Summit', 'Conference focused on SQL and databases', '2026-09-05'),
('Tech Future', 'Conference about future software technologies', '2026-10-12'),
('Cloud Expo', 'Conference about cloud platforms and deployment', '2026-11-03'),
('Code Fest', 'Developer conference for modern programming', '2026-11-25'),
('Architecture Live', 'Event about software architecture', '2026-12-10'),
('Winter IT Forum', 'Winter conference for IT professionals', '2027-01-18');

INSERT INTO conference_talks (conference_id, talk_id) VALUES
(1, 1),
(1, 2),
(2, 3),
(2, 4),
(3, 5),
(3, 9),
(4, 4),
(4, 6),
(5, 3),
(5, 8),
(6, 1),
(6, 10),
(7, 5),
(7, 9),
(8, 2),
(8, 7),
(9, 4),
(9, 8),
(10, 6),
(10, 10);