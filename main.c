#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir
{
    char *name;
    struct Dir* parent;
    struct File* head_children_files;
    struct Dir* head_children_dirs;
    struct Dir* next;
} Dir;

typedef struct File
{
    char *name;
    struct Dir* parent;
    struct File* next;
} File;

void touch (Dir* parent, char* name)
{
    File *node=(File *)malloc(sizeof(File));   //aloc memorie pentru fisier
    node->name=(char *)malloc(strlen(name)+1 ); //aloc memorie pentru nume
    strcpy(node->name,name);
    node->next = NULL;
    node->parent=parent;  //pointer catre fisierul parinte

    Dir *tmp=parent->head_children_dirs;
    while(tmp!=NULL)   //caut daca exista directoare cu acelasi nume in dir parinte
    {
        if(strcmp(tmp->name,name)==0)   //daca am gasit director cu acelasi nume eliberez memoria alocata anterior
        {
            free(node->name);
            free(node);
            printf("File already exists\n");
            return;
        }
        tmp=tmp->next;
    }

    File *aux=parent->head_children_files;
    if(aux==NULL)  // daca nu contine fisiere directorul parinte
    {
        parent->head_children_files=node;  //pointer catre primul fisier din director
    }

    else
    {
        while(aux->next!=NULL)  //caut daca exista fisiere cu acelasi nume in directorul parinte
        {
            if(strcmp(aux->name,name)==0)   //daca am gasit fisier cu acelasi nume eliberez memoria alocata anterior
            {
                free(node->name);
                free(node);
                printf("File already exists\n");
                return;
            }
            else
                aux=aux->next;
        }

        if(strcmp(aux->name,name)==0)  //verific si ultimul fisier
        {
            free(node->name);
            free(node);
            printf("File already exists\n");
            return;
        }
        aux->next=node;   //adaug la finalul listei de fisiere noul fisier
    }
}

void mkdir (Dir* parent, char* name)  //analog, dar nu mai initializez inainte de adaugare
{
    File *aux=parent->head_children_files;
    while(aux!=NULL)
    {
        if(strcmp(aux->name,name)==0)
        {
            printf("Directory already exists\n");
            return;
        }
        aux=aux->next;
    }

    Dir *tmp=parent->head_children_dirs;
    if(tmp==NULL) //daca nu contine alte directoare directorul parinte
    {
        Dir *node=(Dir*)malloc(sizeof(Dir));
        node->name=(char *)malloc(strlen(name)+1);
        strcpy(node->name,name);
        node->next = NULL;
        node->head_children_dirs=NULL;
        node->head_children_files=NULL;
        node->parent=parent;
        parent->head_children_dirs=node;
    }

    else
    {
        while(tmp->next!=NULL)
        {
            if(strcmp(tmp->name,name)==0)
            {
                printf("Directory already exists\n");
                return;
            }
            else
                tmp=tmp->next;
        }

        if(strcmp(tmp->name,name)==0)
        {
            printf("Directory already exists\n");
            return;
        }
        Dir *node=(Dir*)malloc(sizeof(Dir));
        node->name=(char *)malloc(strlen(name)+1);
        strcpy(node->name,name);
        node->next = NULL;
        node->head_children_dirs=NULL;
        node->head_children_files=NULL;
        node->parent=parent;
        tmp->next=node;   //adaug noul director la final
    }
}

void ls (Dir* parent)
{

    Dir *tmp=parent->head_children_dirs;
    while(tmp!=NULL)   //afisez mai intai numele directoarelor
    {
        printf("%s\n", tmp->name);
        tmp=tmp->next;
    }

    File *aux=parent->head_children_files;
    while(aux!=NULL)  //apoi afisez numele fisierelor
    {
        printf("%s\n", aux->name);
        aux=aux->next;
    }
}

void rm (Dir* parent, char* name)
{
    int ok=0;  //folosesc variabila ok pentru a verifica daca am gasit sau nu fisierul
    if(parent->head_children_files==NULL)  //fisierul cautat nu exista
    {
        printf("Could not find the file\n");
        return;
    }

    if(strcmp(parent->head_children_files->name,name)==0)   //daca este primul fisier
    {
        File *tmp=parent->head_children_files;
        parent->head_children_files=parent->head_children_files->next;
        free(tmp->name);  //eliberez numele si fisierul
        free(tmp);
        ok=1;  //gasit
        return;
    }

    File *aux=parent->head_children_files;   //daca se afla intre 2 fisiere
    while(aux->next!=NULL)
    {
        if(strcmp(aux->next->name,name)==0)
        {
            File *tmp=aux->next;
            aux->next=tmp->next;
            free(tmp->name);  //analog
            free(tmp);
            ok=1;  //gasit
            return;
        }
        aux=aux->next;
    }

    if(ok==0)  //daca nu a gasit fisierul
    {
        printf("Could not find the file\n");
        return;
    }
}


