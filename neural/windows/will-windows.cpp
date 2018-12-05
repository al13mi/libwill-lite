#include "Application.h"
#include "cmdline.h"
#include <cstdio>

int main(int argc, char* argv[])
{
    cmdline::parser cmd;

    cmd.add<std::string>("config", 'c', "config file (ini format) of the net", false, "will.ini");
    cmd.add<std::string>("add-config", 'a', "additional config string ([sec1]key1=v1;key2=v2[sec2]key1=v1...)", false, "");
    cmd.add("version", 'v', "version information");
    cmd.parse_check(argc, argv);

    if (cmd.exist("config"))
    {
        Application will;
        will.setIniFile(cmd.get<std::string>("config"));
        will.setIniString(cmd.get<std::string>("add-config"));
        will.run();
    }
    else if (cmd.exist("version"))
    {
        fprintf(stdout, "LIBWILL (A Deep Neural Net library) command line tool.\n");
        fprintf(stdout, "Built at %s %s, ", __TIME__, __DATE__);
#if defined(_MSC_VER)
        fprintf(stdout, "Microsoft Visual Studio %d.\n", _MSC_VER);
#elif defined(__clang__)
        fprintf(stdout, "Clang %s.\n", __clang_version__);
#elif defined(__GNUC__)
        fprintf(stdout, "GNU C %s.\n", __VERSION__);
#else
        fprintf(stdout, "Unknown complier.\n");
#endif
        fprintf(stdout, "Commemorating my great teacher and friend Dr. Yu Wang.\n");
    }
    else
    {
        if (argc >= 1)
        {
            cmd.parse_check({ argv[0], "--help" });
        }
    }

    return 0;
}
