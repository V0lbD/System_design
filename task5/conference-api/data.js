const database = db.getSiblingDB("conference_mongo");

database.dropDatabase();

const userIds = [
  ObjectId("660000000000000000000001"),
  ObjectId("660000000000000000000002"),
  ObjectId("660000000000000000000003"),
  ObjectId("660000000000000000000004"),
  ObjectId("660000000000000000000005"),
  ObjectId("660000000000000000000006"),
  ObjectId("660000000000000000000007"),
  ObjectId("660000000000000000000008"),
  ObjectId("660000000000000000000009"),
  ObjectId("660000000000000000000010")
];

const talkIds = [
  ObjectId("770000000000000000000001"),
  ObjectId("770000000000000000000002"),
  ObjectId("770000000000000000000003"),
  ObjectId("770000000000000000000004"),
  ObjectId("770000000000000000000005"),
  ObjectId("770000000000000000000006"),
  ObjectId("770000000000000000000007"),
  ObjectId("770000000000000000000008"),
  ObjectId("770000000000000000000009"),
  ObjectId("770000000000000000000010")
];

const conferenceIds = [
  ObjectId("880000000000000000000001"),
  ObjectId("880000000000000000000002"),
  ObjectId("880000000000000000000003"),
  ObjectId("880000000000000000000004"),
  ObjectId("880000000000000000000005"),
  ObjectId("880000000000000000000006"),
  ObjectId("880000000000000000000007"),
  ObjectId("880000000000000000000008"),
  ObjectId("880000000000000000000009"),
  ObjectId("880000000000000000000010")
];

