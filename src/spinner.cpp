#include "spinner.h"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

Spinner::Spinner(const std::string &initialMessage)
    : message(initialMessage), spinning(false) {}

void Spinner::start() {
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
      maxMessageLength = std::max(maxMessageLength, message.size());

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
