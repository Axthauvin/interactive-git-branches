#pragma once

// Returns a dynamically allocated string with the user's input, or NULL if cancelled (ESC).
// Caller must free() the returned string if it is not NULL.
char* popup_input(const char* title);

// Returns 1 for opt1, 2 for opt2, or 0 if cancelled (ESC).
int popup_choice(const char* title, const char* opt1, const char* opt2);

// Displays a message popup with an [OK] button.
void popup_message(const char* title, const char* message);
