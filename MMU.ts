
export class MMUC {
    _inbios:1;
    _bios:[]; _rom:[]; _extram:[]; _wram:[]; _zpram:[]; 
    constructor(){}

    rb(addr:number){
        // Read 8 bit from addr
    }

    rw(addr:number){
        // Read 16 bit from addr
    }

    wb(addr:number, value:any){
        // Write 8 bit into the address
    }

    ww(addr:number, value:any){
        // Write 16 bit into the addresss
    }
}