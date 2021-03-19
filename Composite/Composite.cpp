#include "StdUtil.h"
#include "smartmemory.h"
class Entry ;
SMART_PTR_IMPL( Entry )

class Entry {
public:
  virtual std::string GetName() const noexcept = 0 ;
  virtual int GetSize() const noexcept = 0 ;
  virtual std::stringstream PrintList() const = 0 ;
  virtual void Add( const Entry_sp& entry ) {}
} ;

class File : public Entry {
public:
  File( const std::string& name, const int size )
  : m_name( name ), m_size( size ) {}

  std::string GetName() const noexcept override {
    return m_name ;
  }

  int GetSize() const noexcept override {
    return m_size ;
  }
  std::stringstream PrintList() const {
    std::stringstream list ;
    list << " " << GetName() << " (" << GetSize() << ")" << std::endl ;
    return list ;
  }
private:
  std::string m_name ;
  int m_size ;
} ;
SMART_PTR_IMPL( File )

class Directory : public Entry {
public:
  Directory( const std::string& name )
  : m_name( name ) {}

  std::string GetName() const noexcept override {
    return m_name ;
  }

  int GetSize() const noexcept override {
    return std::accumulate( m_entries.cbegin(), m_entries.cend(), 0,
                            []( int res, const Entry_sp& entry ) { return entry->GetSize() + res ; } ) ;
  }

  void Add( const Entry_sp& entry ) override {
    if ( entry.get() == this ) {
      std::cout << "error" << std::endl ;
      return ;
    }
    auto it = std::find( m_entries.cbegin(), m_entries.cend(), entry ) ;
    if ( it != m_entries.cend() ) {
      std::cout << "error" << std::endl ;
      return ;
    }
    m_entries.push_back( entry ) ;
  }
  std::stringstream PrintList() const {
    std::stringstream list ;
    list << GetName() << " (" << GetSize() << ")" << std::endl ;
    std::for_each( m_entries.cbegin(), m_entries.cend(),
                  [&list]( const Entry_sp& entry){ list << " " << entry->PrintList().str() ; }) ;
    return list ;
  }
private:
  std::string m_name ;
  std::vector<Entry_sp> m_entries ;
} ;

SMART_PTR_IMPL( Directory )
int main() {
  Directory_sp simsim = std::make_shared<Directory>( "simsim" ) ;
  auto fai = std::make_shared<File>( "fai", 5 ) ;
  auto el = std::make_shared<File>( "el", 5 ) ;
  simsim->Add( fai ) ;
  simsim->Add( el ) ;
  auto ohon = std::make_shared<Directory>( "ohon" ) ;
  auto goo  = std::make_shared<Directory>( "goo" ) ;
  ohon->Add( simsim ) ;
  ohon->Add( goo ) ;
  std::cout << ohon->GetSize() ;
  std::cout << ohon->PrintList().str() ;
  
}