#include <string>

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

// using PetVisitor = Visitor<class Dog, class Cat> ; // 上と同じ

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

  void Visit( Cat* c ) override {};
  void Visit( Dog* d ) override {};

  private:
} ;

}

int main() {
  Cat c( "red" ) ;
  ColorVisitor visitor ;
  c.Accept( visitor ) ;

}