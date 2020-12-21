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

既存のクラスを再利用する。この時、既存クラスはAdaptee役になる。新しいクラスはAdapter役になる。
ソフトウェアのバージョンアップの時。古いバージョンとの互換性を保つ必要がある。この時、新しいバージョンをAdaptee役として、古いバージョンをAdapter役とします。

## Factory Method

```plantuml
@startuml
package Framework {
class Factory {
+ Create()
- {abstract} CreateProduct()
- {abstract} RegisterProduct()
}

class Product {
+ {abstract} Use()
}

Factory -right-> Product
}

package IDCard {
class IDCardFactory {
- owners
- CreateProduct()
- RegisterProduct()
+ GetOwners()
}

class IDCard {
- owner
+ Use()
+ GetOwner()
}

 IDCardFactory -up-|> Factory
 IDCard -up-|> Product
 IDCardFactory -right-> IDCard
}
@enduml
```

## Command Pattern

オブジェクト指向プログラミングでは、クラスが仕事を行う。
クラスが仕事を行うときは、自分のクラスや他のクラスのメソッドを呼び出す。
その際、メソッドの履歴は通常残らない。
履歴を管理するためには、命令を表すクラスを作成しておくと良いです。命令一つ一つをそのクラスのインスタインスとして表現する。履歴を管理するときは、このインスタンスたちを管理しておけばよい。

```plantuml
@startuml
package Command {
interface Command {
  + {abstract} Execute()
}
class MacroCommand {
  + Execute()
  + Append()
  + Undo()
  + Clear()
  - commands
}
MacroCommand -up-|> Command
MacroCommand o--> Command
}

package Drawer {
  class DrawCommand {
    + Execute()
    - drawable
    - position
  }
  interface Drawable {
    + {abstract} Draw()
  }
  class DrawCanvas {
    + Draw()
    - Paint()
    - history
    - color
    - radius
  }
  DrawCommand -up-|> Command
  DrawCommand o-right-> Drawable
  DrawCanvas -up-|> Drawable
  DrawCanvas o--> MacroCommand
}
@enduml
```