database.users.insertMany([
  {
    _id: userIds[0],
    login: "ivanov",
    password_hash: "hash_ivanov",
    first_name: "Иван",
    last_name: "Иванов",
    role: "speaker",
    email: "ivanov@example.com",
    bio: "Backend-разработчик, интересуется распределёнными системами",
    contacts: { telegram: "@ivanov", github: "ivanov-dev" },
    interests: ["backend", "databases", "system design"],
    rating: 4.8,
    created_at: ISODate("2026-05-01T10:00:00Z"),
    updated_at: ISODate("2026-05-01T10:00:00Z")
  },
  {
    _id: userIds[1],
    login: "petrova",
    password_hash: "hash_petrova",
    first_name: "Анна",
    last_name: "Петрова",
    role: "organizer",
    email: "petrova@example.com",
    bio: "Организатор технических конференций",
    contacts: { telegram: "@petrova", github: "petrova-events" },
    interests: ["events", "architecture", "management"],
    rating: 4.9,
    created_at: ISODate("2026-05-02T10:00:00Z"),
    updated_at: ISODate("2026-05-02T10:00:00Z")
  },
  {
    _id: userIds[2],
    login: "sidorov",
    password_hash: "hash_sidorov",
    first_name: "Павел",
    last_name: "Сидоров",
    role: "speaker",
    email: "sidorov@example.com",
    bio: "Инженер по базам данных",
    contacts: { telegram: "@sidorov", github: "sidorov-db" },
    interests: ["postgresql", "mongodb", "storage"],
    rating: 4.7,
    created_at: ISODate("2026-05-03T10:00:00Z"),
    updated_at: ISODate("2026-05-03T10:00:00Z")
  },
  {
    _id: userIds[3],
    login: "smirnova",
    password_hash: "hash_smirnova",
    first_name: "Мария",
    last_name: "Смирнова",
    role: "participant",
    email: "smirnova@example.com",
    bio: "Frontend-разработчик",
    contacts: { telegram: "@smirnova", github: "smirnova-ui" },
    interests: ["frontend", "ux", "web"],
    rating: 4.4,
    created_at: ISODate("2026-05-04T10:00:00Z"),
    updated_at: ISODate("2026-05-04T10:00:00Z")
  },
  {
    _id: userIds[4],
    login: "kozlov",
    password_hash: "hash_kozlov",
    first_name: "Дмитрий",
    last_name: "Козлов",
    role: "speaker",
    email: "kozlov@example.com",
    bio: "DevOps-инженер",
    contacts: { telegram: "@kozlov", github: "kozlov-devops" },
    interests: ["docker", "kubernetes", "ci/cd"],
    rating: 4.6,
    created_at: ISODate("2026-05-05T10:00:00Z"),
    updated_at: ISODate("2026-05-05T10:00:00Z")
  },
  {
    _id: userIds[5],
    login: "volkova",
    password_hash: "hash_volkova",
    first_name: "Елена",
    last_name: "Волкова",
    role: "organizer",
    email: "volkova@example.com",
    bio: "Координатор программы конференций",
    contacts: { telegram: "@volkova", github: "volkova-conf" },
    interests: ["program committee", "talk review", "community"],
    rating: 4.9,
    created_at: ISODate("2026-05-06T10:00:00Z"),
    updated_at: ISODate("2026-05-06T10:00:00Z")
  },
  {
    _id: userIds[6],
    login: "orlov",
    password_hash: "hash_orlov",
    first_name: "Алексей",
    last_name: "Орлов",
    role: "speaker",
    email: "orlov@example.com",
    bio: "Архитектор высоконагруженных систем",
    contacts: { telegram: "@orlov", github: "orlov-arch" },
    interests: ["highload", "microservices", "observability"],
    rating: 4.95,
    created_at: ISODate("2026-05-07T10:00:00Z"),
    updated_at: ISODate("2026-05-07T10:00:00Z")
  },
  {
    _id: userIds[7],
    login: "morozova",
    password_hash: "hash_morozova",
    first_name: "Ольга",
    last_name: "Морозова",
    role: "participant",
    email: "morozova@example.com",
    bio: "Студент магистратуры",
    contacts: { telegram: "@morozova", github: "morozova-study" },
    interests: ["system design", "api", "testing"],
    rating: 4.2,
    created_at: ISODate("2026-05-08T10:00:00Z"),
    updated_at: ISODate("2026-05-08T10:00:00Z")
  },
  {
    _id: userIds[8],
    login: "fedorov",
    password_hash: "hash_fedorov",
    first_name: "Никита",
    last_name: "Фёдоров",
    role: "speaker",
    email: "fedorov@example.com",
    bio: "Разработчик C++ backend-сервисов",
    contacts: { telegram: "@fedorov", github: "fedorov-cpp" },
    interests: ["cpp", "userver", "performance"],
    rating: 4.65,
    created_at: ISODate("2026-05-09T10:00:00Z"),
    updated_at: ISODate("2026-05-09T10:00:00Z")
  },
  {
    _id: userIds[9],
    login: "nikolaeva",
    password_hash: "hash_nikolaeva",
    first_name: "Софья",
    last_name: "Николаева",
    role: "participant",
    email: "nikolaeva@example.com",
    bio: "QA-инженер",
    contacts: { telegram: "@nikolaeva", github: "nikolaeva-qa" },
    interests: ["testing", "automation", "quality"],
    rating: 4.3,
    created_at: ISODate("2026-05-10T10:00:00Z"),
    updated_at: ISODate("2026-05-10T10:00:00Z")
  }
]);

