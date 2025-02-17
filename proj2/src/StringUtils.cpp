#include "StringUtils.h"

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    std::string ans;
    //Set end to end of string when end == 0
    if (end == 0)
    {
        end = str.length();
    }
    //Translate negative indecies to positive
    else if (end < 0)
    {
        end = end + str.length();
    }
    if (start < 0)
    {
        start = start + str.length();
    }
    //Bad bounds edge case
    if (end - start <= 0)
    {
        return "";
    }
    //Slices str using substring method
    ans = str.substr(start, end-start);
    return ans;
}


std::string Capitalize(const std::string &str) noexcept{
    //Empty string edge case
    if (str.length() == 0)
    {
        return str;
    }
    std::string ans;
    //Capitalize first char
    ans += toupper(str[0]);
    //Lowercase the rest of the string
    for (size_t i = 1; i < str.length(); i++)
    {
        ans += tolower(str[i]);
    }
    
    return ans;
}

std::string Upper(const std::string &str) noexcept{
    //Empty string edge case
    if (str.length() == 0)
    {
        return str;
    }
    std::string ans;
    //Uppercase every char in string
    for (size_t i = 0; i < str.length(); i++)
    {
        ans += toupper(str[i]);
    }
    return ans;
}

std::string Lower(const std::string &str) noexcept{
    //Empty string edge case
    if (str.length() == 0)
    {
        return str;
    }
    std::string ans;
    //Lowercase every char in string
    for (size_t i = 0; i < str.length(); i++)
    {
        ans += tolower(str[i]);
    }
    return ans;
}

std::string LStrip(const std::string &str) noexcept{
    //Empty string edge case
    if (str.length() == 0)
    {
        return str;
    }
    std::string ans;
    //Finds first non white space
    size_t start = str.find_first_not_of(' ');
    //Iterates over string after white space
    for (size_t i = start; i < str.length(); i++)
    {
        ans += str[i];
    }
   return ans;
}

std::string RStrip(const std::string &str) noexcept{
    //Empty string edge case
    if (str.length() == 0)
    {
        return str;
    }
    std::string ans;
    size_t end = str.find_last_not_of(' ');
    //Iterates over string until ending white space
    for (size_t i = 0; i < end+1; i++)
    {
        ans += str[i];
    }
   return ans;
}

std::string Strip(const std::string &str) noexcept{
    //Empty string edge case
    if (str.length() == 0)
    {
        return str;
    }
    std::string ans;
    size_t start = str.find_first_not_of(' ');
    size_t end = str.find_last_not_of(' ');
    //Iterates without left and right white space
    for (size_t i = start; i < end+1; i++)
    {
        ans += str[i];
    }
    return ans;
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    //Small width edge case
    if (width <= str.length())
    {
        return str;
    }
    std::string ans;
    size_t left_char = 0;
    size_t right_char = 0;
    //Number of whitespace chars to add
    size_t extra_char = width - str.length();
    //Even whitespace split evenly left and right
    if (extra_char % 2 == 0)
    {
        left_char = extra_char / 2;
        right_char = extra_char / 2;
    }
    //Not even whitespace, add extra to right
    else{
        left_char = (extra_char-1) / 2;
        right_char = (extra_char+1) / 2;
    }
    //Add the whitespace to left and right of str
    ans = std::string(left_char, fill) + str + std::string(right_char, fill);
    return ans;
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    if (width <= str.length())
    {
        return str;
    }
    std::string ans;
    //Calculate extra whitespace and add to right
    size_t extra_char = width - str.length();
    ans = str + std::string(extra_char, fill);
    return ans;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    if (width <= str.length())
    {
        return str;
    }
    std::string ans;
    //Calculate extra whitespace and add to left
    size_t extra_char = width - str.length();
    ans = std::string(extra_char, fill) + str;
    return ans;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    std::string ans;
    size_t position = 0;
    //Handles edge case old string can't be found
    if (str.find(old) == std::string::npos)
    {
        return str;
    }
    //iterates over string to find all isntances of old and replaces it with rep
    while (str.find(old, position) != std::string::npos)
    {
        size_t start = str.find(old, position);
        ans.append(str.substr(position, start-position) + rep);
        position = start + old.length();
    }
    //adds everything after last old in str
    ans.append(str.substr(position));
    return ans;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
    std::vector<std::string> ans;
    //Split on whitespace if delimiter is empty
    if (splt.empty() == true)  
    {
        std::istringstream stream(str);
        std::string word;
        while (stream >> word) 
        {
            ans.push_back(word);
        }
    } 
    else 
    {
    //Iterates through string to find split char and adds strings to vector
        size_t start = 0;
        size_t end;
        while ((end = str.find(splt, start)) != std::string::npos) 
        {
            ans.push_back(str.substr(start, end - start));
            start = end + splt.length();
        }
    //Add the last segment after last split char
        ans.push_back(str.substr(start)); 
    }
    
    return ans;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    std::string ans;
    //Iterates through vector concatenating strings
    for (std::string string : vect)
    {
        ans += string + str;
    }
    ans = ans.substr(0, ans.length()-str.length());
    return ans;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    std::string ans;
    int column = 0;
    //Iterate over each char in str
    for (char c : str) 
    {
        //If tab character add spaces to reach next tabspace
        if (c == '\t') 
        {
            if (tabsize <= 0)
            {
                column += 1;
            }
            else
            {
                int spaces = tabsize - (column % tabsize);
                ans.append(spaces, ' ');
                column += spaces;
            }
        } 
        else 
        //normal char add to ans and move column over
        {
            ans += c;
            //newline check to reset column
            if (c == '\n')
            {
                column = 0;
            }
            else
            {
                column += 1;
            }
        }
    }
    return ans;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    //For ignorecase calculate edit distance on lowercase left and right
    if (ignorecase == true)
    {
        return EditDistance(StringUtils::Lower(left), StringUtils::Lower(right));
    }
    //Calculate Levenshtein distance using the naive recursive implementation
    if (right.length() == 0)
    {
        return left.length();
    }
    else if(left.length() == 0)
    {
        return right.length();
    }
    else if(left[0] == right[0])
    {
        return EditDistance(left.substr(1), right.substr(1));
    }
    else
    {
        return 1 + std::min(std::min(EditDistance(left.substr(1), right), EditDistance(left, right.substr(1))), EditDistance(left.substr(1), right.substr(1)));
    }
}

}