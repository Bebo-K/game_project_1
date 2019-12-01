#include "json.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

const char* JSONTYPE_BOOL = "bool";
const char* JSONTYPE_INT = "int";
const char* JSONTYPE_FLOAT = "float";
const char* JSONTYPE_STRING = "string";
const char* JSONTYPE_OBJECT = "object";
const char* JSONTYPE_ARRAY = "array";

const char* JSON_TYPE_STRINGS[] = {JSONTYPE_BOOL,JSONTYPE_INT,JSONTYPE_FLOAT,JSONTYPE_STRING,JSONTYPE_OBJECT,JSONTYPE_ARRAY};

//------------------------------------------------------------------------------JSON STRING
JSONString::JSONString(char* data,int length){
	string = (char*)calloc(length+1,1);
	memcpy(string,data,length);
	string[length]=0;
	len=length;
}
void JSONString::destroy(){
	free(string);
}
void JSONString::print(int tabs){
	log("'%s'",string);
}
void JSONString::print(){
	log("%s",string);
}
bool JSONString::equals(const char* str){
	for(int i=0;i< len;i++){
		if(str[i] != string[i])return false;
	}
	return true;
}

//------------------------------------------------------------------------------JSON VALUE
JSONValue::JSONValue(){
	value_type = -1;
	value.int_value=0;
}
JSONValue::JSONValue(bool bvalue) : JSONValue() {value.bool_value = bvalue;value_type = 0;}
JSONValue::JSONValue(int ivalue) : JSONValue() {value.int_value = ivalue;value_type = 1;}
JSONValue::JSONValue(float fvalue) : JSONValue() {value.float_value = fvalue;value_type = 2;}
JSONValue::JSONValue(JSONString* svalue)  : JSONValue() {value.str_value = svalue;value_type = 3;}
JSONValue::JSONValue(JSONObject* ovalue)  : JSONValue() {value.obj_value = ovalue;value_type = 4;}
JSONValue::JSONValue(JSONArray* avalue) : JSONValue() {value.array_value = avalue;value_type = 5;}

