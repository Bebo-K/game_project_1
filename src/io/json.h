#ifndef JSON_H
#define JSON_H


struct JSONObject;
struct JSONArray;

struct JSONString{//allocate new memory and conver to ascii null terminated string
	char*		string;
	int			len;
	
	JSONString(char* data,int length);
	void destroy();
	void print();
	void print(int tabs);
	bool equals(const char* str);
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

	int intValue();
	bool boolValue();
	float floatValue();
	JSONString* stringValue();
	JSONObject* objectValue();
	JSONArray* arrayValue();

	void destroy();
	void print(int tabs);
};

struct JSONArray{
	int _value_slots;
	int	count;
	JSONValue* elements;
	
	JSONArray();
	void add_value(JSONValue* value_data);
	JSONValue* at(int index);
	void destroy();
	void print(int tabs);
};

struct JSONObject{
	int			_value_slots;
	int			count;
	JSONString*	keys;
	JSONValue*	values;
	
	JSONObject();
	void add_member(JSONString* key, JSONValue* value_data);
	JSONValue* get(const char* key);

	bool hasInt(const char* key);
	int getInt(const char* key);

	bool hasBool(const char* key);
	bool getBool(const char* key);
	
	bool hasFloat(const char* key);
	float getFloat(const char* key);

	bool hasString(const char* key);
	JSONString* getString(const char* key);
	
	bool hasObject(const char* key);
	JSONObject* getObject(const char* key);

	bool hasArray(const char* key);
	JSONArray* getArray(const char* key);

	void destroy();
	void print(int tabs);
};

class JSONParser{
public:

	JSONParser(char* JSON,int length);

	char* data;
	int end;
	
	JSONObject* 	parse();
	JSONValue* 		parse_value(int* index);
	JSONObject* 	parse_object(int* index);//sets index to position of ending '}'
	JSONArray* 		parse_array(int* index);//sets index to position of ending ']'
	JSONString* 	parse_string(int* index);//sets index to position of second non-escaped '"'
	JSONValue* 		parse_number(int* index);//sets index to position of last character digit in number
	bool 			parse_boolean(int* index);//sets index to position of last letter of true/false string
	JSONValue*		parse_null(int* index);//sets index to position of second 'l'

	int next_non_whitespace(int start);
	int next_instance_of(char token,int start);
};

#endif