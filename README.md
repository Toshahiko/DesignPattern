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

## Composite Pattern

容器と中身を同一視して再帰的な構造を作る。
例として、ディレクトリ構造をあげる。ディレクトリの中にはディレクトリかファイルが追加される。このとき、ディレクトリ=容器、ファイル＝中身である。この構造をクラスで表す。
クラス図は以下。

```plantuml
@startuml
class Entry {
  + {abstract} GetName()
  + {abstract} GetSize()
  + {abstract} Add()
}

class File {
  - m_name
  - m_size
  + GetName()
  + GetSize()
}

class Directory {
  - m_name
  - m_entries
  + GetName()
  + GetSize()
  + Add()
}
File -up-|> Entry
Directory -up-|> Entry
Directory o-up-> Entry
@enduml
```

ディレクトリのサイズを表すコードを書いてみる。

## Visitor Pattern

訪問者を表すクラスVisitor。データ構造と処理を分離するためにこのパターンを使う。
仮にデータ構造に処理のメソッドを書いていた場合、新しい処理を追加する時に、データ構造のクラスを修正する必要が出てくる。
Visitorパターンを使うとデータ構造に新たな処理を追加することが簡単にできる。

```plantuml
@startuml
class Visitor {
  + {abstract} Visit(File)
  + {abstract} Visit(Directory)
}

class ListVisitor {
  - currentDir
  + Visit(File)
  + Visit(Directory)
}
ListVisitor -up-|> Visitor

interface Element {
  + {abstract} Accept()
}

class Entry {
  + GetName()
  + GetSize()
  + Add()
  + Iterator()
}

class File {
  - name
  - size
  + Accept()
  + GetName()
  + GetSize()
}

class Directory {
  - name
  - dir
  + Accept()
  + GetName()
  + GetSize()
  + Add()
  + Iterator()
}

Entry -up-|> Element
Directory -up-|> Entry
File -up-|> Entry
@enduml
```

コード例はディレクトリとファイルの構造に対してVisitorパターンを適用する。
ディレクトリ構造はComposite Patternで実装する。Leafはファイル、Compositeはディレクトリ。