bool JSONValue::boolValue(){
	if(value_type ==0){return value.bool_value;}
	else{log("Error @ JSONValue::boolValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);return false;}
}
int JSONValue::intValue(){
	if(value_type ==1){return value.int_value;}
	else{log("Error @ JSONValue::intValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);return 0;}
}
float JSONValue::floatValue(){
	if(value_type ==2){return value.float_value;}
	else{log("Error @ JSONValue::floatValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);return 0;}
}
JSONString* JSONValue::stringValue(){
	if(value_type ==3){return value.str_value;}
	else{log("Error @ JSONValue::stringValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);return nullptr;}
}
JSONObject* JSONValue::objectValue(){
	if(value_type ==4){return value.obj_value;}
	else{log("Error @ JSONValue::objectValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);return nullptr;}
}
JSONArray* JSONValue::arrayValue(){
	if(value_type ==5){return value.array_value;}
	else{log("Error @ JSONValue::arrayValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);return nullptr;}
}

void JSONValue::destroy(){
	if(value_type==3){value.str_value->destroy();free(value.str_value);}
	if(value_type==4){value.obj_value->destroy();free(value.obj_value);}
	if(value_type==5){value.array_value->destroy();free(value.array_value);}
}
void JSONValue::print(int tabs){
	switch(value_type){
		case 0:	
			if(value.bool_value){ log("true "); }
			else{ log("false "); }
		break;
		case 1: log("%d ",value.int_value);
		break;
		case 2: log("%f ",value.float_value);
		break;
		case 3: value.str_value->print(tabs);
		break;
		case 4: value.obj_value->print(tabs);
		break;
		case 5: value.array_value ->print(tabs);
		break;
		case 6:log("null ");
		break;
		default:log("ERROR_VALUE ");
		break;
	}
}

//------------------------------------------------------------------------------JSON ARRAY
JSONArray::JSONArray(){
	_value_slots=1;
	count=0;
	elements = (JSONValue*)calloc(_value_slots,sizeof(JSONValue));
}
void JSONArray::add_value(JSONValue* value_data){
	if((count + 1) > _value_slots){
		_value_slots *= 2;
		JSONValue*	new_elements = (JSONValue*)calloc(_value_slots,sizeof(JSONValue));
		memcpy(new_elements,elements,sizeof(JSONValue)*count);	
		free(elements);
		elements = new_elements;
	}
	memcpy(&elements[count],value_data,sizeof(JSONValue));	
	count += 1;
}
void JSONArray::destroy(){
	for(int i=0;i<count;i++){
		elements[i].destroy();
	}
	free(elements);
}
void JSONArray::print(int tabs){
	log("[\n");
	for(int i=0;i<count;i++){
		indent_log(tabs+1);
		elements[i].print(tabs+1);
		if(i+1 < count){log(",\n");}
		else{log("\n");}
	}
	indent_log(tabs);
	log("]");
}
JSONValue* JSONArray::at(int index){
	if(index < 0 || index > count){
	log("Error @ JSONArray::at -> index %d is invalid",index);
		return nullptr;
	}
	else return &elements[index];
}

//------------------------------------------------------------------------------JSON OBJECT
JSONObject::JSONObject(){
	_value_slots=1;
	count=0;
	keys = (JSONString*)calloc(_value_slots,sizeof(JSONString));
	values = (JSONValue*)calloc(_value_slots,sizeof(JSONValue));
}
void JSONObject::add_member(JSONString* key, JSONValue* value_data){
	if((count + 1) > _value_slots){
		_value_slots *= 2;
		JSONString*	new_keys = (JSONString*)calloc(_value_slots,sizeof(JSONString));
		JSONValue*	new_values = (JSONValue*)calloc(_value_slots,sizeof(JSONValue));
		
		memcpy(new_keys,keys,sizeof(JSONString)*count);
		memcpy(new_values,values,sizeof(JSONValue)*count);	

		free(keys);
		free(values);
		keys = new_keys;
		values = new_values;
	}

	memcpy(&keys[count],key,sizeof(JSONString));
	memcpy(&values[count],value_data,sizeof(JSONValue));	

	count += 1;
}
JSONValue* JSONObject::get(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].equals(key)){return &values[i];}
	}
	log("Error @ JSONObject::get-> key '%s' not found.",key);
	return nullptr;
}
int JSONObject::getInt(const char* key){return get(key)->intValue();}
bool JSONObject::getBool(const char* key){return get(key)->boolValue();}
float JSONObject::getFloat(const char* key){return get(key)->floatValue();}
JSONString* JSONObject::getString(const char* key){return get(key)->stringValue();}
JSONObject* JSONObject::getObject(const char* key){return get(key)->objectValue();}
JSONArray* JSONObject::getArray(const char* key){return get(key)->arrayValue();}

bool JSONObject::hasInt(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].equals(key) && values[i].value_type==1){return true;}
	}
	return false;
}
bool JSONObject::hasBool(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].equals(key) && values[i].value_type==0){return true;}
	}
	return false;
}
bool JSONObject::hasFloat(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].equals(key) && values[i].value_type==2){return true;}
	}
	return false;
}
bool JSONObject::hasString(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].equals(key) && values[i].value_type==3){return true;}
	}
	return false;
}
bool JSONObject::hasObject(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].equals(key) && values[i].value_type==4){return true;}
	}
	return false;
}
bool JSONObject::hasArray(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].equals(key) && values[i].value_type==5){return true;}
	}
	return false;
}

void JSONObject::destroy(){
	for(int i=0;i<count;i++){
		keys[i].destroy();
		values[i].destroy();
	}
	free(keys);
	free(values);
}
void JSONObject::print(int tabs){
	log("{\n");
	indent_log(tabs);
	for(int i=0;i<count;i++){
		keys[i].print(tabs+1);
		log(":");
		values[i].print(tabs+1);
		if(i+1 < count){log(",\n");indent_log(tabs);}
	}
	log("\n");indent_log(tabs);
	log("}\n");
}

//------------------------------------------------------------------------------JSON PARSER
JSONParser::JSONParser(char* JSON,int length){
	data = JSON;
	end=length;
}

int JSONParser::next_instance_of(char token,int start){
	int index = start;
	while(data[index] != token && index < end){index++;}
	if(index >= end){
		log("Error @ JSONParser::next_instance_of -> End of stream reached looking for %c",token);
	}
	return index;
}

