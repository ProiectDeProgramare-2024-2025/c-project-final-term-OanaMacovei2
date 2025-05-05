    #include <ctype.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define RED   "\033[31m"
    #define GREEN "\033[32m"
    #define YELLOW "\033[33m"
    #define BLUE  "\033[94m"
    #define RESET "\033[0m"

    typedef struct Facilitati {
        char *nume[100];
        int nrFacilitati;
    }Facilitati;

    typedef struct Camera {
        char *tip;
        float pret;
        int nrPersoane;
        Facilitati facilitati;
    }Camera;

    typedef struct Rezervare {
        int id;
        char *numeCamera, *data, *dataSfarsit;
        float pretTotal;
        int nrPersoane;
    }Rezervare;

    typedef struct Camere {
        Camera camere[20];
        int nrCamere;
    }Camere;

    typedef struct Rezervari {
        Rezervare rezervari[20];
        int nrRezervari;
    }Rezervari;

    Camere citire(char path[]) {
        FILE *file;
        file = fopen(path, "r");
        char linie[200];
        Camere listaCamere;
        listaCamere.nrCamere = 0;
        fgets(linie, sizeof(linie), file); // antetul
        linie[strcspn(linie, "\n")] = '\0';
        if (file == NULL) {
            printf("Fisierul nu poate fi accesat\n");
            return listaCamere;
        }

        while (fgets(linie, sizeof(linie), file)) {
            if (linie[0] != '\n') {
                linie[strcspn(linie, "\n")] = '\0';
                Camera camera;
                camera.facilitati.nrFacilitati = 0;
                char *contor;
                int j = 0;
                contor = strtok(linie, ",");
                while (contor != NULL) {
                    if (j == 0) {
                        camera.tip = strdup(contor);
                    } else if (j == 1) {
                        camera.pret = atof(contor);
                    } else if (j == 2) {
                        camera.nrPersoane = atoi(contor);
                    } else {
                        camera.facilitati.nume[camera.facilitati.nrFacilitati++] = strdup(contor);
                    }
                    contor = strtok(NULL, ",");
                    j++;
                }
                listaCamere.camere[listaCamere.nrCamere++] = camera;
            }
        }
        fclose(file);
        return listaCamere;
    }

    void afisareCamereDisponibile(Camere listaCamere) {
        printf(BLUE "Tip Camera" RESET"|" BLUE"Pret Pe Noapte" RESET"|" BLUE"Numar Maxim Persoane" RESET"|" BLUE "Facilitate 1" RESET"|" BLUE"Facilitate 2" RESET"|" BLUE"Facilitate 3" RESET"|" BLUE"Facilitate 4" RESET"|" BLUE"Facilitate 5" RESET"...\n");
        for (int i = 0; i < listaCamere.nrCamere; i++) {
            printf("%s, %.2f, %d, ", listaCamere.camere[i].tip, listaCamere.camere[i].pret, listaCamere.camere[i].nrPersoane);
            int j;
            for (j = 0; j < listaCamere.camere[i].facilitati.nrFacilitati - 1; j++) {
                printf("%s, ", listaCamere.camere[i].facilitati.nume[j]);
            }
            printf("%s", listaCamere.camere[i].facilitati.nume[j]);
            printf("\n");
        }
        printf("---------------------------------\n");
    }

    void cautareCameraAfisareFacilitati(Camere listaCamere, int i) {
        printf("%s, %.2f, %d, ", listaCamere.camere[i].tip, listaCamere.camere[i].pret, listaCamere.camere[i].nrPersoane);
        int j;
        for (j = 0; j < listaCamere.camere[i].facilitati.nrFacilitati - 1; j++) {
            printf("%s,", listaCamere.camere[i].facilitati.nume[j]);
        }
        printf("%s", listaCamere.camere[i].facilitati.nume[j]);
    }

    void cautareCameraTip(Camere listaCamere) {
        printf(YELLOW"   Cauta camera dupa tip  \n" RESET);
        printf("--------------------------------------------\n");
        getchar();
        for (int i = 0; i < listaCamere.nrCamere; i++) {
            printf("%s\n", listaCamere.camere[i].tip);
        }
        printf("Introduceti un tip de camera: ");
        char input[100];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        printf("\n");
        char *tip = malloc(sizeof(char) * (strlen(input) + 1));
        strcpy(tip, input);
        int gasit = 0;
        if (tip != NULL) {
            for (int i = 0; i < listaCamere.nrCamere; i++) {
                if (strcmp(tip, listaCamere.camere[i].tip) == 0) {
                    cautareCameraAfisareFacilitati(listaCamere, i);
                    gasit = 1;
                }
            }
        }

        if (!gasit) {
            printf("Nu exista camera de tipul acesta");
        }
        free(tip);
    }

    void cautareCameraFacilitati(Camere listaCamere) {
        printf(YELLOW"   Cauta camera dupa facilitati  \n" RESET);
        printf("--------------------------------------------\n");
        getchar();
        printf("Wi-Fi\nBaie Proprie\nBalcon\nBucatarie\nVedere la mare\n");
        printf("Introduceti o facilitate din lista de mai sus:");
        char input[100];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        printf("\n");

        int gasit = 0;
        char *facilitate = malloc(sizeof(char) * (strlen(input) + 1));
        strcpy(facilitate, input);
        for (int i = 0; i < listaCamere.nrCamere; i++) {
            for (int j = 0; j < listaCamere.camere[i].facilitati.nrFacilitati; j++) {
                if (strcmp(facilitate, listaCamere.camere[i].facilitati.nume[j]) == 0) {
                    cautareCameraAfisareFacilitati(listaCamere, i);
                    printf("\n");
                    gasit = 1;
                }
            }
        }

        if (!gasit) {
            printf(RED"Nu " RESET "exista camere cu aceasta " RED"facilitate"RESET"!\n");
        }
        free(facilitate);
    }

    /**Inceput validare data**/
    int ziInFunctieDeLuna(int luna, int an) {
        if (luna == 2) {
            if (an % 4 == 0) {
                return 29;
            }
            return 28;
        }
        if (luna == 4 || luna == 6 || luna == 9 || luna == 11) {
            return 30;
        }
        return 31;
    }

    int validareFormatData(char data[]) {
        int zi, luna, an;
        if (sscanf(data, "%d/%d/%d", &zi, &luna, &an) == 3) {
            if (an >= 2025 && an <= 2038 && luna >= 1 && luna <= 12) {
                if (zi >= 1 && zi <= ziInFunctieDeLuna(luna, an)) {
                    return 1;
                }
            }
        }
        return 0;
    }


    void validareData(char perioadaInceput[20], char perioadaSfarsit[20]) {
        do {
            printf("Introduceti inceputul rezervarii (DD/MM/YYYY): ");
            scanf("%s", perioadaInceput);
            if (!validareFormatData(perioadaInceput)) {
                printf("Data " RED"invalida\n" RESET);
            }
        }while (!validareFormatData(perioadaInceput));

        do {
            printf("Introduceti sfarsitul rezervarii (DD/MM/YYYY): ");
            scanf("%s", perioadaSfarsit);
            if (!validareFormatData(perioadaSfarsit)) {
                printf("Data " RED"invalida\n" RESET);
            }
        }while (!validareFormatData(perioadaSfarsit));

    }
    /**Sfarsit validare data**/

    Rezervari citireRezervari(char path[]) {
        FILE *file;
        file = fopen(path, "r");
        char linie[200];
        Rezervari listaRezervari;
        listaRezervari.nrRezervari = 0;
        fgets(linie, sizeof(linie), file); // antetul
        linie[strcspn(linie, "\n")] = '\0';
        if (file == NULL) {
            printf("Fisierul nu poate fi accesat\n");
            return listaRezervari;
        }

        while (fgets(linie, sizeof(linie), file)) {
            if (linie[0] != '\n') {
                linie[strcspn(linie, "\n")] = '\0';
                Rezervare rezervare;
                char *contor;
                int j = 0;
                contor = strtok(linie, "-,");
                while (contor != NULL) {
                    if (j == 0) {
                        rezervare.id = atoi(contor); //converteste din string in integer
                    } else if (j == 1) {
                        rezervare.numeCamera = strdup(contor); //strdup creeaza o copie careia ii aloca memorie dinamic
                    } else if (j == 2) {
                        rezervare.nrPersoane = atoi(contor);
                    } else if (j == 3) {
                        rezervare.data = strdup(contor);
                    }
                    else if (j == 4) {
                        rezervare.dataSfarsit = strdup(contor);
                    }
                    else if (j == 5) {
                        rezervare.pretTotal = atof(contor); //converteste din string in float
                    }
                    contor = strtok(NULL, "-,");
                    j++;
                }
                listaRezervari.rezervari[listaRezervari.nrRezervari++] = rezervare;
            }
        }
        fclose(file);
        return listaRezervari;
    }

    int IdUrmator(Rezervari listaRezervari) {
        int maxId = 0;
        for (int i = 0; i < listaRezervari.nrRezervari; i++) {
            if (listaRezervari.rezervari[i].id > maxId) {
                maxId = listaRezervari.rezervari[i].id;
            }
        }
        return maxId + 1;
    }
    void adaugareRezervare(Camere listaCamere, Rezervari *listaRezervari, char pathRezervari[] ,char cameraInput[], int *indexId) {
        FILE *file;
        file = fopen(pathRezervari, "a");
        if (file == NULL) {
            printf("Fisierul nu poate fi accesat");
        }

        Camera *cameraRezervare = NULL;
        for (int i = 0; i < listaCamere.nrCamere; i++) {
            if (strcmp(cameraInput, listaCamere.camere[i].tip) == 0) {
                cameraRezervare = &listaCamere.camere[i]; //daca e gasita camera introdusa de utilizator cameraRezervare va primia dresa camerei gasite cu toate informatiile
                break;
            }
        }

        if (cameraRezervare == NULL) {
            printf("Camera %s nu a fost gasita\n", cameraInput);
            fclose(file);
        }
        int nrPersoane;
        char perioadaInceput[20], perioadaSfarsit[20];
        printf("Introduceti numarul de persoane: ");
        scanf("%d", &nrPersoane);

        while (nrPersoane > cameraRezervare->nrPersoane || nrPersoane < 1) {
            printf(RED "Numarul de persoane" RESET " introdus este " RED "invalid" RESET". Capacitatea camerei este: " YELLOW"%d" RESET". Incercati din nou.\n", cameraRezervare->nrPersoane);
            printf("Introduceti numarul de persoane: ");
            scanf("%d", &nrPersoane);
        }
        getchar();
        validareData(perioadaInceput, perioadaSfarsit);

        Rezervare rezervareNoua;
        rezervareNoua.id = IdUrmator(*listaRezervari);
        rezervareNoua.numeCamera = strdup(cameraRezervare->tip);
        rezervareNoua.data = strdup(perioadaInceput);
        rezervareNoua.dataSfarsit = strdup(perioadaSfarsit);
        rezervareNoua.pretTotal = cameraRezervare->pret;
        rezervareNoua.nrPersoane = nrPersoane;
        listaRezervari->rezervari[listaRezervari->nrRezervari++] = rezervareNoua;

        fprintf(file, "%d,%s,%d,%s-%s,%.2f\n", rezervareNoua.id, rezervareNoua.numeCamera, rezervareNoua.nrPersoane, rezervareNoua.data, rezervareNoua.dataSfarsit, rezervareNoua.pretTotal);
        printf("Rezervarea (" BLUE"%s" RESET " de" BLUE " %d" RESET " persoane in perioada " BLUE "%s" RESET" - " BLUE "%s" RESET ") a fost realizata cu " GREEN"succes" RESET".", cameraRezervare->tip, nrPersoane, perioadaInceput, perioadaSfarsit);
        fclose(file);
    }

    void rezervare(Camere listaCamere, Rezervari *listaRezervari,char pathRezervari[], int *indexId) {
        getchar();
        afisareCamereDisponibile(listaCamere);
        printf("Selectati o camera (numele camerei):\n");
        char sir[100];
        fgets(sir, sizeof(sir), stdin);
        sir[strcspn(sir, "\n")] = '\0';
        adaugareRezervare(listaCamere, listaRezervari,pathRezervari,sir, indexId);
    }

    void afisareRezervari(Camere listaCamere, Rezervari *listaRezervari) {
        int i, j;
        for (i = 0; i < listaRezervari->nrRezervari - 1; i++) {
            printf("%d,%s,%s-%s,%.2f,", listaRezervari->rezervari[i].id, listaRezervari->rezervari[i].numeCamera, listaRezervari->rezervari[i].data, listaRezervari->rezervari[i].dataSfarsit, listaRezervari->rezervari[i].pretTotal);
            for (j = 0; j < listaCamere.camere[i].facilitati.nrFacilitati; j++) {
                printf("%s, ", listaCamere.camere[i].facilitati.nume[j]);
            }
            printf("\n");
        }

        printf("%d,%s,%s-%s,%.2f,", listaRezervari->rezervari[i].id, listaRezervari->rezervari[i].numeCamera, listaRezervari->rezervari[i].data, listaRezervari->rezervari[i].dataSfarsit, listaRezervari->rezervari[i].pretTotal);
        for (j = 0; j < listaCamere.camere[i].facilitati.nrFacilitati; j++) {
            printf("%s, ", listaCamere.camere[i].facilitati.nume[j]);
        }
    }

    void eliminareRezervare(Rezervari *listaRezervari, int idIntrodus, int *gasit) {
        *gasit = 0;
        for (int i = 0; i < listaRezervari->nrRezervari; i++) {
            if (idIntrodus == listaRezervari->rezervari[i].id) {
                *gasit = 1;
                for (int j = i + 1; j <= listaRezervari->nrRezervari; j++) {
                    listaRezervari->rezervari[j - 1] = listaRezervari->rezervari[j];
                }
                listaRezervari->nrRezervari--;
            }
        }
    }

    void anulare(Camere listaCamere, Rezervari *listaRezervari, char pathRezervari[]) {
        printf(YELLOW "   Anuleaza o rezervare  \n" RESET);
        printf("--------------------------------------------\n");
        if (listaRezervari->nrRezervari != 0) {
            afisareRezervari(listaCamere, listaRezervari);
            printf("\n");
            int idIntrodus, gasit;
            printf("Introduceti ID-ul rezervarii dorite: ");
            scanf("%d", &idIntrodus);
            while (idIntrodus < 1) {
                printf("ID " RED"invalid" RESET". Reincercati\n");
                printf("Introduceti ID-ul rezervarii dorite: ");
                scanf("%d", &idIntrodus);
            }
            eliminareRezervare(listaRezervari, idIntrodus, &gasit);

            printf("\n");

            if (listaRezervari->nrRezervari != 0) {
                printf(YELLOW "Afisare rezervari ramase: \n" RESET);
                afisareRezervari(listaCamere, listaRezervari);
            }
            else {
                printf(BLUE"Nu" RESET" mai exista " BLUE"rezervari curente." RESET);
            }


            if (gasit) {
                FILE *file = fopen(pathRezervari, "w");
                fprintf(file, "Id Rezervare|NumeCamera|DataInceput|DataSfarsit|PretTotal\n");

                for (int i = 0; i < listaRezervari->nrRezervari; i++) {
                    fprintf(file, "%d,%s,%d,%s-%s,%.2f\n", listaRezervari->rezervari[i].id, listaRezervari->rezervari[i].numeCamera, listaRezervari->rezervari[i].nrPersoane, listaRezervari->rezervari[i].data, listaRezervari->rezervari[i].dataSfarsit, listaRezervari->rezervari[i].pretTotal);
                }
                fclose(file);
            }
            else {
                printf(BLUE"Nu" RESET" s-a gasit nicio rezervare cu  " BLUE"ID-ul " RESET YELLOW" %d" RESET".\n", idIntrodus);
            }
        }
        else {
            printf(BLUE"Nu" RESET" exista momentan " BLUE"rezervari" RESET ".\n");
        }
    }

    void vizualizareRezervari(Rezervari listaRezervari, Camere listaCamere) {
        printf(YELLOW "   Anuleaza o rezervare  \n" RESET);
        printf("--------------------------------------------\n");
        printf(BLUE "Id Rezervare" RESET"|" BLUE"Nume Camera" RESET"|" BLUE"Numar Persoane" RESET"|" BLUE"Data" RESET"|" BLUE"Pret" RESET"|" BLUE "Facilitate 1" RESET"|" BLUE"Facilitate 2" RESET"|" BLUE"Facilitate 3" RESET"|" BLUE"Facilitate 4" RESET"|" BLUE"Facilitate 5" RESET"...\n");

        if (listaRezervari.nrRezervari < 1) {
            printf(BLUE"Nu " RESET "exista " BLUE "rezervari" RESET".\n");
        }
        for (int i = 0; i < listaRezervari.nrRezervari; i++) {
            printf("%d, %s, %d, %s-%s, %.2f, ", listaRezervari.rezervari[i].id, listaRezervari.rezervari[i].numeCamera, listaRezervari.rezervari[i].nrPersoane, listaRezervari.rezervari[i].data, listaRezervari.rezervari[i].dataSfarsit, listaRezervari.rezervari[i].pretTotal);

            for (int j = 0; j < listaCamere.nrCamere; j++) {
                if (strcmp(listaCamere.camere[j].tip, listaRezervari.rezervari[i].numeCamera) == 0) {
                    int k;
                    for (k = 0; k < listaCamere.camere[j].facilitati.nrFacilitati - 1; k++) {
                        printf("%s, ", listaCamere.camere[j].facilitati.nume[k]);
                    }
                    printf("%s", listaCamere.camere[j].facilitati.nume[k]);
                }
            }
            printf("\n");
        }
        printf("--------------------------------------------\n");
    }


    void optiuni() {
        printf(YELLOW "    Rezervare Hotel\n" RESET);
        printf("--------------------------------------------\n");
        printf(YELLOW  "[1]" RESET " Afisarea tuturor camerelor disponibile\n");
        printf(YELLOW  "[2]" RESET " Cauta camera dupa tip\n");
        printf(YELLOW  "[3]" RESET " Cauta camera dupa facilitati\n");
        printf(YELLOW  "[4]" RESET " Rezerva o cazare\n");
        printf(YELLOW  "[5]" RESET " Anuleaza o rezervare\n");
        printf(YELLOW  "[6]" RESET " Vizualizare rezervari\n");
        printf(YELLOW  "[0]" RESET " Exit\n");
    }

    void back() {
        printf(YELLOW "[0]" RESET" Back\n");
        int comanda;
        do {
            printf("Introduceti optiune: ");
            scanf("%d", &comanda);
            switch (comanda) {
                case 0:
                    system("cls");
                    optiuni();
                    break;
                default:
                    printf("Comanda invalida\n");
                break;
            }
        }while (comanda != 0);
    }

    void meniu(int optiune, Camere listaCamere, Rezervari *listaRezervari, char pathRezervari[], int *indexId) {
        switch (optiune) {
            case 1:
                system("cls");
                printf(YELLOW"   Afisarea tuturor camerelor disponibile  \n" RESET);
                printf("--------------------------------------------\n");
                afisareCamereDisponibile(listaCamere);
                printf("\n");
                back();
                break;
            case 2:
                system("cls");
                cautareCameraTip(listaCamere);
                printf("\n");
                back();
                break;
            case 3:
                system("cls");
                cautareCameraFacilitati(listaCamere);
                printf("\n");
                back();
                break;
            case 4:
                system("cls");
                printf("   Rezerveaza o camera  \n");
                printf("--------------------------------------------\n");
                rezervare(listaCamere, listaRezervari, pathRezervari, indexId);
                printf("\n");
                back();
                break;
            case 5:
                system("cls");
                anulare(listaCamere, listaRezervari, pathRezervari);
                printf("\n");
                back();
                break;
            case 6:
                system("cls");
                vizualizareRezervari(*listaRezervari, listaCamere);
                printf("\n");
                back();
                break;
            case 0:
                printf("Program finalizat");
                break;
            default:
                printf("Comanda invalida\n");
                break;
        }
    }

    int main(void)
    {
        int comanda;
        static int reservareId = 1;
        int *indexId = &reservareId;
        char path[] = "fisier.csv";
        Camere listaCamere = citire(path);
        char pathRezervari[] = "rezervari.csv";
        Rezervari listaRezervari = citireRezervari(pathRezervari);
        optiuni();
        do {
            printf("Introduce o optiune: ");
            scanf("%d", &comanda);
            meniu(comanda, listaCamere, &listaRezervari, pathRezervari, indexId);
        } while(comanda != 0);
        return 0;
    }
