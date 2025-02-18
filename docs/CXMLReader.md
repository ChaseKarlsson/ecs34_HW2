
class CXMLReader {
    CXMLReader(std::shared_ptr< CDataSource > src);
        constructs a CXMLReader object using argument std::shared_ptr<CDataSource> src. 

    ~CXMLReader();
        destructor
        
    The CXMLReader object has two functions:
        bool End() const;
            returns true if all entities have been read from the XML. 


        bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
            returns true if the entity is successfully read
            if skipdata is true, only element type entities will be returned
};
