#ifndef STRUCTLIST_H
#define STRUCTLIST_H
#define MAXINDEX 2
typedef struct struct_message {
	//char a[32];
	int count;
	int id;
	float humidity;
	float temperature;
	//float distance;
	int co2;
	//String d;
	bool update;
} struct_message;

#endif