
class CXMLWriter{
    CXMLWriter(std::shared_ptr< CDataSink > sink);
        constructs a CXMLWriter object using argument std::shared_ptr<CDataSink> sink. Sink is the data destination.

    ~CXMLWriter();
        destructor

    The CXMLWriter object has two functions:
        bool Flush();
            outputs all end elements for those that have been started

        bool WriteEntity(const SXMLEntity &entity);
            writes out the entity to the output stream
};