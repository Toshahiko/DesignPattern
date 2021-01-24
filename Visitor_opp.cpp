
namespace {
class Base {
public:
  virtual void f( Base* p ) = 0 ;
} ;

class Derived1 : public Base {
  void f( Base* p ) override {
    // p->f( this ) ;
  };
} ;

class Derived2 : public Base {
  void f( Base* p ) override {};
} ;

} // anonymous namespace

int main() {
  Base* b = new Derived1 ;
  b->f( b ) ; // Single Dispatch。呼ばれる関数が一つのファクターで決まる。この場合はオブジェクトの実際の型。
}