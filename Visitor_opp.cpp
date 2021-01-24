#include <string>
#include <iostream>
#include <memory>
#include <vector>

namespace {
class PetVisitor ;

class Pet {
public:
  virtual ~Pet() = default ;
  Pet( const std::string& color ) : m_color( color ) {}
  const std::string& Color() const { return m_color ; }
  virtual void Accept( PetVisitor& v, Pet* p = nullptr ) = 0 ;
  void AddChild( Pet* p ) { m_children.push_back( p ) ; }
  friend class FamilyTreeVisitor ;
private:
  std::string m_color ;
  std::vector<Pet*> m_children ;
} ;

class Cat ;
class Dog ;

class PetVisitor {
  public:
  virtual ~PetVisitor() = default ;
  virtual void Visit( Cat* c, Pet* p ) = 0 ;
  virtual void Visit( Dog* d, Pet* p ) = 0 ;
} ;

class Cat : public Pet {
  using Pet::Pet ;
  public:
  void Accept( PetVisitor& v, Pet* p = nullptr ) override { v.Visit( this, p ) ; }
} ;

class Dog : public Pet {
  using Pet::Pet ;
  public:
  void Accept( PetVisitor& v, Pet* p = nullptr ) override { v.Visit( this, p ) ; }
} ;

class BirthVisitor : public PetVisitor {
  public:
  void Visit( Cat* c, Pet* p ) override {
    assert( dynamic_cast<Cat*>( p ) ) ;
    c->AddChild( p ) ;
  }
  void Visit( Dog* d, Pet* p ) override {
    assert( dynamic_cast<Dog*>( p ) ) ;
    d->AddChild( p ) ;
  }

} ;

class FamilyTreeVisitor : public PetVisitor {
  public:
  void Visit( Cat* c, Pet* ) override {

    std::cout << "Kittens: " ;
    std::for_each( c->m_children.begin(), c->m_children.end(), [] ( auto kitten ){ std::cout << kitten->Color() << "" ; } ) ;
    std::cout << std::endl ;
  }
  void Visit( Dog* d, Pet* ) override {
    std::cout << "Puppies: " ;
    std::for_each( d->m_children.begin(), d->m_children.end(), [] ( auto puppy ) { std::cout << puppy->Color() << "" ; } ) ;
    std::cout << std::endl ;
  }

} ;

} // anonymous namespace

int main() {
  std::unique_ptr<Pet> c = std::make_unique<Cat>( "orange" ) ;
  std::unique_ptr<PetVisitor> fv = std::make_unique<BirthVisitor>() ;
  c->Accept( *fv ) ;
}