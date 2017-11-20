char* SmartSplit(char** start, char* split)
{
       // string is empty
       if(*start == NULL || **start == '\0')
             return NULL;

       char* end = *start + strlen(*start) - 1;
       
       // trim end split 
       while(*end == *split && *start != end)
             end--;

       if(*start != end)
             *(end + 1) = '\0';

       // trim prefix split 
       while(**start == *split && **start != '\0')
             (*start)++;

       char* ret = *start;

       while(**start != *split && **start != '\0')
             (*start)++;

       if(**start != '\0')
             **start = '\0', (*start)++;
       
       if(**start == '\0')
             *start = NULL;

       if(*ret == '\0')
             ret = NULL;

       return ret;
}

int main()
{
       char str0[] = "";
       char str1[] = ",,,1abc,,,,1edf,";
       char str2[] = ",,,,,,2abc,,,,2edf";
       char str3[] = ",,,,,,3abc,,,,3edf,,,,,";
       char str4[] = ",,,,,,,,,";
       char str5[] = "5abc,5edf,5hij,,,,";
       char str6[] = "6abc,6edf,6hij";
       char str7[] = "7abc";

       char* start = NULL;
       char split = ',';
       char* word = NULL;
       vector<string> words;

       start = (char*)str0;
       while(word = SmartSplit(&start, &split)){
             words.push_back(string(word));
       }
       
       start = (char*)str1;
       while(word = SmartSplit(&start, &split)){
             words.push_back(string(word));
       }
       start = (char*)str2;
       while(word = SmartSplit(&start, &split)){
             words.push_back(string(word));
       }
       start = (char*)str3;
       while(word = SmartSplit(&start, &split)){
             words.push_back(string(word));
       }
       start = (char*)str4;
       while(word = SmartSplit(&start, &split)){
             words.push_back(string(word));
       }
       start = (char*)str5;
       while(word = SmartSplit(&start, &split)){
             words.push_back(string(word));
       }
       start = (char*)str6;
       while(word = SmartSplit(&start, &split)){
             words.push_back(string(word));
       }
       start = (char*)str7;
       while(word = SmartSplit(&start, &split)){
             words.push_back(string(word));
       }

       
       getchar();
}
