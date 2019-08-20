#include <string>
#include <tcp.h>
#include <udp.h>
#include <raw_udp.h>
int main (int argc, char** argv)
{
    bool udp = false;
    bool connect = false;
    bool server = false;
    bool raw = false;
    for (int i = 1; i < argc; i++)
    {
        std::string str = argv[i];
        switch (str[1])
        {
            case 's' : server = true; break;
            case 'u' : udp = true; break;
            case 'c' : connect = true; break;
            case 'r' : raw = true; break;
        }
    }
    if (udp)
    {
        if (raw)
        {
            if(server)
                raw_udp_server();
            else
                raw_udp_client();
        }
        else
        {
            if(server)
                udp_server(connect);
            else
                udp_client(connect);
        }

    }
    else
    {
        if (server)
            tcp_server();
        else
            tcp_client();
    }

}
