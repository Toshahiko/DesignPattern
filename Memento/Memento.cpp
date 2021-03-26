#include "Memento.h"
#include "../smartmemory.h"
#include "../StdUtil.h"

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
    class Emotion{
      public:
      virtual void Voice() const = 0 ;
      virtual ~Emotion() = default ;
    } ;
    class NegativeEmotion : public Emotion {
      void Voice() const override {
        std::cout << "dousiyoumonai" << std::endl ;
      }
    } ;

    class PositiveEmotion : public Emotion {
      void Voice() const override {
        std::cout << "yatta" << std::endl ;
      }
    } ;

    class NeutralEmotion : public Emotion {
      void Voice() const override {
        std::cout << "maramara" << std::endl ;
      }
    } ;

    public:
    explicit Person() : m_money( 0 ), m_emotion( std::make_unique<NeutralEmotion>() ) {
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
      m_emotion = std::make_unique<NegativeEmotion>() ;
    }
    void EarnMoney() {
      m_money += 100 ;
      m_emotion = std::make_unique<NeutralEmotion>() ;
    }
    void TwiceMoney(){
      m_money *= 2 ;
      m_emotion = std::make_unique<PositiveEmotion>() ;
    }
    void AquireFruit( const Fruit& fruit ) {
      m_fruit.push_back( fruit ) ;
    }
    
    void Voice() const {
      m_emotion->Voice() ;
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
    std::unique_ptr<Emotion> m_emotion ;
  } ;
}

void MementoPattern() {
  Person buhin ;
  std::vector<PersonMemento> buhin_part ;
  buhin_part.push_back( buhin.CreateMemento() ) ;
  for ( int i = 0 ; i < 100 ; ++i ) {
    auto dice = buhin.Diceroll() ;
    if (dice <= 3) {
      buhin.LoseMoney() ;
      buhin.Voice() ;
    } else if ( dice <= 5 ) { // aaa
      buhin.EarnMoney() ;
      buhin.Voice() ;
    } else if ( dice == 6 ) {
      buhin.TwiceMoney() ;
      buhin.Voice() ;
    } else {
      assert(!"不正なダイス") ;
    }
    if ( buhin.GetMoney() > 0 ) {
      buhin_part.push_back( buhin.CreateMemento() ) ;
    } else {
      buhin.RestoreMemento( *buhin_part.cend() ) ;
      std::cout << "kurushii" << std::endl ;
    }
  }

}


int main() {
  MementoPattern() ;

  return 0 ;
}