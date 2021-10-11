# Test-PC
Să se implementeze în C/C++ o aplicație client-server (folosind TCP) care simulează protocolul SMTP.


Să se implementeze în C/C++ o aplicație client-server (folosind TCP) care simulează protocolul
SMTP. Serverul va putea deservi simultan mai mulți clienți. Toate comenzile trimise de clienți către
server trebuie să fie comenzi SMTP valide. Toate răspunsurile trimise de server către clienți trebuie să
fie răspunsuri SMTP valide. Puteți presupune că tot conținutul unei cereri sau al unui răspuns SMTP
încape într-un buffer de 1024 octeți.
Serverul se va porni folosind comanda:
./server <PORT>
Clienții se vor porni folosind comanda:
./client <ID_CLIENT> <IP_SERVER> <PORT_SERVER>
După conectare, clienții vor putea primi de la tastatură (în orice moment) una din următoarele
comenzi:
1. send <SURSA> <DESTINATIE> <CORP_MAIL>
a. această comandă va trimite către server mail-ul dat ca parametru
b. sursa și destinația sunt șiruri de caractere în format de adresă e-mail
c. dacă sursa sau destinația nu sunt în format de adresă de mail, clientul va primi o
eroare și mail-ul nu se va trimite
d. corpul mail-ului este un șir de caractere care poate avea spații
e. dialogul între client și server trebuie să urmeze pașii trimiterii unui e-mail folosind
SMTP, astfel:
i. clientul trimite HELO ID_CLIENT (unde ID_CLIENT este ID-ul său dat ca
parametru la pornire)
ii. serverul răspunde cu 250
iii. clientul trimite MAIL FROM: <SURSA>
iv. serverul răspunde cu 250
v. clientul trimite RCPT TO: <DESTINATIE>
vi. serverul răspunde cu 250
vii. clientul trimite DATA
viii. serverul răspunde cu 354
ix. clientul trimite corpul mail-ului (care poate fi orice șir de caractere, nu
neapărat în format de e-mail)
x. serverul răspunde cu 250
xi. clientul trimite QUIT
xii. serverul răspunde cu 221
f. clientul va afișa pe ecran comenzile trimise de el și răspunsurile de la server
2. quit
a. clientul va închide conexiunea cu serverul
Serverul va putea primi de la tastatură următoarele comenzi:
1. status
a. se va afișa numărul de clienți conectați
2. info <ID>
a. se va afișa o listă cu toate mail-urile trimise de clientul ID (în orice format, cu
mențiunea că trebuie să afișați cel puțin sursa și destinația pentru fiecare e-mail)
3. quit
a. se va închide întreaga aplicație (inclusiv clienții)
Dacă este util, se poate considera că serverul acceptă maxim 10 clienți conectați simultan
