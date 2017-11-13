#include "cjsonserializer.h"

bool CJsonSerializer::Serialize( IJsonSerializable * pObj, std::string & output )
{
    if (pObj == NULL)
        return false;

    Json::Value serializeRoot;
    pObj->Serialize(serializeRoot);

    Json::StyledWriter writer;
    output = writer.write( serializeRoot );

    return true;
}

bool CJsonSerializer::Deserialize( IJsonSerializable* pObj, std::string& input )
{
    if (pObj == NULL)
        return false;

    Json::Value deserializeRoot;
    Json::Reader reader;

    if ( !reader.parse(input, deserializeRoot) )
        return false;

    pObj->Deserialize(deserializeRoot);

    return true;
}

bool CJsonSerializer::Serialize( std::vector <IJsonSerializable *> pObjs, std::string & output )
{
    if (pObjs.empty())
        return false;

    int i = 0;
    Json::Value serializeRoot;
    for (auto it = pObjs.begin(); it != pObjs.end(); it++) {
        Json::Value serializePart;
        (*it)->Serialize( serializePart );
        serializeRoot[i] = serializePart;
        i++;
    }
    Json::StyledWriter writer;
    output = writer.write( serializeRoot );

    return true;
}

bool CJsonSerializer::Deserialize( std::vector <BoundingBox *> & pObjs, std::string filename )
{
    std::string input;
    std::ifstream file(filename);
    input.assign( (std::istreambuf_iterator<char>(file) ), (std::istreambuf_iterator<char>() ) );
    file.close();
    Json::Value deserializeRoot;
    Json::Reader reader;
    if ( !reader.parse(input, deserializeRoot) )
        return false;

    for (Json::Value::ArrayIndex i = 0; i != deserializeRoot.size(); i++) {
        BoundingBox * bb = new BoundingBox();
        bb->Deserialize(deserializeRoot[i]);
        pObjs.push_back( bb );
   }
   return true;
}
