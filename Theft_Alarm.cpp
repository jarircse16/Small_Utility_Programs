#include <iostream>
#include <windows.h>

// Function to turn off the laptop screen
void turnOffScreen() {
    // Prevent sleep and display power off
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
    SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
}

// Function to turn on the laptop screen and play a beep sound
void turnOnScreenAndBeep() {

    // Play a beep sound
    Beep(1000, 500);  // Frequency: 1000 Hz, Duration: 500 ms

    // Allow sleep and reset display power settings
    SetThreadExecutionState(ES_CONTINUOUS);
    SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, -1);


}

int main() {
    // Turn off the screen immediately
    turnOffScreen();

    std::cout << "Move the mouse or press any key to turn on the screen and produce a beep sound..." << std::endl;

    // Check for mouse or keyboard input every second
    while (true) {
        if (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON) || GetAsyncKeyState(VK_SPACE)) {
            // Mouse or keyboard event detected
            // Turn on the screen and play a beep sound at the same time
            turnOnScreenAndBeep();
            break;
        }

        Sleep(1000); // Wait for 1 second before checking again
    }

    return 0;
}
