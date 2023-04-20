#ifndef JSON_H
#define JSON_H
#include <fstream>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <string>

//Extension of rapidjson
namespace rapidjson {
  //Helper class for reading
  class JsonReader{
    public:
      template<typename T>
    void FromJson(T& obj,rapidjson::Value& value)const
    {
      obj.FromJson(value);
    }
  };
  //Helper class for writeing
  class JsonWriter{
    public:
      template<typename T>
      void ToJson(const T& obj, rapidjson::Value& value,rapidjson::Document::AllocatorType& allocator) const
      {
        obj.ToJson(value,allocator);
      }
  };
}

//Custom wrapper class around rapidjson
class Json{
  public:
  //local variable to store informations
  rapidjson::Document document;
  //blank constructor for now,may be needed for some other stuff ahead
  Json()
  {
  }
  //Simply call with path to read from and stores info in Json::document
  void Read(std::string path)
  {
    std::ifstream json(path);
    if(!json.is_open())
    {
      std::cerr<<"Could not open file to read"<<std::endl;
    }

    rapidjson::IStreamWrapper isw(json);
    document.ParseStream(isw);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};

    document.Accept(writer);
  }
  //Simply call with path to store,Stores the contents of Json::document in the file
  void Write(std::string path)
  {
    std::ofstream ofs(path);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open file for writing" << std::endl;
    }

    // Write the JSON data to the file
    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    document.Accept(writer);

  }
  //function to convert Json to object takes reference of class to convert and class type as template argument
  //Might not work if FromJson function is not implemented in the template class argument
   template<class T>
void ToObject(T& obj)
{
  rapidjson::JsonReader jsonreader;
  rapidjson::Value value(rapidjson::kObjectType);
  value.CopyFrom(document, document.GetAllocator());
  jsonreader.FromJson(obj, value);
}
//function to convert to Json from an object ,takes reference to class and class type for template argument
//Might not work if ToJson not implemented in the template class argument
template<class T>
void FromObject(const T& obj)
{
  rapidjson::JsonWriter jsonwriter;
  rapidjson::Value value(rapidjson::kObjectType);
  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
  jsonwriter.ToJson(obj, value, allocator);
  document.CopyFrom(value, allocator);
}
};
#endif // !JSON_H
