workspace "Conference Website" "LW 1" {

    model {
        // роли
        attendee = person "Слушатель" 
        speaker = person "Докладчик" 
        organizer = person "Организатор"

        // software system
        conferenceSite = softwareSystem "Сайт конференции" {
            description "Позволяет управлять пользователями, отчетами и конференциями. Предоставляет API для интеграции."

            // контейнеры
            webApp = container "Web Application" "Веб-приложение" "React"
            apiGateway = container "API Gateway" "API-сервер" "FastApi"

            userService = container "User Service" "Управляет регистрацией пользователей, входом в систему и поиском" "Spring Boot / Kotlin"
            reportService = container "Report Service" "Управляет докладами: создание, перечисление" "Spring Boot / Kotlin"
            conferenceService = container "Conference Service" "Управляет конференциями и связями между докладами и конференциями" "Spring Boot / Kotlin"

            userDb = container "User Database" "Хранит данные о пользователях" "PostgreSQL"
            reportDb = container "Report Database" "Хранит данные о докладах" "PostgreSQL"
            conferenceDb = container "Conference Database" "Хранит данные конференции и связи" "PostgreSQL"

            // связи между контейнерами
            webApp -> apiGateway "Отправляет запросы из браузера" "HTTPS/REST"

            apiGateway -> userService "Перенаправляет запросы, связанные с пользователями" "REST"
            apiGateway -> reportService "Перенаправляет запросы, связанные с докладами" "REST"
            apiGateway -> conferenceService "Перенаправляет запросы, связанные с конференцией" "REST"

            userService -> userDb "Чтение/запись данных о пользователях" "JDBC"
            reportService -> reportDb "Чтение/запись данных о докладах" "JDBC"
            conferenceService -> conferenceDb "Чтение/запись данных о конференциях" "JDBC"

            conferenceService -> reportService "Проверяет, существует ли доклад" "REST"
            conferenceService -> userService "Проверяет права докладчика" "REST"

            attendee -> webApp "Просматривает расписание и доклады"

            speaker -> webApp "Регистрируется, создаёт доклады, добавляет их на конференции"

            organizer -> webApp "Управляет конференциями и модерирует доклады"
        }

        // внешние системы
        emailService = softwareSystem "Почтовая рассылка" {
            tags "External"
        }
        paymentService = softwareSystem "Платёжная система" {
            tags "External"
        }

        conferenceSite -> emailService "Уведомляет о конференции через email-рассылку" 
        conferenceSite -> paymentService "Обрабатывает платёжные запросы" 

        conferenceService -> emailService "Отправляет уведомления о конференциях" "SMTP/API"
        conferenceService -> paymentService "Обрабатывает платежи за регистрацию" "HTTPS/REST"
    }

    views {
        systemContext conferenceSite "SystemContext" "System Context diagram веб-сайта конференции { 
            include *
            autolayout tb
        }

        container conferenceSite "Containers" "Контейнеры веб-сайта конференции" {
            include * emailService paymentService
            autolayout tb
        }

        dynamic conferenceSite "AddReportToConference" {
            description "Последовательность действий при добавлении доклада в конференцию докладчиком"
            autolayout tb

            speaker -> webApp "Нажимает 'Добавить доклад в конференцию'"
            webApp -> apiGateway "POST /api/conferences/{id}/reports"
            apiGateway -> conferenceService "Перенаправляет запрос в ConferenceService"

            conferenceService -> reportService "GET /reports/{reportId} (проверка существования доклада)"
            reportService -> reportDb "SELECT * FROM reports WHERE id = ?"
            reportDb -> reportService "Возвращает данные доклада"
            reportService -> conferenceService "200 OK (доклад существует)"

            conferenceService -> userService "GET /users/{speakerId}/permissions (проверка прав)"
            userService -> userDb "SELECT permissions FROM users WHERE id = ?"
            userDb -> userService "Возвращает права"
            userService -> conferenceService "Permissions OK"

            conferenceService -> conferenceDb "INSERT INTO conference_reports (conference_id, report_id)"
            conferenceDb -> conferenceService "Вставка выполнена успешно"

            conferenceService -> apiGateway "201 Created"
            apiGateway -> webApp "Ответ"
            webApp -> speaker "Обновление UI (доклад добавлен)"
        }

        styles {
            element "Person" {
                shape person
                background #08428a
                color #ffffff
            }
            element "Software System" {
                background #1178be
                color #ffffff
            }
            element "External" {
                background #999999
                color #ffffff
            }
            element "Container" {
                background #438dd5
                color #ffffff
            }
            element "Database" {
                shape cylinder
                background #438dd5
                color #ffffff
            }
        }
    }
}