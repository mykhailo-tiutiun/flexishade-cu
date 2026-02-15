#ifndef MQTT_TOPIC_PARSER_HPP
#define MQTT_TOPIC_PARSER_HPP

#include <string>
#include <string_view>

// dev/cu/1/relay/set_state
//
//   dev/cu/1  /     relay     / set_state
//  client_id  | controller_id | method_id

class MqttTopicParser
{
    public:
        MqttTopicParser(std::string client_id, std::string_view topic);
        bool isValid() const;
        const std::string& getControllerId() const;
        const std::string& getMethodId() const;
    private:
        std::string client_id_;
        std::string controller_id_;
        std::string method_id_;

        bool is_valid_;

        int client_id_slash_total_;

        void setClientId(std::string client_id);

        void parse(std::string_view s);

        void checkSlash(std::string_view s, int at);
};

#endif
