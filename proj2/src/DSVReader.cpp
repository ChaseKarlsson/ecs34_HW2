#include "DSVReader.h"


struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> src;  // data source
    char delimiter;                    
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter) : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->src = src;
    DImplementation->delimiter = delimiter;
}

CDSVReader::~CDSVReader() {

}

bool CDSVReader::End() const {
    if (DImplementation->src->End()) {
        return true;
    }
    return false;
}

bool CDSVReader::ReadRow(std::vector<std::string> &row) {
    row.clear();  // Clear any previous row

    std::string currentField;  // Store the current field being read
    bool insideQuotes = false; // To track if we are inside quotes
    char chr;

    // Loop until we reach the end of the row or data source
    while (true) {
        if (!DImplementation->src->Get(chr)) {
            // If we can't get a character, we're done
            break;
        }

        if (chr == '"') {
            // Handle quote inside the field
            if (insideQuotes) {
                // If we're already inside quotes, check if this is an escaped quote
                if (DImplementation->src->Peek(chr) && chr == '"') {
                    currentField += '"'; // Add the quote to the field
                    DImplementation->src->Get(chr); // Consume the quote
                } else {
                    insideQuotes = false; // Exit from quotes
                }
            } else {
                // Entering quoted field
                insideQuotes = true;
            }
        } else if (chr == DImplementation->delimiter && !insideQuotes) {
            // Delimiter is outside quotes, indicating end of current field
            row.push_back(currentField);
            currentField.clear();  // Reset for the next field
        } else if (chr == '\n' && !insideQuotes) {
            // End of row if not inside quotes
            if (!currentField.empty()) {
                row.push_back(currentField);  // Add last field
            }
            break;  // Exit the loop
        } else {
            // Normal character, append to the current field
            currentField += chr;
        }
    }

    return !row.empty();  // Return true if the row contains any data
}