
#----------------------------------#
#-------- MailCleaner v0.1 --------#
#----------------------------------#
#                                  #
#   by: Alessandro Carrara(alkz)   #
#   email: alkz.0x80@gmail.com     #
#   build date: 2011-03-22         #
#   for: PoliGrafica SRL           #
#                                  #
#----------------------------------#
#----------------------------------#


import os
import sys
import glob
import pprint
import re
import smtplib
from email.mime.text import MIMEText

DEBUG = False

#-----------------------------#
#---------- Consts -----------#
#-----------------------------#

# Operations
REMOVE = 1
SUB = 2
RESEND = 3
DELETE = 4

#Others
MAILSERVER = "10.0.0.3"

VERSION = "0.1"

#-----------------------------#
#---------- Globals ----------#
#-----------------------------#

rimosse = 0
sostituite = 0
rispedite = 0
cancellate = 0

#-----------------------------#
#--------- Functions ---------#
#-----------------------------#


def mergeEmail(tupla):
	return (tupla[0] + "@" + tupla[1])


#-----------------------------#
#-----------------------------#


def cleanAddress(s):
        extractEmailRegEx = "(\\S+)@(\\S+)"
        pattern = re.compile(extractEmailRegEx)
        result = pattern.findall(s)
        mergedEmail = mergeEmail(result[0])
        mergedEmail = mergedEmail.strip("<>,:\"")

        return mergedEmail


#-----------------------------#
#-----------------------------#


# Ritorna un dict con tutte le info sulla email

def getInfoEmail(filename):
        f = open(filename, "r")
        fields = open("fields.txt", "r")  # Campi dell'header email da cercare
        info = {}
        startLineMessage = 0

        for s in f.readlines():
                if(s == "\n"):    # Header email finito
                        break
                s = s.lower()
                l = s.split(": ", 1)
                fields.seek(0)
                for s1 in fields.readlines():
                        if(s1[0] == '#' or s1[0] == '\n'):   # Commento o riga vuota nel file
                                continue
                        s1 = s1.strip()
                        s1 = s1.lower()
                        if(l[0] == s1):
                                info[l[0]] = l[1].strip()
                startLineMessage += 1
                                
        fields.close()
        f.seek(0)

        s = f.read()
        f.close()
        l = s.split("\n")

        message = l[startLineMessage+1:]
        
        # Let's trim a bit

        clearMessage = []

        for i in range(len(message)):
                message[i] = message[i].strip()
                if(message[i] != ''):
                        clearMessage.append(message[i])

        info["content"] = clearMessage
        return info


#-----------------------------#
#-----------------------------#


def isThereKeyWord(info, fkey):

        found = False
        
        for key in info.keys():
                if(key == "eml" or key == "content"):
                        continue
                
                fkey.seek(0)
                
                for word in fkey.readlines():
                        word = word.strip()

                        # Scarto righe vuote o commenti
                        if(word == ''):
                                continue
                        if(len(word) > 1 and word[0] == '#'):
                                continue

                        if(info[key].find(word) > -1):   # Parola chiave trovata, da rimuovere
                                if(DEBUG):
                                        print("TROVATA! Parola: " + word + " Nel campo: " + key + "("+ info[key] + ")")
                                return True
        return False


#-----------------------------#
#-----------------------------#


