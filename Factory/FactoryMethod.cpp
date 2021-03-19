#include "StdUtil.h"

namespace Framework {

class Product {
public:
  virtual void Use() = 0 ;
} ;

class Factory {
public:
  std::shared_ptr<Product> Create( const std::string& owner ) {
    std::shared_ptr<Product> product = CreateProduct( owner ) ;
    RegisterProduct( product ) ;
    return product ;
  }
private:
  virtual std::unique_ptr<Product> CreateProduct( const std::string& owner ) = 0 ;

  virtual void RegisterProduct( const std::shared_ptr<Product>& product ) = 0 ;
} ;

} // Framework

namespace IDCard {

class IDCard : public Framework::Product {
public:
  explicit IDCard( const std::string& owner ) : m_owner( owner ) {
  }
  std::string GetOwner() {
    return m_owner ;
  }
  void Use() override {
    std::cout << m_owner << std::endl ;
  }
private:
  std::string m_owner ;
} ;

class IDCardFactory : public Framework::Factory {
private:
  std::unique_ptr<Framework::Product> CreateProduct( const std::string& owner ) override {
    return std::make_unique<IDCard>( owner ) ;
  }

  void RegisterProduct( const std::shared_ptr<Framework::Product>& product ) override {
    owners.push_back( std::dynamic_pointer_cast<IDCard>( product )->GetOwner() ) ;
  }
private:
  std::vector<std::string> owners ;
} ;

} // IDCard

int main() {
  IDCard::IDCardFactory idCardfactory ;
  auto idCard = idCardfactory.Create( "buhin" ) ;
  idCard->Use() ;
}