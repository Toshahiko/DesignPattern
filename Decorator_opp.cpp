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

template<class U>
class VeteranUnit : public U {
public:
  VeteranUnit( U&& unit, double strength_bonus, double armor_bonus )
    : U( unit ),
      m_strength_bonus( strength_bonus ),
      m_armor_bonus( armor_bonus ) {}

  double attack() override {
    return U::attack() + m_strength_bonus ;
  }

  double defense() override {
    return U::defense() + m_armor_bonus ;
  }
private :
  double m_strength_bonus ;
  double m_armor_bonus ;
} ;

} // anonymous

int main() {
  Knight akatuki( 1, 2 ) ;
  Ogre ogre( 3, 5 ) ;
  akatuki.charge() ;
  VeteranUnit<Knight> aka( std::move( akatuki ), 4, 2 ) ;
  aka.charge() ;
  VeteranUnit<Ogre> o( std::move( ogre) , 3, 1 ) ;

  Knight hakken( 2, 3 ) ;
  // VeteranUnit<Knight> hak( hakken, 4 , 2 ) ; // copyは不可
  std::cout << std::boolalpha ;
  std::cout << aka.hit( o ) ;
}