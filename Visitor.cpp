#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>

class Visitor ;

class Element {
  public:
  virtual void Accept( Visitor* visitor ) const = 0 ;
} ;

class Entry : public Element {
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
  void Accept( Visitor* visitor ) const override ;
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

  void Accept( Visitor* visitor ) const override ;

  auto GetEntries() const {
    return m_entries ;
  }
private:
  std::string m_name ;
  std::vector<std::shared_ptr<Entry>> m_entries ;
} ;

class Visitor {
  public :
  virtual void Visit( const File& file ) = 0 ;
  virtual void Visit( const Directory& directory ) = 0 ;
} ;

class SizeCalc : public Visitor {
  public :
  void Visit( const File& file ) override {
    m_allSize += file.GetSize() ;
  }
  void Visit( const Directory& directory ) override {
    for ( const auto entry : directory.GetEntries() ) {
      entry->Accept( this ) ;
    }
  }
  int GetResult() const {
    return m_allSize ;
  }
  private :
  int m_allSize = 0 ;
} ;

void Directory::Accept( Visitor* visitor ) const {
  visitor->Visit( *this ) ;
}

void File::Accept( Visitor* visitor ) const {
  visitor->Visit( *this ) ;
}

int main() {
  auto dirProject = std::make_shared<Directory>( "Project" ) ;
  auto hogeCpp = std::make_shared<File>( "hoge.cpp", 6 ) ;
  auto hogeHeader = std::make_shared<File>( "hoge.h", 6 ) ;
  dirProject->Add( hogeCpp ) ;
  dirProject->Add( hogeHeader ) ;

  auto dirPEERLESS = std::make_shared<Directory>( "PEERLESS" ) ;
  auto dirBin = std::make_shared<Directory>( "Bin" ) ;

  dirPEERLESS->Add( dirProject ) ;
  dirPEERLESS->Add( dirBin ) ;

  SizeCalc calc ;
  dirPEERLESS->Accept( &calc ) ;

  std::cout << calc.GetResult() ;
}