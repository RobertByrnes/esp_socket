#include <unity.h>

void va_start(va_list arg, const char *format) {}
void va_copy(va_list copy, va_list arg) {}
void va_end(va_list arg) {}

#include "ESPSocket.h"

AsyncWebServer mockServer;

/**
 * @brief Default message callback of ESPSocket.
 *
 * @param data uint8_t *
 * @param len size_t
 */
void recvMsgs(uint8_t *data, size_t len) {
    ESPSocket.println("Received Data...");
    String d = "";
    for (int i = 0; i < len; i++) {
        d += char(data[i]);
    }
    ESPSocket.println(d);
}

void setUp(void) {}

void tearDown(void) {}

void test_ESPSocket_begin() {
    // Arrange
    ESPSocketClass socket;

    // Act
    socket.begin(&mockServer, 1, "/ws");
    
    // Assert
    TEST_ASSERT_EQUAL_PTR(&mockServer, socket.getServer());
    TEST_ASSERT_EQUAL_STRING("/ws", socket.getUrl());
}

void test_ESPSocket_msgCallback() {
    // Arrange
    ESPSocketClass socket;

    // Act
    socket.msgCallback(recvMsgs);

    // Assert
    TEST_ASSERT_EQUAL_PTR(recvMsgs, socket.getMsgCallback());
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_ESPSocket_begin);
    // RUN_TEST(test_ESPSocket_msgCallback);
    UNITY_END();
    return 0;
}

// #endif
