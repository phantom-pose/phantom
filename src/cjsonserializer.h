#pragma once
#include "json/json.h"
#include "ijsonserializable.h"
#include "boundingbox.h"

class CJsonSerializer
{
public:
    static bool Serialize( IJsonSerializable * pObj, std::string & output );
    static bool Serialize( std::vector <IJsonSerializable *> pObjs, std::string & output );
    static bool Deserialize( IJsonSerializable * pObj, std::string & input );
    static bool Deserialize(std::vector<BoundingBox *> & pObjs, std::string filename );
private:
    CJsonSerializer();
};
