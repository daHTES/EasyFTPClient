#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>




int init_Sock() 
{
    int len;
    sockaddr_in address;
    int result;
    int s;
    s = socket(AF_INET, SOCK_STREAM,0); //создаем сокет
    address.sin_family = AF_INET;   //интернет домен; здесь же описываем все семейство сокета
    address.sin_addr.s_addr = inet_addr("127.0.0.1");   //соединяемся с 127-0-0-1
    address.sin_port = htons(21);    // 21 порт для конекта
    len = sizeof(address); // получаем размер
    result = connect(s, (sockaddr *)&address, len);   //установка соединения
    if (result == -1) 
    {
        std::cout<<("Увы: клиент офф");
        return -1;
    }
    return s; // все гуд, возвращем готовый и чистый сокет
}




int readServerResponse(int s)  // передаем сокет
{
    int rc;
    fd_set fdr;
    FD_ZERO(&fdr);
    FD_SET(s,&fdr);
    timeval timeout; // запилим структуру времени 
    timeout.tv_sec = 2;   // и зададим зна. 2 сек, к примеру
    timeout.tv_usec = 0;  
    do {
        char buff[512] ={' '}; // размер буффера 512
        recv(s,buff,512,0);   //получаем данные из потока
        std::cout << buff;
        rc = select(s+1,&fdr,NULL,NULL,&timeout);  //ждём данные для чтения в потоке 2 сек.
    } while(rc);     //проверяем результат на валид
    return 2;
}


int init_Data_Connect() 
{
    send(s,"PASV\r\n",strlen("PASV\r\n"),0);
    char buff[128]; // пилим буфер для приема
    recv(s,buff,128,0); // отправляем 
    std::cout << buff; //выводим на экран полученную от сервера строку
    int a,b;
    char *tmp_char; // обязательно указатель
    tmp_char = strtok(buff,"(");
    tmp_char = strtok(NULL,"(");
    tmp_char = strtok(tmp_char, ")");
    int c,d,e,f;
    sscanf(tmp_char, "%d,%d,%d,%d,%d,%d",&c,&d,&e,&f,&a,&b);
    int len;
    sockaddr_in address;
    int result;
    int port = a*256 + b;
    s = socket(AF_INET, SOCK_STREAM,0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(&address);    //addr - глобальная переменная с адресом сервера
    address.sin_port = htons(port);
    len = sizeof(address);
    result = connect(s, (sockaddr *)&address, len);
    if (result == -1) {
        std::cout<<("oops: client");
        return -1;
    }
    return 0;
}



int loginOnServer() 
{
    std::cout << "Введите имя: "; 
    char name[128]; 
    std::cin >> name;
    char str[512];
    sprintf(str,"USER %s\r\n",name);
    send(s,str,strlen(str),0);
    readServerResponse();
    std::cout << "Введите пароль: "; 
    char pass[64]; 
    std::cin >> pass;
    sprintf(str,"PASS %s\r\n",pass);
    send(s,str,strlen(str),0);
    readServerResponse();
    return 0;
}


int get_File_Server(char *file) 
{
    char str[512];
    sprintf(str,"RETR %s\r\n",file);
    send(s,str,strlen(str),0);
 
    /* получаем размер файла */
    char size[512];
    recv(s,size,512,0);
    std::cout << size;
 
    char *tmp_size;
    tmp_size = strtok(size,"(");
    tmp_size = strtok(NULL,"(");
    tmp_size = strtok(tmp_size, ")");
    tmp_size = strtok(tmp_size, " ");
 
    int file_size;
    sscanf(tmp_size,"%d",&file_size);
    FILE *newFile;
    newFile = fopen(file, "wb");   //важно чтобы файл писался в бинарном режиме
    int read = 0;  //изначально прочитано 0 байт
    do 
    {
            char buff[2048];  //буфе для данных
            int readed = recv(ds,buff,sizeof(buff),0);  //считываем данные с сервера. из сокета данных
            fwrite(buff,1,readed,f);   //записываем считанные данные в файл
            read += readed;  //увеличиваем количество скачанных данных
    } while (read < file_size);
    fclose(newFile);
    std::cout << "Готово. Ожидание ответа сервера...\n";
    return 0;
}



