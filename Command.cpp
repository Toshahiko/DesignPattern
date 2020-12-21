#include "Command.h"

int main() {
  auto drawCanvas = std::make_unique<DrawCanvas>( 1, 1 ) ;
  auto drawCommand = std::make_unique<DrawCommand>( Position2D( 1, 2 ), std::move( drawCanvas ) ) ;
  drawCommand->Execute() ;
  return 0 ;
}