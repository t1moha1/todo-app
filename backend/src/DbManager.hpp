#pragma once
#include <vector>
#include <string>
#include <pqxx/pqxx>
#include "model/Task.hpp"
#include <optional>

class DbManager {
public:
    explicit DbManager(const std::string& conn_str);

    std::vector<Task> getAll();
    Task getById(int id);
    int create(const Task& t);
    std::optional<Task> update(const Task& t);
    bool remove(int id);

private:
    pqxx::connection db_;
};