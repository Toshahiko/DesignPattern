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

template <class To, class From>
std::unique_ptr<To> move_cast( std::unique_ptr<From>& p ) {
#ifndef NDEBUG
  auto p1 = std::unique_ptr<To>( dynamic_cast<To*>( p.release() ) ) ;
  assert( p1 ) ;
  return p1 ;
#else
  return std::unique_ptr<To>( static_cast<To*>( p.release() ) ) ;
#endif
}

template<class U>
class VeteranUnit : public U {
public:
template<typename P>
  VeteranUnit( P&& p, double strength_bonus, double armor_bonus )
    : U( std::move( *move_cast<U>( p ) ) ),
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

using Unit_ptr = std::unique_ptr<Unit> ;
using Knight_ptr = std::unique_ptr<Knight> ;
using Ogre_ptr = std::unique_ptr<Ogre> ;



} // anonymous

int main() {
  Knight_ptr akatuki( new Knight( 1, 2 ) ) ;
  Ogre_ptr ogre( new Ogre( 3, 5 ) ) ;
  akatuki->charge() ;
  Knight_ptr aka( std::make_unique<VeteranUnit<Knight>>( akatuki, 4, 2 ) ) ;
  aka->charge() ;
  Unit_ptr o( new VeteranUnit<Ogre>( ogre, 3, 1 ) ) ;
  // o->charge() ; // Kinghtではないのでできない。
  Knight hakken( 2, 3 ) ;
  // VeteranUnit<Knight> hak( hakken, 4 , 2 ) ; // copyは不可
  std::cout << std::boolalpha ;
  std::cout << aka->hit( *o ) ;
}