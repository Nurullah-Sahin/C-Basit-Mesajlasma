#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>



//metodlar
char *Sifrele(char sifre[50]);
int Client();
int RequestDondur();
int ServerConnection(char *istek);
int Server(char name[50], char pass[50]);
char *SifreCoz(char string[50]);
void GetClients();



//user
struct user 
{
    char name[50];
    char pass[50];
};

//clients
char clients[20][50];


//client sayisi
int sayi = 0;




int main(){
    int loginsonuc = 0;

    //sisteme giris yapacak kullanici
    struct user user1;
    strcpy(user1.name,"VPGR");
    strcpy(user1.pass,"BXOO");

    //sisteme giris yapacak kullanici
    struct user user2;
    strcpy(user2.name,"NLCG");
    strcpy(user2.pass,"OIMM");
    
    //1. kullanici login islemi
    loginsonuc = Client(user1.name, user1.pass);
    if(loginsonuc == 0){
        return 0;
    }
    loginsonuc = 0;
    //2. kullanici login islemi
    loginsonuc = Client(user2.name, user2.pass);
    if(loginsonuc == 0){
        return 0;
    }

    //sisteme consol ile giris yapacak kullanici
    char name[50];
    char pass[50];
    printf(">>login icin isim girin\n");
    scanf("%s",name);
    printf(">>login icin sifre girin\n");
    scanf("%s",pass);
    printf("\n");
    struct user user3;
    strcpy(user3.name,name);
    strcpy(user3.pass,pass);

    loginsonuc = Client(user3.name, user3.pass);
    if(loginsonuc == 0){
        return 0;
    }


    char getIslemi[50];
    printf(">>get islemi icin 'clients' yazabilirsiniz\n");
    scanf("%s",getIslemi);

    if(strcmp(getIslemi, "clients") == 0){
        GetClients();
    }
    else
    {
        printf("\n\n");
        printf(">>hatali ifade yazdiniz.\n");
    }


    printf(">>mesaj atmak istediğiniz kullanicinin id'sini giriniz.\n");
    char id[50];
    scanf("%s",id);

    int a = 0;
    for(int i=0; i<sayi-1; i++){
        
        if(strcmp(clients[i], id) == 0){
            a++;
        }
    }
    if(a == 0){
        printf(">>bu kullanici aktif degil\n");
        return 0;
    }

    printf(">>mesajinizi yaziniz en fazla 50 karakter\n");
    char mesaj[50];
    scanf("%s",mesaj);

    printf(">>'%s' mesajiniz %s isimli kullaniciya iletildi",mesaj, id);

     
   

    return 0;
}

//Sifreleme islemlerinin yapilmasi icin kullanilacak token
int token = 0;



//Bagli client'leri getiren metod
void GetClients(){
    for (int i = 0; i < sayi-1; i++)
    {
        printf(">>%s isimli kullanici aktif\n", clients[i]);
    }
    
}

//criypte islemleri bu metodla yapılacak
char *Sifrele(char string[50]){
    int i= 0;
    for  (i; i < 4; i++)
    {
        int intValue = string[i];
        char value = intValue+12;

        string[i] = value;
    }
    

    return string;
   

}

//encriypte islemi bu metod ile yailacak
char *SifreCoz(char string[50]){
    int i= 0;
    for  (i; i < 4; i++)
    {
        int intValue = string[i];
        char value = intValue-12;

        string[i] = value;
    }
    

    return string;
   

}

//client islemleri bu metod ile yapilacak
int Client(char name[50], char pass[50]){
    int loginsonuc = 0;
    token = RequestDondur();

    char newname[50];
    char newpass[50];
    strcpy(newname, Sifrele(name));
    strcpy(newpass, Sifrele(pass));

    //printf("%s %s", newname, newpass);

    loginsonuc = Server(newname, newpass);

    if(loginsonuc == 0){
        printf(">>login islemi basarisiz oldu\n");

        return loginsonuc;
    }



}

//Request islemleri bu  metoda yapilacak
int RequestDondur(){
    int sonuc;
    //baglanma sitegi mesaji
    char *istek = "baglanti";
    sonuc = ServerConnection(istek);

    return sonuc;

}

//sunucu baglanma
int ServerConnection(char *istek){
    return 12;
}

//server islemlerini bu metod taklid edecek
int Server(char name[50], char pass[50]){
    int loginsonuc = 0;
    char newname[50];
    char newpass[50];

    strcpy(newname, SifreCoz(name));
    strcpy(newpass, SifreCoz(pass));

    //printf("%s %s", newname, newpass);
    FILE *dosya;

    char isimler[50][50];
    char sifreler[50][50];

    if((dosya = fopen("users.txt", "r")) != NULL){

        int i = 0;
        while (!feof(dosya))
        {
            fscanf(dosya, "%s %s", isimler[i], sifreler[i]);
            
            i++;
            
        }
        char *isi;
        char *sif;
        for (int i=0;i<32; i++){
            isi = isimler[i];
            sif = sifreler[i];
            if(strcmp(isi, newname) == 0 && strcmp(sif, sifreler[i]) == 0){
                printf(">>login success\n");
                printf(">>%s isimli kullanici giris yapti \n\n", newname);
                loginsonuc = 1;
            }
        }

        strcpy(clients[sayi], newname);
        sayi++;
        
        
            
        

    }
    else{
        printf(">>dosya bulunamadı");
    }

    fclose(dosya);

    return loginsonuc;

}
