#include <iostream>

namespace {
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

  class Single : public SingletonBase<Single>
  {
  private:
    Single() {}
    friend class SingletonBase<Single> ;
  public:
    void demo() { std::cout << "demo" << std::endl ; }
  } ;
}

int main() {
  Single::instance().demo() ;

}