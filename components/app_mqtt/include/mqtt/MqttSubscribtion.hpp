#ifndef MQTT_SUBSCRIBTION_HPP
#define MQTT_SUBSCRIBTION_HPP

#include <functional>
#include <string>

class MqttSubscribtion
{
    public:
        using Handler = std::function<void(std::string&, void*)>;

        MqttSubscribtion(std::string topic, std::string content_type, Handler handler, void* handler_args);

        const std::string& getTopic() const;
        const std::string& getContentType() const;

        void handleData(std::string& data) const;
    private:
        std::string topic_;
        std::string content_type_;
        Handler handler_;
        void* handler_args_;
};

#endif
