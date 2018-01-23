%{
#include<stdio.h>
#include<string.h>
int yylex(void);
void yyerror(const char *s);
%}
%union
{
  char *sval;
}
%token <sval> STRING
%token <sval> STRING1
%token <sval> DESIGNATION
%token <sval> END
%token <sval> NAME
%token <sval> CONTACT
%token <sval> EMAIL
%token <sval> RESPLIST
%token <sval> RESEARCHSTMT
%token <sval> LISTEND
%token <sval> START
%type <sval> state
%type <sval> data
%type <sval> Stmt1

%type <sval> ListData

%token <sval> STARTLI
%token <sval> WEBSITESTART
%token <sval> AwardsAndAccolades
%token <sval> PUBLICATION
%token <sval> PubString
%token <sval> ANYSTRING
%token <sval> SPACES
%token <sval> CURRENTPROJ
%token <sval> GROUPMEM
%token <sval> DIVEND


%%
  state : state STRING { }
        | state PubString { }
        | state NAME data END { printf("\n******NAME = %s $",$3);fflush(stdout);}
        | state DESIGNATION data END { printf("\n******DESIGNATION = %s $",$3);fflush(stdout);}
        | state EMAIL data END { printf("\n******EMAIL = %s $",$3);fflush(stdout);}
        | state CONTACT data END { printf("\n******CONTACT =  %s $",$3);fflush(stdout);}
        | state RESPLIST  { printf("\n******Responsibilities =  ");fflush(stdout);} ListData {printf(" $\n");}
        | state AwardsAndAccolades  { printf("\n******Awards And Accolades =  ");fflush(stdout);} ListData {printf(" $\n");}
        | state RESEARCHSTMT Stmt1 { printf("\n******Research Statement=  %s ",$3);fflush(stdout);}
        | state PUBLICATION {printf("\n***** Publication Tag %s",$2); fflush(stdout);} 
        | state CURRENTPROJ {printf("\n***** Current Proj %s",$2); fflush(stdout);}
        | state GROUPMEM {printf("\n***** Group Members %s",$2); fflush(stdout);}  
        | state WEBSITESTART { printf("\n**** Website = %s $\n",$2); fflush(stdout);}
        | STRING { } 
        | PubString { }
        | state END
        | state SPACES
        | END
        ;
        
  Stmt1 : %empty {$$ = "";}
        | STRING {$$ = $1;}
        ;
                      
                      
  ListData :  ListData  STRING {printf("\n----- %s",$2);}
           |   STRING  {printf("\n----- %s",$1);}  
         ;
                         
  data : data STRING {$1=strcat($1," ");$$=strcat($1,$2);} 
	|  STRING {$$=$1;}
	;
  
%%
