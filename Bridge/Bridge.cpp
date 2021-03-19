#include "StdUtil.h"

class DisplayImpl {
  public:
  virtual void RawOpen() = 0 ;
  virtual void RawPrint() = 0 ;
  virtual void RawClose() = 0 ;
} ;

class StringDisplayImpl : public DisplayImpl {
  public:
  StringDisplayImpl( const std::string& str )
  : m_str( str ), m_width( str.length() ) {}
  void RawOpen() override {
    PrintLine() ;
  }

  void RawPrint() override {
    std::cout << "|" + m_str + "|" << std::endl ;
  }

  void RawClose() override {
    PrintLine() ;
  }

  private:
  void PrintLine() {
    std::stringstream ss ;
    ss << "*" ;
    for ( size_t i = 0 ; i < m_width ; ++i ) {
      ss << "-" ;
    }
    ss << "*" << std::endl ;

    std::cout << ss.str() ;
  }

  private:
  std::string m_str ;
  size_t m_width ;
} ;

class Display {
  public:
  Display( std::unique_ptr<DisplayImpl>&& impl )
  : m_pImpl( std::move( impl ) ) {}
  void Draw() {
    Open() ;
    Print() ;
    Close() ;
  }
  protected:
  void Open() {
    m_pImpl->RawOpen() ;
  }
  void Print() {
    m_pImpl->RawPrint() ;
  }
  void Close() {
    m_pImpl->RawClose() ;
  }
private:
  std::unique_ptr<DisplayImpl> m_pImpl ;
} ;

class CountDisplay : public Display {
  public:
  CountDisplay( std::unique_ptr<DisplayImpl>&& impl )
  : Display( std::move( impl ) ) {}
  void MultiDisplay( int times ) {
    Open() ;
    for ( int i = 0 ; i < times ; i++ ) {
      Print() ;
    }
    Close() ;
  }

} ;

int main() {
  Display display( std::make_unique<StringDisplayImpl>("Hello") ) ;
  display.Draw() ;

  CountDisplay multiDisplay( std::make_unique<StringDisplayImpl>("Hey") ) ;
  multiDisplay.MultiDisplay( 4 ) ;
}