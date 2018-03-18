#pragma once

template <typename T, typename U>
class StateHandler;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

A self-referring templated State class.

The first type represents the type of State this class is.
The second type represents the type of StateHandler this State is handled by.

The Enter and Leave functions are called automatically by the StateHandler
when transitions between states occur.

---------------------------------------------------------*/
template <typename StateType, typename StateHandlerType>
class State
{
template <typename T, typename U>
friend class StateHandler;

public:
    virtual ~State() = default;

    virtual void onStateEnter() = 0;
    virtual void onStateLeave() = 0;

    virtual void tick() = 0;

protected:
    State()
        : handler(nullptr)
    {
    }

    StateHandler<StateType, StateHandlerType>* getHandler() const
    {
        return handler;
    }

private:
    void setHandler(StateHandler<StateType, StateHandlerType>* _handler)
    {
        handler = _handler;
    }

    StateHandler<StateType, StateHandlerType>* handler;

};
