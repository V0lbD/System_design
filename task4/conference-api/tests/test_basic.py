import uuid


def _unique(prefix: str) -> str:
    return f'{prefix}_{uuid.uuid4().hex[:10]}'


async def _register_user(service_client, login: str, role: str):
    response = await service_client.post(
        '/auth/register',
        json={
            'login': login,
            'password': '123456',
            'first_name': 'Test',
            'last_name': 'User',
            'role': role,
        },
    )

    assert response.status == 201, response.text
    body = response.json()
    assert body['login'] == login
    assert body['role'] == role
    return body


async def _login(service_client, login: str) -> str:
    response = await service_client.post(
        '/auth/login',
        json={
            'login': login,
            'password': '123456',
        },
    )

    assert response.status == 200, response.text
    body = response.json()
    assert body['token']
    return body['token']


async def test_ping(service_client):
    response = await service_client.get('/ping')
    assert response.status == 200


async def test_register_and_login(service_client):
    login = _unique('organizer')

    user = await _register_user(service_client, login, 'organizer')
    assert user['id'] > 0

    token = await _login(service_client, login)
    assert token.startswith('token_')


async def test_create_talk_requires_auth(service_client):
    response = await service_client.post(
        '/talks',
        json={
            'title': 'Unauthorized talk',
            'description': 'This request must be rejected',
        },
    )

    assert response.status == 401


async def test_conference_talk_flow_and_duplicate_conflict(service_client):
    login = _unique('organizer')
    await _register_user(service_client, login, 'organizer')
    token = await _login(service_client, login)

    headers = {'Authorization': f'Bearer {token}'}

    talk_response = await service_client.post(
        '/talks',
        headers=headers,
        json={
            'title': 'MongoDB Migration',
            'description': 'Document model design for conference API',
        },
    )
    assert talk_response.status == 201, talk_response.text
    talk = talk_response.json()
    talk_id = talk['id']

    talks_response = await service_client.get('/talks')
    assert talks_response.status == 200, talks_response.text
    talks = talks_response.json()
    assert any(item['id'] == talk_id for item in talks)

    conference_response = await service_client.post(
        '/conferences',
        headers=headers,
        json={
            'title': 'System Design Lab Conference',
            'description': 'Conference created from test',
            'date': '2026-05-01',
        },
    )
    assert conference_response.status == 201, conference_response.text
    conference = conference_response.json()
    conference_id = conference['id']

    add_response = await service_client.post(
        f'/conferences/{conference_id}/talks/{talk_id}',
        headers=headers,
    )
    assert add_response.status == 201, add_response.text

    duplicate_response = await service_client.post(
        f'/conferences/{conference_id}/talks/{talk_id}',
        headers=headers,
    )
    assert duplicate_response.status == 409, duplicate_response.text

    conference_talks_response = await service_client.get(
        f'/conferences/{conference_id}/talks',
    )
    assert conference_talks_response.status == 200, conference_talks_response.text

    conference_talks = conference_talks_response.json()
    assert any(item['id'] == talk_id for item in conference_talks)