void rmdir (Dir* parent, char* name)
{
    int ok=0;  //folosesc variabila ok pentru a verifica daca am gasit sau nu directorul

    if(parent->head_children_dirs==NULL)
    {
        printf("Could not find the dir\n");
        return;
    }

    if(strcmp(parent->head_children_dirs->name,name)==0)   //daca este primul fisier
    {
        Dir *tmp=parent->head_children_dirs;
        while(tmp->head_children_files!=NULL)  //sterg toate fiserele din director
        {
            File *remove=tmp->head_children_files;
            tmp->head_children_files=tmp->head_children_files->next;
            free(remove->name);
            free(remove);
        }
        while(tmp->head_children_dirs!=NULL)  //sterg toate directoarele copil
        {
            Dir *remove=tmp->head_children_dirs->next;
            rmdir(tmp,tmp->head_children_dirs->name);
            tmp->head_children_dirs=remove;  //trec la urmatorul director
        }
        Dir *remove=parent->head_children_dirs;  //sterg directorul cautat
        parent->head_children_dirs=parent->head_children_dirs->next;
        free(remove->name);
        free(remove);
        ok=1;  //gasit
        return;
    }

    Dir *aux=parent->head_children_dirs;
    while(aux->next!=NULL)   //daca se afla intre 2 directoare
    {
        if(strcmp(aux->next->name,name)==0)
        {
            Dir *tmp=aux->next;
            while(tmp->head_children_files!=NULL)
            {
                File *remove=tmp->head_children_files;
                tmp->head_children_files=tmp->head_children_files->next;
                free(remove->name);
                free(remove);
                remove=tmp->head_children_files;
                tmp->head_children_files=tmp->head_children_files->next;
            }
            while(tmp->head_children_dirs!=NULL)
            {
                Dir *remove=tmp->head_children_dirs;
                tmp->head_children_dirs=tmp->head_children_dirs->next;
                free(remove->name);
                free(remove);
                remove=tmp->head_children_dirs;
                tmp->head_children_dirs=tmp->head_children_dirs->next;
            }
            aux->next=tmp->next;
            free(tmp->name);
            free(tmp);
            ok=1;
            return;
        }
        aux=aux->next;
    }

    if(ok==0)  //daca nu a gasit directorul
    {
        printf("Could not find the dir\n");
        return;
    }
}

void cd(Dir** target, char *name)
{
    int ok=0;  //verific daca exista directorul
    if((*target)->parent==NULL && strcmp(name,"..")==0)
    {
        return;  //daca am ajuns la radacina
    }

    if((*target)->parent!=NULL && strcmp(name,"..")==0)
    {
        ok=1;  //gasit
        *target=(*target)->parent;  //merg la directorul parinte
    }
    else
    {
        Dir *aux=(*target)->head_children_dirs;
        while(aux!=NULL)
        {
            if(strcmp(aux->name,name)==0)  //caut directorul in care ma voi muta
            {
                *target=aux;
                ok=1;  //gasit
            }
            aux=aux->next;

        }
        if(ok==0)
            printf("No directories found!\n");
    }
}

char *pwd (Dir* target)
{
    char *rez=(char *)malloc(sizeof(char)*100);  //string-ul in care retin calea
    strcpy(rez,"/home");  //directorul radacina

    if(strcmp(target->name,"home")==0)
        return rez;

    char **s=(char **)malloc(sizeof(char*)*100);  //creez un vector de string-uri unde retin calea pana la directorul curent
    int n=0;
    s[n]=malloc(sizeof(strlen(target->name)+1));
    strcpy(s[n],target->name);

    Dir *aux=target->parent;
    while(aux->parent!=NULL)
    {
        s[++n]=malloc(sizeof(strlen(aux->name)+1));  //aloc memorie pt fiecare nume
        strcpy(s[n],aux->name);
        aux=aux->parent;
    }

    int i;
    for(i=n; i>=0; i--)  //creez string-ul in care am calea
    {
        strcat(rez,"/");
        strcat(rez,s[i]);
        free(s[i]);  //eliberez fiecare nume
    }

    free(s);  //eliberez vectorul de string-uri
    return rez;
}

void stop (Dir* target)
{
    Dir *tmp=target->head_children_dirs;
    while(tmp!=NULL)  //sterg directoarele copil ale radacinei
    {
        rmdir(target,tmp->name);
        tmp=target->head_children_dirs;

    }

    File *aux=target->head_children_files;
    while(aux!=NULL)  //sterg fisierele copil ale radacinei
    {
        rm(target,aux->name);
        aux=target->head_children_files;
    }

    free(target->name);  //sterg radacina
    free(target);
}

void tree (Dir* node, int level)
{
    int i;
    Dir *tmp=node->head_children_dirs;
    while(tmp!=NULL)  //afisez mai intai directoareke
    {
        for(i=0; i<level; i++)
            printf("    ");
        printf("%s\n", tmp->name);
        tree(tmp,level+1);  //apel recursiv pentru directorul copil
        tmp=tmp->next;
    }

    File *aux=node->head_children_files;
    while(aux!=NULL)  //afisez apoi fisierele din director
    {
        for(i=0; i<level; i++)
            printf("    ");
        printf("%s\n", aux->name);
        aux=aux->next;
    }
}

