#include "Memento.h"
#include "smartmemory.h"
#include <random>
#include <vector>
#include <string>
#include <assert.h>
#include <iostream>

class Fruit{
  public:
  enum class EnKind{
    Apple,
    Orange,
  } ;
  Fruit( const EnKind& kind) : m_kind( kind )
  {}
  private:
  EnKind m_kind ;
 } ;

namespace {
  class Person{
    public:
    explicit Person() : m_money( 1000 ) {
      std::random_device seed ;
      m_engine.seed( seed() ) ;
    }

    int Diceroll() {
      std::uniform_int_distribution<> dist( 1, 6 ) ;
      return dist( m_engine ) ;
    }
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
      m_fruit.push_back( fu)
    }
    private:
    std::mt19937_64 m_engine ;
    int m_money ;
    std::vector<Fruit> m_fruit ;
  } ;
}

void MementoPattern() {
  Person mara ;
  // while( true ) {
  for ( int i = 0 ; i < 100 ; ++i ) {
    auto dice = mara.Diceroll();

    if (dice <= 3) {
      mara.LoseMoney();
    } else if ( dice <= 5 ) {
      mara.EarnMoney() ;
      std::cout << "mara yatta6" << std::endl ;
    } else if ( dice == 6 ) {
      mara.TwiceMoney() ;
    } else {
     assert(!"不正なダイス") ;
    }
  }
}


int main() {
  MementoPattern() ;

  return 0 ;
}