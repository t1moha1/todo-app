#include "httplib.h"
#include "DbManager.hpp"
#include "model/Task.hpp"
#include <json.hpp>
#include <cstdlib>
#include <iostream>

using json = nlohmann::json;
using namespace httplib;

int main() {
    std::string conn =
        "host=" + std::string(std::getenv("DB_HOST")) +
        " port=" + std::string(std::getenv("DB_PORT")) +
        " dbname=" + std::string(std::getenv("DB_NAME")) +
        " user=" + std::string(std::getenv("DB_USER")) +
        " password=" + std::string(std::getenv("DB_PASS"));

    DbManager db{conn};
    Server svr;

    svr.Get("/tasks", [&](const Request&, Response& res) {
        auto tasks = db.getAll();
        json arr = json::array();
        for (auto& t : tasks) arr.push_back(t.to_json());
        res.set_content(arr.dump(), "application/json");
    });

    svr.Post("/tasks", [&](const Request& req, Response& res) {
        auto j = json::parse(req.body);
        Task t;
        t.title = j.at("title").get<std::string>();
        t.done  = false;
        int id = db.create(t);
        res.set_content(json({{"id", id}}).dump(), "application/json");
    });

    svr.Put(R"(/tasks/(\d+))", [&](const Request& req, Response& res) {
        int id = 0;
        try { id = std::stoi(req.matches[1]); }
        catch(...) {
            res.status = 400;
            res.set_content(R"({"error":"invalid task id"})", "application/json");
            return;
        }

        json j;
        try {
            j = json::parse(req.body);
        } catch (...) {
            res.status = 400;
             res.set_content(R"({"error":"invalid json"})", "application/json");
        }

        Task t;
        t.id = id;

        if (j.contains("title") && j["title"].is_string()) {
            t.title = j["title"].get<std::string>();
        }

        if(j.contains("done") && j["done"].is_boolean()) {
            t.done = j["done"].get<bool>();
        }

        auto updated_opt = db.update(t);
        if (!updated_opt) {
            res.status = 404;
            res.set_content(R"({"error":"task not found or not updated"})", "application/json");
            return;
        }

        res.set_content(updated_opt->to_json().dump(), "application/json");
        res.status = 200;
    });

    svr.Delete(R"(/tasks/(\d+))", [&](const Request& req, Response& res) {
        int id = std::stoi(req.matches[1]);
        bool ok = db.remove(id);
        res.status = ok ? 200 : 404;
    });

    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });
    svr.Options(R"(/.*)", [&](auto&, auto& res) { res.status = 200; });

    std::cout << "Listening on 0.0.0.0:8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}