void mv(Dir* parent, char *oldname, char *newname)
{
    int p=0; // p va deveni 1 daca dorim sa schimbam numele unui director/ 2 pentru un fisier
    int ok=0;

    Dir *tmp=parent->head_children_dirs;
    Dir *dirchild;
    File *filechild;

    while(tmp!=NULL)
    {
        if(strcmp(tmp->name,oldname)==0)   //caut daca exista directorul cu numele 'oldname'
        {
            p=1;
            ok=1;  //gasit
            dirchild=tmp->head_children_dirs;   //retin continutul directorului cautat
            filechild=tmp->head_children_files;
            break;
        }
        tmp=tmp->next;
    }

    File *aux=parent->head_children_files;
    while(aux!=NULL)
    {
        if(strcmp(aux->name,oldname)==0)   //caut daca exista fisierul cu numele 'oldname'
        {
            p=2;
            ok=1;  //gasit
            break;
        }
        aux=aux->next;
    }

    if(ok==0)
        printf("File/Director not found\n");

    File *aux2=parent->head_children_files;
    while(aux2!=NULL)
    {
        if(strcmp(aux2->name,newname)==0)   //caut daca exista fisiere cu acelasi nume 'newname' in directorul parinte
        {
            printf("File/Director already exists\n");
            return;
        }
        aux2=aux2->next;
    }

    Dir *tmp2=parent->head_children_dirs;
    while(tmp2!=NULL)
    {
        if(strcmp(tmp2->name,newname)==0)   //caut daca exista directoare cu acelasi nume 'newname' in directorul parinte
        {
            printf("File/Director already exists\n");
            return;
        }
        tmp2=tmp2->next;
    }

    if(p==1 && ok==1)  //daca am gasit directorul si nu se afla alt director cu numele 'newname'
    {
        rmdir(parent,tmp->name);  //sterg directorul cautat
        mkdir(parent,newname);  //adaug director la capatul listei cu numele 'newname'
        if(dirchild!=NULL)
        {
            while(parent->head_children_dirs->next!=NULL)
                parent->head_children_dirs=parent->head_children_dirs->next;
            parent->head_children_dirs->next=dirchild;  //adaug continutul directorului
            parent->head_children_files->next=filechild;
        }
    }

    if(p==2)  //analog
    {
        rm(parent,aux->name);
        touch(parent,newname);
    }
}

int main ()
{
    Dir *node = (Dir*)malloc(sizeof(Dir));
    node->name=(char *)malloc(10);
    strcpy(node->name,"home"); //radacina este 'home'
    node->parent=NULL;  //radacina cu parinte NULL
    node->next = NULL;
    node->head_children_dirs=NULL;
    node->head_children_files=NULL;

    char *str = (char *)malloc(MAX_INPUT_LINE_SIZE);   //string-ul in care citesc
    char *s1 = (char *)malloc(MAX_INPUT_LINE_SIZE);
    char *s2 = (char *)malloc(MAX_INPUT_LINE_SIZE);
    char *s3 = (char *)malloc(MAX_INPUT_LINE_SIZE);

    while(fgets(str,MAX_INPUT_LINE_SIZE,stdin)!=NULL)
    {
        int nr1=0;
        int nr2=0;
        int nr3=0;
        int i=0;

        //construiesc 3 string-uri diferite in care retin comenzile, respectiv numele fisierelor/directoarelor
        while(i<strlen(str)-1)
        {
            s1[nr1++]=str[i++];
            if(str[i]==' ')
            {
                i++;
                break;
            }
        }
        while(i<strlen(str)-1)
        {
            s2[nr2++]=str[i++];
            if(str[i]==' ')
            {
                i++;
                break;
            }
        }
        while(i<strlen(str)-1)
        {
            s3[nr3++]=str[i++];
        }

        s1[nr1++]='\0';
        s2[nr2++]='\0';
        s3[nr3++]='\0';

        if(strcmp(s2,"")!=0)  //comenzile
        {
            if(strcmp(s1,"touch")==0)
                touch(node,s2);
            if(strcmp(s1,"mkdir")==0)
                mkdir(node,s2);
            if(strcmp(s1,"cd")==0)
                cd(&node,s2);
            if(strcmp(s1,"rm")==0)
                rm(node,s2);
            if(strcmp(s1,"rmdir")==0)
                rmdir(node,s2);
            if(strcmp(s1,"mv")==0)
                mv(node,s2,s3);
        }
        else
        {
            if(strcmp(s1,"ls")==0)
                ls(node);
            if(strcmp(s1,"pwd")==0)
            {
                char *p=pwd(node);
                printf("%s\n",p);
                free(p);  //eliberez memoria lui 'rez'
            }
            if(strcmp(s1,"tree")==0)
            {
                tree(node,0);
                printf("\n");
            }
            if(strcmp(s1,"stop")==0)
            {
                while(node->parent!=NULL)
                    cd(&node,"..");  //merg pana la radacina
                stop(node);
            }
        }
    }

    free(str);  //eliberez memoria alocata dinamic
    free(s1);
    free(s2);
    free(s3);
    return 0;
}
