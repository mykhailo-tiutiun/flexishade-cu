#ifndef APP_STATE_FACTORY
#define APP_STATE_FACTORY

class AppState;

class AppStateFactory {
    public:
        AppState* init() const;
        AppState* zigbeeInit() const;
        AppState* normal() const;
        AppState* config() const;
};

#endif