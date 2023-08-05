import { MMU } from "./main";

export class Z80C {
    private _clock : {
        m: number ;
        t: number;
    }

    _R : {
        a:number, b:number, c:number, d: number, e:number, h:number, l:number, f:number;
        sp: number, pc: number,
        m: number, t:number;
    }

    constructor(){
        this._clock = { m: 0, t:0};
        this._R = {a:0, b:0, c:0, d:0, e:0, h:0, l:0, f:0, sp:0, pc:0, m:0, t:0};
    }

    /**
    * ADD A, E ; adds E to A
    */
    ADDR_e(){
        this._R.a += this._R.e;
        this._R.f = 0;
        if(!(this._R.a & 255)) this._R.f |= 0x80;
        if(this._R.a > 255) this._R.f |= 0x10;
        this._R.a &= 255;
        this._R.m = 1;
        this._R.t = 4;
    }

    /**
    * CP A, B ; compare b to a
    */
    CPR_b(){
        const temp = this._R.a - this._R.b;
        this._R.f |= 0x40;
        if(!(temp & 255)) this._R.f |= 0x80;
        if(temp < 0) this._R.f |= 0x10;
        this._R.m = 1;
        this._R.t = 4;
    }

    /**
    * No operation
    */
    NOP(){
        this._R.m = 1;
        this._R.t = 4;
    }

}
