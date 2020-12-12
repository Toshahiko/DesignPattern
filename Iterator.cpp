#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <utility>

class Object {
public:
  virtual ~Object() = default ;
} ;

class Iterator {
public:
  virtual ~Iterator() = default ;
  virtual bool HasNext() const = 0 ;
  virtual void Next() = 0 ;
  virtual Object* Item() = 0 ;
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
  Book* GetBookAt( int i ) const {
    return books.at( i ) ;
  }
  
  void AppendBook( Book* book ) {
    books.push_back( book ) ;
  }

  size_t GetLength() const {
    return books.size() ;
  }

  std::unique_ptr<Iterator> Iter() override ;

private:
  std::vector<Book*> books ;
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

  Object* Item() override {
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
  bookShelf.AppendBook(  new Book( "Googirl" ) ) ;
  bookShelf.AppendBook(  new Book( "Yamato" ) ) ;

  auto it = bookShelf.Iter() ;
  for ( ; it->HasNext() ; it->Next() )
  {
    auto elm = it->Item() ;
    auto ee = dynamic_cast<Book*>(elm) ;
    std::cout << ee->GetName() << std::endl ;
  }
}