database.talks.insertMany([
  {
    _id: talkIds[0],
    title: "Проектирование REST API для конференции",
    description: "Доклад о проектировании API, контрактах и обработке ошибок",
    speaker_id: userIds[0],
    status: "submitted",
    level: "intermediate",
    duration_minutes: 45,
    tags: ["api", "backend", "architecture"],
    materials: [
      { type: "slides", url: "https://example.com/slides/rest-api.pdf" },
      { type: "repository", url: "https://github.com/example/rest-api" }
    ],
    created_at: ISODate("2026-05-11T10:00:00Z"),
    updated_at: ISODate("2026-05-11T10:00:00Z")
  },
  {
    _id: talkIds[1],
    title: "MongoDB в системном дизайне",
    description: "Документная модель, embedded documents и references",
    speaker_id: userIds[2],
    status: "accepted",
    level: "beginner",
    duration_minutes: 40,
    tags: ["mongodb", "database", "nosql"],
    materials: [
      { type: "slides", url: "https://example.com/slides/mongodb-design.pdf" }
    ],
    created_at: ISODate("2026-05-12T10:00:00Z"),
    updated_at: ISODate("2026-05-12T10:00:00Z")
  },
  {
    _id: talkIds[2],
    title: "PostgreSQL против MongoDB",
    description: "Сравнение реляционного и документного подхода",
    speaker_id: userIds[2],
    status: "accepted",
    level: "intermediate",
    duration_minutes: 50,
    tags: ["postgresql", "mongodb", "database"],
    materials: [
      { type: "article", url: "https://example.com/articles/postgres-vs-mongo" }
    ],
    created_at: ISODate("2026-05-13T10:00:00Z"),
    updated_at: ISODate("2026-05-13T10:00:00Z")
  },
  {
    _id: talkIds[3],
    title: "Docker для локальной разработки",
    description: "Devcontainer, docker-compose и сетевое взаимодействие контейнеров",
    speaker_id: userIds[4],
    status: "accepted",
    level: "beginner",
    duration_minutes: 35,
    tags: ["docker", "devcontainer", "tools"],
    materials: [
      { type: "repository", url: "https://github.com/example/docker-lab" }
    ],
    created_at: ISODate("2026-05-14T10:00:00Z"),
    updated_at: ISODate("2026-05-14T10:00:00Z")
  },
  {
    _id: talkIds[4],
    title: "Наблюдаемость backend-сервисов",
    description: "Метрики, логи, трассировка и health checks",
    speaker_id: userIds[6],
    status: "submitted",
    level: "advanced",
    duration_minutes: 55,
    tags: ["observability", "metrics", "logging"],
    materials: [
      { type: "slides", url: "https://example.com/slides/observability.pdf" }
    ],
    created_at: ISODate("2026-05-15T10:00:00Z"),
    updated_at: ISODate("2026-05-15T10:00:00Z")
  },
  {
    _id: talkIds[5],
    title: "C++ backend на userver",
    description: "Компоненты, handlers, configs и тестирование userver-сервисов",
    speaker_id: userIds[8],
    status: "accepted",
    level: "advanced",
    duration_minutes: 60,
    tags: ["cpp", "userver", "backend"],
    materials: [
      { type: "repository", url: "https://github.com/example/userver-backend" }
    ],
    created_at: ISODate("2026-05-16T10:00:00Z"),
    updated_at: ISODate("2026-05-16T10:00:00Z")
  },
  {
    _id: talkIds[6],
    title: "Автоматизация тестирования API",
    description: "Подходы к тестированию REST API и интеграционных сценариев",
    speaker_id: userIds[9],
    status: "submitted",
    level: "intermediate",
    duration_minutes: 45,
    tags: ["testing", "api", "automation"],
    materials: [
      { type: "article", url: "https://example.com/articles/api-testing" }
    ],
    created_at: ISODate("2026-05-17T10:00:00Z"),
    updated_at: ISODate("2026-05-17T10:00:00Z")
  },
  {
    _id: talkIds[7],
    title: "Микросервисы без лишней сложности",
    description: "Когда микросервисы полезны, а когда монолит проще",
    speaker_id: userIds[6],
    status: "rejected",
    level: "intermediate",
    duration_minutes: 50,
    tags: ["microservices", "architecture", "backend"],
    materials: [
      { type: "slides", url: "https://example.com/slides/microservices.pdf" }
    ],
    created_at: ISODate("2026-05-18T10:00:00Z"),
    updated_at: ISODate("2026-05-18T10:00:00Z")
  },
  {
    _id: talkIds[8],
    title: "CI/CD для учебных проектов",
    description: "Сборка, тесты и проверка качества кода",
    speaker_id: userIds[4],
    status: "accepted",
    level: "beginner",
    duration_minutes: 30,
    tags: ["ci/cd", "devops", "testing"],
    materials: [
      { type: "repository", url: "https://github.com/example/ci-cd" }
    ],
    created_at: ISODate("2026-05-19T10:00:00Z"),
    updated_at: ISODate("2026-05-19T10:00:00Z")
  },
  {
    _id: talkIds[9],
    title: "UX для технических конференций",
    description: "Как сделать программу конференции удобной для участников",
    speaker_id: userIds[3],
    status: "submitted",
    level: "beginner",
    duration_minutes: 35,
    tags: ["ux", "frontend", "conference"],
    materials: [
      { type: "slides", url: "https://example.com/slides/conference-ux.pdf" }
    ],
    created_at: ISODate("2026-05-20T10:00:00Z"),
    updated_at: ISODate("2026-05-20T10:00:00Z")
  }
]);

