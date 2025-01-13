#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
// Kargo yapısı
typedef struct{
    int takipNo;
    char gonderici[100];
    char alici[100];
    char kargoDurum[100];
}Kargo;


#define TABLO_BOYUTU 10
Kargo* hash_tablo[TABLO_BOYUTU];

void kargoListesiGoruntule();
int hash_fonksiyonu_2(int);
int double_hashing_indeks(int, int);
void kargoEkle(Kargo*);
Kargo* kargoGetir(int);
void kargoBilgileriniDosyayaYaz(const char*);
void kargoBilgileriniDosyadanOku(const char* dosyaAdi);
void kargoTeslim(int takipNo);

int main(){
    int secim;
    do{
        printf("\nMenu:\n");
        printf("0. Texten Veri Girisi Yap\n");
        printf("1. Tum Kargolari Listele\n");
        printf("2. Kargo Girisi\n");
        printf("3. Kargo Takip\n");
        printf("4. Kargo Teslim\n");
        printf("5. Dosyaya Yaz ve Cikis\n");
        printf("Seciminizi yapin (1-5): ");
        scanf("%d", &secim);

        switch (secim){
            case 0 :{
                kargoBilgileriniDosyadanOku("veri.txt");
                break;
            }
            case 1:{
                // Kargo listesini görüntüle
                kargoListesiGoruntule();
                break;
            }
            case 2:{
                system("cls");
                // Kullanıcıdan kargo bilgilerini al
                Kargo yeniKargo;
                printf("Takip No: ");
                scanf("%d", &yeniKargo.takipNo);

                printf("Gonderici: ");
                scanf("%s", yeniKargo.gonderici);

                printf("Alici: ");
                scanf("%s", yeniKargo.alici);

                printf("Kargo Durumu: ");
                scanf("%s", yeniKargo.kargoDurum);

                kargoEkle(&yeniKargo);

                printf("Kargo basariyla eklendi.\n");
                break;
            }
            case 3:{
                system("cls");
                int takipNoAranan;
                printf("Aranan Takip No: ");
                scanf("%d", &takipNoAranan);

                Kargo* arananKargo = kargoGetir(takipNoAranan);

                if(arananKargo != NULL){
                    printf("Takip No: %d\n", arananKargo->takipNo);
                    printf("Gonderici: %s\n", arananKargo->gonderici);
                    printf("Alici: %s\n", arananKargo->alici);
                    printf("Durum: %s\n", arananKargo->kargoDurum);
                }else{
                    printf("Kargo bulunamadi.\n");
                }
                break;
            }
            case 4:{
                system("cls");
                printf("Teslim almak istediginiz kargonun takip numarasini girin: ");
                int teslimAlTakipNo;
                scanf("%d", &teslimAlTakipNo);
                kargoTeslim(teslimAlTakipNo);
                break;

            }
            case 5:
                // Dosyaya yaz ve programdan çık
                int kaydet;
                system("cls");
                printf("Programdan cikiliyor. Yeni kargo bilgileri dosyanin uzerine yazilsin mi? \n");
                printf("\n 1- Evet      2-Hayir\n Secim : ");
                scanf("%d",&kaydet);
                if(kaydet==1){
                    kargoBilgileriniDosyayaYaz("veri.txt");
                }
                printf("Kapatmak icin herhangi bir tusa basiniz...");
                getch();
                break;
            default:
                printf("Gecersiz secim. Lutfen tekrar deneyin.\n");
        }

    }while(secim != 5);

    return 0;
}



void kargoListesiGoruntule(){
    system("cls");
    printf("\nKargo Listesi:\n");
    printf("------------------------------------------------\n");
    printf("| Takip No  | Gonderici     | Alici         | Kargo Durumu |\n");
    printf("------------------------------------------------\n");

    for(int i = 0; i < TABLO_BOYUTU; i++){
        if(hash_tablo[i] != NULL){
            printf("| %-10d| %-14s| %-14s| %-13s|\n", hash_tablo[i]->takipNo, hash_tablo[i]->gonderici, hash_tablo[i]->alici, hash_tablo[i]->kargoDurum);
        }else{
            printf("| %-10s| %-14s| %-14s| %-13s|\n", "NULL", "NULL", "NULL", "NULL");
        }
    }

    printf("------------------------------------------------\n");
}

int hash_fonksiyonu_2(int takipNo){
    return 7 - (takipNo % 7);
}

int double_hashing_indeks(int takipNo, int deneme){
    int indeks1 = takipNo % TABLO_BOYUTU;
    int indeks2 = hash_fonksiyonu_2(takipNo);
    return (indeks1 + deneme * indeks2) % TABLO_BOYUTU;
}


