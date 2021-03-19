#include <vector>
#include <sstream>
#include <iostream>

namespace {
class Point ;
class Circle ;
class Line ;
class Visitor ;
class Geometry {
  public:
  virtual ~Geometry()= default ;
  virtual void Accept( Visitor& v ) = 0 ;
  enum type_tag { POINT = 100, CIRCLE, LINE, INTERSECTION } ;
  virtual type_tag tag() const = 0 ;
} ;

class Visitor {
  public:
  static Geometry* make_geometry( Geometry::type_tag tag ) ;
  virtual void Visit( Geometry::type_tag& tag ) = 0 ;
} ;


class Point : public Geometry {
  public:
  Point() = default ;
  Point( double x , double y ) : m_x( x ) , m_y( y ) {}
  void Accept( Visitor& v ) override {
    v.Visit( m_x ) ;
    v.Visit( m_y ) ;
  }
  type_tag tag() const override { return POINT ; }
  private:
  double m_x ;
  double m_y ;
} ;

class Circle : public Geometry {
  public:
  Circle() = default ;
  Circle( const Point& c , double r ) : m_c( c ) , m_r( r ) {}
  void Accept( Visitor& v ) override {
    v.Visit( m_c ) ;
    v.Visit( m_r ) ;
  }
  type_tag tag() const override { return CIRCLE ; }
  private:
  Point m_c ;
  double m_r ;
} ;

class Line : public Geometry {
  public:
  Line() = default ;
  Line( const Point& p1 , const Point& p2 ) : m_p1( p1 ) , m_p2( p2 ) {}
  void Accept( Visitor& v ) override {
    v.Visit( m_p1 ) ;
    v.Visit( m_p2 ) ;
  }
  type_tag tag() const override { return LINE ; }
  private:
  Point m_p1 ;
  Point m_p2 ;
} ;

class Intersection : public Geometry {
  public:
  Intersection() = default ;
  Intersection( Geometry* g1, Geometry* g2 ) : m_g1( g1 ), m_g2( g2 ) {}
  void Accept( Visitor& v ) override {
    m_g1->Accept( v ) ;
    m_g2->Accept( v ) ;
  }
  type_tag tag() const override { return INTERSECTION ; }
  private:
  std::unique_ptr<Geometry> m_g1 ;
  std::unique_ptr<Geometry> m_g2 ;
} ;


Geometry* Visitor::make_geometry( Geometry:type_tag tag ) {
  switch ( tag ) {
    case Geometry::POINT: return new Point ;
    case Geometry::CIRCLE: return new Circle ;
    case Geometry::LINE: return new Line ;
    case Geometry::INTERSECTION: return new Intersection ;
    default : assert( false ) ;
  }
}


class StringSerializeVisitor : public Visitor {
  public:
  void Visit( double& x ) override { m_ss << x << " " ; }
  void Visit( Point& p ) override { p.Accept( *this ) ; }
  void Visit( Circle& c ) override { c.Accept( *this ) ; }
  void Visit( Line& l ) override { l.Accept( *this ) ; }
  std::string str() const { return m_ss.str() ; }

  private:
  std::stringstream m_ss ;
} ;

}

int main() {
  Point p1 ( 1.2, 2.2 ) ;
  Point p2 ( 3.0, 4.0 ) ;

  Line l1( p1, p2 ) ;
  
  StringSerializeVisitor serialize ;
  l1.Accept( serialize ) ;
  
  std::cout << serialize.str() << std::endl ;
}