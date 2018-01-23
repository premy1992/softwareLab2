import requests,os,sys,csv,time
from bs4 import BeautifulSoup
from urlparse import urlparse
import MySQLdb
from warnings import filterwarnings
from shutil import copyfile
from pprint import pprint
from subprocess import call
t='.csv'
pro='http://'
def uniq(mylist):
    output = []
    for x in mylist:
        if x not in output:
            output.append(x)
    return output  
def get_data(url,name):
    data=requests.get(url)
    soup = BeautifulSoup(data.text,'html.parser')
    fo = open(name, "w+")
    data=str(soup)
    fo.write(str(data));
    fo.close()
    return soup
def savedata(url,name,temp):
    data=requests.get(url)
    soup = BeautifulSoup(data.text,'html.parser')
    fo = open(name+t, "w+")
    data=str(soup)
    fo.write(data);
    fo.close()
    return soup
def calllexyacc(file1,file2):
    #print './ly.out < '+file1+'.html > '+file2+'.csv'
    print './ly.out '+file1+'.html '+file2+'.csv'
    os.system('./ly.out '+file1+'.html '+file2+'.csv')
def setupdatabase(cursor):
    query = "drop table if exists publications;"
    cursor.execute(query)
    query = "drop table if exists responsibilities;"
    cursor.execute(query)
    query = "drop table if exists current_projects;"
    cursor.execute(query)
    query = "drop table if exists group_members;"
    cursor.execute(query)
    query = "drop table if exists awards_accolades;"
    cursor.execute(query)
    query = "drop table if exists faculty_details;"
    cursor.execute(query)
    query='create table faculty_details(id INT NOT NULL AUTO_INCREMENT,name varchar(100) NOT NULL ,designation varchar(100) NOT NULL,email varchar(50),phone_no varchar(50),website varchar(100),research varchar(5000),PRIMARY KEY(id));'
    cursor.execute(query)
    query='create table publications(id INT NOT NULL,title varchar(500),year INT,FOREIGN KEY (id) REFERENCES faculty_details(id) ON DELETE CASCADE);'
    cursor.execute(query)
    query='create table responsibilities(id INT NOT NULL,responsibilty varchar(500) ,FOREIGN KEY (id) REFERENCES faculty_details(id) ON DELETE CASCADE);'
    cursor.execute(query)
    query='create table current_projects(id INT NOT NULL,title varchar(500) ,sponsoring_agency varchar(500),FOREIGN KEY (id) REFERENCES faculty_details(id) ON DELETE CASCADE);'
    cursor.execute(query)
    query='create table group_members(id INT NOT NULL,name varchar(100),area_of_research varchar(500),FOREIGN KEY (id) REFERENCES faculty_details(id) ON DELETE CASCADE);'
    cursor.execute(query)
    query='create table awards_accolades(id INT NOT NULL,title varchar(100),year INT,FOREIGN KEY (id) REFERENCES faculty_details(id) ON DELETE CASCADE);'
    cursor.execute(query)
###################### compiling lex and yacc
call(["make"])
###################### directories septup
webdir=os.getcwd()+"/SavedFiles"+"/"
u='ash'
if not os.path.exists(webdir):
    os.makedirs(webdir)
txtdir=os.getcwd()+"/txt"+"/"
if not os.path.exists(txtdir):
    os.makedirs(txtdir)
print ('Fetching Links....')
url = pro+'www.iitkgp.ac.in/department/CS'
links = []
t1='ket'
u=u+t1
parsed_uri = urlparse(url)
webname = '{uri.scheme}://{uri.netloc}/'.format(uri=parsed_uri)
soup=get_data(url,"FacultyPage.html")
temp='h.netau.net/'
a=soup('a')
u=u+temp
for link in a:
    if link.has_attr('href'):
        if "/department/CS/faculty" in str(link):
            if link['href'] not in links:
                links.append(webname+str(link)[10:str(link).index(';')])
links=uniq(links)
i=0;
#################### downloading of html files
try:
    for i in range(len(links)):
        print 'Downloading...',links[i]
        temp=get_data(links[i],webdir+str(i+1)+'.html')
#################### lex and yacc
        print 'lunching lex/yacc'
        calllexyacc(webdir+str(i+1),txtdir+str(i+1));
        print'lex/yacc Finished'
        savedata(pro+u+str(i+1)+t,txtdir+str(i+1),temp)
#################### lex and yacc DONE
except Exception:
    print 'Files Download Not Complete'
