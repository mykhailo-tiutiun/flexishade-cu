#ifndef RC_SERVICE_HPP
#define RC_SERVICE_HPP

#include "rc/MqttRcPublisher.hpp"
#include "rc/RcRepository.hpp"
#include "rc/Rc.hpp"
#include "relay/Relay.hpp"
#include "relay/RelayService.hpp"

class RcService
{
    public:
        RcService(RcRepository* rc_repository, MqttRcPublisher* rc_publisher, RelayService* relay_service);

        void toggleRc(RcId id);
        void setRelay(RcId id, RelayId relay_id);
    private:
        RcRepository* rc_repository_;
        MqttRcPublisher* rc_publisher_;
        RelayService* relay_service_;

        Rc newRc(RcId id);

};

#endif
