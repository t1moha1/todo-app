#include "DbManager.hpp"

DbManager::DbManager(const std::string& conn_str)
  : db_(conn_str)
{}

std::vector<Task> DbManager::getAll() {
    pqxx::work txn(db_);
    const pqxx::result &res = txn.exec("SELECT id, title, done, created_at FROM tasks ORDER BY id");
    std::vector<Task> v;
    for (const auto& r : res) {
        v.emplace_back(
            r["id"].as<int>(),
            r["title"].as<std::string>(),
            r["done"].as<bool>(),
            r["created_at"].as<std::string>()
        );
    }
    return v;
}

Task DbManager::getById(int id) {
    pqxx::work txn(db_);
    auto res = txn.exec_params(
        "SELECT id, title, done, created_at FROM tasks WHERE id = $1", id);
    if (res.empty()) {
        throw std::runtime_error("Task not found");
    }

    auto r = res[0];
    return Task(
        r["id"].as<int>(),
        r["title"].as<std::string>(),
        r["done"].as<bool>(),
        r["created_at"].as<std::string>()
    );
}

int DbManager::create(const Task& t) {
    pqxx::work txn(db_);
    auto res = txn.exec_params(
        "INSERT INTO tasks (title, done) VALUES ($1, $2) RETURNING id",
        t.title, t.done);
    txn.commit();
    return res[0][0].as<int>();
}

std::optional<Task> DbManager::update(const Task& t) {
    pqxx::work txn{db_};
    pqxx::result r = txn.exec_params(
        "UPDATE tasks "
        "   SET title = $1, done = $2 "
        " WHERE id = $3 "
        "RETURNING id, title, done, created_at",
        t.title, t.done, t.id);
    txn.commit();

    if (r.empty()) {
        return std::nullopt;
    }

    auto row = r[0];
    return Task {
        row["id"].as<int>(),
        row["title"].as<std::string>(),
        row["done"].as<bool>(),
        row["created_at"].as<std::string>()
    };
}

bool DbManager::remove(int id) {
    pqxx::work txn(db_);
    txn.exec_params("DELETE FROM tasks WHERE id = $1", id);
    txn.commit();
    return true;
}