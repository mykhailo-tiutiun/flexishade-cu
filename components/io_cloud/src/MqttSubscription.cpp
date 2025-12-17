#include "io/cloud/MqttSubscribtion.hpp"

#include <functional>
#include <string>

MqttSubscribtion::MqttSubscribtion
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

const std::string& MqttSubscribtion::getTopic() const
{
    return topic_;
}

const std::string& MqttSubscribtion::getContentType() const
{
    return content_type_;
}

void MqttSubscribtion::handleData(std::string& data)
{
    (handler_) (data, handler_args_);
}
