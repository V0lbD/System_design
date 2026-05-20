CREATE TABLE users (
    id BIGSERIAL PRIMARY KEY,
    login VARCHAR(100) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    role VARCHAR(20) NOT NULL CHECK (role IN ('attendee', 'speaker', 'organizer'))
);

CREATE TABLE talks (
    id BIGSERIAL PRIMARY KEY,
    title VARCHAR(200) NOT NULL,
    description TEXT NOT NULL,
    speaker_id BIGINT NOT NULL REFERENCES users(id)
);

CREATE TABLE conferences (
    id BIGSERIAL PRIMARY KEY,
    title VARCHAR(200) NOT NULL,
    description TEXT NOT NULL,
    conference_date DATE NOT NULL
);

CREATE TABLE conference_talks (
    conference_id BIGINT NOT NULL REFERENCES conferences(id) ON DELETE CASCADE,
    talk_id BIGINT NOT NULL REFERENCES talks(id) ON DELETE CASCADE,
    PRIMARY KEY (conference_id, talk_id)
);

CREATE TABLE tokens (
    token VARCHAR(255) PRIMARY KEY,
    user_id BIGINT NOT NULL REFERENCES users(id) ON DELETE CASCADE
);

CREATE INDEX idx_talks_speaker_id ON talks(speaker_id);
CREATE INDEX idx_conference_talks_conference_id ON conference_talks(conference_id);
CREATE INDEX idx_conference_talks_talk_id ON conference_talks(talk_id);
CREATE INDEX idx_conferences_conference_date ON conferences(conference_date);

CREATE INDEX idx_tokens_user_id ON tokens(user_id);