database.conferences.insertMany([
  {
    _id: conferenceIds[0],
    title: "System Design Conference 2026",
    description: "Конференция по архитектуре программных систем",
    date: ISODate("2026-06-01T00:00:00Z"),
    location: { city: "Москва", address: "ул. Примерная, 10", online: false },
    organizer_id: userIds[1],
    talks: [
      { talk_id: talkIds[0], order: 1, room: "Main Hall", starts_at: ISODate("2026-06-01T10:00:00Z"), status: "accepted" },
      { talk_id: talkIds[1], order: 2, room: "Main Hall", starts_at: ISODate("2026-06-01T11:00:00Z"), status: "accepted" }
    ],
    tags: ["architecture", "backend", "databases"],
    capacity: 300,
    paid: true,
    price: 5000,
    created_at: ISODate("2026-05-21T10:00:00Z"),
    updated_at: ISODate("2026-05-21T10:00:00Z")
  },
  {
    _id: conferenceIds[1],
    title: "MongoDB Meetup",
    description: "Практика проектирования документных моделей",
    date: ISODate("2026-06-05T00:00:00Z"),
    location: { city: "Санкт-Петербург", address: "Невский проспект, 20", online: false },
    organizer_id: userIds[5],
    talks: [
      { talk_id: talkIds[1], order: 1, room: "Room A", starts_at: ISODate("2026-06-05T12:00:00Z"), status: "accepted" },
      { talk_id: talkIds[2], order: 2, room: "Room A", starts_at: ISODate("2026-06-05T13:00:00Z"), status: "accepted" }
    ],
    tags: ["mongodb", "nosql", "database"],
    capacity: 150,
    paid: false,
    price: 0,
    created_at: ISODate("2026-05-22T10:00:00Z"),
    updated_at: ISODate("2026-05-22T10:00:00Z")
  },
  {
    _id: conferenceIds[2],
    title: "DevOps Day",
    description: "День практик DevOps и контейнеризации",
    date: ISODate("2026-06-10T00:00:00Z"),
    location: { city: "Казань", address: "ИТ-парк", online: false },
    organizer_id: userIds[1],
    talks: [
      { talk_id: talkIds[3], order: 1, room: "DevOps Hall", starts_at: ISODate("2026-06-10T09:30:00Z"), status: "accepted" },
      { talk_id: talkIds[8], order: 2, room: "DevOps Hall", starts_at: ISODate("2026-06-10T10:30:00Z"), status: "accepted" }
    ],
    tags: ["docker", "devops", "ci/cd"],
    capacity: 200,
    paid: true,
    price: 3000,
    created_at: ISODate("2026-05-23T10:00:00Z"),
    updated_at: ISODate("2026-05-23T10:00:00Z")
  },
  {
    _id: conferenceIds[3],
    title: "Backend Architecture Online",
    description: "Онлайн-конференция про backend и API",
    date: ISODate("2026-06-15T00:00:00Z"),
    location: { city: "Online", address: "https://conference.example.com/backend", online: true },
    organizer_id: userIds[5],
    talks: [
      { talk_id: talkIds[0], order: 1, room: "Stream 1", starts_at: ISODate("2026-06-15T10:00:00Z"), status: "accepted" },
      { talk_id: talkIds[5], order: 2, room: "Stream 1", starts_at: ISODate("2026-06-15T11:00:00Z"), status: "accepted" }
    ],
    tags: ["backend", "api", "cpp"],
    capacity: 1000,
    paid: false,
    price: 0,
    created_at: ISODate("2026-05-24T10:00:00Z"),
    updated_at: ISODate("2026-05-24T10:00:00Z")
  },
  {
    _id: conferenceIds[4],
    title: "Testing Conf",
    description: "Конференция по тестированию и качеству",
    date: ISODate("2026-06-20T00:00:00Z"),
    location: { city: "Новосибирск", address: "Академгородок, 1", online: false },
    organizer_id: userIds[1],
    talks: [
      { talk_id: talkIds[6], order: 1, room: "QA Hall", starts_at: ISODate("2026-06-20T10:00:00Z"), status: "accepted" },
      { talk_id: talkIds[8], order: 2, room: "QA Hall", starts_at: ISODate("2026-06-20T11:00:00Z"), status: "accepted" }
    ],
    tags: ["testing", "qa", "automation"],
    capacity: 180,
    paid: true,
    price: 2500,
    created_at: ISODate("2026-05-25T10:00:00Z"),
    updated_at: ISODate("2026-05-25T10:00:00Z")
  },
  {
    _id: conferenceIds[5],
    title: "Highload Evening",
    description: "Вечерние доклады про высоконагруженные системы",
    date: ISODate("2026-06-25T00:00:00Z"),
    location: { city: "Москва", address: "Технопарк, 5", online: false },
    organizer_id: userIds[5],
    talks: [
      { talk_id: talkIds[4], order: 1, room: "Hall B", starts_at: ISODate("2026-06-25T18:00:00Z"), status: "accepted" },
      { talk_id: talkIds[7], order: 2, room: "Hall B", starts_at: ISODate("2026-06-25T19:00:00Z"), status: "rejected" }
    ],
    tags: ["highload", "observability", "microservices"],
    capacity: 120,
    paid: true,
    price: 2000,
    created_at: ISODate("2026-05-26T10:00:00Z"),
    updated_at: ISODate("2026-05-26T10:00:00Z")
  },
  {
    _id: conferenceIds[6],
    title: "Frontend and UX Conference",
    description: "Конференция про frontend и пользовательский опыт",
    date: ISODate("2026-07-01T00:00:00Z"),
    location: { city: "Екатеринбург", address: "ул. Ленина, 50", online: false },
    organizer_id: userIds[1],
    talks: [
      { talk_id: talkIds[9], order: 1, room: "UX Room", starts_at: ISODate("2026-07-01T10:00:00Z"), status: "accepted" }
    ],
    tags: ["frontend", "ux", "web"],
    capacity: 220,
    paid: false,
    price: 0,
    created_at: ISODate("2026-05-27T10:00:00Z"),
    updated_at: ISODate("2026-05-27T10:00:00Z")
  },
  {
    _id: conferenceIds[7],
    title: "C++ Backend Summit",
    description: "Саммит по производительным backend-сервисам на C++",
    date: ISODate("2026-07-05T00:00:00Z"),
    location: { city: "Нижний Новгород", address: "Конгресс-центр", online: false },
    organizer_id: userIds[5],
    talks: [
      { talk_id: talkIds[5], order: 1, room: "C++ Hall", starts_at: ISODate("2026-07-05T10:00:00Z"), status: "accepted" }
    ],
    tags: ["cpp", "backend", "performance"],
    capacity: 250,
    paid: true,
    price: 4500,
    created_at: ISODate("2026-05-28T10:00:00Z"),
    updated_at: ISODate("2026-05-28T10:00:00Z")
  },
  {
    _id: conferenceIds[8],
    title: "Student System Design Day",
    description: "Учебная конференция по системному дизайну",
    date: ISODate("2026-07-10T00:00:00Z"),
    location: { city: "Online", address: "https://conference.example.com/students", online: true },
    organizer_id: userIds[1],
    talks: [
      { talk_id: talkIds[0], order: 1, room: "Stream A", starts_at: ISODate("2026-07-10T10:00:00Z"), status: "accepted" },
      { talk_id: talkIds[6], order: 2, room: "Stream A", starts_at: ISODate("2026-07-10T11:00:00Z"), status: "accepted" }
    ],
    tags: ["education", "system design", "api"],
    capacity: 500,
    paid: false,
    price: 0,
    created_at: ISODate("2026-05-29T10:00:00Z"),
    updated_at: ISODate("2026-05-29T10:00:00Z")
  },
  {
    _id: conferenceIds[9],
    title: "Database Practical Workshop",
    description: "Практический воркшоп по работе с базами данных",
    date: ISODate("2026-07-15T00:00:00Z"),
    location: { city: "Москва", address: "Лабораторный корпус, 3", online: false },
    organizer_id: userIds[5],
    talks: [
      { talk_id: talkIds[1], order: 1, room: "DB Room", starts_at: ISODate("2026-07-15T10:00:00Z"), status: "accepted" },
      { talk_id: talkIds[2], order: 2, room: "DB Room", starts_at: ISODate("2026-07-15T11:00:00Z"), status: "accepted" }
    ],
    tags: ["database", "mongodb", "postgresql"],
    capacity: 80,
    paid: true,
    price: 1500,
    created_at: ISODate("2026-05-30T10:00:00Z"),
    updated_at: ISODate("2026-05-30T10:00:00Z")
  }
]);

