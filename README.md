# File-System-Simulator-C-
• C Application that simulates a structured collection of files with common Linux bash commands for file modifications 
• Used linked lists, stacks and queues

-----------------------------------------------------------------------------------------------------------------------


Am scris functiile “touch” si “mkdir” dupa modelul “list_append”, iar “rm” si “rmdir” dupa modelul “list_delete”( avand 
cazurile aferente: nod la inceputul listei sau nod intre alte 2 noduri). In plus, pentru functia “rmdir” am eliberat si 
continutul directorului(fisierele si directoarele copil, daca exista). De asemenea, modul in care au fost scrise 
structurile “Dir” si “File” au simplificat rationamentul.Pentru funcția “cd” am utilizat un pointer catre Dir, pentru a 
schimba directorul curent.Functia “tree” am gandit-o ca pe o functie recursiva, astfel: afisez mai intai in modul dorit 
directoarele si fisierele copil, iar apoi apelez recursiv functia pentru fiecare director copil, incrementand si 
“level”. 
Pentru functia “pwd” am creat un vector de string-uri alocat dinamic in care am adaugat numele directoarelor 
până la directorul curent. Apoi, intr-un for descrescator, am concatenat intr-un string fiecare nume urmat de 
‘/‘.Pentru funcția “stop” am ales să utilizez funcțiile “rmdir” și “rm” pentru a sterge directoarele copil, respectiv 
fisierele copil din directorul radacina, iar apoi am sters si directorul “home”.La funcția bonus “mv” am gandit astfel: 
fișierul/directorul cu numele “oldname” va fi șters utilizand functiile deja scrise “rm”/“rmdir” (în cazul unui  
director vom reține si conținutul acestuia inainte de a-l sterge), urmand sa adaugam cu “touch”/“mkdir” un nou fișier, 
respectiv director, avand numele “newname”( pentru director ii adaugam continutul directorului sters).
