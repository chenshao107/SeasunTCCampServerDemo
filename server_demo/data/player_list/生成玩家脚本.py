
import json  
import random

  
json_file_path = './zhongjunqi'  
  
# 打开文件以进行写入  
with open(json_file_path, 'r') as file:  
    # 将数据写入文件  
    data=json.load(file)


field=["Name","PlayerID","Score","money"]
      
for i in range(6,50):
    
    data["Name"]="player"+str(i)
    data["PlayerID"]=data["Name"]
    data["Score"]=random.randint(0,500)
    data["money"]=random.randint(0,500)
    with open(data["PlayerID"],'w')  as file:
        json.dump(data,file)