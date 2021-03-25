#pragma once
#include "SingletonBase.h"
#include <memory>

namespace App {
using R = uint8_t ;
using G = uint8_t ;
using B = uint8_t ;
class SettingManager : public Design::SingletonBase<SettingManager>
{
private:
  SettingManager() ;
  friend class Design::SingletonBase<SettingManager> ;
public:
  void Display() ;
  void SetBGColor( R red, G green, B blue ) ;
private:
  struct Impl ;
  std::unique_ptr<Impl> m_pImpl ;
} ;

}