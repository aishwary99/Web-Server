#include<iostream>
#include<map>
#include<forward_list>
#include<unistd.h>
#ifdef _WIN32
#include<string.h>
#include<windows.h>
#endif
#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif
#include<sys/stat.h>            //for file operations 
#include<map>
#include<bits/stdc++.h>
//arpa/inet.h                   for linux
//sys/socket.h                  for linux

using namespace std;
//Amit [The bro programmer]
class BroUtilities
{
  private:
  BroUtilities(){}
  public:
  static bool isHexChar(int w)
  {
    if(w>=48 && w<=57) return true;
    if(w>='A' && w<='F') return true;
    if(w>='a' && w<='f') return true;
    return false;
  }
  static void decode(char *encodedString,char *decodedString)
  {
    char *ptr=encodedString;
    char *d=decodedString;
    int i,m;
    i=0;
    while(*ptr!='\0')
    {
      if(*ptr=='+')
      {
        d[i]=' ';
        i++;
        ptr++;
        continue;
      }
      if(*ptr!='%')
      {
        d[i]=*ptr;
        i++;
        ptr++;
        continue;
      }
      ptr++;
      if(isHexChar(*ptr) && isHexChar(*(ptr+1)))
      {
        sscanf(ptr,"%2x",&m);
        d[i]=m;
        i++;
        ptr+=2;
      }
      else
      {
        i=0;
        break;
      }
    }
    d[i]='\0';
  }
  static void loadMIMETypes(map<string,string> &mimeTypesMap)
  {
    FILE *file;
    file=fopen("bro-data/mime.types","r");
    if(!file) return;
    char *mimeType;
    char *extension;
    char line[200];   //each line will not have more than 200 char's
    int x;
    while(true)
    {
      fgets(line,200,file);
      if(feof(file)) break;
      if(line[0]=='#') continue;   //ignore those lines which are starting with #
      //logic to remove \r \n from the end of strings starts here...
      x=strlen(line)-1;
      while(true)
      {
        if(line[x]=='\r' || line[x]=='\n')
        {
          line[x]='\0';
          x--;
        }
        else break;
      }
      //logic to remove \r \n from the end of strings ends here...
      mimeType=&line[0];
      for(x=0;line[x]!='\t';x++);
      line[x]='\0';
      x++;
      while(line[x]=='\t') x++;
      while(true)
      {
        extension=&line[x];
        while(line[x]!='\0' && line[x]!=' ') x++;
        if(line[x]=='\0')
        {
          //add entry to map & break the loop
          mimeTypesMap.insert(pair<string,string> (string(extension),string(mimeType)));
          break;
        }
        else
        {
          //place \0 at xth index , add entry to map and increment the value of x
          line[x]='\0';
          mimeTypesMap.insert(pair<string,string> (string(extension),string(mimeType)));
          x++;
        }
      }   //parsing extensions ends here
    }
    fclose(file);
  }
};
class FileSystemUtility
{
  private:
  FileSystemUtility(){}
  public:
  static bool fileExists(const char *path)
  {
    struct stat s;
    int x;
    x=stat(path,&s);
    if(x!=0) return false;  //-ve means not exists...
    if(s.st_mode & S_IFDIR) return false;
    return true;
  }
  static bool directoryExists(const char *path)
  {
    struct stat s;
    int x;
    x=stat(path,&s);
    if(x!=0) return false;
    if(s.st_mode & S_IFDIR) return true;
    return false;
  }
  static string getFileExtension(const char *path)
  {
    int x;
    x=strlen(path)-1;
    while(x>=0 && path[x]!='.') x--;
    if(x==-1 || path[x]!='.') return string("");
    return string(path+(x+1));
  }
};
class StringUtility
{
  private:
  StringUtility();
  public:
  //utility to convert string to lower case
  static void toLowerCase(char *str)
  {
    if(str==NULL) return;
    for(;*str;str++) if(*str>=65 && *str<=90) *str+=32;
  }
};
class HttpErrorStatusUtility
{
  private:
  HttpErrorStatusUtility(){}
  public:
  static void sendBadRequestError(int clientSocketDescriptor)
  {
    //will complete later on...
  }
  static void sendHttpVersionNotSupportedError(int clientSocketDescriptor,char *httpVersion)
  {
    //will complete later on...
  }
  static void sendNotFoundError(int clientSocketDescriptor,char *requestURI)
  {
    //will optimize it later on...
    char content[2000];
    char header[200];
    char response[2000];
    //preparing response content...
    sprintf(content,"<!Doctype html><html><head><meta charset='utf-8'/><title>Error : 404</title></head><body><center><h1>Request Resource : {%s} Not Found !</h1></center></body></html>",requestURI);
    int contentLength=strlen(content);
    //preparing header...
    sprintf(header,"HTTP/1.1 404 Not Found\r\nContent-Type:text/html\nContent-Length:%d\nConnection:close\r\n\r\n",contentLength);
    strcpy(response,header);
    strcat(response,content);
    send(clientSocketDescriptor,response,strlen(response),0);
  }
  static void sendMethodNotAllowedError(int clientSocketDescriptor,char *method,char *requestURI)
  {
    //will complete later on...
  }
};
class Validator
{
private:
Validator()			//kept private so that this class can't be instantiated
{
}
public:
static bool isValidMIMEType(string &mimeType)
{
//do nothing
return true;
}
static bool isValidPath(string &path)
{
  //pass (char *) type argument
  return FileSystemUtility::directoryExists(path.c_str());  //base address of actual string
}
static bool isValidURLFormat(string &url)
{
//do nothing
return true;
}
};
class Error
{
private:
string error;
public:
Error(string error)
{
this->error=error;
}
bool hasError()
{
return this->error.length()>0;
}
string getError()
{
return this->error;
}
};
class Request
{
  private:
  char *method;
  char *requestURI;
  char *httpVersion;
  map<string,string> dataMap;
  Request(char *method,char *requestURI,char *httpVersion,char *dataInRequest)
  {
    this->method=method;
    this->requestURI=requestURI;
    this->httpVersion=httpVersion;
    if(dataInRequest!=NULL && strcmp(this->method,"get")==0)
    {
      createDataMap(dataInRequest,dataMap);
    }
  }
  void createDataMap(char *str,map<string,string> &dataMap)
  {
    char *ptr1,*ptr2;
    char *decoded;
    int keyLength,valueLength;
    ptr1=str;
    ptr2=str;
    while(true)
    {
      while(*ptr2!='\0' && *ptr2!='=') ptr2++;
      if(*ptr2=='\0') return;
      *ptr2='\0';
      keyLength=ptr2-ptr1;
      decoded=new char[keyLength+1];
      BroUtilities::decode(ptr1,decoded);
      string key=string(decoded);
      delete [] decoded;
      ptr1=ptr2+1;
      ptr2=ptr1;
      while(*ptr2!='\0' && *ptr2!='&') ptr2++;
      if(*ptr2=='\0')
      {
        valueLength=ptr2-ptr1;
        decoded=new char[valueLength+1];
        BroUtilities::decode(ptr1,decoded);
        dataMap.insert(pair<string,string> (key,string(decoded)));
        delete [] decoded;
        break;
      }
      else
      {
        *ptr2='\0';
        valueLength=ptr2-ptr1;
        decoded=new char[valueLength+1];
        BroUtilities::decode(ptr1,decoded);
        dataMap.insert(pair<string,string> (key,string(decoded)));
        delete [] decoded;
        ptr1=ptr2+1;
        ptr2=ptr1;
      }
    }
  }
  public:
  string operator[](string key)
  {
    auto iterator=dataMap.find(key);
    if(iterator==dataMap.end())
    {
      return string("");
    }
    return iterator->second;
  }
  friend class Bro; //because bro will be accessing private member's of this class
};
class Response
{
private:
string contentType;
forward_list<string> content;
forward_list<string>::iterator contentIterator;
unsigned long contentLength;
public:
Response()
{
this->contentLength=0;
this->contentIterator=this->content.before_begin();
}
~Response()
{
}
void setContentType(string contentType)
{
//validate the content type
if(Validator::isValidMIMEType(contentType)) this->contentType=contentType;
}
Response & operator<<(string content)
{
this->contentLength+=content.length();
this->contentIterator=this->content.insert_after(this->contentIterator,content);
return *this;
}
friend class HttpResponseUtility;
};
class HttpResponseUtility
{
  private:
  HttpResponseUtility(){}
  public:
  static void sendResponse(int clientSocketDescriptor,Response &response)
  {
    char header[200];
    int contentLength=response.contentLength;
    sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type:text/html\nContent-Length:%d\nConnection:close\r\n\r\n",contentLength);
    send(clientSocketDescriptor,header,strlen(header),0);
    auto contentIterator=response.content.begin();
    while(contentIterator!=response.content.end())
    {
      string str=*contentIterator;
      send(clientSocketDescriptor,str.c_str(),str.length(),0);
      ++contentIterator;
    }
  }
};
enum __request_method__ {__GET__,__POST__,__PUT__,__DELETE__,__TRACE__,__OPTIONS__,__HEAD__,__CONNECT__};
typedef struct __url__mapping
{
__request_method__ requestMethod;
void (*mappedFunction) (Request &,Response &);
}URLMapping;

