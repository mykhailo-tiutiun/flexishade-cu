#ifndef APP_CONTEXT_HPP
#define APP_CONTEXT_HPP

#include "../StateLed.hpp"
#include <any>
#include <optional>
#include <typeindex>
#include <unordered_map>
#include "AppState.hpp"


class Component
{
    public:
        virtual void onContextRegister(AppContext* context) = 0;
};

class AppContext {
    public:
        AppContext();
        void run_app();

        void transit_state(AppStateType type);
        AppState* getCurrentState() const;


        template<typename T>
        void registerComponent(T* component)
        {
            components_[std::type_index(typeid(T))] = component;
        }

        template<typename T>
        std::optional<T*> tryGetComponent()
        {
            auto it = components_.find(std::type_index(typeid(T)));
            if (it == components_.end()) {
                return std::nullopt;
            }


            if (T** ptr = std::any_cast<T*>(&it->second)) {
                return *ptr;
            }

            return std::nullopt;
        }

    private:
        AppState *state_;
        std::unordered_map<std::type_index, std::any> components_;
        std::unordered_map<AppStateType, AppState*> states_;

        AppState* getState(AppStateType type);
        void addState(AppState* state);
};

#endif