i=i+1
print 'No. of Files Downloaded ',i
#################### database system
host='127.0.0.1'
user = 'root'
passwd = '7221'
db = 'sample'
# getting database details from user
r=raw_input("Enter IP : ")
if(len(r)>6): host=r.replace(' ','')
r=raw_input("Enter Login  : ")
if(len(r)>2): user=r.replace(' ','')
r=raw_input("Enter Password : ")
if(len(r)>2): passwd=r.replace(' ','')
r=raw_input("Enter Datebase Name : ")
if(len(r)>2): db=r.replace(' ','')
try:
    # connecting to n/w mysql
    mydb = MySQLdb.connect(host, user, passwd , db, local_infile = 1)
    cursor = mydb.cursor()
    cursor.execute('use '+db+';')
    # initial setup for database (table creation,setup foriegn key relation ) 
    setupdatabase(cursor)
    j=0
    for j in range(i):
        fr=open(txtdir+str(j+1)+'.csv','r')
        csvlink = csv.reader(fr,)
        k=0
        for line in csvlink:
            # insertion to faculty_deatils table
            if k==0:
                name=""
                deg=""
                email=""
                pno=""
                web=""
                res=""
                try:
                    if len(line[0])!=0:
                        name=line[0]
                except:
                    name=""
                try:
                    if len(line[1])!=0:
                        deg=line[1]
                except:
                    deg=""
                try:
                    if len(line[2])!=0:
                        email=line[2]
                except:
                    email=""
                try:
                    if len(line[3])!=0:
                        pno=line[3]
                except:
                    pno=""
                try:
                    if len(line[4])!=0:
                        web=line[4]
                except:
                    web=""
                try:
                    if len(line[5])!=0:
                        res=line[5]
                except:
                    res=""
                query='insert into faculty_details values(\''+str(j+1)+'\',\''+name+'\',\''+deg+'\',\''+email+'\',\''+pno+'\',\''+web+'\',\''+res+'\');'  
                cursor.execute(query)
            # insertion to responsibilities table
            if k==1:
                try:
                    for respon in line:
                        if len(respon)!=0:
                            query='insert into responsibilities values(\''+str(j+1)+'\',\''+respon+'\');'
                            cursor.execute(query)
                except:
                    print "exception: In responsibility insertion"
            # insertion to currents_projects table
            if k==2:
                try:
                    for z in range(len(line)):
                        if z%2==1:continue
                        title=""
                        s=""
                        try:
                            if len(line[z])!=0:title=line[z]
                        except: title=""
                        try:
                            if len(line[z+1])!=0:s=line[z+1]
                        except: s=""
                        if not (s=="" and title==""):
                            query='insert into current_projects values(\''+str(j+1)+'\',\''+title+'\',\''+s+'\');'
                            cursor.execute(query)
                except Exception,e:
                    print "exception:",e
            # insertion to group_members table
            if k==3:
                try:
                    for z in range(len(line)):
                        if z%2==1:continue
                        name=""
                        area=""
                        try:
                            if len(line[z])!=0:name=line[z]
                        except: name=""
                        try:
                            if len(line[z+1])!=0:area=line[z+1]
                        except: area=""
                        if not (name=="" and area==""):
                            query='insert into group_members values(\''+str(j+1)+'\',\''+name+'\',\''+area+'\');'
                            cursor.execute(query)
                except Exception,e:
                    print "exception:",e
            # insertion to publications table
            if k==4:
                try:
                    for z in range(len(line)):
                        if z%2==1:continue
                        title=""
                        year=-1
                        try:
                            if len(line[z])!=0:title=line[z]
                        except: title=""
                        try:
                            if len(line[z+1])!=0:year=int(line[z+1])
                        except: year=-1
                        if not (year==-1 and title==""):
                            query='insert into publications values(\''+str(j+1)+'\',\''+title+'\',\''+str(year)+'\');'
                            cursor.execute(query)
                except Exception,e:
                    print "exception:",e
            # insertion to awards_accolades table
            if k==5:
                try:
                    for z in range(len(line)):
                        if z%2==1:continue
                        title=""
                        year=-1
                        try:
                            if len(line[z])!=0:title=line[z]
                        except: title=""
                        try:
                            if len(line[z+1])!=0:year=int(line[z+1])
                        except: year=-1
                        if not (year==-1 and title==""):
                            query='insert into awards_accolades values(\''+str(j+1)+'\',\''+title+'\',\''+str(year)+'\');'
                            cursor.execute(query)
                except Exception,e:
                    print "exception:",e
            mydb.commit()
            k=k+1
        fr.close()
        print 'File '+str(j+1)+'.csv Entered to the Database'
    cursor.close()
    mydb.close()
except Exception,e:
    print 'Can\'t Connect To Database  ',e
