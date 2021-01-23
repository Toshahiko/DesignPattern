#include <iostream>


namespace {
class Unit {
public:
  Unit( double strength, double armor )
  : m_strength( strength ), m_armor( armor ) {}

  virtual bool hit( Unit& target ) {
    return attack() > target.defense() ;
  }

  virtual double attack() = 0 ;
  virtual double defense() = 0 ;

protected:
  double m_strength ;
  double m_armor ;
} ;

class Knight : public Unit {
public:
  Knight( double strength, double armor )
    : Unit( strength, armor ), m_charge( 0 ) {}
  double attack() override {
    return m_strength + m_sword_bonus + m_charge ;
  }

  double defense() override {
    return m_armor + m_plate_bonus ;
  }
  void charge() {
    m_charge = 1 ;
  }
protected:
  double m_charge ;
  static constexpr double m_sword_bonus = 2.0 ;
  static constexpr double m_plate_bonus = 3.0 ;
} ;

class Ogre : public Unit {
public:
  using Unit::Unit ;
  double attack() override {
    return m_strength + m_club_penalty ;
  }

  double defense() override {
    return m_armor + m_leather_penalty ;
  }
protected:
  static constexpr double m_club_penalty = -1 ;
  static constexpr double m_leather_penalty = -1 ;
} ;

class VeteranUnit : public Unit {
public:
  VeteranUnit( Unit& unit, double strength, double armor )
    : Unit( strength, armor ), m_unit( unit ) {}

  double attack() override {
    return m_unit.attack() + m_strength ;
  }

  double defense() override {
    return m_unit.defense() + m_armor ;
  }
private :
  Unit& m_unit ;
} ;

} // anonymous

int main() {
  Knight akatuki( 1, 2 ) ;
  Ogre ogre( 3, 5 ) ;
  akatuki.charge() ;
  VeteranUnit aka( akatuki, 4, 2 ) ;
  // VeteranUnit.charge() ; // できない
  VeteranUnit o( ogre, 3, 1 ) ;
  std::cout << std::boolalpha ;
  std::cout << aka.hit( o ) ;
}