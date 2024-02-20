#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_TREE_HT 100

//Alfabedeki karakter sayısı 37 olarak alinmıstır , 29 harf arti , . ! ? : ; (bosluk) ve ' .
struct aramaTampon *ilk1 = NULL, *son1 = NULL,*iter=NULL,*iter1=NULL,*temp1=NULL;
struct ileriTampon *ilk2 = NULL, *son2 = NULL,*iter2=NULL,*temp2=NULL;
float lz77_before_compress=0;
float lz77_after_compree=0;
float lz77_compress_rate;
struct aramaTampon
{
    struct aramaTampon *ileri;
    struct aramaTampon *geri;
    struct aramaTampon *iter;
    char karakter;

};

struct ileriTampon
{
    struct ileriTampon *ileri;
    struct ileriTampon *geri;
    struct ileriTampon *iter;
    char karakter;

};

void karakter_ekle(char krktr)
{
    struct ileriTampon *dugum2 = (struct ileriTampon*)malloc(sizeof(struct ileriTampon));

    dugum2->karakter = krktr;
    lz77_before_compress=lz77_before_compress+sizeof(dugum2->karakter);
    if(ilk2 == NULL)
    {
        ilk2 = dugum2;
        son2 = dugum2;
        son2->ileri = NULL;
        son2->geri = ilk2;
    }

    else
    {
        son2->ileri = dugum2;
        dugum2->geri = son2;
        son2 = dugum2;
        dugum2->ileri = NULL;
    }

}

