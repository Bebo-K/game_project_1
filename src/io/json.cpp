#include "json.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------Js STRING
JsString::JsString(char* data,int length){
	string = (char*)calloc(length+1,1);
	memcpy(string,data,length);
	string[length]=0;
	len=length;
}
void JsString::destroy(){
	free(string);
}
void JsString::print(int tabs){
	log("'%s'",string);
}

//------------------------------------------------------------------------------Js VALUE
JsValue::JsValue(){
	value_type = -1;
	bool_value = false;
	int_value = 0;
	float_value = 0;
	str_value = nullptr;
	obj_value = nullptr;
	array_value = nullptr;
}
JsValue::JsValue(bool value) : JsValue() {bool_value = value;value_type = 0;}
JsValue::JsValue(int value) : JsValue() {int_value = value;value_type = 1;}
JsValue::JsValue(float value) : JsValue() {float_value = value;value_type = 2;}
JsValue::JsValue(JsString* value)  : JsValue() {str_value = value;value_type = 3;}
JsValue::JsValue(JsObject* value)  : JsValue() {obj_value = value;value_type = 4;}
JsValue::JsValue(JsArray* value) : JsValue() {array_value = value;value_type = 5;}

void JsValue::destroy(){
	if(value_type==3){str_value->destroy();free(str_value);}
	if(value_type==4){obj_value->destroy();free(obj_value);}
	if(value_type==5){array_value->destroy();free(array_value);}
}
void JsValue::print(int tabs){
	switch(value_type){
		case 0:	
			if(bool_value){ log("true "); }
			else{ log("false "); }
		break;
		case 1: log("%d ",int_value);
		break;
		case 2: log("%f ",float_value);
		break;
		case 3: str_value->print(tabs);
		break;
		case 4: obj_value->print(tabs);
		break;
		case 5: array_value ->print(tabs);
		break;
		case 6:log("null ");
		break;
		default:log("ERROR_VALUE ");
		break;
	}
}

