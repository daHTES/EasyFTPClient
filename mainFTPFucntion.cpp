#include "InterFaceForClientFTP.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main() 
{
   int mySocket;
   mySocket = init_Sock(); // сделали сокет
   readServerResponse(mySocket); //  передали его в нашу функцию и получили ответ
   closesocket(mySocket);  //закрыли соединения по правилам этика 
   return 0;
   }

