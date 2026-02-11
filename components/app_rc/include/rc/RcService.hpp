#ifndef RC_SERVICE_HPP
#define RC_SERVICE_HPP

#include "rc/RcRepository.hpp"
#include "rc/Rc.hpp"
#include "relay/RelayService.hpp"
#include <cstdint>

class RcService
{
    public:
        RcService(RcRepository* rc_repository, RelayService* relay_service);

        void toggleRc(std::uint64_t addr);
        void setRelay(std::uint64_t addr, RelayId relay_id);
    private:
        RcRepository* rc_repository_;
        RelayService* relay_service_;

        Rc newRc(std::uint64_t addr);

};

#endif