void kargoEkle(Kargo* yeniKargo){
    Kargo* kopyaKargo = (Kargo*)malloc(sizeof(Kargo));
    kopyaKargo->takipNo = yeniKargo->takipNo;
    strcpy(kopyaKargo->gonderici, yeniKargo->gonderici);
    strcpy(kopyaKargo->alici, yeniKargo->alici);
    strcpy(kopyaKargo->kargoDurum, yeniKargo->kargoDurum);

    int deneme = 0;
    int indeks1 = kopyaKargo->takipNo % TABLO_BOYUTU;
    int indeks2 = hash_fonksiyonu_2(kopyaKargo->takipNo);
    int indeks = (indeks1 + deneme * indeks2) % TABLO_BOYUTU;

    // Double hashing ile indeks bulana kadar ilerle
    while (hash_tablo[indeks] != NULL){
        deneme++;
        indeks = double_hashing_indeks(kopyaKargo->takipNo, deneme);

        // Tablo dolu ise ekleme işlemi yapma
        if (deneme >= TABLO_BOYUTU){
            printf("Hata: Tablo dolu.\n");
            free(kopyaKargo); // Bellek sızıntısını önlemek için belleği serbest bırak
            return;
        }
    }

    hash_tablo[indeks] = kopyaKargo;
}

Kargo* kargoGetir(int takipNo){
    int deneme = 0;
    int indeks1 = takipNo % TABLO_BOYUTU;
    int indeks2 = hash_fonksiyonu_2(takipNo);
    int indeks = (indeks1 + deneme * indeks2) % TABLO_BOYUTU;

    // Double hashing ile indeks bulana kadar ilerle
    while(hash_tablo[indeks] != NULL){
        if(hash_tablo[indeks]->takipNo == takipNo){
            return hash_tablo[indeks];
        }

        deneme++;
        indeks = double_hashing_indeks(takipNo, deneme);
    }

    // Kargo bulunamadı
    return NULL;
}

// Kargo bilgilerini dosyaya yazma fonksiyonu
void kargoBilgileriniDosyayaYaz(const char* dosyaAdi){
    FILE* dosya = fopen(dosyaAdi, "w");

    if(dosya == NULL){
        printf("Hata: Dosya acilamadi.\n");
        return;
    }

    for(int i = 0; i < TABLO_BOYUTU; i++){
        if(hash_tablo[i] != NULL){
            fprintf(dosya, "%d\t%s\t%s\t%s\n", hash_tablo[i]->takipNo, hash_tablo[i]->gonderici, hash_tablo[i]->alici, hash_tablo[i]->kargoDurum);
        }
    }

    fclose(dosya);
}

void kargoBilgileriniDosyadanOku(const char* dosyaAdi){
    system("cls");
    FILE* dosya = fopen(dosyaAdi, "r");

    if(dosya == NULL){
        printf("Hata: Dosya acilamadi.\n");
        return;
    }

    // Dosyanın boş olup olmadığını kontrol et
    fseek(dosya, 0, SEEK_END);
    long dosyaUzunlugu = ftell(dosya);

    if(dosyaUzunlugu == 0){
        printf("Dosya bos.\n");
        fclose(dosya);
        return;
    }

    // Dosyanın başına dön
    fseek(dosya, 0, SEEK_SET);

    while(!feof(dosya)){
        Kargo yeniKargo;
        if(fscanf(dosya, "%d%s%s%s", &yeniKargo.takipNo, yeniKargo.gonderici, yeniKargo.alici, yeniKargo.kargoDurum) == 4){
            kargoEkle(&yeniKargo);
        }else{
            fclose(dosya);
            return;
        }
    }

    fclose(dosya);
}

void kargoTeslim(int takipNo){
    system("cls");

    Kargo* teslimAlinanKargo = kargoGetir(takipNo);

    if(teslimAlinanKargo != NULL){
        int deneme = 0;
        int indeks1 = teslimAlinanKargo->takipNo % TABLO_BOYUTU;
        int indeks2 = hash_fonksiyonu_2(teslimAlinanKargo->takipNo);
        int indeks = (indeks1 + deneme * indeks2) % TABLO_BOYUTU;

        // Double hashing ile indeks bulana kadar ilerle
        while(hash_tablo[indeks] != NULL){
            if(hash_tablo[indeks]->takipNo == teslimAlinanKargo->takipNo){
                free(hash_tablo[indeks]); // Belleği serbest bırak
                hash_tablo[indeks] = NULL; // Hash tablosundan sil
                printf("Kargo teslim alindi ve hash tablosundan silindi.\n");
                return;
            }

            deneme++;
            indeks = double_hashing_indeks(teslimAlinanKargo->takipNo, deneme);
        }
    }else{
        printf("Belirtilen takip numarasina sahip kargo bulunamadi.\n");
    }
}

