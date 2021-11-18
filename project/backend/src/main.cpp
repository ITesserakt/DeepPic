#include <iostream>

#include "SharedDocument.h"

int SharedDocument::start_since_port = 5555;

int main() {
    SharedDocument document;
    document.startShared();
}