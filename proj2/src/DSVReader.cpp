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
    row.clear();  // start with new row

    std::string field;  // current field
    bool quotes = false; // are we in quotes
    char chr;
    bool containschar = false;

    while (true) {
        if (!DImplementation->src->Get(chr)) {
            // break if no new chr
            break;
        }
        containschar = true;
         if (chr == '"') {
            if (quotes) {
                // If we're inside quotes, check if the next character is another quote for escaping
                char nextChar;
                if (DImplementation->src->Peek(nextChar) && nextChar == '"') {
                    field += '"'; // append the escaped quote
                    DImplementation->src->Get(nextChar); // consume the extra quote
                } else {
                    quotes = false; // closing quote
                }
            } else {
                // start quoted field
                quotes = true;
            }
        } else if (chr == DImplementation->delimiter && !quotes) {
            // end of current field
            row.push_back(field);
            field.clear();  
        } else if (chr == '\n' && !quotes) {
            // end of row if not inside quotes
            row.push_back(field);  // add last field
            return true;  
        } else {
            // append normally
            field += chr;
        }
    }
    if (containschar){
        row.push_back(field);
        return true;
    }
    return !row.empty();  // true if the row contains any data
}
