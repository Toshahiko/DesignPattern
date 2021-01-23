#include <string>
#include <iostream>

namespace {

template<typename Callable>
class DebugDecorator {
public :
  DebugDecorator( const Callable& c, const char* s )
  : m_c( c ), m_s( s ) {}
  
  template<typename ... Args>
  auto operator()( Args&& ... args ) const {
    std::cout << "Invoking " << m_s << std::endl ;
    auto res = m_c( std::forward<Args>( args ) ... ) ;
    std::cout << "Result: " << res << std::endl ;
    return res ;
  }

private :
  const Callable& m_c ;
  const std::string m_s ;
} ;

template<typename Callable>
auto decorate_debug( const Callable& c, const char* s ) {
  return DebugDecorator<Callable>( c, s ) ;
}

template<typename Callable>
class LockDecorator {
public:
  LockDecorator( const Callable& c, std::mutex& m )
    : m_c( c ), m_m( m ) {}

  template<typename ... Args>
  auto operator()( Args&& ... args ) const {
    std::lock_guard<std::mutex> l( m_m ) ;
    return m_c( std::forward<Args>( args ) ... ) ;
  }
private:
  const Callable& m_c ;
  std::mutex& m_m ;
} ;

template <typename Callable>
auto decorate_lock( const Callable& c, std::mutex& m ) {
  return LockDecorator<Callable>( c, m ) ;
}

int Plus( int a, int b ) {
  return a + b ;
}

int unsafe_f( int x ) {
  return x ;
}

} // anonymous

int main() {
  auto deb = decorate_debug ( Plus, "Plus " ) ;

  deb( 1, 2 ) ;

  auto deb1 = decorate_debug( []( int& x ) { return x ; }, "x" ) ;
  int x = 4 ;
  deb1( x ) ;

  std::mutex m ;
  auto safe_f = decorate_debug( decorate_lock( [] ( int x ) { return unsafe_f( x ) ; }, m ), "f(x) " ) ;
  safe_f( 1 ) ;

}