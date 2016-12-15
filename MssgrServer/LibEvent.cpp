#include "LibEvent.h"
#include "ServerUserDataFile.h"
#include "Parser.h"

int LibEvent::iResult = 0;
event_base* LibEvent::base = NULL;
evconnlistener* LibEvent::listener = NULL;
int LibEvent::port = 0;
SOCKADDR_IN LibEvent::sock_addr;
WSADATA LibEvent::wsaData;
std::vector<evutil_socket_t> fd_vec;
std::vector<UserData> ud_vec;

LibEvent::LibEvent()
{
}

LibEvent::~LibEvent()
{
}

//Read callback function
//We create the input buffer evbuffer
//We get it's size
//We copy data from the buffer
//And we send the buffer back to client
//@param buf_ev the evbuffer where we read data
//@param arg
void LibEvent::data_read_cb(struct bufferevent *buf_ev, void *arg)
{
	char buf[DEFAULT_BUFLEN];
	int checkResult = 0;
	UserData ud;

	ud.in_buf = bufferevent_get_input(buf_ev);
	ud.out_buf = bufferevent_get_output(buf_ev);
	ud.fd = bufferevent_getfd(buf_ev);
	size_t buf_input_size = evbuffer_get_length(ud.in_buf); //Getting input length
	evbuffer_copyout(ud.in_buf, buf, buf_input_size);
	evbuffer_drain(ud.in_buf, buf_input_size);

	//If received registration request
	if (buf[0] == '<' && tolower(buf[1]) == 'r' && tolower(buf[2]) == 'e' && tolower(buf[3]) == 'g' && tolower(buf[4]) == '>' && buf[5] == '\n') {
		on_registration_receive(&ud, buf);
	}

	//if received login request
	if (buf[0] == '<' && tolower(buf[1]) == 'r' && tolower(buf[2]) == 'e' && tolower(buf[3]) == 'q' 
		&& tolower(buf[4]) == 'l' && tolower(buf[5]) == 'o' && tolower(buf[6]) == 'g' && tolower(buf[7]) == '>') {
		on_login_receive(&ud, buf);
		checkResult = check_fd_existence(buf_ev);

		if (checkResult == 0) { ud_vec.push_back(ud); }
	}

	//If received contacts request
	if (buf[0] == '<' && buf[1] == 'c' && buf[2] == 'l' && buf[3] == 'r' && buf[4] == 'e' && buf[5] == 'q' && buf[6] == '>') {
		on_clreq_receive(&ud, buf);
	}

	//If received add contact to contact list request
	if (buf[0] == '<' && buf[1] == 'c' && buf[2] == 'l' && buf[3] == 'a' && buf[4] == 'd' && buf[5] == 'd' && buf[6] == '>') {
		on_cladd_receive(&ud, buf);
	}

	//If received contact list request
	if (buf[0] == '<' && buf[1] == 'c' && buf[2] == 'l' && buf[3] == '>') {
		on_cl_receive(&ud, buf);
	}
	//If received message
	if (buf[0] == '<' && tolower(buf[1]) == 'r' && buf[2] == 'e' && buf [3] == 'c' && buf[4] == '>') {
		on_message_receive(buf_ev, buf);
	}
}

//Echo event callback function
//Buffer event callback function
//@param buf_ev the evbuffer structure connected to the event
//@param events event flags passed to the function
//@arg argument pointer passed to a function
void LibEvent::data_event_cb(struct bufferevent *buf_ev, short events, void *arg)
{
	int disconnectedFd = bufferevent_getfd(buf_ev);
	if (events & BEV_EVENT_ERROR) {
		perror("bufferevent object error");
		if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
			for (size_t i = 0; i < fd_vec.size(); i++) {
				if (fd_vec.at(i) == disconnectedFd) {
					fd_vec.erase(fd_vec.begin() + i);
				}
			}
		}
	}
}

