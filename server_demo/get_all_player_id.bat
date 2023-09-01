
set "folder=./data"  
set "output=./player_list/player_list.txt" 
  
del .\data\player_list\player_list.txt

cd %folder%  

  
for %%f in (*) do (  
    echo %%f >> %output%  
)

cd  ..\