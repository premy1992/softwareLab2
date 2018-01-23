import csv
import matplotlib.pyplot as plt;
import numpy as np
def uniq(mylist):
    output = []
    for x in mylist:
        if x not in output:
            output.append(x)
    return output    
filename="log.csv"#input("Enter Input Filename : ")
fr=open(filename,'r')
csvlink = csv.reader(fr,)
senlist=[];
msglist=[];
clist=[];
next(csvlink, None)
for line in csvlink:
    t1=[line[2]]+[line[3]]
    t2=[line[4]]+[line[5]]
    try:
        index=senlist.index(t1)
        msglist[index].append(t2+[len(line[6])])
    except Exception:
        senlist.append(t1)
        msglist.append([t2+[len(line[6])]])
    clist.append(t2)
fr.close()
objects =[];
for t in senlist: 
    objects.append(t[0]+':'+t[1])
performance=[];
for t1 in msglist:
    count=0;
    for t2 in t1:
        count=count+(t2[2])
    performance.append(count)
y_pos = np.arange(len(objects))
plt.barh(y_pos, performance, align='center', alpha=.1)
plt.yticks(y_pos, objects)
plt.xlabel('#characters send by clients')
plt.title('load imposed by various clients')
plt.savefig('barchart.png')
plt.show()
plt.close()
clist=clist+senlist
clist=uniq(clist)
l=len(clist)
j=0
count=0;
data=[];
for t in clist:
    data.append([0]*l)
    try:
        index=senlist.index(t)
        for t1 in msglist[index]:
            try:
                i=clist.index([t1[0]]+[t1[1]])
                data[j][i]=data[j][i]+int(t1[2])
                count=count+int(t1[2])
            except Exception:
                None
    except Exception:
        None
    j=j+1
temp=[]
fw=open('gen.csv','w')
fw.write(' ') 
for t in clist: 
    temp.append(t[0]+':'+t[1])
    fw.write(','+t[0]+':'+t[1])
fw.write('\n')
for t in range(len(temp)):
    fw.write(temp[t])
    for t2 in data[t]:
        fw.write(' , '+str(t2))
    fw.write('\n')
fw.close()
column_labels = temp
row_labels = temp
data=np.array(data)
fig, ax = plt.subplots()
heatmap = ax.pcolor(data, cmap=plt.cm.Blues)
ax.set_xticks(np.arange(data.shape[1]) + 0.5, minor=False)
ax.set_yticks(np.arange(data.shape[0]) + 0.5, minor=False)
ax.invert_yaxis()
ax.xaxis.tick_top()
ax.set_xticklabels(column_labels, minor=False)
ax.set_yticklabels(row_labels, minor=False)
plt.title('load imposed by various clients',y=1.07,fontsize=20)
plt.savefig('headmap.png')
plt.show()