class Bro
{
private:
string staticResourcesFolder;
map<string,URLMapping> urlMappings;
map<string,string> mimeTypes;
public:
Bro()
{
  BroUtilities::loadMIMETypes(mimeTypes);
  if(mimeTypes.size()==0) throw string("bro-data folder has been tempered");
}
~Bro()
{
}
void setStaticResourcesFolder(string staticResourcesFolder)
{
if(Validator::isValidPath(staticResourcesFolder)) this->staticResourcesFolder=staticResourcesFolder;
else
{
//throw an exception as folder doesn't  exists
  string exception=string("Invalid static resource folder path : ") + staticResourcesFolder;
  throw exception;
}
}
bool serveStaticResource(int clientSocketDescriptor,const char *requestURI)
{
  if(this->staticResourcesFolder.length()==0) return false;
  if(!FileSystemUtility::directoryExists(this->staticResourcesFolder.c_str())) return false;
  string resourcePath=this->staticResourcesFolder+string(requestURI);
  if(!FileSystemUtility::fileExists(resourcePath.c_str())) return false;
  FILE *file=fopen(resourcePath.c_str(),"rb");
  if(file==NULL) return false; 
  long fileSize;
  fseek(file,0,SEEK_END);
  fileSize=ftell(file);
  if(fileSize==0) 
  {
    fclose(file);
    return false;
  }
  rewind(file); //to move the internal file pointer to the start of file
  string extension,mimeType;
  extension=FileSystemUtility::getFileExtension(resourcePath.c_str());
  if(extension.length()>0) 
  {
    transform(extension.begin(),extension.end(),extension.begin(),::tolower);
    auto mimeTypesIterator=mimeTypes.find(extension);
    if(mimeTypesIterator!=mimeTypes.end())
    {
      mimeType=mimeTypesIterator->second;
    }
    else
    {
      mimeType=string("text/html");
    }
  }
  else
  {
    //there is no extension or the '.' is missing
    mimeType=string("text/html");
  }
  char header[200];
  sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",mimeType.c_str(),fileSize);
  send(clientSocketDescriptor,header,strlen(header),0);
  //header is sent...
  //logic to send file contents in chunks starts here...
  long bytesLeftToRead;
  int bytesToRead;
  char buffer[4096];
  bytesToRead=4096;
  bytesLeftToRead=fileSize;
  while(bytesLeftToRead>0)
  {
    if(bytesLeftToRead<bytesToRead) bytesToRead=bytesLeftToRead;
    fread(buffer,bytesToRead,1,file);
    if(feof(file)) break;
    send(clientSocketDescriptor,buffer,bytesToRead,0); 
    bytesLeftToRead=bytesLeftToRead-bytesToRead;
  }
  fclose(file);
  //logic to send file contents in chunks ends here...
  return true;
}
void get(string url,void (*callBack) (Request &,Response &))
{
if(Validator::isValidURLFormat(url))
{
  urlMappings.insert(pair<string,URLMapping> (url,{__GET__,callBack}));
}
}

void post(string url,void (*callBack) (Request &,Response &))
{
if(Validator::isValidURLFormat(url))
{
  urlMappings.insert(pair<string,URLMapping> (url,{__POST__,callBack}));
}
}

void listen(int portNumber,void (*callBack) (Error &))
{
#ifdef _WIN32
WSADATA wsaData;
WORD ver;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);
#endif
int serverSocketDescriptor;
int clientSocketDescriptor;
char requestBuffer[4097];
int requestLength;
int x;
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(serverSocketDescriptor<0)
{
#ifdef _WIN32
WSACleanup();
#endif
Error error("Unable to create socket");
callBack(error);
return;
}
struct sockaddr_in serverSocketInformation;
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(portNumber);		//network byte order
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
int successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
close(serverSocketDescriptor);
#ifdef _WIN32
WSACleanup();
#endif
char a[101];
sprintf(a,"Unable to bind socket on port : %d",portNumber);
Error error(a);
callBack(error);
return;
}
//::listen is a global function referenced from windows.h library
successCode=::listen(serverSocketDescriptor,10);
if(successCode<0)
{
close(serverSocketDescriptor);
#ifdef _WIN32
WSACleanup();
#endif
Error error("Unable to accept client connection");
callBack(error);
return;
}
Error error("");
callBack(error);
struct sockaddr_in clientSocketInformation;
#ifdef linux
socketlen_t len=sizeof(clientSocketInformation);
#endif
#ifdef _WIN32
int len=sizeof(clientSocketInformation);
#endif
while(1)
{
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
//recv returns the length of request to be arrive...
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer),0);
if(requestLength==0 || requestLength==-1)
{
  close(clientSocketDescriptor);
  continue;
}
int i;
char *method,*requestURI,*httpVersion,*dataInRequest;
requestBuffer[requestLength]='\0';
//request lint is suppose to arrive...
//Request lint -> REQUEST_METHOD SPACE URI SPACE HTTPVersionCRLF
//Parsing for request lint starts here...

