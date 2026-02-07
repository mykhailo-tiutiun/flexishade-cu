#ifndef NORMAL_ERR_STATE_HPP
#define NORMAL_ERR_STATE_HPP

#include "app/AppState.hpp"
#include <atomic>
#include <optional>
#include <thread>

#define RETRY_DELAY_SECONDS 100

class LocalOnlyState: public AppState {
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override
        {
            return "LocalOnlyState";
        }

        AppStateType getType() const override
        {
            return LOCAL_ONLY;
        }

    private:
        std::optional<std::thread> retry_thread_;
        std::atomic<bool> is_retry_canceled_;

        void retryTask() const;
        void retryAfter();
        void cancelRetry();
};

#endif
