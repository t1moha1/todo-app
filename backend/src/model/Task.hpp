#pragma once
#include <string>
#include <chrono>
#include <json.hpp>

struct Task {
    int id;
    std::string title;
    bool done;
    std::string created_at;

    Task() = default;
    Task(int id_, const std::string& title_, bool done_,
         const std::string& created_at_)
      : id(id_), title(title_), done(done_), created_at(created_at_) {}

    nlohmann::json to_json() const;
};