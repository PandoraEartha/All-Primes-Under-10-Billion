//  primesEurl
//  Created by Pandora on 2022/11/20

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct previousPrimeStruct{
    long long unsigned int thisPrime;
    long long unsigned int indexLLU;
    unsigned indexBit;
    unsigned indexArray;
};

struct readFromBitsStruct{
    long long unsigned int primeInnerCount;
    bool maxiumReached;
    long long unsigned int theLastPrime;
};

struct locateStruct{
    long long unsigned int indexArray;
    long long unsigned int indexLLU;
    long long unsigned int indexBit;
    long long unsigned int number;
};

typedef struct previousPrimeStruct PPS;
typedef struct readFromBitsStruct RFBS;
typedef struct locateStruct LS;

LS *getLocation(long long unsigned int number,LS *locationS){
    locationS->indexArray=number>>33;
    locationS->indexLLU=(number&((1ULL<<33)-1))>>6;
    locationS->indexBit=number&63ULL;
    locationS->number=number;
    return locationS;
}

PPS *getNexePrime(long long unsigned int *primArray,long long unsigned int indexArray,long long unsigned int indexLLU,unsigned indexBit,PPS *primeStruct){
    indexBit++;
    while(!(*(&primArray[indexArray]+indexLLU)&(1ULL<<(64-indexBit-1)))){
        indexBit++;
        if(indexBit>63){
            indexBit=1;
            indexLLU++;
        }
        if(indexLLU>=(1<<27)){
            indexBit=1;
            indexLLU=0;
            indexArray++;
        }
    }
    long long unsigned int thisPrime=((indexLLU<<6)+indexBit)^(indexArray<<33ULL);
    primeStruct->thisPrime=thisPrime;
    primeStruct->indexLLU=indexLLU;
    primeStruct->indexBit=indexBit;
    return primeStruct;
}

bool isPrime(LS *location,long long unsigned int primeLLU){
    if(primeLLU&(1ULL<<(64-location->indexBit-1))){
        return true;
    }
    return false;
}

RFBS *readFromBits(long long unsigned int bitData,long long unsigned int indexArray,long long unsigned int index,RFBS *readFromBitsS){
    long long unsigned int prime=index<<6;
    long long unsigned int toAnd=1ULL<<63;
    unsigned primeInnerCount=0;
    readFromBitsS->maxiumReached=false;
    for(int innerIndex=0;innerIndex<64;innerIndex++){
        if(((prime+innerIndex)^(indexArray<<33))>100000000000ULL){
            readFromBitsS->maxiumReached=true;
            readFromBitsS->primeInnerCount=primeInnerCount;
            return readFromBitsS;
        }
        if(bitData&toAnd){
            printf("%llu\n",(prime+innerIndex)^(indexArray<<33));
            readFromBitsS->theLastPrime=(prime+innerIndex)^(indexArray<<33);
            primeInnerCount++;
        }
        toAnd=toAnd>>1;
    }
    readFromBitsS->primeInnerCount=primeInnerCount;
    return readFromBitsS;
}

int main(int argc, const char * argv[]) {
    long long unsigned int *primArray[12]={0};
    for(int indexPoint=0;indexPoint<12;indexPoint++){
        primArray[indexPoint]=(long long unsigned int*)malloc(1<<30);
        memset(primArray[indexPoint],255,1<<30);
    }
    *(primArray[0]+0)=3824771065533498388ULL;
    long long unsigned int number=2;
    long long unsigned int prime=2;
    long long unsigned int indexLLU=1;
    long long unsigned int toGetOut=0;
    long long unsigned int primeLLU=0;
    unsigned indexBit=1;
    unsigned indexArray=0;
    PPS *primeStruct=malloc(sizeof(PPS));
    LS *location=malloc(sizeof(LS));
    while(number<50000000001ULL){
        indexArray=0;
        indexLLU=0;
        indexBit=2;
        prime=2;
        location=getLocation(number,location);
        primeLLU=*(primArray[location->indexArray]+location->indexLLU);
        if(isPrime(location,primeLLU)){
            while(prime<=number && number*prime<100000000001ULL){
                toGetOut=number*prime;
                location=getLocation(toGetOut,location);
                *(primArray[location->indexArray]+location->indexLLU)=*(primArray[location->indexArray]+location->indexLLU)&(~(1ULL<<(64-location->indexBit-1)));
                
                primeStruct=getNexePrime(*primArray,indexArray,indexLLU,indexBit,primeStruct);
                prime=primeStruct->thisPrime;
                indexArray=primeStruct->indexArray;
                indexLLU=primeStruct->indexLLU;
                indexBit=primeStruct->indexBit;
            }
        }
        else{
            toGetOut=number*prime;
            location=getLocation(toGetOut,location);
            *(primArray[location->indexArray]+location->indexLLU)=*(primArray[location->indexArray]+location->indexLLU)&(~(1ULL<<(64-location->indexBit-1)));
            while(number%prime && number*prime<100000000001ULL){
                primeStruct=getNexePrime(*primArray,indexArray,indexLLU,indexBit,primeStruct);
                prime=primeStruct->thisPrime;
                indexArray=primeStruct->indexArray;
                indexLLU=primeStruct->indexLLU;
                indexBit=primeStruct->indexBit;
                
                toGetOut=number*prime;
                location=getLocation(toGetOut,location);
                if(location->indexArray>11){
                    break;
                }
                *(primArray[location->indexArray]+location->indexLLU)=*(primArray[location->indexArray]+location->indexLLU)&(~(1ULL<<(64-location->indexBit-1)));
            }
        }
        number++;
    }
    RFBS *readFromBitsS=malloc(sizeof(RFBS));
    readFromBitsS->maxiumReached=false;
    indexLLU=0;
    indexArray=0;
    long long unsigned int primeCount=0;
    while(!(readFromBitsS->maxiumReached)){
        primeLLU=*(primArray[indexArray]+indexLLU);
        readFromBitsS=readFromBits(primeLLU,indexArray,indexLLU,readFromBitsS);
        indexLLU++;
        primeCount=primeCount+readFromBitsS->primeInnerCount;
        if(indexLLU>=(1<<27)){
            indexLLU=0;
            indexArray++;
        }
    }
    printf("THE LAST PRIME IS: %llu\n",readFromBitsS->theLastPrime);
    printf("THE NUMBER OF PRIMES UNDER 100,000,000,000 IS %llu.\n",primeCount);
    return 0;
}