def getEmailAddress(content):
        tmp = []
        emails = []

        isValidEmailRegEx = "^.+\\@(\\[?)[a-zA-Z0-9\\-\\.]+\\.([a-zA-Z]{2,3}|[0-9]{1,3})(\\]?)$"
        extractEmailRegEx = "(\\S+)@(\\S+)"

        pattern = re.compile(extractEmailRegEx)
        patternValid = re.compile(isValidEmailRegEx)

        for line in content:
                result = pattern.findall(line)
                if(len(result) > 0):
                        tmp.append(result[0])    #N.B.: Struttura tmp = [ tupla1(nome, domain), tupla2(nome, domain) ]

        for i in range(len(tmp)):
                mergedEmail = mergeEmail(tmp[i])
                mergedEmal = mergedEmail.strip("<>,:\"")
                if(patternValid.match(mergedEmail) != None):
                        emails.append(mergedEmail)

        toReturn = []
        #pprint.pprint(emails)

        if (len(emails) > 0):
                for email in emails:
                        if( (email.split("@"))[1].find("quippe.it") > -1 ):    # Scarto le email di quippe
                                email = "-1"
                        else:
                                toReturn.append(email)    # Mi tengo le email valide
             
                          
        return toReturn

        
#-----------------------------#
#-----------------------------#


# Identifica se l'email e' da rimuovere dal db, sostituire, rispedire oppure semplicemente da cancellare

def riddleMail(info):

        global rimosse
        global sostituite
        global rispedite
        global cancellate

        key_words = open("keyWords/rimuovere.txt", "r")
        
        if(isThereKeyWord(info, key_words)):
                op = REMOVE
                key_words.close()
                
        else:   # Se non e' da rimuovere controllo se e' da sostituire
                key_words = open("keyWords/sostituire.txt", "r")
                
                if(isThereKeyWord(info, key_words)):
                        op = SUB
                        key_words.close()
                        
                else:   # Oppure da rispedire
                        key_words = open("keyWords/rispedire.txt", "r")
                        
                        if(isThereKeyWord(info, key_words)):
                                op = RESEND
                                key_words.close()
                                
                        else:   # Insomma e' da cancellare
                                op = DELETE
                                       

        if(op == REMOVE):

                # 'Ghetto' lol gli indirizzi email da rimuovere
                emails = getEmailAddress(info["content"])
                if(DEBUG):
                        pprint.pprint(emails)
                
                if(len(emails) == 0):    # Email passata come remove, ma in realta' da cancellare
                        op = DELETE
                        cancellate += 1
                        
                        if(DEBUG):
                                print ("Email da cancellare\n")
                        return op
                
                out = open("output/daRimuovere.txt", "a")
                out.write(emails[0] + "\n")
                out.close()
                rimosse += 1
                
                if(DEBUG):
                        print ("Email da rimuovere\n")
                
        elif(op == SUB):

                # 'Ghetto' lol gli indirizzi email da sostituire
                emails = getEmailAddress(info["content"])
                if(DEBUG):
                        pprint.pprint(emails)
                
                if(len(emails) == 0):    # Non hanno specificato quello nuovo, cancello l'email
                        op = DELETE
                        cancellate += 1

                        if(DEBUG):
                                print ("Email da cancellare\n")
                        return op

                # Nel caso avessero specificato piu' di un indirizzo si usa comunque il primo
                
                out = open("output/daSostituire.txt", "a")
                toWrite = cleanAddress(info["from"])
                out.write(toWrite + ";" + emails[0] + "\n")
                out.close()
                sostituite += 1
                
                if(DEBUG):
                        print ("Email da sostituire\n")
                
        elif(op == RESEND):
                rispedite += 1
                
                toWrite = cleanAddress(info["from"])
                
                out = open("output/daRispedire.txt", "a")
                out.write(toWrite + "\n")
                out.close()
                
                if(DEBUG):
                        print("Email da rispedire a: " + toWrite + "\n")
                
        else:
                cancellate += 1

                if(DEBUG):
                        print ("Email da cancellare\n")
                
        return op
        

#-----------------------------#
#-----------------------------#


def deleteDuplicates(filename):
        f = open(filename, "r")
        pathNewFile = os.path.dirname(filename)+ "/tmp.txt"
        fNew = open(pathNewFile, "w+")

        exists = False

        for line in f.readlines():
                exists = False
                fNew.seek(0)
                for line1 in fNew.readlines():
                        if(line == line1):
                                exists = True
                                break
                if(exists == False):
                        fNew.seek(0, os.SEEK_END)
                        fNew.write(line)
                        
        fNew.close()
        f.close()
        os.remove(filename)
        os.renames(pathNewFile, filename)


