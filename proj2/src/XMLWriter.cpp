#include "XMLWriter.h"
#include "DataSink.h"
#include "XMLEntity.h"

#include <string>
#include <vector>
#include <memory>

// Private implementation structure for CXMLWriter (the PIMPL idiom)
struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> DSink; // Data sink to write output
    std::vector<std::string> DOpenElements; // Stack of open element names

    // Constructor: store the data sink
    SImplementation(std::shared_ptr<CDataSink> sink)
        : DSink(sink){
    }

    // Helper function writes a string to the sink
    // Returns false if DSink isn't valid or writing fails
    bool Write(const std::string &data)
    {
        if (DSink)
        {
            std::vector<char> vec(data.begin(), data.end());
            return DSink->Write(vec);
        }
        return false;
    }
};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(new SImplementation(sink)){
}

CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::Flush()
{
    bool success = true;
    // Write a closing tag for every open element (in LIFO order)
    while (!DImplementation->DOpenElements.empty())
    {
        std::string elementName = DImplementation->DOpenElements.back();
        DImplementation->DOpenElements.pop_back();
        std::string closingTag = "</" + elementName + ">";
        success = success && DImplementation->Write(closingTag);
    }
    return success;
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity)
{
    bool written = false;
    
    // Depending on the entity type, do the appropriate writing.
    switch (entity.DType)
    {
        case SXMLEntity::EType::StartElement:
        {
            // Build the start tag: <element attr="value" ...>
            std::string tag = "<" + entity.DNameData;
            for (const auto &attr : entity.DAttributes)
            {
                // Assume TAttribute is defined as std::pair<std::string, std::string>
                tag += " " + attr.first + "=\"" + attr.second + "\"";
            }
            tag += ">";
            written = DImplementation->Write(tag);
            if (written == true)
            {
                // Record that this element is now open so we can later flush it or match an end element.
                DImplementation->DOpenElements.push_back(entity.DNameData);
            }
            break;
        }
        case SXMLEntity::EType::EndElement:
        {
            // Build the end tag </element>
            std::string tag = "</" + entity.DNameData + ">";
            written = DImplementation->Write(tag);
            // Remove the matching open element (if it is the last one)
            if (!DImplementation->DOpenElements.empty() &&
                DImplementation->DOpenElements.back() == entity.DNameData)
            {
                DImplementation->DOpenElements.pop_back();
            }
            break;
        }
        case SXMLEntity::EType::CompleteElement:
        {
            // Build a self-closing tag <element attr="value" .../>
            std::string tag = "<" + entity.DNameData;
            for (const auto &attr : entity.DAttributes)
            {
                tag += " " + attr.first + "=\"" + attr.second + "\"";
            }
            tag += "/>";
            written = DImplementation->Write(tag);
            break;
        }
        default:
            // Unknown type return false.
            written = false;
            break;
    }
    return written;
}
