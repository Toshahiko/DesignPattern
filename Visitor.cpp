#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>

class Entry {
public:
  virtual std::string GetName() const noexcept = 0 ;
  virtual int GetSize() const noexcept = 0 ;
  virtual void Add( const std::shared_ptr<Entry>& entry ) {} ;
} ;

class File : public Entry {
public:
  File( const std::string& name, int size )
  : m_name( name ), m_size( size ) {}
  std::string GetName() const noexcept final { return m_name ; }
  int GetSize() const noexcept final { return m_size ; }
private:
  std::string m_name ;
  int m_size ;
} ;

class Directory : public Entry {
public:
  Directory( const std::string& name )
  : m_name( name ) {}

  std::string GetName() const noexcept final {
    return m_name ;
  }

  int GetSize() const noexcept final {
    return std::accumulate( m_entries.cbegin(), m_entries.cend(), 0,
                            []( int result, const std::shared_ptr<Entry>& entry ) { return entry->GetSize() + result ; } ) ;
  }

  void Add( const std::shared_ptr<Entry>& entry ) override {
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

private:
  std::string m_name ;
  std::vector<std::shared_ptr<Entry>> m_entries ;
} ;

int main() {

  
}