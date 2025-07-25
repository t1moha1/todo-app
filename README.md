# ToDo App

TODO-приложение с бэкендом на C++ упакованное в Docker Compose.

##  Обзор

* **База данных**: PostgreSQL 15
* **API-сервер**: C++ (httplib + nlohmann::json + libpqxx)
* **Фронтенд**: статический сайт (HTML/CSS/JS) под управлением Nginx
* **Коммуникация между сервисами**: Docker Compose


## Архитектура

```text
┌─────────────────┐        ┌─────────────────┐
│     frontend    │◀──────▶│      api        │◀──────▶ PostgreSQL
│ (NGINX + Static)│  HTTP  │ (C++ Server)    │  libpqxx
└─────────────────┘        └─────────────────┘
```

* **frontend** (порт 80)

  * `index.html`, `styles.css`, `app.js`
  * Подключается к API на `http://<hostname>:8080`

* **api** (порт 8080)

  * C++ приложение с `cpp-httplib`, `nlohmann::json`, `libpqxx`
  * Ендпоинты:

    * `GET    /tasks`
    * `POST   /tasks`
    * `PUT    /tasks/:id`
    * `DELETE /tasks/:id`

* **db**

  * PostgreSQL 15
  * Инициализация схемы из `backend/db/init.sql`



## Запуск

1. **Клонировать репозиторий**

   ```bash
   git clone https://github.com/youruser/todo-app.git
   cd todo-app
   ```

2. **Запустить контейнеры**

   ```bash
   docker-compose up --build
   ```

3. **Открыть приложение**

   * Фронтенд:  [http://localhost](http://localhost)
   * API (пример): [http://localhost:8080/tasks](http://localhost:8080/tasks)

4. **Остановить контейнеры**

   ```bash
   docker-compose down
   ```



## API

Все ответы в формате JSON, заголовок `Content-Type: application/json`.

| Метод  | Путь         | Тело запроса                             | Описание                                    |
| ------ | ------------ | ---------------------------------------- | ------------------------------------------- |
| GET    | `/tasks`     | —                                        | Получить список всех задач                  |
| POST   | `/tasks`     | `{ "title": "Новая задача" }`            | Создать задачу (возвращает `{ "id": ... }`) |
| PUT    | `/tasks/:id` | `{ "title": "...", "done": true/false }` | Обновить задачу по `id`                     |
| DELETE | `/tasks/:id` | —                                        | Удалить задачу по `id`                      |


