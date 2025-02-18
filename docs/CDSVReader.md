
class CDSVReader {
    CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter);
        creates a CDSVReader object from a CDataSource object src (ie a string/CStringDataSource) and a delimiter character.
        The delimiter argument is surrounded by quotes. If the double quote argument '"' is passed as the delimiter, it uses the comma ',' as a delimiter.
        for instance, given a CStringDataSource="hello, world" and a comma delimiter, we can make the following:

        CDSVReader reader(string, ',')
        
    CDSVReader::~CDSVReader();
        destructor

    The CDSVReader object has two functions:
        bool CDSVReader::ReadRow(std::vector<std::string> &row);
            returns true if the row was successfully read, and adds it to the row.
            we can access each delimiter-separated field in the added row
            std::string string = "this,is,a,test";
            std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>(source);
            CDSVReader reader(source, ',');
            std::vector<std::string> row;
            reader.ReadRow(row);     // returns true
            printf(row[0]);          // "this"            
            printf(row[1]);          // "is"
            printf(row[2]);          // "a"
            printf(row[3]);          // "test"


        bool CDSVReader::End() const;
            returns true if all rows have been read from the DSV
            from the previous example:
            reader.End();            // returns true
}