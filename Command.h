#pragma once

#include <memory>
#include <vector>
class Command ;

class Invoker {
public:
  void AddCommand( const std::unique_ptr<Command>& command ) ;
  bool UndoCommand( const std::unique_ptr<Command>& command ) ;
  void Execute() ;
private:
  std::vector<std::unique_ptr<Command>> m_commands ;
} ;

class Command {
public:
  virtual void Execute() ;
} ;

class Receiver ;

class ConcreteCommand : public Command
{
public:
  void SetReceiver( const std::unique_ptr<Receiver>& receiver ) ;
  void Execute() override ;
private:
  int m_id ;
  std::unique_ptr<Receiver> m_receiver ;
} ;

class Receiver {
public:
  virtual void Action() ;
} ;

class ConcreteReceiver : public Receiver
{
public:
  void Action() override ;
} ;