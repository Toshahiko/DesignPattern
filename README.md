# Design Pattern

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