//Accept connection callback function
//Is called when there is a new incoming connection to the server
//@param listener evconnlistener object where the incoming connection arrived
//@param fd connection file descriptor number
//@param addr SOCKADDR structure with incoming connection info
//@param sock_len
//@param arg
void LibEvent::accept_connection_cb(struct evconnlistener *listener, evutil_socket_t fd, SOCKADDR *addr, int sock_len, void *arg) {
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *buf_ev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	fd_vec.push_back(fd);

	char inetAddress[INET_ADDRSTRLEN];
	SOCKADDR_IN *inAddr = (SOCKADDR_IN*)addr;
	int fd_vec_size = fd_vec.size();

	std::cout << "There are now " << fd_vec_size << " open connections:" << std::endl;
	for (int i = 0; i < fd_vec_size; i++) {
		std::cout << "Connection " << i << " file descriptor:" << fd_vec.at(i) << std::endl;
	}
	printf("Incoming connection from: %s\n", inet_ntop(inAddr->sin_family, &inAddr->sin_addr, inetAddress, 14));
	bufferevent_setcb(buf_ev, data_read_cb, NULL, data_event_cb, NULL);
	bufferevent_enable(buf_ev, (EV_READ | EV_WRITE));
}

//Error callback function
//Is called when some error occured
//@param evconnlistener listener where error occured
//@arg
void LibEvent::accept_error_cb(evconnlistener *listener, void *arg) {
	struct event_base *base = evconnlistener_get_base(listener);
	int error = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Error %d (%s) in connections monitor. Shutting down.\n", error, evutil_socket_error_to_string(error));
	event_base_loopexit(base, NULL);
}

int LibEvent::check_fd_existence(bufferevent *buf_ev)
{
	int checkResult = 0;

	//Look if there is this fd in vec
	for (size_t i = 0; i < ud_vec.size(); i++) {
		if (ud_vec.at(i).fd == bufferevent_getfd(buf_ev))
			checkResult = 1;
		else checkResult = 0;
	}

	return 0;
}

void LibEvent::on_message_receive(bufferevent *buf_ev, char * buf)
{
	std::string incUsername;
	std::string incMessage;
	std::string receiverUsername;
	int n = 0;

	//Get sender's username
	if (buf[n] == '<' && tolower(buf[n + 1]) == 'r' && tolower(buf[n + 2]) == 'e' && tolower(buf[n + 3]) == 'c' && buf[n + 4] == '>') {
		n += 5;
		while (buf[n] != '<' || buf[n + 1] != '/' || tolower(buf[n + 2]) != 'r' || tolower(buf[n + 3]) != 'e' || tolower(buf[n + 4]) != 'c' || buf[n + 5] != '>' || buf[n +6] != '\n') {
			receiverUsername += buf[n];
			++n;
		}
		n += 7;
	}

	//Get incoming message
	if (buf[n] == '<' && tolower(buf[n + 1]) == 'm' && tolower(buf[n + 2]) == 's' && tolower(buf[n + 3]) == 'g' && buf[n + 4] == '>') {
		n += 5;
		while (!(buf[n] == '<' && buf[n + 1] == '/' && tolower(buf[n + 2]) == 'm' && tolower(buf[n + 3]) == 's' && tolower(buf[n + 4]) == 'g' && buf[n + 5] == '>')) {
			incMessage += buf[n];
			++n;
		}
	}

	//Here we compare incoming buffer file descriptor with fds we have in our vector
	//When we find a match we retrieve username for that fd
	for (size_t i = 0; i < ud_vec.size(); i++) {
		if (ud_vec.at(i).fd == bufferevent_getfd(buf_ev)) {
			incUsername = ud_vec.at(i).username;
		}
	}

	for (size_t i = 0; i < ud_vec.size(); i++) {
		if (ud_vec.at(i).username == receiverUsername) {
			evbuffer_add_printf(ud_vec.at(i).out_buf, "<fu>%s</fu><im>%s</im>\n", incUsername.c_str(), incMessage.c_str());
		}
	}
}

