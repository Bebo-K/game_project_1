#include "json.h"
#include "../log.h"
#include <stdlib.h>
#include <string.h>

const char* JSONTYPE_BOOL = "bool";
const char* JSONTYPE_INT = "int";
const char* JSONTYPE_FLOAT = "float";
const char* JSONTYPE_STRING = "string";
const char* JSONTYPE_OBJECT = "object";
const char* JSONTYPE_ARRAY = "array";

const char* JSON_TYPE_STRINGS[] = {JSONTYPE_BOOL,JSONTYPE_INT,JSONTYPE_FLOAT,JSONTYPE_STRING,JSONTYPE_OBJECT,JSONTYPE_ARRAY};


//utility print method
void indent_log(int tabs){
  for(int i=0;i<tabs;i++){
    printf("    ");
  }
}

//------------------------------------------------------------------------------JSON STRING
JSONString::JSONString(char* data,int length){
	string = (char*)calloc(length+1,1);
	memcpy(string,data,length);
	string[length]=0;
	len=length;
}
void JSONString::Destroy(){
	free(string);
}
void JSONString::Print(int tabs){
	printf("'%s'",string);
}
void JSONString::Print(){
	printf("%s",string);
}
bool JSONString::Equals(const char* str){
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

bool JSONValue::BoolValue(){
	if(value_type ==0){return value.bool_value;}
	else{logger::exception("JSONValue::boolValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);return false;}
}
int JSONValue::IntValue(){
	if(value_type ==1){return value.int_value;}
	else{logger::exception("JSONValue::intValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);return 0;}
}
float JSONValue::FloatValue(){
	if(value_type ==2){return value.float_value;}
	else{
		logger::exception("JSONValue::floatValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);
		return 0;
	}
}
JSONString* JSONValue::StringValue(){
	if(value_type ==3){return value.str_value;}
	else{
		logger::exception("JSONValue::stringValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);
		return nullptr;
	}
}
JSONObject* JSONValue::ObjectValue(){
	if(value_type ==4){return value.obj_value;}
	else{
		logger::exception("JSONValue::objectValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);
		return nullptr;
	}
}
JSONArray* JSONValue::ArrayValue(){
	if(value_type ==5){return value.array_value;}
	else{
		logger::exception("JSONValue::arrayValue -> invalid access of data type: %s",JSON_TYPE_STRINGS[value_type]);
		return nullptr;
	}
}

void JSONValue::Destroy(){
	if(value_type==3){value.str_value->Destroy();free(value.str_value);}
	if(value_type==4){value.obj_value->Destroy();free(value.obj_value);}
	if(value_type==5){value.array_value->Destroy();free(value.array_value);}
}
void JSONValue::Print(int tabs){
	switch(value_type){
		case 0:	
			if(value.bool_value){ printf("true "); }
			else{ printf("false "); }
		break;
		case 1: printf("%d ",value.int_value);
		break;
		case 2: printf("%f ",value.float_value);
		break;
		case 3: value.str_value->Print(tabs);
		break;
		case 4: value.obj_value->Print(tabs);
		break;
		case 5: value.array_value->Print(tabs);
		break;
		case 6:printf("null ");
		break;
		default:printf("ERROR_VALUE ");
		break;
	}
}

//------------------------------------------------------------------------------JSON ARRAY
JSONArray::JSONArray(){
	_value_slots=1;
	count=0;
	elements = (JSONValue*)calloc(_value_slots,sizeof(JSONValue));
}
void JSONArray::AddValue(JSONValue* value_data){
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
void JSONArray::Destroy(){
	for(int i=0;i<count;i++){
		elements[i].Destroy();
	}
	free(elements);
}
void JSONArray::Print(int tabs){
	printf("[\n");
	for(int i=0;i<count;i++){
		indent_log(tabs+1);
		elements[i].Print(tabs+1);
		if(i+1 < count){printf(",\n");}
		else{printf("\n");}
	}
	indent_log(tabs);
	printf("]");
}
JSONValue* JSONArray::At(int index){
	if(index < 0 || index > count){
		logger::exception("JSONArray::at -> index %d is invalid",index);
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
void JSONObject::AddMember(JSONString* key, JSONValue* value_data){
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
JSONValue* JSONObject::Get(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].Equals(key)){return &values[i];}
	}
	logger::warn("JSONObject::get-> key '%s' not found.",key);
	return nullptr;
}
int JSONObject::GetInt(const char* key){return Get(key)->IntValue();}
bool JSONObject::GetBool(const char* key){return Get(key)->BoolValue();}
float JSONObject::GetFloat(const char* key){return Get(key)->FloatValue();}
JSONString* JSONObject::GetString(const char* key){return Get(key)->StringValue();}
JSONObject* JSONObject::GetObject(const char* key){return Get(key)->ObjectValue();}
JSONArray* JSONObject::GetArray(const char* key){return Get(key)->ArrayValue();}

bool JSONObject::HasInt(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].Equals(key) && values[i].value_type==1){return true;}
	}
	return false;
}
bool JSONObject::HasBool(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].Equals(key) && values[i].value_type==0){return true;}
	}
	return false;
}
bool JSONObject::HasFloat(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].Equals(key) && values[i].value_type==2){return true;}
	}
	return false;
}
bool JSONObject::HasString(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].Equals(key) && values[i].value_type==3){return true;}
	}
	return false;
}
bool JSONObject::HasObject(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].Equals(key) && values[i].value_type==4){return true;}
	}
	return false;
}
bool JSONObject::HasArray(const char* key){
	for(int i=0; i< count; i++){
		if(keys[i].Equals(key) && values[i].value_type==5){return true;}
	}
	return false;
}

