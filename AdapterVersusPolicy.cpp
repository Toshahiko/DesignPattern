#include <iostream>

namespace {
template<typename T>
class Value {
public:
  typedef T basic_type ;
  typedef Value value_type ;

  explicit Value() : m_value( T() ) {}
  explicit Value( T v ) : m_value( v ) {}

  Value( const Value& rhs ) : m_value( rhs.m_value ) {}
  Value& operator=( Value rhs ) { m_value = rhs.m_value; return *this; }
  Value& operator=( basic_type rhs ) { m_value = rhs ; return *this ; }
  friend std::ostream& operator<<( std::ostream& out, Value x ) {
    out << x.m_value ; return out ;
  }

  friend std::istream& operator>>( std::istream& in, Value& x ) {
    in >> x.m_value ; return in ;
  }

protected:
  T m_value ;
} ;

template<typename V>
class Comparable : public V {
public:
  using V::V ;
  typedef typename V::value_type value_type ;
  typedef typename value_type::basic_type basic_type ;
  Comparable( value_type v ) : V( v ) {}
  friend bool operator==( Comparable lhs, Comparable rhs ) {
    return lhs.m_value == rhs.m_value ;
  }

  friend bool operator !=( Comparable lhs, Comparable rhs ) {
    return lhs.m_value != rhs.m_value ;
  }
  friend bool operator==( Comparable lhs, basic_type rhs ) {
    return lhs.m_value == rhs ;
  }
  friend bool operator==( basic_type lhs, Comparable rhs ) {
    return lhs == rhs.m_value ;
  }
  friend bool operator!=( Comparable lhs, basic_type rhs ) {
    return lhs.m_value != rhs ;
  }
  friend bool operator!=( basic_type lhs, Comparable rhs ) {
    return lhs != rhs.m_value ;
  }
} ;

template<typename V>
class Ordered : public V {
public:
  using V::V ;
  typedef typename V::value_type value_type ;
  typedef typename value_type::basic_type basic_type ;
  Ordered( value_type v ) : V( v ) {}

  friend bool operator<( Ordered lhs, Ordered rhs ) {
    return lhs.m_value < rhs.m_value ;
  }
  friend bool operator<( basic_type lhs, Ordered rhs ) {
    return lhs < rhs.m_value ;
  }
    friend bool operator<( Ordered lhs, basic_type rhs ) {
    return lhs.m_value < rhs ;
  }

  friend bool operator>( Ordered lhs, Ordered rhs ) {
    return lhs.m_value > rhs.m_value ;
  }
  friend bool operator>( basic_type lhs, Ordered rhs ) {
    return lhs > rhs.m_value ;
  }
    friend bool operator>( Ordered lhs, basic_type rhs ) {
    return lhs.m_value > rhs ;
  }

} ;

} // anonymous namespace

int main() {
 using V = Comparable<Value<int>> ;
 V i( 3 ), j( 5 ) ;
 i == j ;

 using V_OC = Ordered<Comparable<Value<int>>> ;

 V_OC k( 5 ), l( 2 ) ;

 k < l ;

}