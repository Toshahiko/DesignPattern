#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

class Command {
public:
  virtual void Execute() = 0 ;
  virtual ~Command() = default ;
} ;

class MacroCommand : public Command
{
public:
  void Append( std::unique_ptr<Command>&& command ) {
    m_commands.push_back( std::move( command ) ) ;
  }
  void Undo() {
    m_commands.pop_back() ;
  }
  void Clear() {
    m_commands.clear() ;
  }
  void Execute() override {
    std::for_each( m_commands.begin(), m_commands.end(),
    []( std::shared_ptr<Command> command ) { command->Execute() ; } ) ;
  }
private:
  std::vector<std::shared_ptr<Command>> m_commands ;
} ;

using Position2D = std::pair<int, int> ;

class Drawable {
public:
  virtual void Draw( const Position2D& position ) = 0 ;
  // Drawable( const Drawable& ) = default ;
  Drawable( Drawable&& ) = default ;
  Drawable() = default ;
  virtual ~Drawable() = default ;
} ;

class DrawCommand : public Command
{
public:
  DrawCommand( const Position2D& position, std::unique_ptr<Drawable> drawable)
    : m_position( position ), m_drawable( std::move( drawable ) ) {
  }
  void Execute() override {
    m_drawable->Draw( m_position ) ;
  }
private:
  std::shared_ptr<Drawable> m_drawable ;
  Position2D m_position ;
} ;

class DrawCanvas : public Drawable
{
public:
  DrawCanvas( int color, int radius ) : m_color( color ), m_radius( radius ) {
  }
  void Draw( const Position2D& position ) override {
    Paint( position ) ;
  }
private:
  void Paint( const Position2D& position ) {
    std::cout << "color" << std::endl ;
  }

private:
  int m_color ;
  int m_radius ;

} ;