//------------------------------------------------------------------------------Js ARRAY
JsArray::JsArray(){
	_value_slots=1;
	count=0;
	elements = (JsValue*)calloc(_value_slots,sizeof(JsValue));
}
void JsArray::add_value(JsValue* value_data){
	if((count + 1) > _value_slots){
		_value_slots *= 2;
		JsValue*	new_elements = (JsValue*)calloc(_value_slots,sizeof(JsValue));
		memcpy(new_elements,elements,sizeof(JsValue)*count);	
		free(elements);
		elements = new_elements;
	}
	memcpy(&elements[count],value_data,sizeof(JsValue));	
	count += 1;
}
void JsArray::destroy(){
	for(int i=0;i<count;i++){
		elements[i].destroy();
	}
	free(elements);
}
void JsArray::print(int tabs){
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

//------------------------------------------------------------------------------Js OBJECT
JsObject::JsObject(){
	_value_slots=1;
	count=0;
	keys = (JsString*)calloc(_value_slots,sizeof(JsString));
	values = (JsValue*)calloc(_value_slots,sizeof(JsValue));
}
void JsObject::add_member(JsString* key, JsValue* value_data){
	if((count + 1) > _value_slots){
		_value_slots *= 2;
		JsString*	new_keys = (JsString*)calloc(_value_slots,sizeof(JsString));
		JsValue*	new_values = (JsValue*)calloc(_value_slots,sizeof(JsValue));
		
		memcpy(new_keys,keys,sizeof(JsString)*count);
		memcpy(new_values,values,sizeof(JsValue)*count);	

		free(keys);
		free(values);
		keys = new_keys;
		values = new_values;
	}

	memcpy(&keys[count],key,sizeof(JsString));
	memcpy(&values[count],value_data,sizeof(JsValue));	

	count += 1;
}
void JsObject::destroy(){
	for(int i=0;i<count;i++){
		keys[i].destroy();
		values[i].destroy();
	}
	free(keys);
	free(values);
}
void JsObject::print(int tabs){
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

//------------------------------------------------------------------------------Js PARSER
JsParser::JsParser(char* Js,int length){
	data = Js;
	end=length;
}

int JsParser::next_instance_of(char token,int start){
	int index = start;
	while(data[index] != token && index < end){index++;}
	if(index >= end){
		log("Error @ JsParser::next_instance_of -> End of stream reached looking for %c",token);
	}
	return index;
}

int JsParser::next_non_whitespace(int start){
	int index = start;
	bool whitespace_only = true;
	char next_char;
	while(whitespace_only){
		next_char = data[index];
		if(next_char != ' ' && next_char != '\t' && next_char != '\n' && next_char != '\r'){whitespace_only = false;}
		else{index++;}
	}
	if(index >= end){
		log("Error @ JsParser::next_non_whitespace -> End of stream reached looking for non_whitespace");
	}
	return index;
}


JsObject* JsParser::parse(){
int initial_mark=0;
return parse_object(&initial_mark);
}


JsObject* JsParser::parse_object(int* index){
	JsObject* new_object = new JsObject();

	int pos = next_instance_of('{',*index) +1;
	char token = 0;

	while(token != '}'){
		pos = next_non_whitespace(pos);
		token = data[pos];
		
		switch(token){
			case '\'':
			case '"': {
				JsString* key = parse_string(&pos);
				pos = next_instance_of(':',pos)+1; 
				JsValue* value = parse_value(&pos);
				new_object->add_member(key,value);
				pos++;
				break;
			}
			case '}': break;
			case ',': pos ++;break;
			default:
				log("Error @ JsParser::parse_object -> Parsing error at index %d",pos);
				return nullptr;
		}
	}
	*index = pos;
	return new_object;
}

JsArray* JsParser::parse_array(int* index){
	JsArray* new_array = new JsArray();

	int pos = next_instance_of('[',*index) +1;
	char token = 0;

	while(token != ']'){
		pos = next_non_whitespace(pos);
		token = data[pos];
		
		if(token == ','){pos++;}
		if(token == ']')break;
		else{
			JsValue* value = parse_value(&pos);
			new_array->add_value(value);
			pos++;
		}
	}
	*index = pos;
	return new_array;
}
JsValue* JsParser::parse_value(int* index){
	JsValue* new_value = nullptr;

	int pos = next_non_whitespace(*index);
	char token = data[pos];

	if(token == '\'' || token == '"'){
		JsString* str = parse_string(&pos);
		new_value = new JsValue(str);
		pos++;
	}
	else if((token >= '0' && token <= '9')|| token =='-'){
		new_value = parse_number(&pos);
		pos++;
	}
	else if(token == 't' || token =='T' || token == 'f' || token =='F'){
		bool val = parse_boolean(&pos);
		new_value = new JsValue(val);
		pos++;
	}	
	else if(token == '{'){
		JsObject* obj = parse_object(&pos);
		new_value = new JsValue(obj);
		pos++;
	}
	else if(token == '['){
		JsArray* array = parse_array(&pos);
		new_value = new JsValue(array);
		pos++;
	}
	else if(token == 'n' || token =='N'){
		new_value = parse_null(&pos);
	}
	else{
		log("Error @ JsParser::parse_value -> Undefined value at index %d, first character %c",*index,token);
	}

	*index = pos;
	return new_value;
}
JsString* JsParser::parse_string(int* index){
	int pos = *index;
	char quote = data[pos];

	int begin_index = pos+1;
	int end_index = pos+1;
	while(data[end_index] != quote || data[end_index-1] == '\\'){end_index++;}
	
	int string_length = (end_index-begin_index);
	
	*index=end_index;
	return new JsString(&data[begin_index],string_length);
}
JsValue* JsParser::parse_number(int* index){
	JsValue* new_value = nullptr;
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
		new_value = new JsValue(ivalue);
	}
	else{
		float fvalue = strtof(&data[*index],nullptr);
		new_value = new JsValue(fvalue);
	}
	
	*index = end_index;
	return new_value;
}

bool JsParser::parse_boolean(int* index){
	char token = data[*index];
	bool result = false;
	if(token == 't' ||token == 'T'){result=true; *index += 4;}//true
	if(token == 'f' ||token == 'F'){result=false;*index += 5;}//false
	return result;
}
JsValue* JsParser::parse_null(int* index){
	*index += 4;
	JsValue* nullval = new JsValue();
	nullval->value_type=6;
	return nullval;
}

