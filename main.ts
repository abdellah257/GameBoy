import { MMUC } from './MMU'
import { Z80C } from './z80';

export let MMU: MMUC;
export let Z80: Z80C; 

function main(){
    MMU = new MMUC();
    Z80 = new Z80C();
}