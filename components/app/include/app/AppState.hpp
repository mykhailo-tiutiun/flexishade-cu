#ifndef APPLICATION_STATE_HPP
#define APPLICATION_STATE_HPP

class AppContext;

enum AppStateType {
    INIT,
    ZIGBEE_INIT,
    NORMAL,
    CONFIG,
    NORMAL_ERR,
    CONFIG_ERR,
    CRITICAL,
};

class AppState {
    protected:
        AppContext *context_;
    public:
        virtual ~AppState() = default;

        void setContext(AppContext* context);

        virtual void onEnter() = 0;
        virtual void onExit() = 0;

        virtual void toogleConfigMode() = 0;
        virtual const char* getName() const = 0;
        virtual AppStateType getType() const = 0;
};

#endif
