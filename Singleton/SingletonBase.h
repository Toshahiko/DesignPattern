#pragma once

namespace Design {

template <class T>
class SingletonBase
{
protected:
  SingletonBase() {}
public:
  SingletonBase(SingletonBase const &) = delete ;
  SingletonBase& operator=(SingletonBase const&) = delete ;
  static T& instance()
  {
    static T single;
    return single;
  }
} ;

}