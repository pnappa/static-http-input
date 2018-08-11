
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


// it shouldn't matter, but let's whitelist for security
const char* whitelisted_colors[] = {"red", "blue"};

bool is_valid_color(const char* col) {
    for (size_t i = 0; i < sizeof(whitelisted_colors)/sizeof(whitelisted_colors[0]); ++i) {
        if (strcmp(col, whitelisted_colors[i]) == 0) return true;
    }
    return false;
}
void setcolor(const char* col) {
    // for security...
    if (!is_valid_color(col)) return; 

    // change the colour
    execl("/bin/bash", "bash", "./setcolor.sh", col, (char *) 0);

    // TODO: update the last modified text
}

int main() {
    setcolor("red");
}
