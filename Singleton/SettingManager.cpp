#include <iostream>
#include <memory>
#include "SettingManager.h"
#include <tuple>

namespace App {

using Color = std::tuple<R, G, B> ;
struct SettingManager::Impl {
  Color backGroundColor = std::make_tuple( 0, 0, 0 ) ;
} ;

SettingManager::SettingManager()
  : m_pImpl( std::make_unique<Impl>() ) {}

void SettingManager::Display() {
  std::cout << +std::get<0>( m_pImpl->backGroundColor )
            << +std::get<1>( m_pImpl->backGroundColor )
            << +std::get<2>( m_pImpl->backGroundColor )
            << std::endl ;
}

void SettingManager::SetBGColor( R red, G green, B blue ) {
  m_pImpl->backGroundColor = std::make_tuple( red, green, blue ) ;
}

} // namespace App

int main() {
  App::SettingManager::instance().SetBGColor( 9, 8, 6 ) ;
  App::SettingManager::instance().Display() ;

}