#-----------------------------#
#-----------------------------#


def sendConfirm(dest):
        msg = MIMEText("Conferma Email - Quippe.it")
        
        sender = "info@quippe.it"
        msg['Subject'] = 'Conferma Email - Quippe.it'
        msg['From'] = sender
        msg['To'] = dest

        s = smtplib.SMTP(MAILSERVER)    # ServerMail 10.0.0.3

        try:
                s.sendmail(sender, dest, msg.as_string())
        except:
                print("Errore imprevisto nell'invio dell'email")

        s.quit()
        

#-----------------------------#
#-----------------------------#
                
os.system("cls")
print ("MailCleaner v" + VERSION + " - by alkz 2011\n\n")

if(len(sys.argv) < 2):
        print("Argomenti mancanti!\n")
        input("")
        exit(1)


if(len(sys.argv) > 2):
        print("Troppi Argomenti!\n")
        input("")
        exit(2)

print("File: " + sys.argv[1])
input("Per continuare premere un tasto, altrimenti CTRL+C\n")
os.chdir(os.path.dirname(sys.argv[0]))

# Pulisco i file
f = open("output/daRimuovere.txt", "w")
f.close()
f = open("output/daSostituire.txt", "w")
f.close()
f = open("output/daRispedire.txt", "w")
f.close()

# Converto in formato eml
os.system("DbxConv.exe -overwrite -eml " + sys.argv[1])

# Rimuovo l'estensione del file .dbx per avere la directory in cui dbxconv ha sbattuto i file eml
for i in range(len(sys.argv[1])-1, 0, -1):
        if(sys.argv[1][i] == '.'):
            break  

path = sys.argv[1][:i] + "\\"
print("Directory files .eml: " + path)
inp = input("\nVuoi visualizzare dei messaggi per eventuale debugging? <y/n>(default n): ")

if(inp == 'y'):
        DEBUG = True

print("\nRiddling emails...\n")
if(DEBUG):
        print("#------------------------------------------------------------------#\n")

emails = 0
InfoEmails = []

# Ogni email ha il proprio dict descrittivo

for infile in glob.glob(os.path.join(path, '*.eml')):
    e = {}
    emails += 1
    if(DEBUG):
            print ("File: " + str(infile.encode("utf-8")) + "\n")
    try:
            e = getInfoEmail(infile)

            # Crivello l'email
            op = riddleMail(e)

            e["eml"] = infile
            e["operation"] = op

            InfoEmails.append(e)
    except:
            print ("File: " + str(infile.encode("utf-8")) + "\n")
            input("Errore di lettura imprevisto")
        
    if(DEBUG):
            print("#------------------------------------------------------------------#\n")


deleteDuplicates("output/daRispedire.txt")
deleteDuplicates("output/daRimuovere.txt")

print ("\nFinished! gli outputs sono stati salvati nella directory output.\n")

risp = input("Ci sono " + str(rispedite) + " email da rispedire, Rispedirle ora? <y/n>(default y): ")
if(risp != 'n'):
        for e in InfoEmails:
                if(e["operation"] == RESEND):
                        f = open("output/daRispedire.txt", "r")
                        for email in f.readlines():
                                sendConfirm(email)
        f = open("output/daRispedire.txt", "w")    # Cancella il contenuto
else:
        print("Email non rispedite")

            
print ("\nEmail totali scansionate: " + str(emails))
print ("Email da rimuovere dal db:  " + str(rimosse))
print ("Email da sostituire nel db: " + str(sostituite))
print ("Email rispedite o da rispedire: " + str(rispedite))
print ("Email da cancellare: " + str(cancellate))
print ("Email impossibili da leggere: " + str(emails-rimosse-sostituite-rispedite-cancellate))
input("")
exit(0)
