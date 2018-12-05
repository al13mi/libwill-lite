#include "Application.h"
#include "Brain.h"
#include "File.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::start()
{
}

void Application::stop()
{
}

void Application::run()
{
    start();
    if (!File::fileExist(ini_file_))
    {
        fprintf(stderr, "%s doesn't exist!\n", ini_file_.c_str());
        return;
    }
    Brain brain;

    brain.getOption()->loadIniFile(ini_file_);
    //format the string into ini style by inserting '\n'
    convert::replaceAllString(ini_string_, "[", "\n[");
    convert::replaceAllString(ini_string_, "]", "]\n");
    convert::replaceAllString(ini_string_, ";", "\n");
    brain.getOption()->loadIniString(ini_string_);

    if (brain.init() != 0)
    {
        return;
    }
    loop_ = true;
    while (loop_)
    {
        brain.run();
        loop_ = false;
    }
    stop();
}

void Application::mainLoop()
{
}

void Application::callback(void* net_pointer)
{
}
