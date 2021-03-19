#include <iostream>
#include <vector>

namespace {
class ControlBase {
  public:
  virtual void Draw() = 0 ;
  virtual ~ControlBase() = default ;
} ;

template<class T>
class Control : public ControlBase {
  public:
  void Draw() override {
    Derived()->EraseBackground() ;
    Derived()->Paint() ;
  }

  private:
  T* Derived() { return static_cast<T*>( this ) ; }
} ;

class Button : public Control<Button> {
  private :
  friend class Control<Button> ;
  void EraseBackground() {
    std::cout << "Erase Background" << std::endl ;
  } ;
  void Paint() {
    std::cout << "Paint Button" << std::endl ;
  } ;
} ;

class CheckBox : public Control<CheckBox> {
  private :
  friend class Control<CheckBox> ;
  void EraseBackground() {
    std::cout << "Erase Background" << std::endl ;
  } ;
  void Paint() {
    std::cout << "Paint CheckBox" << std::endl ;
  } ;
} ;

}

int main() {
  Button bn ;
  CheckBox cb ;
  std::vector<ControlBase*> controls ;
  controls.push_back( &bn ) ;
  controls.push_back( &cb ) ;
  for( const auto& c : controls ) {
    c->Draw() ;
  }
}