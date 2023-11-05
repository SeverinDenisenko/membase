#include "handler.hpp"

#include <boost/tokenizer.hpp>
#include <fmt/format.h>
#include <glog/logging.h>

mb::Handler::Handler(DB& db) noexcept
    : db(db)
{
}

std::string mb::Handler::operator()(const std::string& request) noexcept
{
    std::string res;

    {
        std::string truncated = truncate(request);
        using tokenizer_t = boost::tokenizer<boost::char_separator<char>>;
        boost::char_separator<char> sep(" ");
        tokenizer_t tokenizer(truncated, sep);
        tokenizer_t::iterator begin = tokenizer.begin();
        tokenizer_t::iterator end = tokenizer.end();

        if (begin == end)
            goto error;
        size_t tokens = std::distance(begin, end);
        std::string command = *begin++;
        std::optional<std::string> key = (begin != end) ? std::make_optional(*begin++) : std::nullopt;
        std::optional<std::string> value = (begin != end) ? std::make_optional(*begin++) : std::nullopt;

        if (command == "GET" && key && !value && tokens == 2) {
            std::optional<std::string> ans = db.get(std::move(*key));
            if (!ans)
                goto error;
            res = fmt::format("VALUE {}\n", *ans);
        } else if (command == "PUT" && key && value && tokens == 3) {
            db.put(std::move(*key), std::move(*value));
            res = fmt::format("OK\n");
        } else if (command == "REMOVE" && key && !value && tokens == 2) {
            db.remove(std::move(*key));
            res = fmt::format("OK\n");
        } else if (command == "WIPE" && !key && !value && tokens == 1) {
            db.wipe();
            res = fmt::format("OK\n");
        } else if (command == "FINDKEY" && key && !value && tokens == 2) {
            auto data = db.findKey(std::move(*key));
            for (auto& a : data) {
                res += fmt::format("KEY {}\n", a);
            }
            res += fmt::format("OK\n");
        } else if (command == "FINDVALUE" && key && !value && tokens == 2) {
            auto data = db.findValue(std::move(*key));
            for (auto& a : data) {
                res += fmt::format("KEY {}\n", a);
            }
            res += fmt::format("OK\n");
        } else {
            goto error;
        }
    }

    return res;
error:
    res = "ERROR\n";
    return res;
}

std::string mb::Handler::truncate(const std::string& request) noexcept
{
    std::string res;

    auto it = std::find_if(request.begin(), request.end(), [](const char c) {
        return std::iscntrl(c);
    });

    if (it == request.end()) {
        return res;
    }

    size_t len = std::distance(request.begin(), it);
    res = request.substr(0, len);

    return res;
}
