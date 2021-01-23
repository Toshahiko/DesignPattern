#include <queue>
#include <mutex>

namespace {
template<typename T>
class locking_queue {
  using mutex = std::mutex ;
  using lock_guard = std::lock_guard<mutex> ;
  using value_type = typename std::queue<T>::value_type ;

public:
  void push( const value_type& value ) {
    lock_guard l( m_m ) ;
    m_q.push( value ) ;
  }

  void push( value_type&& value ) {
    lock_guard l( m_m ) ;
    m_q.push( value ) ;
  }

  bool pop( value_type& value ) {
    lock_guard l( m_m ) ;
    if ( m_q.empty() ) return false ;
    value = std::move( m_q.front() ) ;
    m_q.pop() ;
    return true ;
  }

private:
  std::queue<T> m_q ;
  mutex m_m ;
} ;

} // anonymous

int main() {
  locking_queue<int> q ;
  q.push( 5 ) ;
  int res = -1 ;
  if ( !q.pop( res ) ) {

  }

}