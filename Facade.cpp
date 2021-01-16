#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "boost/tokenizer.hpp"

namespace {

using tokenizer_separator = boost::tokenizer<boost::escaped_list_separator<char>> ;
using Property = std::pair< std::string, std::string> ;

class Database {
  public:
  std::vector<Property> GetProperties( const std::string& filename ) const {
    if ( !std::filesystem::exists( filename ) ) throw std::runtime_error("") ;

    std::ifstream ifs( filename ) ;
    std::string header ;
    std::getline( ifs, header ) ;
    std::vector<Property> properties ;
    std::string line ;
    while( std::getline( ifs, line ) ) {
      tokenizer_separator token( line ) ;
      auto it = token.begin() ;
      const auto adress = *it ;
      const auto name = *( ++it ) ;
      properties.emplace_back( adress, name ) ;
    }
    return properties ;
  }
  // Database( const char* path ) : m_pathCSV( path ) {}

  private:
  std::filesystem::path m_pathCSV ;
} ;

class HtmlWriter {
  public:
  explicit HtmlWriter( const std::filesystem::path& path ) : m_writer( path ) {}
  ~HtmlWriter() {
    Close() ;
  }

  void Write( const std::string& str ) {
    m_writer.Write( str ) ;
  }

  public:
  void Title( const std::string& title ) {
    m_writer.Write( "<html>" ) ;
    m_writer.Write( "<head>" ) ;
    m_writer.Write( "<title>" + title + "</title>" ) ;
    m_writer.Write( "</head>" ) ;
    m_writer.Write( "<body>\n" ) ;
    m_writer.Write( "<h1>" + title + "</h1>\n" ) ;
  }

  void Paragraph( const std::string& str ) {
    m_writer.Write( "<p>" + str + "</p>\n" ) ;
  }

  void Link( const std::string& href, const std::string& caption ) {
    m_writer.Write( "<a href=¥" + href + "¥>" + caption + "</a>" ) ;
  }

  void Mailto( const std::string& mailAdress, const std::string& name ) {
    Link( "mailto :" + mailAdress, name ) ;
  }

  private:
  void Close() {
    m_writer.Write( "</head>" ) ;
    m_writer.Write( "</html>" ) ;
  }
  class Writer {
    public:
    Writer( const std::filesystem::path& path ) : m_path( path ){
      m_file.open( m_path ) ;
      if ( !m_file ) {
        std::cout << "Can't open file " ;
      }
    }
    ~Writer(){
      m_file.close() ;
    }
    void Write( const std::string& str ) {
      m_file << str ;
    }

    private:
    std::ofstream m_file ;
    std::filesystem::path m_path ;
  } ;

  private:
  Writer m_writer ;
} ;

class PageMaker {
public:
  static PageMaker* Instance() {
    static PageMaker a ;
    return &a ;
  }
  void MakeWelcomePage( const std::string& emailAdress, const std::string& htmlName ) {
    Database db;
    try {
    const auto properties = db.GetProperties( "customer.csv" );
    const auto it = std::find_if(
        properties.begin(), properties.end(),
        [emailAdress]( const auto& property ) { return property.first == emailAdress ; } );
    const auto name = (*it).second ;

    HtmlWriter writer( htmlName ) ;

    writer.Title( "Welcome to " + name + " Page " ) ;
    writer.Paragraph( name + "のページへようこそ" ) ;

    writer.Paragraph( "メールを待っています" ) ;
    writer.Link( emailAdress, "tanimoto" ) ;
    } catch( std::exception& e ) {
      std::cout << e.what() << std::endl ;
    }
  }

  PageMaker( const PageMaker& ) = delete ;
  PageMaker operator=( const PageMaker& ) = delete ;
private:
  PageMaker() = default ;
} ;

} // anonymous

int main() {
  PageMaker::Instance()->MakeWelcomePage( "tanimoto@gmail", "tanimoto.html" ) ;
}