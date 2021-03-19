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
  FamilyTreeVisitor() : m_child_count( 0 ) {}
  void Reset() { m_child_count = 0 ; }
  size_t ChildCount() const { return m_child_count ; }

  virtual void Visit( Cat* c, Pet* p ) override {};
  virtual void Visit( Dog* d, Pet* p ) override {};
  
  private:
  size_t m_child_count ;
} ;

class Shelter {
  public :
  void Add( Pet* p ) {
    m_pets.emplace_back( p ) ;
  }
  void Accept( PetVisitor& v ) {
    for ( auto & p : m_pets ) {
      p->Accept( v ) ;
    }
  }

  private :
  std::vector<std::unique_ptr<Pet>> m_pets ;
} ;

class Family {
  public:
  Family( const char* cat_color, const char* dog_color )
    : m_cat( cat_color ), m_dog( dog_color ) {}
  void Accept( PetVisitor& v ) {
    m_cat.Accept( v ) ;
    m_dog.Accept( v ) ;
  }
  private:
  Cat m_cat ;
  Dog m_dog ;
} ;

} // anonymous namespace

int main() {
  std::unique_ptr<Pet> c = std::make_unique<Cat>( "orange" ) ;
  auto fv = std::make_unique<FamilyTreeVisitor>() ;
  c->Accept( *fv ) ;
  std::cout << fv->ChildCount() << " kitten total " << std::endl ;
}