#include "StdUtil.h"

namespace {

class Cat ;
class Dog ;

class PetVisitor {
  public:
  virtual ~PetVisitor() = default ;
} ;

class VisitorBase {
public:
  virtual ~VisitorBase() = default ;
} ;

template<typename Visitable>
class Visitor {
public:
  virtual void Visit( Visitable* v ) = 0 ;
} ;

class Pet {
public:
  virtual ~Pet() = default ;
  Pet( const std::string& color ) : m_color( color ) {}
  const std::string& Color() const { return m_color ; }
  virtual void Accept( VisitorBase& v ) = 0 ;
  void AddChild( Pet* p ) { m_children.push_back( p ) ; }
  friend class FamilyTreeVisitor ;
private:
  std::string m_color ;
  std::vector<Pet*> m_children ;
} ;

template<typename Visitable>
class PetVisitable : public Pet {
public:
  using Pet::Pet ;
  void Accept( VisitorBase& v ) override {
    if ( Visitor<Visitable>* pv = dynamic_cast<Visitor<Visitable>*>( &v ) ) {
      pv->Visit( static_cast<Visitable*>( this ) ) ;
    } else {
      assert( false ) ;
    }
  }
} ;



class Cat : public PetVisitable<Cat> {
  using PetVisitable<Cat>::PetVisitable ;
  public:

} ;

class Dog : public PetVisitable<Dog> {
  using PetVisitable<Dog>::PetVisitable ;
  public:

} ;


class FamilyTreeVisitor : public VisitorBase, public Visitor<Cat>, public Visitor<Dog> {
  public:
  FamilyTreeVisitor() : m_child_count( 0 ) {}
  void Reset() { m_child_count = 0 ; }
  size_t ChildCount() const { return m_child_count ; }

  virtual void Visit( Cat* c ) override { ++m_child_count ; };
  virtual void Visit( Dog* d ) override {};

  private:
  size_t m_child_count ;
} ;

} // anonymous namespace

int main() {
  std::unique_ptr<Pet> c = std::make_unique<Cat>( "orange" ) ;
  auto fv = std::make_unique<FamilyTreeVisitor>() ;
  // Pet* cat = new Cat( "orange " ) ;
  // FamilyTreeVisitor fv ;
  c->Accept( *fv ) ;
  std::cout << fv->ChildCount() << " kitten total " << std::endl ;
}