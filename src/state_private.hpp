/**
* @author Aldebaran Robotics
* Aldebaran Robotics (c) 2012 All Rights Reserved
*/

#pragma once

#ifndef STATE_PRIVATE_H_
# define STATE_PRIVATE_H_

# include <list>

# include <qicore/state.hpp>
# include <qicore/transition.hpp>

class Diagram;

class StatePrivate
{
  friend class TransitionPrivate;
  friend class State;

  public:
    StatePrivate(State* s);
    ~StatePrivate();

    void addTransition(Transition* tr);
    void removeTransition(Transition* tr);

    void setDiagram(Diagram* d);

  private:
    std::string                     _name;
    State*                          _parent;
    Diagram*                        _diagram;
    std::list<Transition*>          _transitions;
};

#endif /* !STATE_PRIVATE_H_ */
