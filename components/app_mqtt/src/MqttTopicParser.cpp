#include "mqtt/MqttTopicParser.hpp"
#include <string>
#include <string_view>

MqttTopicParser::MqttTopicParser(std::string client_id, std::string_view topic)
: client_id_(client_id)
, is_valid_(true)
{
    parse(topic);
}

bool MqttTopicParser::isValid() const
{
    return is_valid_;
}

const std::string& MqttTopicParser::getControllerId() const
{
    return controller_id_;
}

const std::string& MqttTopicParser::getMethodId() const
{
    return method_id_;
}

void MqttTopicParser::setClientId(std::string client_id)
{
    client_id_ = client_id;
}

void MqttTopicParser::parse(std::string_view s)
{
    auto s_client_id = s.substr(0, client_id_.size());

    checkSlash(s, client_id_.size());

    if (s_client_id != client_id_) {
        is_valid_ = false;
        return;
    }

    auto other = s.substr(client_id_.size() + 1, s.size());
    auto other_first_slash_at = other.find('/');

    auto s_controller_id_ = other.substr(0, other_first_slash_at);
    auto s_method_id_ = other.substr(other_first_slash_at + 1, other.size());

    controller_id_ = std::string(s_controller_id_);
    method_id_ = std::string(s_method_id_);
}

void MqttTopicParser::checkSlash(std::string_view s, int at)
{
    if (s.at(at) != '/') {
        is_valid_ = false;
    }
}
