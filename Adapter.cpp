#include "StdUtil.h"

class Banner {
public:
  explicit Banner( const std::string& message ) : m_message( message ) {}
  void ShowWithParen() {
    std::cout << '(' + m_message + ')' << std::endl ;
  }
  
  void ShowWithAster() {
    std::cout << "*" + m_message + "*" << std::endl ; 
  }

private:
  std::string m_message ;
} ;

class Print {
public:
  virtual void PrintWeak() = 0 ;
  virtual void PrintStrong() = 0 ;
} ;

class PrintBanner : private Banner, public Print {
public:
  PrintBanner( const std::string& message ) : Banner( message ) {}
  void PrintWeak() override {
    Banner::ShowWithParen() ;
  }

  void PrintStrong() override {
    Banner::ShowWithAster() ;
  }
} ;

int main() {

  // Banner banner( "hello" ) ;
  // banner.ShowWithAster() ;
  // banner.ShowWithParen() ;

  std::unique_ptr<Print> print = std::make_unique<PrintBanner>("hello") ;

  print->PrintWeak() ;
  print->PrintStrong() ;
}