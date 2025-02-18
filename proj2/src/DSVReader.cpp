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

    while (true) {
        if (!DImplementation->src->Get(chr)) {
            // break if no new chr
            break;
        }
        if (chr == '"') {
            if (quotes) {
                // check if end quote
                if (DImplementation->src->Peek(chr) && chr == '"') {
                    field += '"'; //   
                    DImplementation->src->Get(chr); // next char
                } else {
                    quotes = false; // no longer in quotes
                }
            } else {
                // enter quotes
                quotes = true;
            }
        } else if (chr == DImplementation->delimiter && !quotes) {
            // end of current field
            row.push_back(field);
            field.clear();  
        } else if (chr == '\n' && !quotes) {
            // end of row if not inside quotes
            if (!field.empty()) {
                row.push_back(field);  // add last field
            }
            break;  
        } else {
            // append normally
            field += chr;
        }
    }

    return !row.empty();  // true if the row contains any data
}