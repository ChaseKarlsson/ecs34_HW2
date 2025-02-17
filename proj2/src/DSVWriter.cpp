#include "DSVWriter.h"



struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> sink;
    char delimiter;
    bool quoteall;
};

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall = false) : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->sink = sink;
    DImplementation->delimiter = delimiter;
    DImplementation->quoteall = quoteall;
}

CDSVWriter::~CDSVWriter() {
    
}


bool CDSVWriter::WriteRow(const std::vector<std::string>& row) {
    bool first_field = true; // track if first field, else add delimiter to front of field
    for (const auto& field : row) {
        if (!first_field) {
            DImplementation->sink->Put(DImplementation->delimiter);
        }
        // if quoteall==true, or if field contains a delimiter, double quotes, or newline, quotes are required
        if (DImplementation->quoteall || field.find(DImplementation->delimiter) != std::string::npos ||
            field.find('"') != std::string::npos || field.find('\n') != std::string::npos) {
                DImplementation->sink->Put('"');  // start quotes
                // replace each '"' with two '"' characters (escaping)
                for (char chr : field) {
                    if (chr == '"') {
                        DImplementation->sink->Put('"');  // extra quote to escape
                    }
                    DImplementation->sink->Put(chr);
                }
                DImplementation->sink->Put('"');       // end quotes
        }
        else {
            for (char chr : field) {
                DImplementation->sink->Put(chr);
            }
        }
        first_field = false;  
    }
    DImplementation->sink->Put('\n');   // newline for end of row
    return true;
}