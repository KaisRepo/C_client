#include <iostream>	//cout and cin
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>     //close() function for sockets
#include <cstring>
#include <vector>
#include <sstream>

using namespace std;




int receivedata(int sock2, char * server_reply2) {
	int buffer2;
	if( buffer2 = recv(sock2, server_reply2, 2000 , 0) < 0)
	{
		cout << "recv failed" << endl;
		return buffer2;
	}
	cout << "Server reply: ";
	cout << server_reply2 << endl;
}


int senddata(int sock2, char * message2) {

	return send(sock2, message2, strlen(message2), 0);
}


int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	int buffer;
	
	char servername[25] ="irc.gamesurge.net";
	int rport=6667;
	char nickname[25]="tester";
	char identity[18] ="tester2";
	char realname[25] = "realname";
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		cout << "Could not create socket" << endl;
	}
	cout << "Socket created: ";
	
	server.sin_addr.s_addr = inet_addr("192.223.27.109");
	server.sin_family = AF_INET;
	server.sin_port = htons( 6667 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		cerr << "connect failed. Error" << endl;
		return 1;
	}
	
	cout << "Connected" << endl;
        char data_to_be_sent[255] = "NICK ";
	strcat(data_to_be_sent, nickname);
	strcat(data_to_be_sent, "\r\n");
	senddata(sock, data_to_be_sent);
	char ident_string[200] = "USER ";
	strcat(ident_string, identity);
	strcat(ident_string, " ");
	strcat(ident_string, servername);
	strcat(ident_string, " bla :");
	strcat(ident_string, realname);
	strcat(ident_string, "\r\n");
	senddata(sock, ident_string);
	//keep communicating with server
	while(1)
	{
		//char thisshouldsolveeverything[1000];
		//Send some data
		/*if( send(sock , message , strlen(message) , 0) < 0)
		{
			cout << "Send failed" << endl;
			return 1;
		}*/
		/*if( senddata(sock, message) < 0) {
			cout << "Send failed" << endl;
			return 1;
		}*/
		//refresh buffer for real
		char server_reply[2000] = "";
		
		
		//thread getdata(receivedata, sock, server_reply);



		//future<int> ret = async(&receivedata, sock, server_reply);
		//getdata.join();

		//if( ret.get()  < 0 ) {
			
			//break;
			
		//}

		//Receive a reply from the server
		if( buffer = recv(sock , server_reply , 2000 , 0) < 0)
		{
			cout << "recv failed" << endl;
			break;
		}

		server_reply[strlen(server_reply)] = '\0';
		if (strlen(server_reply)>0) {
			istringstream iss(server_reply);
			vector<string> lines;
			vector<string> tokens;
			string token; //so i have to break them up by lines, then break them up by spaces after. FIX IT
			string oneline;
			
			while (getline(iss, oneline, '\n')) {
				if(!oneline.empty()) {
					lines.push_back(oneline);
				}
			}
			cout << "printing line: "  << lines.back() << endl;
			istringstream lineiss(server_reply);
			
			for (int i = 0; i<lines.size()-1; i++) {
				istringstream lineiss(lines[i]);
				getline(lineiss, token, ' ');
				if (!token.empty()) {
					tokens.push_back(token);
				}
			}
			cout << tokens.size();

			//check for ping
			if( tokens.size() > 0 ) {
				cout << "we got tokens, " << tokens[0] << endl;
				cout << tokens[0] << endl;
				if(!strcmp(tokens[0].c_str(), "PING")) {
					 cout << tokens[0] << endl;
					 cout << "Nigga nigga nigga, somebody pinged me " << tokens[0] << endl; //never say nigga in your programs again
                                         string buildpong = "PONG " + tokens[1] + "\r\n";
					 
					 char * pong;
					 strcpy(pong, buildpong.c_str());
					 senddata(sock, pong);
				}
				tokens.clear();
				lines.clear();
			}

			cout << endl;
			cout << "Server reply: " << strlen(server_reply) << " " << endl;
			cout << server_reply << endl; }
			//refresh buffer

			strcpy(server_reply, "");
			strcpy(message, "");
		}
	
	close(sock);
	return 0;
}


