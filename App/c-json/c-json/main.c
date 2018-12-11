#include "../js_parser.h"

void main()
{
	char *buffer = "{\"parent1\":[\"array1\",array2],\"parent2\":{\"child1\":\"string1\"}}";

	PJOBJECT root = NULL;

	int i = 0;
	PJVALUE value, value2; PJSTRING str;

	char pt[100] = {0};

	Js_parser_object(&root, buffer, &i);

	value = (PJVALUE)Js_object_get_value(root, "parent1");

	if(value->value_type == JS_ARRAY)
	{
		value2 = (PJVALUE)Js_array_get_value(value->value_data, 0);

		str = (PJSTRING)value2->value_data;	//array1

		Js_memset(pt, 0x0, 100);
		Js_strncpy(pt, str->str_data, str->str_len);

		printf("%s\n", pt);
		
		value2 = (PJVALUE)Js_array_get_value(value->value_data, 1);

		str = (PJSTRING)value2->value_data;	//array2

		
		Js_memset(pt, 0x0, 100);
		Js_strncpy(pt, str->str_data, str->str_len);

		printf("%s\n", pt);

	}

	value = (PJVALUE)Js_object_get_value(root, "parent2");

	if(value->value_type == JS_OBJECT)
	{
		value2 = (PJVALUE)Js_object_get_value(value->value_data, "child1");

		str = (PJSTRING)value2->value_data;

		Js_memset(pt, 0x0, 100);
		Js_strncpy(pt, str->str_data, str->str_len);

		printf("%s\n", pt);
	}

	Js_parser_object_free(root);

	scanf("%d",&i);

}