#include <RecursivePwmController.h>


RecursivePwmController dev;

void setup()
{
  dev.setup();
  dev.startServer();
}

void loop()
{
  dev.update();
}
