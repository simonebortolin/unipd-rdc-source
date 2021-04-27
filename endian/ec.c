unsigned short int endianConversion(unsigned short int x) {
    unsigned short int *ps = &x;
    char * pc = (char*) ps; //scandisco la memoria a byte
    unsigned short int expected = *(pc)*256 + *(pc+1)*1; //valore aspettato post conversione
    unsigned int start = *ps; //valore iniziale
    char temp; //byte temporaneo sove salvare il dato da spostare

    //indico il tipo di conversione che verrà eseguita
    if (isBigOrLittle() == 0) { //scambio i valori dei due byte
        temp = *pc; //memorizzo il primo byte in un temp
        *pc = *(pc+1);
        *(pc+1) = temp;
    }
    return *ps;
}