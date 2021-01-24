#include <string>
#include <iostream>
#include <memory>

namespace {
class PetVisitor ;

class Pet {
public:
  virtual ~Pet() = default ;
  Pet( const std::string& color ) : m_color( color ) {}
  const std::string& Color() const { return m_color ; }
  virtual void Accept( PetVisitor& v ) = 0 ;
private:
  std::string m_color ;
} ;

class Cat ;
class Dog ;

class PetVisitor {
  public:
  virtual ~PetVisitor() = default ;
  virtual void Visit( Cat* c ) = 0 ;
  virtual void Visit( Dog* d ) = 0 ;
} ;

class Cat : public Pet {
  using Pet::Pet ;
  public:
  void Accept( PetVisitor& v ) override { v.Visit( this ) ; }
} ;

class Dog : public Pet {
  using Pet::Pet ;
  public:
  void Accept( PetVisitor& v ) override { v.Visit( this ) ; }
} ;

class FeedingVisitor : public PetVisitor {
  public:
  void Visit( Cat* c) override {
    std::cout << "Feed tuna to the " << c->Color() << " cat " << std::endl ;
  }
  void Visit( Dog* d) override {
    std::cout << "Feed steak to the " << d->Color() << " dog " << std::endl ;
  }

} ;

class PlayingVisitor : public PetVisitor {
  public:
  void Visit( Cat* c) override {
    std::cout << "Play with feather with the " << c->Color() << " cat " << std::endl ;
  }
  void Visit( Dog* d) override {
    std::cout << "Play fetch with the " << d->Color() << " dog " << std::endl ;
  }

} ;

} // anonymous namespace

int main() {
  std::unique_ptr<Pet> c = std::make_unique<Cat>( "orange" ) ;
  std::unique_ptr<PetVisitor> fv = std::make_unique<FeedingVisitor>() ;
  c->Accept( *fv ) ;
}