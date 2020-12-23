#include <string>
#include <iostream>
#include <vector>
#include <memory>
class Visitor ;

class Element {
public:
  virtual void Accept( const Visitor& visitor ) = 0 ;
} ;

class Entry : public Element {
public:
  virtual std::string GetName() const noexcept ;
  virtual int GetSize() const noexcept ;
  virtual void Add( const std::shared_ptr<Entry> entry ) = 0 ;
} ;

class File : public Entry {
public:
  void Accept( const Visitor& visitor ) override ;
  std::string GetName() const noexcept final { return m_name ; }
  int GetSize() const noexcept final { return m_size ; }
private:
  std::string m_name ;
  int m_size ;
} ;

class Directory : public Entry {
public:
  void Accept( const Visitor& visitor ) override ;
  std::string GetName() const noexcept final ;
  int GetSize() const noexcept final ;
  void Add( const std::shared_ptr<Entry> entry ) override ;
private:
  std::string m_name ;
  std::vector<std::shared_ptr<Entry>> m_dir ;
} ;

class Visitor {
public:
  virtual void Visit( const File& file ) = 0 ;
  virtual void Visit( const Directory& file ) = 0 ;
} ;

class ListVisitor : public Visitor {
  void Visit( const File& file ) override ;
  void Visit( const Directory& directory ) override ;
} ;

int main() {
  return 0 ;
}