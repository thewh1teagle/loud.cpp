#include "spinner.h"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

void hideCursor() {
#ifdef _WIN32
  // Windows-specific code to hide cursor
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(consoleHandle, &cursorInfo);
  cursorInfo.bVisible = FALSE; // Set visibility to FALSE
  SetConsoleCursorInfo(consoleHandle, &cursorInfo);
#else
  std::cout << "\033[?25l"; // ANSI escape code to hide cursor
#endif
}

void showCursor() {
#ifdef _WIN32
  // Windows-specific code to show cursor
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(consoleHandle, &cursorInfo);
  cursorInfo.bVisible = TRUE; // Set visibility to TRUE
  SetConsoleCursorInfo(consoleHandle, &cursorInfo);
#else
  std::cout << "\033[?25h"; // ANSI escape code to show cursor
#endif
}

Spinner::Spinner(const std::string &initialMessage)
    : message(initialMessage), spinning(false) {}

void Spinner::start() {
  hideCursor();
#ifdef _WIN32
  SetConsoleOutputCP(65001);
#endif
  if (spinning.load())
    return; // Don't start if already spinning
  spinning.store(true);

  spinnerThread = std::thread([this]() {
    const std::vector<std::string> spinnerFrames = {"⣾", "⣽", "⣻", "⢿",
                                                    "⡿", "⣟", "⣯", "⣷"};
    size_t frameIndex = 0;

    size_t maxMessageLength = 0;

    // We keep the message updated as we go, calculating its maximum length
    while (spinning.load()) {
#if defined(_MSC_VER) && !defined(__clang__)
      maxMessageLength = max(maxMessageLength, message.size());
#else
      maxMessageLength = std::max(maxMessageLength, message.size());
#endif

      // Clear the line before printing the new spinner and message
      std::cout << "\r" << std::string(maxMessageLength + 3, ' ') << "\r"
                << std::flush;

      // Print the current spinner frame and message
      std::cout << "\r" << spinnerFrames[frameIndex] << " " << message
                << std::flush;

      frameIndex = (frameIndex + 1) % spinnerFrames.size();
      std::this_thread::sleep_for(
          std::chrono::milliseconds(100)); // Adjust speed here
    }
    // Clear line when done
    std::cout << "\r" << std::string(maxMessageLength + 3, ' ') << "\r"
              << std::flush;

    showCursor();
  });
}

void Spinner::updateMessage(const std::string &newMessage) {
  message = newMessage;
}

void Spinner::stop() {
  spinning.store(false);
  if (spinnerThread.joinable()) {
    spinnerThread.join();
  }
}

Spinner::~Spinner() { stop(); }