database.tokens.insertMany([
  {
    _id: ObjectId("990000000000000000000001"),
    token: "token_ivanov_001",
    user_id: userIds[0],
    created_at: ISODate("2026-05-20T12:00:00Z"),
    expires_at: ISODate("2026-05-21T12:00:00Z"),
    user_agent: "curl/8.0",
    active: true
  },
  {
    _id: ObjectId("990000000000000000000002"),
    token: "token_petrova_001",
    user_id: userIds[1],
    created_at: ISODate("2026-05-20T12:05:00Z"),
    expires_at: ISODate("2026-05-21T12:05:00Z"),
    user_agent: "Mozilla/5.0",
    active: true
  },
  {
    _id: ObjectId("990000000000000000000003"),
    token: "token_sidorov_001",
    user_id: userIds[2],
    created_at: ISODate("2026-05-20T12:10:00Z"),
    expires_at: ISODate("2026-05-21T12:10:00Z"),
    user_agent: "PostmanRuntime/7.0",
    active: true
  },
  {
    _id: ObjectId("990000000000000000000004"),
    token: "token_smirnova_001",
    user_id: userIds[3],
    created_at: ISODate("2026-05-20T12:15:00Z"),
    expires_at: ISODate("2026-05-21T12:15:00Z"),
    user_agent: "Mozilla/5.0",
    active: false
  },
  {
    _id: ObjectId("990000000000000000000005"),
    token: "token_kozlov_001",
    user_id: userIds[4],
    created_at: ISODate("2026-05-20T12:20:00Z"),
    expires_at: ISODate("2026-05-21T12:20:00Z"),
    user_agent: "curl/8.0",
    active: true
  },
  {
    _id: ObjectId("990000000000000000000006"),
    token: "token_volkova_001",
    user_id: userIds[5],
    created_at: ISODate("2026-05-20T12:25:00Z"),
    expires_at: ISODate("2026-05-21T12:25:00Z"),
    user_agent: "Mozilla/5.0",
    active: true
  },
  {
    _id: ObjectId("990000000000000000000007"),
    token: "token_orlov_001",
    user_id: userIds[6],
    created_at: ISODate("2026-05-20T12:30:00Z"),
    expires_at: ISODate("2026-05-21T12:30:00Z"),
    user_agent: "PostmanRuntime/7.0",
    active: true
  },
  {
    _id: ObjectId("990000000000000000000008"),
    token: "token_morozova_001",
    user_id: userIds[7],
    created_at: ISODate("2026-05-20T12:35:00Z"),
    expires_at: ISODate("2026-05-21T12:35:00Z"),
    user_agent: "curl/8.0",
    active: false
  },
  {
    _id: ObjectId("990000000000000000000009"),
    token: "token_fedorov_001",
    user_id: userIds[8],
    created_at: ISODate("2026-05-20T12:40:00Z"),
    expires_at: ISODate("2026-05-21T12:40:00Z"),
    user_agent: "Mozilla/5.0",
    active: true
  },
  {
    _id: ObjectId("990000000000000000000010"),
    token: "token_nikolaeva_001",
    user_id: userIds[9],
    created_at: ISODate("2026-05-20T12:45:00Z"),
    expires_at: ISODate("2026-05-21T12:45:00Z"),
    user_agent: "PostmanRuntime/7.0",
    active: true
  }
]);

database.users.createIndex({ login: 1 }, { unique: true });
database.users.createIndex({ role: 1 });
database.users.createIndex({ last_name: 1, first_name: 1 });

database.talks.createIndex({ speaker_id: 1 });
database.talks.createIndex({ title: "text", description: "text" });
database.talks.createIndex({ tags: 1 });

database.conferences.createIndex({ date: 1 });
database.conferences.createIndex({ organizer_id: 1 });
database.conferences.createIndex({ "talks.talk_id": 1 });

database.tokens.createIndex({ token: 1 }, { unique: true });
database.tokens.createIndex({ user_id: 1 });
database.tokens.createIndex({ expires_at: 1 });

print("Inserted users: " + database.users.countDocuments());
print("Inserted talks: " + database.talks.countDocuments());
print("Inserted conferences: " + database.conferences.countDocuments());
print("Inserted tokens: " + database.tokens.countDocuments());
