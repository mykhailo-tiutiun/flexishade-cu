#ifndef MQTT_SUBSCRIBTION_HPP
#define MQTT_SUBSCRIBTION_HPP

#include <functional>
#include <string>

class MqttSubscribtion
{
    public:
        using Handler = std::function<void(std::string&, void*)>;

        MqttSubscribtion
        (
            std::string topic,
            std::string content_type,
            Handler handler,
            void* handler_args
        )
        : topic_(std::move(topic))
            , content_type_(std::move(content_type))
            , handler_(handler)
            , handler_args_(handler_args)
        {}
        
        const std::string& getTopic() const
        {
            return topic_;
        }

        const std::string& getContentType() const
        {
            return content_type_;
        }

        void handleData(std::string& data)
        {
            (handler_) (data, handler_args_);
        }
    private:
        std::string topic_;
        std::string content_type_;
        Handler handler_;
        void* handler_args_;
};

#endif
