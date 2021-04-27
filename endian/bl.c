int isBigOrLittle() {
    //ritorna 0 se little endian, 1 se big endian, -1 altrimenti
    //riservo aree di memorie
    char * pc; //puntatore che scandisce la memoria a byte
    short int * pi;
    short int i = 1;
    //associo le aree di memoria a due puntatori di tipo diverso
    pi = &i;
    pc = (char*) &i;
    //se il primo byte dell'area allocata (short int -> 2 byte) contiene il dato '1' e il secondo '0'
    if ( *pc == i && *(pc + 1) == 0 ) {
            return 0; //little endian
    }
    //se il secondo byte dell'area allocata (short int -> 2 byte) contiene il dato '1' e la prima '0'
    if ( *pc == 0 && *(pc + 1) == 1) {
            return 1; //big endian
    }
    return -1;
}