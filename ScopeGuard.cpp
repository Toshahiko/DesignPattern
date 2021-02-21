#include <exception>
#include <iostream>

namespace {
  class ScopeGuardImplBase {
    public:
    ScopeGuardImplBase() : m_commit(false) {}
    void Commit() const noexcept { m_commit = true ; }
    ScopeGuardImplBase& operator=(const ScopeGuardImplBase&) = delete ;

    protected:
    ScopeGuardImplBase( ScopeGuardImplBase&& other )
    : m_commit(other.m_commit) { other.Commit() ; }
    ~ScopeGuardImplBase() {} ;
    mutable bool m_commit ;
  } ;

  template<class MemFunc, class Obj>
  class ScopeGuardImpl : public ScopeGuardImplBase {
    public:
    ScopeGuardImpl( const MemFunc& memfunc, Obj& obj ) : m_memfunc(memfunc), m_obj(obj) {}
    ~ScopeGuardImpl() {
      if ( !m_commit ) (m_obj.*m_memfunc)() ;
    }
    ScopeGuardImpl( ScopeGuardImpl&& other )
    : ScopeGuardImplBase( std::move( other ) )
    , m_memfunc( other.m_memfunc )
    , m_obj ( other.m_obj ) {

    }
    private:
    const MemFunc& m_memfunc ;
    Obj& m_obj ;
  } ;

  template<class MemFunc, class Obj>
  auto MakeGuard( const MemFunc& memfunc, Obj& obj ) {
    return ScopeGuardImpl<MemFunc, Obj>( memfunc, obj ) ;
  }

  using ScopeGuard = const ScopeGuardImplBase& ; // constがないとビルドエラー。一時オブジェクトを参照できない。

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

    class StorageFinalizer {
      public:
      StorageFinalizer( Storage& s ) : m_s( s ) {

      }
      ~StorageFinalizer() { m_s.Finalize() ; }
      private:
      Storage& m_s ;
    } ;


    class Index{
      public:
      void Insort( const Record& record ) { throw std::exception() ;}
    } ;
    Index I ;
    public:
    void Insert( const Record& record ) {
      S.Insort( record ) ;
      StorageFinalizer SF( S ) ;
      // void ( Storage::*Undo )() = &Storage::Undo ; // メンバ関数ポインタを扱う
      using StorageMemFunc = void( Storage::* )() ; // 上と同じ
      StorageMemFunc undo = &Storage::Undo ;
      ScopeGuard SG = MakeGuard( undo, S ) ;
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