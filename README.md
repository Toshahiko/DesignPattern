# Design Pattern

本書全体の考え方
「具体的なクラスだけでプログラミングするのではなく、**抽象クラスやインターフェイスを使ってプログラミングする**」
## Memento Pattern

```plantuml
@startuml
class Gamer {
  -int: m_money
  -mt19937_64: m_engine
  -vector<Fruit>: m_fruits
  +void StoreFruit( const fruit& )
  +void EarnMoney( int money )
  +Memento CreateMemento()
  +void StoreMemento()
}
class Memento {
  -int: m_money
  -vector<Fruit>: m_fruits
  +int GetMoney()
  -Memento()
}

Gamer -- Memento

@enduml
```

保存されるクラスがある瞬間でのMementoを作成する。CreateMementoメソッド
作成したMementoはプログラムのどこかで保存しておく。
状態を戻す必要があったら、RestoreMementoを使って状態を読み込む

## Iterator Pattern

何かがたくさん集まっている時、それらを順番に指し示していき全体を走査する方法

下のクラス図は本棚全体を走査するItrator Patternの例

```plantuml
@startuml
interface Aggregate {
  + {abstract} Iterator()
}

interface Iterator {
  + {abstract} hasNext()
  + {abstract} Next()
}

class BookShelfIterator {
  - bookShelf
  - index
  + hasNext()
  + Next()
}

class BookShelf {
  - books
  - last
  + GetBookAt()
  + AppendBook()
  + GetLength()
  + Iterator()
}

class Book {
  - name
  + GetName()
}

Aggregate --right--> Iterator
BookShelf -up-|> Aggregate
BookShelfIterator -up-|> Iterator
BookShelfIterator o-left-> BookShelf
BookShelf o-down-> Book
@enduml
```

Iterator Patternは意外と難しかった
BookShelfがIteratorを生成するという発想が思いつかなかった

Iteratorパターンを使う理由
Iteratorを使うことで実装と数え上げを切り離すことができる。

```

while ( it.HasNext() ) {
  Book book = (Book)it.Next() ;
  system.out.println(book.GetName() ) ;
}
```

上のコードでは、HasNextとNextというIteratorのインターフェイスだけを使っている。
BookShelfの実装で使われているメソッドを使っていない。
つまりBookShelfの実装に依存していないコードになっている。

「数え上げの仕組みがAggregte役の外に置かれている」というのはIterator Patternの特徴。
この特徴によって1つのConcreteAggregate役に対して複数のConcreteIterator役を作ることができる。

## Adapter Pattern

すでに提供されているものと必要なものの間のズレを埋めるようなデザインパターンがAdapter Pattern
Adapter Patternには2種類ある

- クラスによるAdapter Pattern（継承をつかったもの）
- インスタンスによるAdapter Pattern（移譲を使った物）

```plantuml
@startuml
interface Print {
  + {abstract} PrintWeak()
  + {abstract} PrintStrong()
}

class PrintBanner {
  + PrintWeak()
  + PrintStrong()
}

class Banner {
  + ShowWithParen()
  + ShowWithAster()
}

PrintBanner -up-|> Print
PrintBanner -up-|> Banner

@enduml
```

```plantuml
@startuml
interface Print {
  + {abstract} PrintWeak()
  + {abstract} PrintStrong()
}

class PrintBanner {
  + PrintWeak()
  + PrintStrong()
}

class Banner {
  + ShowWithParen()
  + ShowWithAster()
}

PrintBanner -up-|> Print
PrintBanner o-right-> Banner

@enduml
```

### どんな時に使うか

