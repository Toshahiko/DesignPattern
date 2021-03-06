#include <string>
#include <iostream>

namespace {

template<typename ... Types>
class Visitor ;

template<typename T>
class Visitor<T> {
public:
  virtual void Visit( T* t ) = 0 ;
} ;

template<typename T, typename ... Types>
class Visitor<T, Types ...> : public Visitor<Types ...> {
public:
  using Visitor<Types...>::Visit ;
  virtual void Visit( T* t ) = 0 ;
} ;

class Cat ;
class Dog ;
using PetVisitor = Visitor<Dog, Cat> ;

// using PetVisitor = Visitor<class Dog, class Cat> ; // δΈγ¨εγ

template<typename Base, typename ... >
class LambdaVisitor ;

template<typename Base, typename T1, typename F1>
class LambdaVisitor<Base, Visitor<T1>, F1> : private F1, public Base {
public:
  LambdaVisitor( F1&& f1 ) : F1( std::move( f1 ) ) {}
  LambdaVisitor( const F1& f1 ) : F1( f1 ) {}
  void Visit( T1* t ) override { return F1::operator()( t ) ; }
} ;

template<typename Base,
         typename T1, typename ... T,
         typename F1, typename ... F>
class LambdaVisitor< Base, Visitor<T1, T ...>, F1, F ...>
  : private F1, public LambdaVisitor<Base, Visitor<T ...>, F ...>
{
  public:
  LambdaVisitor( F1&& f1, F&& ... f )
    : F1( std::move( f1 ) ),
      LambdaVisitor<Base, Visitor<T ...>, F ...>( std::forward<F>( f ) ...) {}

  LambdaVisitor( const F1& f1, F&& ... f )
    : F1( f1 ),
      LambdaVisitor<Base, Visitor<T ...>, F ...>( std::forward<F>( f ) ...) {}

  void Visit( T1* t ) override { return F1::operator()( t ) ; }
} ;

template<typename Base, typename ... F>
auto lambda_visitor( F&& ... f ) {
  return LambdaVisitor<Base, Base, F ...>( std::forward<F>( f ) ... ) ;
}

class Pet {
public:
  virtual ~Pet() = default ;
  Pet( const std::string& color ) : m_color( color ) {}
  const std::string& Color() const { return m_color ; }
  virtual void Accept( PetVisitor& v ) = 0 ;
  friend class FamilyTreeVisitor ;
private:
  std::string m_color ;
} ;

template<typename Derived>
class Visitable : public Pet {
  public:
  using Pet::Pet ;
  void Accept( PetVisitor& v ) override {
    v.Visit( static_cast<Derived*>( this ) ) ;
  }
} ;


class Cat : public Visitable<Cat> {
  using Visitable<Cat>::Visitable ;
} ;

class Dog : public Visitable<Dog> {
  using Visitable<Dog>::Visitable ;
} ;

class ColorVisitor : public PetVisitor {
  public:
  ColorVisitor() {}

  void Visit( Cat* c ) override {
    std::cout << "cat: " << c->Color() <<std::endl ;
  }
  void Visit( Dog* d ) override {
    std::cout << "dog" << d->Color() <<std::endl ;
  }

  private:
} ;

void Walk( Pet& p ) {
  auto v( lambda_visitor<PetVisitor>(
    []( Dog* d ) { // Dogγ¨Catγ?ι ηͺγ―δΈγ¨εγγγεΏθ¦γγγγ
      std::cout << d->Color() << std::endl ;
    },
    []( Cat* c ) {
      std::cout << c->Color() << std::endl ;
    }
  ) );
  p.Accept( v ) ;
}

}

int main() {
  Cat c( "red" ) ;
  ColorVisitor visitor ;
  c.Accept( visitor ) ;

  Walk( c ) ;
  
}