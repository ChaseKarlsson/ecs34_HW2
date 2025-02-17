#include <gtest/gtest.h>
#include "StringDataSink.h"
#include "StringDataSource.h"
#include "DSVWriter.h"
#include "DSVReader.h"

// test DSVWriter
TEST(DSVWriter, WriteRowTest) {
    std::shared_ptr<CDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', false);
    std::vector<std::string> row = {"this", "is", "A", "test"};
    EXPECT_TRUE(writer.WriteRow(row));                           // did we successfully write
    
    CStringDataSink* string_sink = static_cast<CStringDataSink*>(sink.get());
    std::string string = string_sink->String();

    EXPECT_EQ(string, "this,is,A,test");                  // did we write the correct thing
}

TEST(DSVWriter, QuoteallTrueTest) {
    std::shared_ptr<CDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', true);
    std::vector<std::string> row = {"this", "is", "A", "test"};
    EXPECT_TRUE(writer.WriteRow(row));

    CStringDataSink* stringSink = static_cast<CStringDataSink*>(sink.get());
    std::string string = stringSink->String();
    EXPECT_EQ(string, "\"this\",\"is\",\"A\",\"test\"");
}

TEST(DSVWriter, WriteRowMultipleTest) {
    std::shared_ptr<CDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', false);
    std::vector<std::string> row1 = {"ID", "Grades", "Major"};
    std::vector<std::string> row2 = {"12345", "A-", "Computer Science"};
    EXPECT_TRUE(writer.WriteRow(row1));
    EXPECT_TRUE(writer.WriteRow(row2));

    CStringDataSink* stringSink = static_cast<CStringDataSink*>(sink.get());
    std::string string = stringSink->String();
    EXPECT_EQ(string, "ID,Grades,Major\n12345,A-,Computer Science"); 
}

TEST(DSVWriterTest, WriteRowWithEmptyField) {
    std::shared_ptr<CDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', false);

    std::vector<std::string> row = {"a", "", "c"};
    EXPECT_TRUE(writer.WriteRow(row));
    CStringDataSink* stringSink = static_cast<CStringDataSink*>(sink.get());
    std::string string = stringSink->String();
    EXPECT_EQ(string, "a,,c");
}

// test DSVReader
TEST(DSVReader, ReadRowTest) {
    std::string string = "this, is, a, test";
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>(string);
    CDSVReader reader(source, ',');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));     // should return true if successfully read
    EXPECT_EQ(row[0], "this");            
    EXPECT_EQ(row[1], "is");
    EXPECT_EQ(row[2], "a");
    EXPECT_EQ(row[3], "test");

    std::string string2 = "this\tis\ta\ttest";
    std::shared_ptr<CStringDataSource> source2 = std::make_shared<CStringDataSource>(string2);
    CDSVReader reader2(source2, '\t');    // test tab delimiter
    std::vector<std::string> row2;
    EXPECT_TRUE(reader2.ReadRow(row2));    
    EXPECT_EQ(row2[0], "this");            
    EXPECT_EQ(row2[1], "is");
    EXPECT_EQ(row2[2], "a");
    EXPECT_EQ(row2[3], "test");
}

TEST(DSVReader, MultipleReadRowTest) {
    std::string string = "ID, Grades \n 12345, A-\n67890, C";
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>(string);
    CDSVReader reader(source, ',');

    std::vector<std::string> row1;
    EXPECT_TRUE(reader.ReadRow(row1)); 
    EXPECT_EQ(row1[0], "ID");            
    EXPECT_EQ(row1[1], "Grades");

    std::vector<std::string> row2;
    EXPECT_TRUE(reader.ReadRow(row2)); 
    EXPECT_EQ(row2[0], "12345");            
    EXPECT_EQ(row2[1], "A-");

    std::vector<std::string> row3;
    EXPECT_TRUE(reader.ReadRow(row3)); 
    EXPECT_EQ(row3[0], "67890");            
    EXPECT_EQ(row3[1], "C");
}

TEST(DSVReader, EndTest) {
    std::string string = "this,is,a,test";
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>(string);
    CDSVReader reader(source, ',');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));     
    EXPECT_TRUE(reader.End());
    EXPECT_FALSE(reader.ReadRow(row)); // should return false because there are no more rows to read
}

TEST(DSVReader, NuanceTests) {
    // implement test for nuance 1, 2
    std::string string3 = "";  // nuance 3: empty row should return true
    std::shared_ptr<CStringDataSource> source3 = std::make_shared<CStringDataSource>(string3);
    CDSVReader reader3(source3, ',');
    std::vector<std::string> row3;
    EXPECT_TRUE(reader3.ReadRow(row3));

    std::string string4 = "this,is,a,test";
    std::shared_ptr<CStringDataSource> source4 = std::make_shared<CStringDataSource>(string4);
    CDSVReader reader4(source4, '"'); // nuance 5: " as delimiter is interpreted as ,
    std::vector<std::string> row4;
    EXPECT_TRUE(reader4.ReadRow(row4));    
    EXPECT_EQ(row4[0], "this");            
    EXPECT_EQ(row4[1], "is");
    EXPECT_EQ(row4[2], "a");
    EXPECT_EQ(row4[3], "test");
}