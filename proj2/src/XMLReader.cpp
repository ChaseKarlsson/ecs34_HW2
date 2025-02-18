// XMLReader.cpp
#include "XMLReader.h"
#include "expat.h"

#include <vector>
#include <string>
#include <memory>

struct CXMLReader::SImplementation {
    XML_Parser DParser; // Expat parser
    std::shared_ptr<CDataSource> DSource; // Data source for input
    std::vector<SXMLEntity> DQueue; // Queue of pending XML entities
    bool DFinished; // Bool for when parser is finalized or on an error

    SImplementation(std::shared_ptr<CDataSource> src)
        : DSource(src), DFinished(false)
    {
        DParser = XML_ParserCreate(nullptr);
        XML_SetUserData(DParser, this);
        XML_SetElementHandler(DParser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(DParser, CharacterDataHandler);
    }

    ~SImplementation()
    {
        if (DParser)
        {
            XML_ParserFree(DParser);
        }
    }

    // Callback for start element events.
    static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts)
    {
        SImplementation* impl = static_cast<SImplementation*>(userData);
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::StartElement;
        entity.DNameData = name;
        if (atts)
        {
            for (int i = 0; atts[i]; i += 2)
            {
                entity.DAttributes.push_back({ std::string(atts[i]), std::string(atts[i + 1]) });
            }
        }
        impl->DQueue.push_back(entity);
    }

    // Callback for end element events.
    static void EndElementHandler(void *userData, const XML_Char *name)
    {
        SImplementation* impl = static_cast<SImplementation*>(userData);
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;
        impl->DQueue.push_back(entity);
    }

    // Callback for character data events.
    static void CharacterDataHandler(void *userData, const XML_Char *s, int len)
    {
        if (len <= 0)
        {
            return; // Nothing to do
        }
        SImplementation* impl = static_cast<SImplementation*>(userData);
        std::string data(s, len);
        // if character data event after another
        if (!impl->DQueue.empty() && impl->DQueue.back().DType == SXMLEntity::EType::CharData)
        {
        // Append the new data to the previous CharData event
            impl->DQueue.back().DNameData.append(data);
        }
        else
        {
            SXMLEntity entity;
            entity.DType = SXMLEntity::EType::CharData;
            entity.DNameData = data;
            impl->DQueue.push_back(entity);
        }
    }
};

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : DImplementation(std::make_unique<SImplementation>(src)){
}

CXMLReader::~CXMLReader() = default;

bool CXMLReader::End() const
{
    // Parsing is finished when the parser is finalized and no events remain
    return DImplementation->DFinished && DImplementation->DQueue.empty();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata)
{
    // Read more data until we have an event or the parser is finalized
    while (DImplementation->DQueue.empty() && !DImplementation->DFinished)
    {
        std::vector<char> buffer;
        const std::size_t chunkSize = 1024;
        DImplementation->DSource->Read(buffer, chunkSize);
        int isFinal = DImplementation->DSource->End() ? 1 : 0;

        if (buffer.empty() && isFinal == 1)
        {
            // Final call to flush any remaining events.
            if (XML_Parse(DImplementation->DParser, nullptr, 0, 1) == 0)
            {
                // On error: clear queue, mark finished and return false.
                DImplementation->DQueue.clear();
                DImplementation->DFinished = true;
                return false;
            }
        }
        else
        {
            if (XML_Parse(DImplementation->DParser, buffer.data(), static_cast<int>(buffer.size()), isFinal) == 0)
            {
                // On error clear queued events, mark finished, return false
                DImplementation->DQueue.clear();
                DImplementation->DFinished = true;
                return false;
            }
        }
        if (isFinal)
        {
            DImplementation->DFinished = true;
        }
    }

    // Retrieve and return the next event.
    while (!DImplementation->DQueue.empty())
    {
        SXMLEntity evt = DImplementation->DQueue.front();
        DImplementation->DQueue.erase(DImplementation->DQueue.begin());
        if (skipcdata && evt.DType == SXMLEntity::EType::CharData)
        {
            continue;
        }
        entity = evt;
        return true;
    }

    return false;
}
