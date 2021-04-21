#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include "../smartmemory.h"

namespace {
  class Item {
    public:
    virtual ~Item() = default ;
    Item( const std::string& caption ) : m_caption( caption ) {}

    virtual std::string MakeHTML() const = 0 ;
    protected:
    std::string m_caption ;
  } ;
  SMART_PTR_IMPL( Item )

  class Link : public Item {
    public:
    Link( const std::string& caption, const std::string& url ) : Item( caption ), m_url( url )
    {}
    virtual ~Link() = default ;
    protected:
    std::string m_url ;
  } ;
  SMART_PTR_IMPL( Link )

  class Tray : public Item {
    public:
    Tray( const std::string& caption ) : Item( caption ) {}
    virtual ~Tray() = default ;
    void Add( const Item_sp& item ) {
      m_tray.push_back( item ) ;
    }

    protected:
    std::vector<Item_sp> m_tray ;
  } ;
  SMART_PTR_IMPL( Tray )

  class Page {
    public:
    Page( const std::string& title, const std::string& author )
    : m_title( title ), m_author( author ) {}

    virtual ~Page() = default ;
    void Add( const Item_sp& item ) {
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
    std::vector<Item_sp> m_items ;
    std::string m_title ;
    std::string m_author ;
  } ;
  SMART_PTR_IMPL( Page )

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

    virtual Link_up CreateLink( const std::string& caption, const std::string& url ) const = 0 ;
    virtual Tray_up CreateTray( const std::string& caption ) const = 0 ;
    virtual Page_up CreatePage( const std::string& title, const std::string& author ) const = 0 ;
  } ;

  class ListFactory : public Factory {
    public:
    Link_up CreateLink( const std::string& caption, const std::string& url ) const final {
      return std::make_unique<ListLink>( caption, url ) ;
    }

    Tray_up CreateTray( const std::string& caption ) const final {
      return std::make_unique<ListTray>( caption ) ;
    }

    Page_up CreatePage( const std::string& title, const std::string& author ) const final {
      return std::make_unique<ListPage>( title, author ) ;
    }
  } ;
  template<class FactoryImpl>
  Factory* Factory::GetFactory() {
    static FactoryImpl factory ;
    return &factory ;
  }

  class TableLink final : public Link {
    public:
    TableLink( const std::string& caption, const std::string& url )
      : Link( caption, url ) {}

    std::string MakeHTML() const final {
      return "<td><a href=¥" + m_url + "¥>" + m_caption + "</a></td>\n" ;
    }
  } ;

  class TableTray final : public Tray {
    public:
    TableTray( const std::string& caption ) : Tray( caption )
    {}

    std::string MakeHTML() const final {
      std::stringstream ss ;
      ss << "<td>" ;
      ss << "<table width=¥100¥ border=¥1¥><tr>" ;
      ss << "<td bgcolor =¥#cccccc¥ align=¥center¥ colspan=¥"
         << m_tray.size()
         << "¥><b>"
         << m_caption
         << "</b></td>" ;

      ss << "</tr>\n" ;
      ss << "<tr>\n" ;
      std::for_each( m_tray.cbegin(), m_tray.cend(),
        [ &ss ] ( const auto& item ) { ss << item->MakeHTML() ; } ) ;

      ss << "</tr></table>" ;
      ss << "</td>" ;
      return ss.str() ;
    }
  } ;

  class TablePage final : public Page {
    public:
    TablePage( const std::string& title, const std::string& author )
      : Page( title, author) {}

    std::string MakeHTML() const final {
      std::stringstream ss ;
      ss << "<html><head><title>" << m_title << "</title></head>\n" ;
      ss << "<body>\n" ;
      ss << "<h1>" << m_title << "</h1>\n" ;
      ss << "<table width=¥80¥ border=¥3¥>\n" ;
      std::for_each( m_items.begin(), m_items.end(),
        [ &ss ] ( const auto& item ) { ss << "<tr>" << item->MakeHTML() << "</tr>" ; } ) ;

      ss << "</table>\n" ;
      ss << "<hr><address>" << m_author << "</address>" ;
      ss << "</body></html>\n" ;
      return ss.str() ;
    }
  } ;

  class TableFactory final : public Factory {
    public:
    Link_up CreateLink( const std::string& caption, const std::string& url ) const final {
      return std::make_unique<TableLink>( caption, url ) ;
    }

    Tray_up CreateTray( const std::string& caption ) const final {
      return std::make_unique<TableTray>( caption ) ;
    }

    Page_up CreatePage( const std::string& title, const std::string& author ) const final {
      return std::make_unique<TablePage>( title, author ) ;
    }
  } ;
} // anonymous namespace

int main() {
  const auto factory = Factory::GetFactory<TableFactory>() ;
  const Link_sp asahi = factory->CreateLink( "朝日新聞", "http://www.asahi.com/" ) ;
  const Link_sp yomiuri = factory->CreateLink( "読売新聞", "http://www.yomiuri.com/" ) ;
  const Link_sp us_yahoo = factory->CreateLink( "Yahoo!", "http://www.yahoo.com/" ) ;

  const Link_sp jp_yahoo = factory->CreateLink( "Yahoo!Japan", "http://www.yahoo.co.jp" ) ;
  const Link_sp excite = factory->CreateLink( "Excite", "http://www.excite.com/" ) ;
  const Link_sp google = factory->CreateLink( "Google", "http://www.google.com/" ) ;

  const Tray_sp traynews = factory->CreateTray( "新聞" ) ;
  traynews->Add( asahi ) ;
  traynews->Add( yomiuri ) ;

  const Tray_sp trayyahoo = factory->CreateTray( "yahoo!" ) ;
  trayyahoo->Add( us_yahoo ) ;
  trayyahoo->Add( jp_yahoo ) ;

  const Tray_sp traysearch = factory->CreateTray( "サーチエンジン" ) ;
  traysearch->Add( trayyahoo ) ;
  traysearch->Add( excite ) ;
  traysearch->Add( google ) ;

  const auto page = factory->CreatePage( "TablePage", "谷本和彦" ) ;
  page->Add( traynews ) ;
  page->Add( trayyahoo ) ;
  page->Add( traysearch ) ;

  page->Output() ;
}