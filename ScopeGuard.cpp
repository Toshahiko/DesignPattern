#include <exception>
#include <iostream>

namespace {
  class ScopeGuardBase {
    public:
    ScopeGuardBase() : m_commit(false) {}
    void Commit() const noexcept { m_commit = true ; }
    ScopeGuardBase& operator=(const ScopeGuardBase&) = delete ;

    protected:
    ScopeGuardBase( ScopeGuardBase&& other )
    : m_commit(other.m_commit) { other.Commit() ; }
    ~ScopeGuardBase() {} ;
    mutable bool m_commit ;
  } ;

  template<class Func>
  class ScopeGuard : public ScopeGuardBase {
    public:
    ScopeGuard( Func&& func ) : m_func(func) {}
    ScopeGuard( const Func& func ) : m_func(func) {}
    ~ScopeGuard() {
      if ( !m_commit ) m_func() ;
    }
    ScopeGuard( ScopeGuard&& other )
    : ScopeGuardBase( std::move( other ) )
    , m_func( other.m_func ) {

    }
    private:
    const Func& m_func ;
  } ;

  template<class Func>
  auto MakeGuard( Func&& func ) {
    return ScopeGuard<Func>( std::forward<Func>( func ) )  ;
  }

  class Record {

  } ;
  class Database {
    class Storage{
      public:
      void Insort( const Record& record ) {}
      void Finalize() {}
      void Undo() { std::cout << "Undo" << std::endl ; }
    } ;
    Storage S ;

    class Index{
      public:
      void Insort( const Record& record ) { throw std::exception() ;}
    } ;
    Index I ;
    public:
    void Insert( const Record& record ) {
      S.Insort( record ) ;
      auto SF = MakeGuard( [&]() { S.Finalize() ; }) ;
      auto SG = MakeGuard( [&]() { S.Undo() ; }) ;
      I.Insort( record ) ;
      SG.Commit() ;
    }
  } ;
}

int main() {
  Database db ;
  Record rd ;
  try {
    db.Insert( rd ) ;
  } catch(...) {

  }
}