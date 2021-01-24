#include <algorithm>
#include <vector>
#include <iostream>

namespace {
struct much_less {
  template<typename T>
  bool operator()( T x, T y ) const {
    return x < y && std::abs( x - y ) > tolerance*std::max( std::abs(x), std::abs(y) ) ;
  }

  static constexpr double tolerance = 0.02 ;
} ;

template<typename Iterator>
void sort_much_less( Iterator first, Iterator last ) {
  std::sort( first, last, much_less() ) ;
}

template<typename Container>
void sort_much_less( Container& c ) {
  std::sort( c.begin(), c.end(), much_less() ) ;
}

auto sort_much_less_lambda = []( auto first, auto last ) {
  std::sort( first, last, much_less() ) ;
} ;

auto sort_much_less_lambda_container = []( auto& v ) {
  std::sort( v.begin(), v.end(), much_less() ) ;
} ;
} // anonymous namespace

int main() {

  std::vector<double> v{ 1.0, 0.8, 0.5 } ;
  // std::sort( v.begin(), v.end(), much_less() ) ;
  // sort_much_less( v.begin(), v.end() ) ;
  sort_much_less( v ) ;

  std::for_each( v.begin(), v.end(), []( double x ) { std::cout << x << std::endl ; } ) ;
}