int JSONParser::next_non_whitespace(int start){
	int index = start;
	bool whitespace_only = true;
	char next_char;
	while(whitespace_only){
		next_char = data[index];
		if(next_char != ' ' && next_char != '\t' && next_char != '\n' && next_char != '\r'){whitespace_only = false;}
		else{index++;}
	}
	if(index >= end){
		log("Error @ JSONParser::next_non_whitespace -> End of stream reached looking for non_whitespace");
	}
	return index;
}


JSONObject* JSONParser::parse(){
int initial_mark=0;
return parse_object(&initial_mark);
}


JSONObject* JSONParser::parse_object(int* index){
	JSONObject* new_object = new JSONObject();

	int pos = next_instance_of('{',*index) +1;
	char token = 0;

	while(token != '}'){
		pos = next_non_whitespace(pos);
		token = data[pos];
		
		switch(token){
			case '\'':
			case '"': {
				JSONString* key = parse_string(&pos);
				pos = next_instance_of(':',pos)+1; 
				JSONValue* value = parse_value(&pos);
				new_object->add_member(key,value);
				break;
			}
			case '}': break;
			case ',': pos ++;break;
			default:
				log("Error @ JSONParser::parse_object -> Parsing error at index %d",pos);
				return nullptr;
		}
	}
	*index = pos;
	return new_object;
}

JSONArray* JSONParser::parse_array(int* index){
	JSONArray* new_array = new JSONArray();

	int pos = next_instance_of('[',*index) +1;
	char token = 0;

	while(token != ']'){
		pos = next_non_whitespace(pos);
		token = data[pos];
		
		if(token == ','){pos++;}
		if(token == ']')break;
		else{
			JSONValue* value = parse_value(&pos);
			new_array->add_value(value);
			//pos++;
		}
	}
	*index = pos;
	return new_array;
}
JSONValue* JSONParser::parse_value(int* index){
	JSONValue* new_value = nullptr;

	int pos = next_non_whitespace(*index);
	char token = data[pos];

	if(token == '\'' || token == '"'){
		JSONString* str = parse_string(&pos);
		new_value = new JSONValue(str);
	}
	else if((token >= '0' && token <= '9')|| token =='-'){
		new_value = parse_number(&pos);
	}
	else if(token == 't' || token =='T' || token == 'f' || token =='F'){
		bool val = parse_boolean(&pos);
		new_value = new JSONValue(val);
	}	
	else if(token == '{'){
		JSONObject* obj = parse_object(&pos);
		new_value = new JSONValue(obj);
	}
	else if(token == '['){
		JSONArray* array = parse_array(&pos);
		new_value = new JSONValue(array);
	}
	else if(token == 'n' || token =='N'){
		new_value = parse_null(&pos);
	}
	else{
		log("Error @ JSONParser::parse_value -> Undefined value at index %d, first character %c",*index,token);
	}

	pos++;
	*index = pos;
	return new_value;
}
JSONString* JSONParser::parse_string(int* index){
	int pos = *index;
	char quote = data[pos];

	int begin_index = pos+1;
	int end_index = pos+1;
	while(data[end_index] != quote || data[end_index-1] == '\\'){end_index++;}
	
	int string_length = (end_index-begin_index);
	
	*index=end_index;
	return new JSONString(&data[begin_index],string_length);
}
JSONValue* JSONParser::parse_number(int* index){
	JSONValue* new_value = nullptr;
	int pos = *index;

	bool integer = true;
	int end_index = -1;
	char token;

	while (end_index < 0){
		token = data[pos];
		if(token == '-' || (token >= '0' && token <= '9')||token == 'e' || token == 'E' ){pos++;}
		else if(token == '.'){integer=false;pos++;}
		else{end_index=pos-1;}
	}

	if(integer){
		int ivalue = atoi(&data[*index]);
		new_value = new JSONValue(ivalue);
	}
	else{
		float fvalue = strtof(&data[*index],nullptr);
		new_value = new JSONValue(fvalue);
	}
	
	*index = end_index;
	return new_value;
}
bool JSONParser::parse_boolean(int* index){
	char token = data[*index];
	bool result = false;
	if(token == 't' ||token == 'T'){result=true; *index += 4;}//true
	if(token == 'f' ||token == 'F'){result=false;*index += 5;}//false
	return result;
}
JSONValue* JSONParser::parse_null(int* index){
	*index += 3;
	JSONValue* nullval = new JSONValue();
	nullval->value_type=6;
	return nullval;
}

