#include <cassert>
#include <iostream>
#include <string>
#include <cstring>

// Mocking Arduino-specific types and functions
using String = std::string;

#define STATE_ESP_NOW_ADD_PEER 50 // Dummy value
int previousState;
String macInput;
String userInput;
int peerCount = 0;

struct Peer {
  char name[20];
};
Peer peers[5];

// Mocked function that is being tested
void handleKeyPress_mock() {
    if (previousState == STATE_ESP_NOW_ADD_PEER) {
        if (macInput.length() == 0 && userInput.length() > 0) {
            macInput = userInput;
            userInput = ""; // The fix
        } else if (macInput.length() > 0 && userInput.length() > 0) {
            strncpy(peers[peerCount].name, userInput.c_str(), 19);
            peers[peerCount].name[19] = '\0';
            peerCount++;
        }
    }
}

void test_add_peer() {
    // Setup
    previousState = STATE_ESP_NOW_ADD_PEER;
    macInput = "";
    userInput = "11:22:33:44:55:66"; // MAC address input

    // Action 1: Input MAC address
    handleKeyPress_mock();

    // Assertions 1
    assert(macInput == "11:22:33:44:55:66");
    assert(userInput == ""); // This would fail before the fix

    // Setup for name input
    userInput = "TestPeer";

    // Action 2: Input peer name
    handleKeyPress_mock();

    // Assertions 2
    assert(peerCount == 1);
    assert(std::string(peers[0].name) == "TestPeer");

    std::cout << "Test passed!" << std::endl;
}

int main() {
    test_add_peer();
    return 0;
}
