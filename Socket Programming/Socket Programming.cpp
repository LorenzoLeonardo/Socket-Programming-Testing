#include "..\EnzTCP\CSocket.h"
#include <process.h>
#include <vector>
using namespace std;

vector<ISocket*> g_vSocket;

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\EnzTCP.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\EnzTCP.lib")
#endif

unsigned _stdcall HandleClientThreadFunc(void* pArguments)
{
    ISocket* pSocket = (ISocket*)pArguments;
    string sData = "";
    
    for (int i = 0; i < g_vSocket.size(); i++)
    {
        if (g_vSocket[i] != pSocket)
        {
            g_vSocket[i]->Send(pSocket->GetIP() + " has joined the chat.\r\n");
        }
    }
    while (!(sData = pSocket->Receive()).empty())
    {
        for (int i = 0; i < g_vSocket.size(); i++)
        {
            if (g_vSocket[i] != pSocket)
            {
                g_vSocket[i]->Send(pSocket->GetIP()+" : "+ sData+"\r\n");
            }
        }
    }
    for (int i = 0; i < g_vSocket.size(); i++)
    {
        if (g_vSocket[i] != pSocket)
        {
            g_vSocket[i]->Send(pSocket->GetIP() + " has left the chat.\r\n");
        }
    }
    g_vSocket.erase(std::remove(g_vSocket.begin(), g_vSocket.end(), pSocket), g_vSocket.end());
    
    CloseClientConnection(pSocket);
    _endthreadex(0);
    return 0;
}

void NewClientConnection(void* pData)
{
    ((ISocket*)pData)->Send("Welcome to Lorenzo Leonardo's Awesome Chat\r\n");

    g_vSocket.push_back((ISocket*)pData);
    _beginthreadex(NULL, 0, &HandleClientThreadFunc, (ISocket*)pData, 0, 0);
}

int main()
{
    HANDLE hHandle = OpenServer("0611", NewClientConnection);

    RunServer(hHandle);
    CloseServer(hHandle);
    return 0;
}

