#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

namespace {
  class Item {
    public:
    virtual ~Item() = default ;
    Item( const std::string& caption ) : m_caption( caption ) {}

    virtual std::string MakeHTML() const = 0 ;
    protected:
    std::string m_caption ;
  } ;

  class Link : public Item {
    public:
    Link( const std::string& caption, const std::string& url ) : Item( caption ), m_url( url )
    {}
    virtual ~Link() = default ;
    protected:
    std::string m_url ;
  } ;

  class Tray : public Item {
    public:
    Tray( const std::string& caption ) : Item( caption ) {}
    virtual ~Tray() = default ;
    void Add( Item* item ) {
      m_tray.push_back( item ) ;
    }

    protected:
    std::vector<Item*> m_tray ;
  } ;

  class Page {
    public:
    Page( const std::string& title, const std::string& author )
    : m_title( title ), m_author( author ) {}

    virtual ~Page() = default ;
    void Add( Item* item ) {
      m_items.push_back( item ) ;
    }

    virtual std::string MakeHTML() const = 0 ;

    void Output() {
      std::string filename = m_title + ".html" ;
      std::ofstream ofs( filename.c_str() ) ;
      ofs << MakeHTML() ;
      std::cout << filename + "を作成しました" << std::endl ;
    }
    protected:
    std::vector<Item*> m_items ;
    std::string m_title ;
    std::string m_author ;
  } ;

  class ListLink : public Link {
    public:
    ListLink( const std::string& caption, const std::string& url )
      : Link( caption, url ) {}

    std::string MakeHTML() const final {
      return " <li><a href=¥" + m_url + "¥>" + m_caption + "</a></li>\n" ;
    }
  } ;

  class ListTray : public Tray {
    public:
    ListTray( const std::string& caption ) : Tray( caption ) {}
    std::string MakeHTML() const final {
      std::stringstream ss ;
      ss << "<li>\n" ;
      ss << m_caption << "\n" ;
      ss << "<ul>\n" ;
      std::for_each( m_tray.begin(), m_tray.end(),
                      [ &ss ]( const auto & item ) { ss << item->MakeHTML() ; } ) ;
      ss << "</ul>\n" ;
      ss << "</li>\n" ;

      return ss.str() ;
    }
  } ;

  class ListPage : public Page {
    public:
    ListPage( const std::string& caption, const std::string& author )
    : Page( caption, author ) {}
    std::string MakeHTML() const final {
      std::stringstream ss ;
      ss << "<html><head><title>" << m_title << "</title></head>\n" ;
      ss << "<body>\n" ;
      ss << "<h1>" << m_title << "</h1>" ;
      ss << "<ul>\n" ;

      std::for_each( m_items.begin(), m_items.end(),
                     [ &ss ]( const auto& item ) { ss << item->MakeHTML() ; } ) ;
      ss << "</ul>\n" ;
      ss << "<hr><address>" << m_author << "</address>" ;
      ss << "</body></html>\n" ;

      return ss.str() ;
    }

  } ;

  class Factory {
    public:
    template<class FactoryImpl>
    static Factory* GetFactory() ;

    virtual Link* CreateLink( const std::string& caption, const std::string& url ) const = 0 ;
    virtual Tray* CreateTray( const std::string& caption ) const = 0 ;
    virtual Page* CreatePage( const std::string& title, const std::string& author ) const = 0 ;
  } ;

  class ListFactory : public Factory {
    public:
    Link* CreateLink( const std::string& caption, const std::string& url ) const final {
      return new ListLink( caption, url ) ;
    }

    Tray* CreateTray( const std::string& caption ) const final {
      return new ListTray( caption ) ;
    }

    Page* CreatePage( const std::string& title, const std::string& author ) const final {
      return new ListPage( title, author ) ;
    }
  } ;
  template<class FactoryImpl>
  Factory* Factory::GetFactory() {
    static FactoryImpl factory ;
    return &factory ;
  }

  // class TabFactory final : public Factory {

  // } ;
} // anonymous namespace

int main() {
  const auto factory = Factory::GetFactory<ListFactory>() ;
  const auto asahi = factory->CreateLink( "朝日新聞", "http://www.asahi.com/" ) ;
  const auto yomiuri = factory->CreateLink( "読売新聞", "http://www.yomiuri.com/" ) ;
  const auto us_yahoo = factory->CreateLink( "Yahoo!", "http://www.yahoo.com/" ) ;

  const auto jp_yahoo = factory->CreateLink( "Yahoo!Japan", "http://www.yahoo.co.jp" ) ;
  const auto excite = factory->CreateLink( "Excite", "http://www.excite.com/" ) ;
  const auto google = factory->CreateLink( "Google", "http://www.google.com/" ) ;

  const auto traynews = factory->CreateTray( "新聞" ) ;
  traynews->Add( asahi ) ;
  traynews->Add( yomiuri ) ;

  const auto trayyahoo = factory->CreateTray( "yahoo!" ) ;
  trayyahoo->Add( us_yahoo ) ;
  trayyahoo->Add( jp_yahoo ) ;

  const auto traysearch = factory->CreateTray( "サーチエンジン" ) ;
  traysearch->Add( trayyahoo ) ;
  traysearch->Add( excite ) ;
  traysearch->Add( google ) ;

  const auto page = factory->CreatePage( "LinkPage", "谷本和彦" ) ;
  page->Add( traynews ) ;
  page->Add( trayyahoo ) ;
  page->Add( traysearch ) ;

  page->Output() ;
}