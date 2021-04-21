#include <vector>
#include <memory>
#include <iostream>
#include <regex>

class BigArray {
  private:
  std::unique_ptr<char[]> m_big_data ;
  public:
  BigArray( int ) {
    // std::cout << "default" << std::endl ;
  }

  BigArray( const BigArray& other ) = delete ;

  BigArray& operator=( const BigArray& other ) = delete ;

  BigArray( BigArray&& other )
  : m_big_data( std::move( other.m_big_data ) ) {
    other.m_big_data = nullptr ;
    // std::cout << "move" << std::endl ;
  }

  BigArray& operator=( BigArray&& other ) {
    m_big_data = std::move( other.m_big_data ) ;
    other.m_big_data = nullptr ;
    // std::cout << "move" << std::endl ;
    return *this ;
  }

  static BigArray New() {
    BigArray a(1) ;
    // std::cout << "New" << std::endl ;
    return a ;
  }

} ;

BigArray Factory() {
  return BigArray( 2 ) ;
}

BigArray Factory2() {
  BigArray t1(2), t2(3) ;
  if ( true ) {
    return t1 ;
  } else {
    return t2 ;
  }
}

int main() {
  BigArray text(1) ;

  // BigArray text2( text ) ;

  auto text3 = BigArray(1) ;

  const auto text4 = BigArray::New() ; // Named Return Value Optimization(NRVO)が発生


  text3 = BigArray::New() ;

  auto text5 = Factory() ; // Return Value Optimization(RVO)が発生

  text5 = Factory() ;

  auto text6 = Factory2() ; // move constructorが呼ばれる



  const char s[] = R"(She said "He is Hungry.")" ;
  const std::regex re( R"("(\w+) is (\w+).")") ;

  std::cmatch m ;
  if ( std::regex_search( s, m, re ) ) {
    for ( std::size_t i = 0, n = m.size() ; i < n ; ++i ) {
      std::cout << i << ":'" << m.str( i ) << "\', "
      << "position = " << m.position( i ) << ", "
      << "length = " << m.length( i ) << std::endl ;

    }
  }
}