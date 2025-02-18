
CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall = false);
    constructs a CDSVWriter object using parameters std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall=false.
    data sink allows for storing and modifying data (ie. CStringDataSinks). By default, only values containing the delimiter, a double quote, or a newline are quoted. When quoteall is set to "true", all values will be quoted.
    for example:

    std::shared_ptr<CDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', false);


~CDSVWriter();
    destructor

The CDSVWriter object only has one function:
bool WriteRow(const std::vector<std::string> &row)
    returns true if successfully write, one string per column added to row vector
    adding to the previous example:

    std::vector<std::string> row = {"this", "is", "A", "test"};
    writer.WriteRow(row);                           // returs true
    
    CStringDataSink* string_sink = static_cast<CStringDataSink*>(sink.get());
    std::string string = string_sink->String();

    printf(string;                                  // "this,is,A,test")  

