#include <gtest/gtest.h>
#include "XMLEntity.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSink.h"
#include "StringDataSource.h"

#include <memory>
#include <vector>
#include <string>

TEST(XMLWriter, WriteStartAndFlush) {
    // Create a string sink to capture output
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    // Create a start element entity with no attributes
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = "test";

    // Write the start element and flush remaining open tags
    ASSERT_TRUE(writer.WriteEntity(entity));
    ASSERT_TRUE(writer.Flush());

    // The expected output is a start tag followed by its matching end tag
    EXPECT_EQ(sink->String(), "<test></test>");
}

// Test writing nested elements. We write a start tag for "div", then for "span"
// close "span" and then flush to close "div"
TEST(XMLWriter, WriteNestedElements) {
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    // Write outer element "div"
    SXMLEntity startDiv;
    startDiv.DType = SXMLEntity::EType::StartElement;
    startDiv.DNameData = "div";
    ASSERT_TRUE(writer.WriteEntity(startDiv));

    // Write inner element "span"
    SXMLEntity startSpan;
    startSpan.DType = SXMLEntity::EType::StartElement;
    startSpan.DNameData = "span";
    ASSERT_TRUE(writer.WriteEntity(startSpan));

    // Now write an end for "span"
    SXMLEntity endSpan;
    endSpan.DType = SXMLEntity::EType::EndElement;
    endSpan.DNameData = "span";
    ASSERT_TRUE(writer.WriteEntity(endSpan));

    // Flush will close any open elements
    ASSERT_TRUE(writer.Flush());

    // Expect the "div" element to enclose the "span" element
    EXPECT_EQ(sink->String(), "<div><span></span></div>");
}

// Test writing a start element that contains an attribute
TEST(XMLWriter, WriteStartWithAttributes) {
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity start;
    start.DType = SXMLEntity::EType::StartElement;
    start.DNameData = "a";
    start.DAttributes.push_back({ "href", "http://example.com" });
    ASSERT_TRUE(writer.WriteEntity(start));
    ASSERT_TRUE(writer.Flush());

    // Expect the start tag with its attribute and the corresponding end tag
    EXPECT_EQ(sink->String(), "<a href=\"http://example.com\"></a>");
}



TEST(XMLReader, SingleEmptyElement) {
    std::string xml = "<root/>";
    auto dataSource = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(dataSource);
    
    SXMLEntity entity;
    // Expect a start element event
    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "root");
    EXPECT_TRUE(entity.DAttributes.empty());
    
    // Expect an end element event
    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "root");
    
    // No more events should be available
    EXPECT_TRUE(reader.End());
}

// Test nested elements with attributes and character data
TEST(XMLReader, NestedElementsWithText) {
    std::string xml = "<root><child attribute=\"value\">Text</child></root>";
    auto dataSource = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(dataSource);
    
    std::vector<SXMLEntity> events;
    SXMLEntity entity;
    while(reader.ReadEntity(entity)) {
        events.push_back(entity);
    }
    // Expected sequence
    // 0 StartElement "root"
    // 1 StartElement "child" with "attribute" = "value"
    // 2 CharacterData "Text"
    // 3 EndElement "child"
    // 4 EndElement "root"
    ASSERT_EQ(events.size(), 5u);
    
    EXPECT_EQ(events[0].DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(events[0].DNameData, "root");
    
    EXPECT_EQ(events[1].DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(events[1].DNameData, "child");
    ASSERT_EQ(events[1].DAttributes.size(), 1u);
    EXPECT_EQ(events[1].DAttributes[0].first, "attribute");
    EXPECT_EQ(events[1].DAttributes[0].second, "value");
    
    EXPECT_EQ(events[2].DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(events[2].DNameData, "Text");
    
    EXPECT_EQ(events[3].DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(events[3].DNameData, "child");
    
    EXPECT_EQ(events[4].DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(events[4].DNameData, "root");
    
    EXPECT_TRUE(reader.End());
}

// Test the skipcdata flag so that character data events are omitted
TEST(XMLReader, SkipCharData) {
    std::string xml = "<root><child attribute=\"value\">Text</child></root>";
    auto dataSource = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(dataSource);
    
    std::vector<SXMLEntity> events;
    SXMLEntity entity;
    // Pass true for skipcdata
    while(reader.ReadEntity(entity, true)) {
        events.push_back(entity);
    }
    
    // Expected sequence
    // 0 StartElement "root"
    // 1 StartElement "child", "attribute" = "value"
    // 2 EndElement "child"
    // 3 EndElement "root"
    ASSERT_EQ(events.size(), 4u);
    
    EXPECT_EQ(events[0].DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(events[0].DNameData, "root");
    
    EXPECT_EQ(events[1].DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(events[1].DNameData, "child");
    
    EXPECT_EQ(events[2].DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(events[2].DNameData, "child");
    
    EXPECT_EQ(events[3].DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(events[3].DNameData, "root");
    
    EXPECT_TRUE(reader.End());
}

// Test empty input string
TEST(XMLReader, EmptyInput) {
    std::string xml = "";
    auto dataSource = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(dataSource);
    
    SXMLEntity entity;
    // Should not be able to read any entity
    EXPECT_FALSE(reader.ReadEntity(entity));
    EXPECT_TRUE(reader.End());
}