void listeyi_Bastir()
{
    struct ileriTampon *liste = ilk2;
    printf("veri yapimiz:");
    while(liste != NULL)
    {
        printf("%c",liste->karakter);
        liste = liste->ileri;
    }
}
void lz77(int karakterS)
{
    struct aramaTampon *dugum1 = ilk1;
    struct ileriTampon *dugum2 = ilk2;
    int aramaKS = 0;
    int B = 0, U = 0;
    char benzerlik;
    int ilkU = 0, ilkB = 0;
    FILE *p,*f;
    p=fopen("output.bin","wb");
    f=fopen("output.txt","w");
    while(1)
    {
        if(ilk1 == NULL)
        {
            dugum1 = (struct aramaTampon*)malloc(sizeof(struct aramaTampon));
            dugum1->karakter = dugum2->karakter;
            ilk1 = dugum1;
            son1 = dugum1;
            son1->ileri = NULL;
            // son1->geri = ilk1;
            aramaKS++;
            dugum2 = dugum2->ileri;
            dugum2->geri = NULL;
            ilk2 = dugum2;
            printf("\n(%d %d \"%c\")",ilkU,ilkB,ilk1->karakter);
            //printf("a");
            fwrite(ilkU,sizeof(ilkU),1,p);
            // printf("a");
            fwrite(ilkB,sizeof(ilkB),1,p);
            //  printf("a");
            char *b=ilk1->karakter;
            fwrite(&b,sizeof(b),1,p);
            //printf("b");
            fprintf(f,"(%d,%d,P[%c])\n",ilkU,ilkB,ilk1->karakter);
            lz77_after_compree=lz77_after_compree+log2(0+karakterS)+log2(37);
        }
        else
        {

            iter=ilk1;     //gerekli iterler
            iter1=ilk1;
            iter2=dugum2;
            U=0;
            B=0;
            int i;
            int flag=0;

            for(i=0; i<aramaKS; i++)      //math bulmak için aramada dolasiyoruz
            {
                if(iter1->karakter==dugum2->karakter)     //ilk match ariyoruz
                {

                    flag++;
                    iter=iter1;

                    while(iter->karakter==iter2->karakter)   //ilk match bulunduysa mathcin devami var mi diye bakiyoruz
                    {

                        B++;
                        if(iter->ileri!=NULL&&iter2->ileri!=NULL) //match uzadikca veri yapisinda ilerliyoruz
                        {
                            iter=iter->ileri;
                            iter2=iter2->ileri;
                        }
                        else
                        {
                            break;
                        }
                    }

                    benzerlik=iter2->karakter;               //match bittiginde ileri tamponda ulastigimiz son karakter 3lu parametremizin bir parcasidir..

                    printf("\n(%d  %d \"%c\" )",(aramaKS-i),B,benzerlik);
                    int *a;
                    a=aramaKS-i;
                    //printf("b");
                    fwrite(&a,sizeof(int),1,p);
                    // printf("d");
                    fwrite(&B,sizeof(int),1,p);
                    char *c=benzerlik;
                    fwrite(&c,sizeof(char),1,p);
                    fprintf(f,"(%d,%d,P[%c])\n",(aramaKS-i),B,benzerlik);
                    lz77_after_compree=lz77_after_compree+log2(aramaKS)+log2(aramaKS+(karakterS-aramaKS))+log2(37);
                    // printf("\naaaa");
                    while(1)                      //pop yapicaz    //iter2 gelmemiz gerek nokta..
                    {
                        if(iter2->karakter!=dugum2->karakter)
                        {

                            dugum1->ileri = (struct aramaTampon*)malloc(sizeof(struct aramaTampon));
                            dugum1->ileri->karakter=dugum2->karakter;
                            dugum1=dugum1->ileri;

                            if(dugum2->ileri!=NULL)
                            {
                                dugum2=dugum2->ileri;
                            }
                            else
                            {
                                break;
                            }
                            ilk2=dugum2;
                            aramaKS++;

                        }
                        else                                                                           // son bi pop yapip break ile cikiyoruz
                        {

                            // printf("%cIIII  %c  III",iter1->karakter,dugum2->karakter);
                            dugum1->ileri = (struct aramaTampon*)malloc(sizeof(struct aramaTampon));
                            dugum1->ileri->karakter=dugum2->karakter;
                            dugum1=dugum1->ileri;

                            if(dugum2->ileri!=NULL)
                            {
                                dugum2=dugum2->ileri;
                            }
                            else
                            {
                                break;
                            }

                            ilk2=dugum2;
                            //  printf("%cIIII  %c  III",iter1->karakter,dugum2->karakter);
                            aramaKS++;
                            break;
                        }
                    }
                    break;
                }

                else
                {

                    if(iter1->ileri!=NULL)
                    {
                        iter1=iter1->ileri;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if(flag==0)
            {

                dugum1->ileri = (struct aramaTampon*)malloc(sizeof(struct aramaTampon));
                dugum1 = dugum1->ileri;
                dugum1->karakter = dugum2->karakter;
                if(dugum2->ileri==NULL)
                {
                    break;
                }
                dugum2 = dugum2->ileri;
                ilk2 = dugum2;
                dugum2->geri = NULL;
                aramaKS++;
                printf("\n(%d %d \"%c\")",ilkU,B,dugum1->karakter);
                //  printf("c");
                fwrite(&(ilkU),sizeof(int),1,p);
                fwrite(&B,sizeof(int),1,p);
                char *b=dugum1->karakter;
                fwrite(&b,sizeof(char),1,p);
                fprintf(f,"(%d,%d,P[%c])\n",ilkU,B,dugum1->karakter);
                lz77_after_compree=lz77_after_compree+log2(aramaKS)+log2(aramaKS+(karakterS-aramaKS))+log2(37);
            }
            flag=0;
        }
        if(dugum2->ileri==NULL)
        {
            //printf("bitti ya la");
            fclose(p);
            fclose(f);
            break;
        }
        //printf("\na\n");
    }

}
int main()
{
    FILE *p;
    char karakter[100];
    char dosya_adi[20];
    printf("Okunacak dosyanin adini uzantili bicimde giriniz:");
label:
    scanf("%s",&dosya_adi);
    printf("%s adli dosya okunuyor.. \n",dosya_adi);
    if(p=fopen(dosya_adi,"r")==NULL)
    {

        printf("Gecersiz dosya adi girisi , lutfen girdiginiz adi kontrol edip tekrar deneyin:");
        goto label;
    }
    else
    {
        p=fopen(dosya_adi,"r");
    }

    int j=0;
    while(!feof(p))
    {
        fscanf(p,"%c",&karakter[j]);
        j++;
    }
    karakter[j-1]='\0';     //bug fix..
    fclose(p);
    printf("Islem uygulanacak metin:%s\n",karakter);
    int karaktersayisi = 0;
    for(int i=0; i<100; i++)
    {
        if(karakter[i] == '\0')
        {
            break;
        }
        karaktersayisi++;
    }
    // printf("girilen metnin karakter sayisi : %d\n",karaktersayisi);
    for(int i=0; i<karaktersayisi; i++)
    {
        karakter_ekle(karakter[i]);
    }
    //listeyi_Bastir();
    lz77(karaktersayisi);
    printf("\nAlfabedeki harf sayisi en genel noktalama isaretleri ile birlikte 37 olarak alinmistir..\nCompress sonrasi boyut:%1.f bit",lz77_after_compree);
    printf("\nCompress oncesi boyut:%1.f bit",(lz77_before_compress)*8);
    lz77_compress_rate=(100*lz77_before_compress)/lz77_after_compree;
    printf("\nCompress orani:%%%2f",lz77_compress_rate);
    return 0;
}

