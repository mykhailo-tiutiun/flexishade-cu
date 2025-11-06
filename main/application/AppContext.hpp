#ifndef APP_CONTEXT_HPP
#define APP_CONTEXT_HPP

#include "../StateLed.hpp"
#include "AppStateFactory.hpp"

class AppState;

class AppContext {
    private:
        AppState *state_;
        StateLed *state_led_;
        AppStateFactory *state_factory_;
    public:
        AppContext();
        void run_app();

        void transit_state(AppState *state);
        AppState* getState() const;

        void setStateLed(StateLed *led);
        StateLed* getStateLed() const;

        void setAppStateFactory(AppStateFactory *factory);
        AppStateFactory* getAppStateFactory();
};

#endif