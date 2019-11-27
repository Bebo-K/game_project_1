#ifndef JSON_H
#define JSON_H


struct JsObject;
struct JsArray;

struct JsString{//allocate new memory and conver to ascii null terminated string
	char*		string;
	int			len;
	
	JsString(char* data,int length);
	void destroy();
	void print(int tabs);
};

struct JsValue{
	int			value_type;
	bool		bool_value;			//type 0
	int 		int_value;			//type 1
	float		float_value;		//type 2
	JsString*	str_value;			//type 3
	JsObject*	obj_value;			//type 4
	JsArray*	array_value;		//type 5
	
	JsValue();
    JsValue(int value);
    JsValue(bool value);
    JsValue(float value);
    JsValue(JsString* value);
    JsValue(JsObject* value);
    JsValue(JsArray* value);

	void destroy();
	void print(int tabs);
};

struct JsArray{
	int _value_slots;
	int	count;
	JsValue* elements;
	
	JsArray();
	void add_value(JsValue* value_data);
	void destroy();
	void print(int tabs);
};

struct JsObject{
	int			_value_slots;
	int			count;
	JsString*	keys;
	JsValue*	values;
	
	JsObject();
	void add_member(JsString* key, JsValue* value_data);
	void destroy();
	void print(int tabs);
};


class JsParser{
public:

	JsParser(char* Js,int length);

	char* data;
	int end;
	
JsObject* 	parse();
JsObject* 	parse_object(int* index);
JsArray* 	parse_array(int* index);
JsValue* 	parse_value(int* index);
JsString* 	parse_string(int* index);//sets index to position of second non-escaped '"'
JsValue* 	parse_number(int* index);//sets index to position of last character digit in number
bool 		parse_boolean(int* index);//sets index to position of last letter of true/false string
JsValue*	parse_null(int* index);//sets index to position of second 'l'

int next_non_whitespace(int start);
int next_instance_of(char token,int start);

};


#endif