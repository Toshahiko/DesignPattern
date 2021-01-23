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

int Plus( int a, int b ) {
  return a + b ;
}

} // anonymous

int main() {
  auto deb = decorate_debug ( Plus, "Plus " ) ;

  deb( 1, 2 ) ;

  auto deb1 = decorate_debug( []( int& x ) { return x ; }, "x" ) ;
  int x = 4 ;
  deb1( x ) ;
}