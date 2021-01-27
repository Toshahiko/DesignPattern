#include "StdUtil.h"

namespace {
class PetVisitor ;

class Pet {
public:
  virtual ~Pet() = default ;
  Pet( const std::string& color ) : m_color( color ) {}
  const std::string& Color() const { return m_color ; }
  virtual void Accept( PetVisitor& v ) = 0 ;
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
} ;

class CatVisitor {
public:
  virtual void Visit( Cat* c ) = 0 ;
} ;

class DogVisitor {
public:
  virtual void Visit( Dog* d ) = 0 ;
} ;


class Cat : public Pet {
  using Pet::Pet ;
  public:
  void Accept( PetVisitor& v ) override {
    if ( CatVisitor* cv = dynamic_cast<CatVisitor*>( &v ) ) {
      cv->Visit( this ) ;
    } else {
      assert( false ) ;
    }
  }
} ;

class Dog : public Pet {
  using Pet::Pet ;
  public:
  void Accept( PetVisitor& v ) override {
    if ( DogVisitor* dv = dynamic_cast<DogVisitor*>( &v ) ) {
      dv->Visit( this ) ;
    } else {
      assert( false ) ;
    }
  }
} ;

class BirthVisitor : public PetVisitor, public DogVisitor, public CatVisitor {
  public:
  void Visit( Cat* c ) override {

  }
  void Visit( Dog* d) override {
    std::cout << d->Color() << std::endl ;

  }

} ;

class FamilyTreeVisitor : public PetVisitor, public CatVisitor {
  public:
  FamilyTreeVisitor() : m_child_count( 0 ) {}
  void Reset() { m_child_count = 0 ; }
  size_t ChildCount() const { return m_child_count ; }

  virtual void Visit( Cat* c ) override { ++ m_child_count ; };
  // virtual void Visit( Dog* d ) override {}; DogVisitorを継承していないためエラー
  
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