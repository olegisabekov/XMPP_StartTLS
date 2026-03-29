#include <Stream.h>
#include <Print.h>

enum XmppState {
	CLOSED,
	OPEN,
	AUTH,
	AUTH_OPEN,
	BIND,
	AVAILABLE,
	SENDING
};

class XMPP {
private:
  static const uint8_t idLength = 8;
  char uniqueId[idLength];
  static const uint16_t WaitTimeout = 2000;
	char* username;
	char* password;
	char* resource;
	char* server;
	char* jid;
	char* fullJid;
	char* buffer;
	uint16_t currSize;
	const uint8_t bufStep;
	Stream* serial;
	XmppState currentState;
	bool connected;
	Stream* client;
	bool sendData;
	char* recipient;

	bool handleOpenStream(String input);
	bool handleAuthenticate(String input);
	bool handleBindResource(String input);

  bool wait_incoming(void);
  bool is_startTls();
	void sendEHLO();
  void sendOpenStream();
	void authenticate();
	void bindResource();
	void createPresence(const char* status, const char* message);
	void createRoster();
	
	void handlePresence(String input);
	void handleMessage(String input);
	void handleIQ(String input);
	void handlePing(char* id);
	
	bool processInput(char* input);
	void handleStanza(char* input);
	void sendStanza();
	void send(char* data);
	bool checkIfComplete(char* buffer);
	bool checkIfComplete(char* buffer, const prog_char *expected_P);
	bool checkMessage(char* buffer);
	

	char* createUniqueID();
	String findTagBody(String input, String tagName);
	String findAttrValue(String input, String attrName);
	void debug(const char* message);
	void debug(const char* intro, const char* message);

	void flushBuffer();
	void resizeBuffer();

public:
	XMPP(const char* username, const char* password, const char* resource, const char* server, const char* recipient);
	
  bool startTls();
  bool connect();
	void sendMessage(const char* to, const char* body, const char* type);
	void handleIncoming();
	void closeStream();
	bool getRecAvailable();
  XmppState getState(void) { return currentState; };
	
	void setClient(Stream* client);
	void setSerial(Stream* stream);
	void setRecipient(const char* recipient);
	bool getConnected();
	Stream* getClient();
	
	void releaseConnection();
};
