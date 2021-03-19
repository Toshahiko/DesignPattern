#include "StdUtil.h"

class Object {
public:
  Object() = default ;
  virtual ~Object() = default ;
} ;

class Iterator {
public:
  virtual ~Iterator() = default ;
  virtual bool HasNext() const = 0 ;
  virtual void Next() = 0 ;
  virtual std::shared_ptr<Object> Item() = 0 ;
} ;

class Aggregate {
public:
  virtual ~Aggregate() = default ;

  virtual std::unique_ptr<Iterator> Iter() = 0 ;
} ;

class Book : public Object {
public:
  Book( const std::string& name ) : m_name( name ) {}
  std::string GetName() const {
    return m_name ;
  }
private:
  std::string m_name ;
} ;

class BookShelf : public Aggregate {
public:
  ~BookShelf() = default ;
  std::shared_ptr<Book> GetBookAt( int i ) const {
    return books.at( i ) ;
  }
  
  void AppendBook( const std::shared_ptr<Book>& book ) {
    books.push_back( book ) ;
  }

  size_t GetLength() const {
    return books.size() ;
  }

  std::unique_ptr<Iterator> Iter() override ;

private:
  std::vector<std::shared_ptr<Book>> books ;
} ;

class BookShelfIterator : public Iterator {
public:
  BookShelfIterator( const BookShelf* bookshelf)
  : m_bookshelf( bookshelf ), m_index( 0 ) {}

  bool HasNext() const override {
    return m_index < m_bookshelf->GetLength() ;
  }

  void Next() override {
    ++m_index ;
  }

  std::shared_ptr<Object> Item() override {
    return m_bookshelf->GetBookAt( m_index ) ;
  }

private:
  const BookShelf* m_bookshelf ;
  int m_index ;
} ;

std::unique_ptr<Iterator> BookShelf::Iter() {
  return std::make_unique<BookShelfIterator>( this ) ;
}

int main() {
  BookShelf bookShelf ;
  bookShelf.AppendBook( std::make_unique<Book>( "Googirl" ) ) ;
  bookShelf.AppendBook( std::make_unique<Book>( "Yamato" ) ) ;

  auto it = bookShelf.Iter() ;
  for ( ; it->HasNext() ; it->Next() )
  {
    auto elm = it->Item() ;
    auto ee = std::dynamic_pointer_cast<Book>(elm) ;
    std::cout << ee->GetName() << std::endl ;
  }
}