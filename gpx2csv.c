#include <stdio.h>  
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif

enum{
	GEO_NOTDEFINED,
	GEO_POINT,
	GEO_POLY,
	GEO_LINE
};
//#define POI 0
//#define POLY 1

#ifndef bool
#define bool int
#endif

struct pointList
{
	long double lat;
	long double lon;
	int id;
	bool link;
	struct pointList *next;
};

static void print_element_names(xmlNode * a_node);  
int parsing_GeoObject(xmlNode * a_node);
int parsing_posList(xmlChar * text);
void free_posList(struct pointList *List);

struct pointList *posList=0;
int posList_num=0;
struct pointList **posList_last=&posList;
struct pointList *posList_cur=0;
int cur_global_id=-1;
char *Filename = NULL;  
char *linename = NULL;  
char *type = NULL;  

int main(int argv, char **argc)  
{  
		xmlDoc         *doc = NULL;  
		xmlNode        *root_element = NULL;  
		Filename = argc[1];  
		int index;

		// Берём тип содержимого:
		int filename_size=strlen(Filename);
		// Ищем первый подчёрк:
		for(index=filename_size-1;index;index--)
		{
			if(*(Filename+index)=='_')
			{
#ifdef DEBUG
				fprintf(stderr,"filename_size=%i, index=%i\n", filename_size, index);  
#endif
				// Формируем имя линии:
				int size=index;
				linename=(char*)malloc(size+1);
				//memset(linename,0,size+1);
				strncpy(linename,Filename,size);
#ifdef DEBUG
				fprintf(stderr,"linename: %s\n", linename);  
#endif
				// Формируем тип:
				size=filename_size-index-1-strlen(".gpx");;
				type=(char*)malloc(size+1);
				strncpy(type,Filename+index+1,size);
#ifdef DEBUG
				fprintf(stderr,"type: %s\n", type);  
#endif
				break;
			}
		}
		if(!index)
		{
			// Не нашли подчерк. Неверное имя файла
			fprintf(stderr,"Неверное имя файла. Имя файла должно быть вида:\n'Имя линии_тип.gpx'\nГде:\nтип - line (для высоковольтных ВЛ),line04 - для изковольтных линий, station (для подстанций), substation (для ТП)\n\nНапример, для линии 'ВЛ 110 Владивосток:\n'ВЛ 110 Владивосток_line.gpx'\n");  
			exit(1);

		}

		doc = xmlReadFile(Filename, NULL, 0);  

		if (doc == NULL)  
		{  
				fprintf(stderr,"%s:%i: error: could not parse file %s\n", __FILE__,__LINE__, Filename);  
		}  
		else  
		{  

				/* 
				*                    * Get the root element node 
				*                                       */  
				root_element = xmlDocGetRootElement(doc);  

				print_element_names(root_element);  
				
				/* 
				*                    * free the document 
				*                                       */  
				xmlFreeDoc(doc);;  
		}  
		/* 
		*          *Free the global variables that may 
		*                   *have been allocated by the parser. 
		*                            */  
		printf("\n");
		xmlCleanupParser();  
		if (linename)free(linename);
		if (type)free(type);
		return (0);  

}  

/* Recursive function that prints the XML structure */  

static void  
print_element_names(xmlNode * a_node)  
{  
		xmlNode *cur_node = NULL;  

		for (cur_node = a_node; cur_node; cur_node = cur_node->next) {  
				if (cur_node->type == XML_ELEMENT_NODE) {  
					if (strcmp(cur_node->name,"wpt")==0)
					{
#ifdef DEBUG
						fprintf(stderr,"%s:%i: Find wpt!\n",__FILE__,__LINE__);
#endif
						parsing_GeoObject(cur_node);
					}
						//printf("node type: Element, name: %s\n", cur_node->name);  
						//printf("node value: Element, name: %s\n", xmlNodeGetContent(cur_node));  
				}  
		print_element_names(cur_node->children);  
		}  
} 

int parsing_GeoObject(xmlNode * a_node)
{
		int GeoObject_type=GEO_NOTDEFINED;
		xmlNode *cur_node = NULL;  
		xmlChar *name = NULL;  
		xmlChar *description = NULL;  
		xmlChar *lat = NULL;  
		xmlChar *lon = NULL;  
		xmlChar *ele = NULL;  
		// Берём координаты из атрибутов:
		lat=xmlGetProp(a_node, "lat");
#ifdef DEBUG
		fprintf(stderr,"lat: %s\n", lat);  
#endif
		lon=xmlGetProp(a_node, "lon");
#ifdef DEBUG
		fprintf(stderr,"lon: %s\n", lon);  
#endif

		// анализируем дочерние элементы:
		a_node=a_node->children;

		for (cur_node = a_node; cur_node; cur_node = cur_node->next) 
		{  
			if (cur_node->type == XML_ELEMENT_NODE)
			{  
				// Пошёл анализ:
				if (strcmp(cur_node->name,"ele")==0)
				{
#ifdef DEBUG
					fprintf(stderr,"ele: %s\n", xmlNodeGetContent(cur_node));  
#endif
					ele=xmlNodeGetContent(cur_node);  
				}
				if (strcmp(cur_node->name,"name")==0)
				{
#ifdef DEBUG
					fprintf(stderr,"name: %s\n", xmlNodeGetContent(cur_node));  
#endif
					name=xmlNodeGetContent(cur_node);  
				}
				else if (strcmp(cur_node->name,"description")==0)
				{
#ifdef DEBUG
					fprintf(stderr,"description: %s\n", xmlNodeGetContent(cur_node));  
#endif
					description=xmlNodeGetContent(cur_node);  
				}
			}
		}
		if(linename)
				printf("%s;%s;%s;%s;%s;%s\n",type,linename,name,lat,lon,ele);
		else
				printf("%s;%s;%s;%s;%s;%s\n",type,Filename,name,lat,lon,ele);
		return 0;
}

