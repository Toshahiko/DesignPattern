
namespace {
class Base {
public:
  virtual void f() = 0 ;
} ;

class Derived1 : public Base {
  void f() override {};
} ;

class Derived2 : public Base {
  void f() override {};
} ;

} // anonymous namespace

int main() {
  Base* b = new Derived1 ;
  b->f() ; // Single Dispatch。呼ばれる関数が一つのファクターで決まる。この場合はオブジェクトの実際の型。
}