#include "Application.h"

int main(int, char**)
{
    APPLICATION_NAME::Application::getInstance()->Run();
    return 0;
}