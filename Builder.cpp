#include "StdUtil.h"

namespace {

}

class Builder ;

SMART_PTR_IMPL( Builder )

class Builder {
  public:
  virtual void MakeTitle( const std::string& title ) = 0 ;
  virtual void MakeString( const std::string& str ) = 0 ;
  virtual void MakeItem( const std::string& item ) = 0 ;
  virtual void Close() = 0 ;
} ;

class TextBuilder : public Builder {
  public:
  void MakeTitle( const std::string& title ) override {
    m_ss << "Title: " << title << std::endl ;
  }
  void MakeString( const std::string& str ) override {
    m_ss << "    " << str << std::endl ;
  }
  void MakeItem( const std::string& item ) override {
    m_ss << "  Item: " << item << std::endl ;
  }
  void Close() override {

  }
  std::string GetResult() const {
    return m_ss.str() ;
  }
  private:
  std::stringstream m_ss ;
} ;

class HTMLBuilder : public Builder {
  public:
  void MakeTitle( const std::string& title ) override {
    m_ss << "<html><head><title>" + title + "</title></head><body>" << std::endl ;
    m_ss << "<h1>" + title + "</h1>" << std::endl ;
  }
  void MakeString( const std::string& str ) override {
    m_ss << "<p>" + str + "</p>" << std::endl ;
  }
  void MakeItem( const std::string& item ) override {
    m_ss << "<h2>" + item + "</h2>" << std::endl ;
  }
  void Close() override {
    m_ss << "</body></html>" ;
  }
  std::string GetResult() const {
    return m_ss.str() ;
  }
  private:
  std::stringstream m_ss ;
} ;

class Director {
  public:
  explicit Director( const Builder_sp& builder )
  : m_pBuilder( builder ) {}
  void Construct() {
    m_pBuilder->MakeTitle("金太の大冒険") ;
    m_pBuilder->MakeItem("一章：金太生まれる") ;
    m_pBuilder->MakeString("昔昔、金太が生まれました。") ;
    m_pBuilder->Close() ;
  }
  private:
  Builder_sp m_pBuilder ;
} ;

int main() {
  auto textBuilder = std::make_shared<TextBuilder>() ;
  Director director( textBuilder ) ;
  director.Construct() ;
  auto result = textBuilder->GetResult() ;
  std::cout << result ;

  auto htmlBuilder = std::make_shared<HTMLBuilder>() ;
  Director htmldirector( htmlBuilder ) ;
  htmldirector.Construct() ;
  result = htmlBuilder->GetResult() ;
  std::cout << result ;
}