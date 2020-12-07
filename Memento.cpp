#include "Memento.h"
#include "smartmemory.h"
#include <random>
#include <vector>
#include <string>
#include <assert.h>
#include <iostream>
#include <algorithm>

class Fruit{
  public:
  enum class EnKind{
    Apple,
    Orange,
  } ;
  Fruit( const EnKind& kind ) : m_kind( kind )
  {}
  private:
  EnKind m_kind ;
 } ;


namespace {
class Person ;
class PersonMemento {
public:
  int GetMoney() const {
    return m_money ;
  }
  private:
  PersonMemento( int money ) : m_money( money ) {}

  friend Person ;
  private:
  int m_money ;
};
  class Person{
    public:
    explicit Person() : m_money( 0 ) {
      std::random_device seed ;
      m_engine.seed( seed() ) ;
    }

    int Diceroll() {
      std::uniform_int_distribution<> dist( 1, 6 ) ;
      return dist( m_engine ) ;
    }
    int GetMoney() const { return m_money ; }
    void LoseMoney() {
      m_money -= 100 ;
    }
    void EarnMoney() {
      m_money += 100 ;
    }
    void TwiceMoney(){
      m_money *= 2 ;
    }
    void AquireFruit( const Fruit& fruit ) {
      m_fruit.push_back( fruit ) ;
    }
    
    PersonMemento CreateMemento() {
      return PersonMemento( m_money ) ;
    }

    void RestoreMemento( const PersonMemento& memento ) {
      m_money = memento.m_money ;
    }
    private:
    std::mt19937_64 m_engine ;
    int m_money ;
    std::vector<Fruit> m_fruit ;
  } ;
}

void MementoPattern() {
  Person mara ;
  std::vector<PersonMemento> mara_part ;
  mara_part.push_back( mara.CreateMemento() ) ;
  for ( int i = 0 ; i < 100 ; ++i ) {
    auto dice = mara.Diceroll() ;
    if (dice <= 3) {
      mara.LoseMoney() ;
    } else if ( dice <= 5 ) { // aaa
      mara.EarnMoney() ;
      std::cout << "mara yatta6" << std::endl ;
    } else if ( dice == 6 ) {
      mara.TwiceMoney() ;
    } else {
      assert(!"不正なダイス") ;
    }
    if ( mara.GetMoney() > 0 ) {
      mara_part.push_back( mara.CreateMemento() ) ;
    } else {
      mara.RestoreMemento( *mara_part.cbegin() ) ;
      std::cout << "kurushii" << std::endl ;
    }
  }

}


int main() {
  MementoPattern() ;

  return 0 ;
}