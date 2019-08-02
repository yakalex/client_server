#include <string>
#include <tcp.h>
#include <udp.h>
int main (int argc, char** argv)
{
    bool udp = false;
    bool connect = false;
    bool server = false;
    for (int i = 1; i < argc; i++)
    {
        std::string str = argv[i];
        switch (str[1])
        {
            case 's' : server = true; break;
            case 'u' : udp = true; break;
            case 'c' : connect = true; break;
        }
    }
    if (udp)
    {
        if(server)
            udp_server();
        else
            udp_client();

    }
    else
    {
        if (server)
            tcp_server();
        else
            tcp_client();
    }

}
