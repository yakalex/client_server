#include <tcp.h>
int main (int argc, char** argv)
{
    bool server = false;
    if (argc > 1)
    {
        std::string str = argv[1];
        if (str=="-s")
        {
            server = true;
        }

    }
    if(server)
    {
        tcp_server();
    }
    else
    {
        tcp_client();
    }
}
