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

  std::pair<value_type, bool> pop() {
    lock_guard l( m_m ) ;
    if ( m_q.empty() ) return { value_type(), false } ;
    value_type value = std::move( m_q.front() ) ;
    m_q.pop() ;
    return { value, true } ;
  }

private:
  std::queue<T> m_q ;
  mutex m_m ; // pushとpopが同じmutexでlockされるので、thread_safeである。
} ;

} // anonymous

int main() {
  locking_queue<int> q ;
  q.push( 5 ) ;
  int res = -1 ;
  auto [ret, b] = q.pop() ;
  if ( !b ) {

  }

}