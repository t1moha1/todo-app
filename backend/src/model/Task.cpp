#include "Task.hpp"

nlohmann::json Task::to_json() const {
    return {
        {"id", id},
        {"title", title},
        {"done", done},
        {"created_at", created_at}
    };
}