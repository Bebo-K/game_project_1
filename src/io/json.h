#ifndef JSON_H
#define JSON_H


struct JSONObject;
struct JSONArray;

struct JSONString{//allocate new memory and conver to ascii null terminated string
	char*		string;
	int			len;
	
	JSONString(char* data,int length);
	void Destroy();
	void Print();
	void Print(int tabs);
	bool Equals(const char* str);
};

union JSONData{
	bool		bool_value;			//type 0
	int 		int_value;			//type 1
	float		float_value;		//type 2
	JSONString*	str_value;			//type 3
	JSONObject*	obj_value;			//type 4
	JSONArray*	array_value;		//type 5
};

struct JSONValue{
	int			value_type;
	JSONData		value;
	
	JSONValue();
    JSONValue(int value);
    JSONValue(bool value);
    JSONValue(float value);
    JSONValue(JSONString* value);
    JSONValue(JSONObject* value);
    JSONValue(JSONArray* value);

	int IntValue();
	bool BoolValue();
	float FloatValue();
	JSONString* StringValue();
	JSONObject* ObjectValue();
	JSONArray* ArrayValue();

	void Destroy();
	void Print(int tabs);
};

struct JSONArray{
	int _value_slots;
	int	count;
	JSONValue* elements;
	
	JSONArray();
	void AddValue(JSONValue* value_data);
	JSONValue*  At(int index);
	void Destroy();
	void Print(int tabs);
};

struct JSONObject{
	int			_value_slots;
	int			count;
	JSONString*	keys;
	JSONValue*	values;
	
	JSONObject();
	void AddMember(JSONString* key, JSONValue* value_data);
	JSONValue* Get(const char* key);
	JSONValue* &operator[](const char*);

	bool HasInt(const char* key);
	int GetInt(const char* key);

	bool HasBool(const char* key);
	bool GetBool(const char* key);
	
	bool HasFloat(const char* key);
	float GetFloat(const char* key);

	bool HasString(const char* key);
	JSONString* GetString(const char* key);
	
	bool HasJObject(const char* key);//curse you winapi!
	JSONObject* GetJObject(const char* key);

	bool HasArray(const char* key);
	JSONArray* GetArray(const char* key);

	void Destroy();
	void Print(int tabs);
};

class JSONParser{
public:

	JSONParser(char* JSON,int length);

	char* data;
	int end;
	
	JSONObject* 	Parse();
	JSONValue* 		ParseValue(int* index);
	JSONObject* 	ParseObject(int* index);//sets index to position of ending '}'
	JSONArray* 		ParseArray(int* index);//sets index to position of ending ']'
	JSONString* 	ParseString(int* index);//sets index to position of second non-escaped '"'
	JSONValue* 		ParseNumber(int* index);//sets index to position of last character digit in number
	bool 			ParseBoolean(int* index);//sets index to position of last letter of true/false string
	JSONValue*		ParseNull(int* index);//sets index to position of second 'l'

	int NextNonWhitespace(int start);
	int NextInstanceOf(char token,int start);
};

#endif