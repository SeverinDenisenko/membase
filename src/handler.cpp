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

    size_t it = request.find('\0');
    if (it == std::string::npos) {
        goto error;
    }

    {
        boost::tokenizer<> tokenizer(request.substr(0, it));
        boost::tokenizer<>::iterator begin = tokenizer.begin();
        boost::tokenizer<>::iterator end = tokenizer.end();

        if (begin == end)
            goto error;
        size_t tokens = std::distance(begin, end);
        std::string command = *begin++;
        std::optional<std::string> key = (begin != end) ? std::make_optional(*begin++) : std::nullopt;
        std::optional<std::string> value = (begin != end) ? std::make_optional(*begin++) : std::nullopt;

        if (command == "GET" && key && !value && tokens == 2) {
            std::optional<std::string> ans = db.get(*key);
            if (!ans)
                goto error;
            res = fmt::format("VALUE {}\n", *ans);
        } else if (command == "PUT" && key && value && tokens == 3) {
            db.put(*key, *value);
            res = fmt::format("OK\n");
        } else if (command == "REMOVE" && key && !value && tokens == 2) {
            db.remove(*key);
            res = fmt::format("OK\n");
        } else {
            goto error;
        }
    }

    return res;
error:
    res = "ERROR\n";
    return res;
}