//Parsing method type starts here...
method=requestBuffer;
i=0;
while(requestBuffer[i] && requestBuffer[i]!=' ') i++;
if(requestBuffer[i]=='\0')
{
  HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
  close(clientSocketDescriptor);
  continue;
}
requestBuffer[i]='\0';
i++;
if(requestBuffer[i]==' ' || requestBuffer[i]=='\0')
{
  HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
  close(clientSocketDescriptor);
  continue;
}
StringUtility::toLowerCase(method);
if(!(strcmp(method,"get")==0 ||
strcmp(method,"put")==0 ||
strcmp(method,"post")==0 ||
strcmp(method,"delete")==0 ||
strcmp(method,"trace")==0 ||
strcmp(method,"options")==0 ||
strcmp(method,"connect")==0 ||
strcmp(method,"head")==0))
{
  HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
  close(clientSocketDescriptor);
  continue;
}
//Parsing method type ends here...
//Parsing request URI starts here...
requestURI=requestBuffer+i;
while(requestBuffer[i] && requestBuffer[i]!=' ') i++;
if(requestBuffer[i]=='\0')
{
  HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
  close(clientSocketDescriptor);
  continue;
}
requestBuffer[i]='\0';
i++;
if(requestBuffer[i]==' ' || requestBuffer[i]=='\0')
{
  HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
  close(clientSocketDescriptor);
  continue;
}
//Parsing request URI ends here...
//Parsing httpVersion starts here...
httpVersion=requestBuffer+i;
while(requestBuffer[i] && requestBuffer[i]!='\r' && requestBuffer[i]!='\n') i++;
if(requestBuffer[i]=='\0')
{
  HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
  close(clientSocketDescriptor);
  continue;
}
if(requestBuffer[i]=='\r' && requestBuffer[i+1]!='\n')
{
  HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
  close(clientSocketDescriptor);
  continue;
}
if(requestBuffer[i]=='\r')
{
  requestBuffer[i]='\0';
  i+=2;
}
else
{
  requestBuffer[i]='\0';
  i+=1;
}
StringUtility::toLowerCase(httpVersion);
if(strcmp(httpVersion,"http/1.1")!=0)
{
  HttpErrorStatusUtility::sendHttpVersionNotSupportedError(clientSocketDescriptor,httpVersion);
  close(clientSocketDescriptor);
  continue;
}
//Parsing httpVersion ends here...
dataInRequest=NULL;
i=0;
while(requestURI[i]!='\0' && requestURI[i]!='?') i++;
if(requestURI[i]=='?')
{
  requestURI[i]='\0';
  dataInRequest=requestURI+i+1;
}
auto urlMappingsIterator=urlMappings.find(requestURI);
if(urlMappingsIterator==urlMappings.end()) 
{
  //if the static resource doesn't exists , then send necessary error
  if(!serveStaticResource(clientSocketDescriptor,requestURI))
  {
    HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor,requestURI); 
  }
  close(clientSocketDescriptor);
  continue;
}
URLMapping urlMapping=urlMappingsIterator->second;  //callback function extracted against requestURI
if(urlMapping.requestMethod==__GET__ and strcmp(method,"get")!=0)
{
  HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDescriptor,method,requestURI);
  close(clientSocketDescriptor);
  continue;
}
//Parsing for request lint ends here...
//Parsing for header (payload if exists) starts here...
//Parsing for header (payload if exists) ends here...
Request request(method,requestURI,httpVersion,dataInRequest);
Response response;
urlMapping.mappedFunction(request,response);
HttpResponseUtility::sendResponse(clientSocketDescriptor,response);
close(clientSocketDescriptor);
}//infinite loop ends here...
#ifdef _WIN32
WSACleanup();
#endif
}
};
//Bobby [The bro user]
int main()
{
  typedef struct _student
  {
    int rollNumber;
    char name[21];
    char gender;
  }Student;
  try
  {
    Bro bro;		//Creating object of Bro class
    bro.setStaticResourcesFolder("static");
    bro.get("/",[](Request &request,Response &response) void {
      response<<R""""(
        <!Doctype html>
        <html lang='en'>
        <head>
        <meta charset='utf-8'/>
        <title>Bro Test Cases</title>
        </head>
        <body>
        <center>
        <h1>List Of Students</h1>
        <table style='margin:auto; padding:5px; width:50%; text-align:center;' border='1'>
          <thead>
            <tr>
              <th>S.no</th>
              <th>Roll Number</th>
              <th>Name</th>
              <th>Gender</th>
              <th>Edit</th>
              <th>Delete</th>
            </tr>
          </thead>
          <tbody>
      )"""";
      FILE *file;
      file=fopen("student.dat","rb");
      int sno=0;
      if(file!=NULL)
      {
        Student student;
        char str[11];
        while(1)
        {
          fread(&student,sizeof(Student),1,file);
          if(feof(file)) break;
          sno++;
          response<<"<tr>";
          itoa(sno,str,10);
          response<<"<td>"<<str<<"</td>";
          itoa(student.rollNumber,str,10);
          response<<"<td>"<<str<<"</td>";
          response<<"<td>"<<student.name<<"</td>";
          if(student.gender=='M')
          {
            response<<"<td><img src='images/male.png' width='5%'/></td>";
          }
          else
          {
            response<<"<td><img src='images/female.png' width='5%'/></td>";
          }
          response<<"<td><a href='/editStudent?rollNumber="<<str<<"'>Edit</a></td>";
          response<<"<td><a href='/deleteStudent?rollNumber="<<str<<"'>Delete</a></td>";
          response<<"</tr>";
        }
        fclose(file);
      }
      if(sno==0)
      {
        response<<"<tr><td colspan='6' align='center'>No Students</td></tr>";
      }
      response<<R""""(
        </tbody>
        </table>
        <br/>
        <a href='StudentAddForm.html'>Add Student</a>
        </center>
        </body>
        </html>
      )"""";
    });

    bro.get("/addStudent",[](Request &request,Response &response){
      string rollNumber=request["rollNumber"];
      string name=request["name"];
      string gender=request["gender"];
      Student student;
      student.rollNumber=atoi(rollNumber.c_str());
      strcpy(student.name,name.c_str());
      student.gender=gender[0];
      //we should apply duplicacy check if the rollNumber already exists...
      FILE *file=fopen("student.dat","ab");
      fwrite(&student,sizeof(Student),1,file);
      fclose(file);
      response<<R""""(
        <!Doctype html>
        <html lang='en'>
        <head>
        <meta charset='utf-8'/>
        <title>Bro Test Cases</title>
        </head>
        <body>
        <h1>Student (Add Module)</h1>
        <br/>
        <br/>
        <h3> Student Added </h3>
        <br/>
        <form action='/'>
        <button type='submit'>OK</button>
        </form>
        </body>
        </html>
      )"""";
    });
    
    bro.listen(6060,[](Error &error) void {
    if(error.hasError()) 
    {
      cout<<error.getError()<<endl;
      return;
    }
    cout<<"Bro HTTP server is ready to listen on port 6060"<<endl;
    });
  }catch(string exception)
  {
    //in case if something is not going as per the rule
    cout<<"Exception : "<<exception<<endl;
  }
return 0;
}