void JSONObject::Destroy(){
	for(int i=0;i<count;i++){
		keys[i].Destroy();
		values[i].Destroy();
	}
	free(keys);
	free(values);
}
void JSONObject::Print(int tabs){
	printf("{\n");
	indent_log(tabs);
	for(int i=0;i<count;i++){
		keys[i].Print(tabs+1);
		printf(":");
		values[i].Print(tabs+1);
		if(i+1 < count){printf(",\n");indent_log(tabs);}
	}
	printf("\n");indent_log(tabs);
	printf("}\n");
}

//------------------------------------------------------------------------------JSON PARSER
JSONParser::JSONParser(char* JSON,int length){
	data = JSON;
	end=length;
}

int JSONParser::NextInstanceOf(char token,int start){
	int index = start;
	while(data[index] != token && index < end){index++;}
	if(index >= end){
		logger::exception("JSONParser::next_instance_of -> End of stream reached looking for %c",token);
	}
	return index;
}

int JSONParser::NextNonWhitespace(int start){
	int index = start;
	bool whitespace_only = true;
	char next_char;
	while(whitespace_only){
		next_char = data[index];
		if(next_char != ' ' && next_char != '\t' && next_char != '\n' && next_char != '\r'){whitespace_only = false;}
		else{index++;}
	}
	if(index >= end){
		logger::exception("JSONParser::next_non_whitespace -> End of stream reached looking for non_whitespace");
	}
	return index;
}


JSONObject* JSONParser::Parse(){
int initial_mark=0;
return ParseObject(&initial_mark);
}


JSONObject* JSONParser::ParseObject(int* index){
	JSONObject* new_object = new JSONObject();

	int pos = NextInstanceOf('{',*index) +1;
	char token = 0;

	while(token != '}'){
		pos = NextNonWhitespace(pos);
		token = data[pos];
		
		switch(token){
			case '\'':
			case '"': {
				JSONString* key = ParseString(&pos);
				pos = NextInstanceOf(':',pos)+1; 
				JSONValue* value = ParseValue(&pos);
				new_object->AddMember(key,value);
				break;
			}
			case '}': break;
			case ',': pos ++;break;
			default:
				logger::exception("JSONParser::parse_object -> Parsing error at index %d",pos);
				return nullptr;
		}
	}
	*index = pos;
	return new_object;
}

JSONArray* JSONParser::ParseArray(int* index){
	JSONArray* new_array = new JSONArray();

	int pos = NextInstanceOf('[',*index) +1;
	char token = 0;

	while(token != ']'){
		pos = NextNonWhitespace(pos);
		token = data[pos];
		
		if(token == ','){pos++;}
		if(token == ']')break;
		else{
			JSONValue* value = ParseValue(&pos);
			new_array->AddValue(value);
		}
	}
	*index = pos;
	return new_array;
}
JSONValue* JSONParser::ParseValue(int* index){
	JSONValue* new_value = nullptr;

	int pos = NextNonWhitespace(*index);
	char token = data[pos];

	if(token == '\'' || token == '"'){
		JSONString* str = ParseString(&pos);
		new_value = new JSONValue(str);
	}
	else if((token >= '0' && token <= '9')|| token =='-'){
		new_value = ParseNumber(&pos);
	}
	else if(token == 't' || token =='T' || token == 'f' || token =='F'){
		bool val = ParseBoolean(&pos);
		new_value = new JSONValue(val);
	}	
	else if(token == '{'){
		JSONObject* obj = ParseObject(&pos);
		new_value = new JSONValue(obj);
	}
	else if(token == '['){
		JSONArray* array = ParseArray(&pos);
		new_value = new JSONValue(array);
	}
	else if(token == 'n' || token =='N'){
		new_value = ParseNull(&pos);
	}
	else{
		logger::exception("JSONParser::parse_value -> Undefined value at index %d, first character %c",*index,token);
	}

	pos++;
	*index = pos;
	return new_value;
}
JSONString* JSONParser::ParseString(int* index){
	int pos = *index;
	char quote = data[pos];

	int begin_index = pos+1;
	int end_index = pos+1;
	while(data[end_index] != quote || data[end_index-1] == '\\'){end_index++;}
	
	int string_length = (end_index-begin_index);
	
	*index=end_index;
	return new JSONString(&data[begin_index],string_length);
}
JSONValue* JSONParser::ParseNumber(int* index){
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
bool JSONParser::ParseBoolean(int* index){
	char token = data[*index];
	bool result = false;
	if(token == 't' ||token == 'T'){result=true; *index += 4;}//true
	if(token == 'f' ||token == 'F'){result=false;*index += 5;}//false
	return result;
}
JSONValue* JSONParser::ParseNull(int* index){
	*index += 3;
	JSONValue* nullval = new JSONValue();
	nullval->value_type=6;
	return nullval;
}