void LibEvent::on_registration_receive(UserData *ud, char *buf)
{
	std::string registeringUsername;
	std::string registeringPassword;
	char *checkResult = NULL;
	Parser * pParser = new Parser;
	ServerUserDataFile * pSrvUsrDataFile = new ServerUserDataFile();

	registeringUsername = pParser->parse_login(buf); //Parse login from a received packet
	registeringPassword = pParser->parse_password(buf); //Parse password from a received packet
	delete pParser;

	checkResult = pSrvUsrDataFile->CheckLoginInMsf(registeringUsername); //Check if it exists in MCF

	if (checkResult == LOGIN_OK) {
		pSrvUsrDataFile->SaveAccountDataToFile(registeringUsername, registeringPassword);
		evbuffer_add_printf(ud->out_buf, "%s", LOGIN_OK);
	}
	else if (checkResult == LOGIN_IN_USE) {
		evbuffer_add_printf(ud->out_buf, "%s", LOGIN_IN_USE);
	}
	else evbuffer_add_printf(ud->out_buf, "%s", LOGIN_CHECK_PROBLEMS);

}

char * LibEvent::on_login_receive(UserData *ud, char *buf) {

	std::string incomingUsername;
	std::string incomingPassword;
	char *checkResult = NULL;
	Parser * pParser = new Parser;
	ServerUserDataFile * pSrvUsrDataFile = new ServerUserDataFile();

	incomingUsername = pParser->parse_login(buf);
	incomingPassword = pParser->parse_password(buf);
	delete pParser;

	checkResult = pSrvUsrDataFile->CheckLoginResult(incomingUsername, incomingPassword);

	if (checkResult == LOGIN_OK) {
		evbuffer_add_printf(ud->out_buf, "%s", LOGIN_OK);
		ud->username = incomingUsername;
		return LOGIN_OK;
	}

	else if (checkResult == LOGIN_OR_PASSWORD_INCORRECT) {
		evbuffer_add_printf(ud->out_buf, "%s", LOGIN_OR_PASSWORD_INCORRECT);
	}
	else evbuffer_add_printf(ud->out_buf, "%s", LOGIN_CHECK_PROBLEMS);
}

void LibEvent::on_clreq_receive(UserData *ud, char *buf) {
	std::vector<std::string> loadedUsernamesVec;
	ServerUserDataFile * pSrvUsrDataFile = new ServerUserDataFile();
	pSrvUsrDataFile->LoadAllUsernames(&loadedUsernamesVec);
	std::string usernames;

	for (size_t i = 0; i < loadedUsernamesVec.size(); i++) {
		usernames.append("<un>");
		usernames.append(loadedUsernamesVec.at(i));
		usernames.append("</un>");
	}
	evbuffer_add_printf(ud->out_buf, "%s", usernames.c_str());
}

void LibEvent::on_cladd_receive(UserData * ud, char * buf)
{
	int n = 0;
	Parser *pParser = new Parser;
	ServerUserDataFile * pSrvUsrDataFile = new ServerUserDataFile;
	std::string contactName = pParser->parse_contact_name(buf);
	while (ud_vec.at(n).fd != ud->fd) {
		++n;
	}
	ud->username = ud_vec.at(n).username;
	pSrvUsrDataFile->SaveAccountCLToFile(ud->username, contactName);
}

void LibEvent::on_cl_receive(UserData * ud, char * buf) {
	int n = 0;
	std::string contactListData;
	ServerUserDataFile * pSrvUsrDataFile = new ServerUserDataFile;
	while (ud_vec.at(n).fd != ud->fd) {
		++n;
	}
	ud->username = ud_vec.at(n).username;
	pSrvUsrDataFile->LoadAccountCLFromFile(&contactListData, ud->username);
	evbuffer_add_printf(ud->out_buf, "%s", contactListData.c_str());
}
void LibEvent::initialize_libevent()
{
	std::cout << "Please enter port number where server will listen.\n";
	std::cin >> LibEvent::port;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		system("pause");
		exit(iResult);
	}

	base = event_base_new();
	if (!base) {
		exit(1);
	}

	ZeroMemory(&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sock_addr.sin_port = htons(port);

	listener = evconnlistener_new_bind(base, accept_connection_cb, NULL, LEV_OPT_REUSEABLE, -1, (SOCKADDR*)&sock_addr, sizeof(sock_addr)); 

	if (!listener) {
		perror("Error creating evconnlistener:");
		system("pause");
		exit(-1);
	}

	//Setting libevent error callback function
	evconnlistener_set_error_cb(listener, accept_error_cb);

	event_base_dispatch(base);

}